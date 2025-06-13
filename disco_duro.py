class DISCODURO:
    def __init__(self, platos=2, pistas=10, sectores=100, tamano_sector=64):
        self.platos = [[[None for _ in range(sectores)] for _ in range(pistas)] for _ in range(platos)]
        self.mapa_ubicacion = {}  # {id: [(plato, pista, sector)]}
        self.tamano_sector = tamano_sector

    def guardar_dato(self, dato, id_registro):
        bytes_restantes = len(dato.encode('utf-8'))
        bloques_usados = []
        
        while bytes_restantes > 0:
            espacio = min(bytes_restantes, self.tamano_sector)
            sector_libre = self._buscar_sector_libre()
            
            if not sector_libre:
                self._liberar_bloques(bloques_usados)
                raise MemoryError("¡Disco lleno! No se pudo guardar el dato completo.")
            
            plato, pista, sector = sector_libre
            fragmento = dato[:espacio]
            self.platos[plato][pista][sector] = fragmento
            bloques_usados.append((plato, pista, sector))
            dato = dato[espacio:]
            bytes_restantes -= espacio
        
        self.mapa_ubicacion[id_registro] = bloques_usados
        return True

    def _buscar_sector_libre(self):
        for plato in range(len(self.platos)):
            for pista in range(len(self.platos[plato])):
                for sector in range(len(self.platos[plato][pista])):
                    if self.platos[plato][pista][sector] is None:
                        return (plato, pista, sector)
        return None

    def _liberar_bloques(self, bloques):
        for (plato, pista, sector) in bloques:
            self.platos[plato][pista][sector] = None

    def recuperar_dato(self, id_registro):
        if id_registro not in self.mapa_ubicacion:
            return None
        
        dato = ""
        for (plato, pista, sector) in self.mapa_ubicacion[id_registro]:
            dato += self.platos[plato][pista][sector]
        return dato

    def mostrar_ubicacion_por_id(self):
        print("\n=== Ubicación de datos por ID ===")
        for id_registro, bloques in self.mapa_ubicacion.items():
            print(f"\nID: {id_registro}")
            for i, (plato, pista, sector) in enumerate(bloques, 1):
                fragmento = self.platos[plato][pista][sector]
                print(f"  Bloque {i}: Plato {plato}, Pista {pista}, Sector {sector}")
                print(f"    Contenido: {fragmento[:20]}... ({len(fragmento)} bytes)")
    def resumen_almacenamiento(self, id_registro):
        """Devuelve un resumen breve para mostrar inicialmente"""
        if id_registro not in self.mapa_ubicacion:
            return None
        bloques = self.mapa_ubicacion[id_registro]
        return f"Dato '{id_registro}' guardado en {len(bloques)} sectores."

    def detalle_almacenamiento(self, id_registro):
        """Devuelve detalles técnicos para mostrar al expandir"""
        if id_registro not in self.mapa_ubicacion:
            return None
        
        detalle = f"🔍 Detalles de almacenamiento - ID: {id_registro}\n\n"
        bloques = self.mapa_ubicacion[id_registro]
        dato_completo = self.recuperar_dato(id_registro)
        
        for i, (plato, pista, sector) in enumerate(bloques, 1):
            fragmento = self.platos[plato][pista][sector]
            inicio_byte = (i-1) * self.tamano_sector
            fin_byte = inicio_byte + len(fragmento.encode('utf-8')) - 1
            
            detalle += (
                f"🔹 Bloque {i}: Plato {plato}, Pista {pista}, Sector {sector}\n"
                f"   ▶ Bytes: {inicio_byte}-{fin_byte}\n"
                f"   ▶ Fragmento: '{fragmento}'\n"
                f"   ▶ Tamaño: {len(fragmento.encode('utf-8'))} bytes\n"
                f"   {'─'*40}\n"
            )
        
        detalle += f"\n📝 Dato completo reunido:\n{dato_completo}"
        return detalle