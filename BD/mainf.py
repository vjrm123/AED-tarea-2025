import sys
import os
import csv
from PyQt6.QtWidgets import QApplication, QMessageBox
from basededatos import BaseDeDatos
from estructura import leer_sql, parse_create_table, convertir_fila
from interfaz import DiscoInterfaz
from configuracion import ConfigDialog
from disco import DISCO
from archivos import Archivos 


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

    selector = Archivos()
    selector.show()
    app.exec()  

    if not selector.rutas_seleccionadas:
        QMessageBox.critical(None, "Error", "No se seleccionaron archivos validos")
        return

    ruta_txt, ruta_csv = selector.rutas_seleccionadas

    try:
        sql = leer_sql(ruta_txt)
        nombre_tabla, estructura = parse_create_table(sql)

    except Exception as e:
        QMessageBox.critical(None, "Error", f"Error al procesar el archivo .txt:\n{e}")
        return

    if not os.path.exists(ruta_csv):
        QMessageBox.critical(None, "Error", f"No se encontró el archivo CSV:\n{ruta_csv}")
        return

    try:
        cargar_csv_con_estructura(ruta_csv, estructura, bd, nombre_tabla)
    except Exception as e:
        QMessageBox.critical(None, "Error", f"Error al cargar datos del CSV:\n{e}")
        return
    ventana = DiscoInterfaz(disco, bd)
    ventana.resize(1200, 700)
    ventana.show()

    sys.exit(app.exec())


if __name__ == "__main__":
    main()


