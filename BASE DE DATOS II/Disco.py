class Sector:
    def __init__(self, tamano):
        self.tamano_total = tamano
        self.fragmentos = []  # Cada fragmento: {'id_registro', 'datos', 'inicio', 'fin'}
        self.tamano_ocupado = 0

    def espacio_disponible(self):
        return self.tamano_total - self.tamano_ocupado

    def agregar_fragmento(self, id_registro, fragmento_bytes):
        longitud = len(fragmento_bytes)
        if self.espacio_disponible() < longitud:
            return False

        inicio = self.tamano_ocupado
        fin = inicio + longitud - 1
        self.fragmentos.append({
            'id_registro': id_registro,
            'datos': fragmento_bytes.decode('utf-8', errors='ignore'),
            'inicio': inicio,
            'fin': fin
        })
        self.tamano_ocupado += longitud
        return True

    def obtener_fragmentos(self, id_registro):
        return [frag['datos'] for frag in self.fragmentos if frag['id_registro'] == id_registro]


import json

class DISCOLBA:
    def __init__(self, platos=2, pistas=10, sectores_por_pista=20, tamano_sector=16):
        self.platos = platos
        self.pistas_por_superficie = pistas
        self.sectores_por_pista = sectores_por_pista
        self.superficies_por_plato = 2
        self.tamano_sector = tamano_sector

        self.total_sectores = platos * self.superficies_por_plato * pistas * sectores_por_pista
        self.sectores = [None] * self.total_sectores
        self.mapa_ubicacion_fisica = {}  # id_registro -> lista de (lba, inicio, fin)

    def _lba_a_chs(self, lba):
        if lba >= self.total_sectores:
            raise ValueError("LBA fuera de rango")

        spp = self.sectores_por_pista
        pps = self.pistas_por_superficie
        spf = self.superficies_por_plato

        sectores_por_plato = spf * pps * spp
        sectores_por_superficie = pps * spp

        plato = lba // sectores_por_plato
        resto_plato = lba % sectores_por_plato
        superficie = resto_plato // sectores_por_superficie
        resto_sup = resto_plato % sectores_por_superficie
        pista = resto_sup // spp
        sector = resto_sup % spp

        return (plato, superficie, pista, sector)

    def _chs_a_lba(self, plato, superficie, pista, sector):
        spp = self.sectores_por_pista
        pps = self.pistas_por_superficie
        spf = self.superficies_por_plato

        sectores_por_plato = spf * pps * spp
        sectores_por_superficie = pps * spp

        return (plato * sectores_por_plato) + (superficie * sectores_por_superficie) + (pista * spp) + sector

    def guardar_dato(self, registro_dict, id_registro):
        """Guarda un registro como JSON binario fragmentado en sectores disponibles sin desperdicio"""
        dato_json = json.dumps(registro_dict)
        dato_bytes = dato_json.encode('utf-8')
        offset = 0
        total = len(dato_bytes)
        ubicaciones = []

        while offset < total:
            fragmento_bytes = dato_bytes[offset: offset + self.tamano_sector]

            for lba, sector in enumerate(self.sectores):
                if sector is None:
                    # Creamos un sector vacío si no existe
                    sector = Sector(self.tamano_sector)
                    self.sectores[lba] = sector

                if sector.agregar_fragmento(id_registro, fragmento_bytes):
                    # Registrar ubicación del fragmento
                    frag = sector.fragmentos[-1]
                    ubicaciones.append((lba, frag['inicio'], frag['fin']))
                    offset += (frag['fin'] - frag['inicio'] + 1)
                    break
            else:
                raise MemoryError("¡Disco lleno! No hay espacio en sectores disponibles.")

        self.mapa_ubicacion_fisica[id_registro] = ubicaciones

    def recuperar_dato(self, id_registro):
        if id_registro not in self.mapa_ubicacion_fisica:
            return None

        fragmentos = []
        for lba, _, _ in self.mapa_ubicacion_fisica[id_registro]:
            sector = self.sectores[lba]
            if not sector:
                continue
            fragmentos.extend(sector.obtener_fragmentos(id_registro))

        try:
            json_string = ''.join(fragmentos)
            return json.loads(json_string)
        except json.JSONDecodeError:
            return None

    def obtener_ubicacion(self, id_registro):
        return self.mapa_ubicacion_fisica.get(id_registro, None)

    def obtener_registro_formateado(self, id_registro):
        if id_registro not in self.mapa_ubicacion_fisica:
            return None

        contenido = self.recuperar_dato(id_registro)
        if contenido is None:
            return None

        return {
            id_registro: {
                "ubicaciones": self.mapa_ubicacion_fisica[id_registro],
                "contenido": json.dumps(contenido)
            }
        }
