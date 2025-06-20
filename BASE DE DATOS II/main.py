from PyQt6.QtWidgets import QApplication
from BD import BaseDeDatosFiltrado
from Interfaz import DiscoInterfaz
import sys

def main():
    app = QApplication(sys.argv)

    db = BaseDeDatosFiltrado()

    empleados = [
        {"id": 1, "nombre": "Juan", "edad": 30, "departamento": "IT", "salario": 45000},
        {"id": 2, "nombre": "Ana", "edad": 25, "departamento": "Ventas", "salario": 38000},
        {"id": 3, "nombre": "Juan", "edad": 30, "departamento": "RRHH", "salario": 42000},
        {"id": 4, "nombre": "Marta", "edad": 35, "departamento": "IT", "salario": 50000},
        {"id": 5, "nombre": "Carlos", "edad": 28, "departamento": "Ventas", "salario": 36000},
        {"id": 6, "nombre": "Sofía", "edad": 32, "departamento": "Marketing", "salario": 48000},
        {"id": 7, "nombre": "Pedro", "edad": 40, "departamento": "IT", "salario": 65000},
        {"id": 8, "nombre": "Laura", "edad": 29, "departamento": "RRHH", "salario": 41000},
        {"id": 9, "nombre": "Diego", "edad": 27, "departamento": "Ventas", "salario": 37000},
        {"id": 10, "nombre": "Elena", "edad": 38, "departamento": "IT", "salario": 55000},
        {"id": 11, "nombre": "Miguel", "edad": 31, "departamento": "Marketing", "salario": 49000},
        {"id": 12, "nombre": "Isabel", "edad": 26, "departamento": "Ventas", "salario": 39000},
        {"id": 13, "nombre": "Javier", "edad": 33, "departamento": "IT", "salario": 52000},
        {"id": 14, "nombre": "Carmen", "edad": 40, "departamento": "RRHH", "salario": 47000},
        {"id": 15, "nombre": "Ricardo", "edad": 29, "departamento": "Marketing", "salario": 43000},
        {"id": 16, "nombre": "Patricia", "edad": 36, "departamento": "IT", "salario": 58000},
        {"id": 17, "nombre": "Fernando", "edad": 40, "departamento": "Ventas", "salario": 51000},
        {"id": 18, "nombre": "Juan", "edad": 24, "departamento": "Marketing", "salario": 40000},
        {"id": 19, "nombre": "Roberto", "edad": 37, "departamento": "IT", "salario": 60000},
        {"id": 20, "nombre": "Beatriz", "edad": 20, "departamento": "RRHH", "salario": 44000},
        {"id": 21, "nombre": "Alberto", "edad": 31, "departamento": "Ventas", "salario": 46000},
        {"id": 22, "nombre": "Silvia", "edad": 40, "departamento": "Marketing", "salario": 52000},
        {"id": 23, "nombre": "Juan", "edad": 39, "departamento": "IT", "salario": 62000},
        {"id": 24, "nombre": "Olga", "edad": 27, "departamento": "RRHH", "salario": 43000}
    ]
    db.cargar_datos("empleados", empleados)

    ventana = DiscoInterfaz(db.disco, db)
    ventana.resize(1200, 800)
    ventana.show()

    sys.exit(app.exec())

if __name__ == "__main__":
    main()
