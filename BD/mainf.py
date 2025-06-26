import sys
import os
import csv
from PyQt6.QtWidgets import QApplication
from basededatos import BaseDeDatos
from estructura import leer_sql, parse_create_table, convertir_fila
from interfaz import DiscoInterfaz
from configuracion import ConfigDialog
from disco import DISCO

def cargar_csv_con_estructura(csv_path, estructura, base_datos, nombre_tabla, separador=","):
    filas = []
    with open(csv_path, encoding='utf-8') as archivo:
        lector = csv.DictReader(archivo, delimiter=separador, quotechar='"')
        for row in lector:
            fila = convertir_fila(estructura, row)
            filas.append(fila)

    base_datos.definir_tabla(nombre_tabla, estructura)
    base_datos.cargar_datos(nombre_tabla, filas)

def main():
    app = QApplication(sys.argv)

    config_dialog = ConfigDialog()
    if config_dialog.exec():
        valores = config_dialog.get_valores()
        disco = DISCO(
            platos=valores["platos"],
            pistas=valores["pistas"],
            sectores_por_pista=valores["sectores"],
            tamano_sector=valores["tamano"]
        )
    else:
        disco = DISCO()

    bd = BaseDeDatos(disco)

    BASE_DIR = os.path.dirname(os.path.abspath(__file__))
    struct_txt = os.path.join(BASE_DIR, "struct_table.txt")
    try:
        sql = leer_sql(struct_txt)
        estructura = parse_create_table(sql)
        nombre_tabla = "producto"  
    except Exception as e:
        print(f"Error al procesar 'struct_table.txt': {e}")
        sys.exit(1)

    csv_path = os.path.join(BASE_DIR, "producto.csv")
    if not os.path.exists(csv_path):
        print(f"No se encontró el archivo CSV: {csv_path}")
        sys.exit(1)

    cargar_csv_con_estructura(csv_path, estructura, bd, nombre_tabla)

    ventana = DiscoInterfaz(disco, bd)
    ventana.resize(1200, 700)
    ventana.show()

    sys.exit(app.exec())

if __name__ == "__main__":
    main()
