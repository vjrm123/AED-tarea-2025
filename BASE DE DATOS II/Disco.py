import json

class Sector:
    def __init__(self, datos=None):
        self.datos = datos  # string JSON fragmentado
        self.ocupado = datos is not None
        self.tamano = len(datos.encode('utf-8')) if datos else 0

class DISCOLBA:
    def __init__(self, platos=2, pistas=10, sectores_por_pista=20, tamano_sector=64):
        self.platos = platos
        self.pistas_por_superficie = pistas
        self.sectores_por_pista = sectores_por_pista
        self.superficies_por_plato = 2  # Cara superior e inferior
        self.tamano_sector = tamano_sector

        # Total de sectores en el disco
        self.total_sectores = platos * self.superficies_por_plato * pistas * sectores_por_pista

        # Almacenamiento (lista lineal de sectores)
        self.sectores = [None] * self.total_sectores

        # Mapa de ubicación {id_registro: [(plato, superficie, pista, sector), ...]}
        self.mapa_ubicacion_fisica = {}

    def _lba_a_chs(self, lba):
        """Convierte dirección LBA a coordenadas físicas (plato, superficie, pista, sector)"""
        if lba >= self.total_sectores:
            raise ValueError("LBA excede capacidad del disco")

        sectores_por_plato = self.superficies_por_plato * self.pistas_por_superficie * self.sectores_por_pista
        plato = lba // sectores_por_plato
        resto_plato = lba % sectores_por_plato

        sectores_por_superficie = self.pistas_por_superficie * self.sectores_por_pista
        superficie = resto_plato // sectores_por_superficie
        resto_superficie = resto_plato % sectores_por_superficie

        pista = resto_superficie // self.sectores_por_pista
        sector = resto_superficie % self.sectores_por_pista

        return (plato, superficie, pista, sector)

    def _chs_a_lba(self, plato, superficie, pista, sector):
        """Convierte coordenadas físicas a dirección LBA"""
        sectores_por_plato = self.superficies_por_plato * self.pistas_por_superficie * self.sectores_por_pista
        sectores_por_superficie = self.pistas_por_superficie * self.sectores_por_pista

        lba = (plato * sectores_por_plato) + \
                (superficie * sectores_por_superficie) + \
                (pista * self.sectores_por_pista) + \
                sector

        return lba

    def _buscar_sector_libre(self):
        for i, sector in enumerate(self.sectores):
            if sector is None:
                return i
        return None

    def _liberar_bloques(self, bloques):
        for (plato, superficie, pista, sector) in bloques:
            lba = self._chs_a_lba(plato, superficie, pista, sector)
            self.sectores[lba] = None

    def guardar_dato(self, registro_dict, id_registro):
        """Guarda un registro completo (dict) como JSON fragmentado en sectores"""
        dato_json = json.dumps(registro_dict)
        bytes_restantes = len(dato_json.encode('utf-8'))
        bloques_usados = []

        while bytes_restantes > 0:
            espacio = min(bytes_restantes, self.tamano_sector)
            lba_index = self._buscar_sector_libre()
            if lba_index is None:
                self._liberar_bloques(bloques_usados)
                raise MemoryError("¡Disco lleno!")

            # Cortar fragmento respetando longitud de caracteres
            fragmento = dato_json[:espacio]
            self.sectores[lba_index] = Sector(fragmento)

            # Guardar ubicación física
            ubicacion_fisica = self._lba_a_chs(lba_index)
            bloques_usados.append(ubicacion_fisica)

            # Actualizar fragmento restante
            dato_json = dato_json[espacio:]
            bytes_restantes -= espacio

        # Registrar mapeo
        self.mapa_ubicacion_fisica[id_registro] = bloques_usados

    def recuperar_dato(self, id_registro):
        """Reconstruye el registro JSON original como dict"""
        if id_registro not in self.mapa_ubicacion_fisica:
            return None

        datos = []
        for (plato, superficie, pista, sector) in self.mapa_ubicacion_fisica[id_registro]:
            lba = self._chs_a_lba(plato, superficie, pista, sector)
            if self.sectores[lba] and self.sectores[lba].datos:
                datos.append(self.sectores[lba].datos)

        try:
            registro = ''.join(datos)
            return json.loads(registro)
        except json.JSONDecodeError:
            return None  # Registro dañado o mal reconstruido

    def obtener_ubicacion(self, id_registro):
        """Devuelve la ubicación física del registro como lista de tuplas"""
        return self.mapa_ubicacion_fisica.get(id_registro, None)
    
    def obtener_registro_formateado(self, id_registro):
        """Devuelve un dict con 'contenido' y 'ubicaciones' del registro"""
        if id_registro not in self.mapa_ubicacion_fisica:
            return None

        ubicaciones = []
        for (plato, superficie, pista, sector) in self.mapa_ubicacion_fisica[id_registro]:
            lba = self._chs_a_lba(plato, superficie, pista, sector)
            # Calculamos inicio y fin en bytes
            inicio = 0
            if len(ubicaciones) > 0:
                inicio = ubicaciones[-1][2] + 1
            fin = inicio + (self.sectores[lba].tamano if self.sectores[lba] else 0) - 1
            ubicaciones.append((lba, inicio, fin))

        contenido_dict = self.recuperar_dato(id_registro)
        if contenido_dict is None:
            return None

        return {
            id_registro: {
                "ubicaciones": ubicaciones,
                "contenido": json.dumps(contenido_dict)
            }
        }
