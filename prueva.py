import sys
from PyQt6.QtWidgets import (
    QApplication, QWidget, QPushButton, QLabel,
    QFileDialog, QVBoxLayout, QHBoxLayout, QFrame
)

class SelectorArchivos(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Seleccionar archivos")
        self.setFixedSize(600, 320)
        self.setStyleSheet("""
            QWidget {
                background-color: #000000;
                color: #EDEDED;
                font-family: 'Segoe UI';
                font-size: 13px;
            }
            QFrame {
                background-color: #1C1C1C;
                border: 1px solid #3A3A3A;
                border-radius: 10px;
                padding: 15px;
            }
            QPushButton {
                background-color: #3A3F5C;
                color: white;
                border-radius: 6px;
                padding: 10px;
            }
            QPushButton:disabled {
                background-color: #2A2A2A;
                color: #777777;
            }
            QPushButton:hover:enabled {
                background-color: #555A7F;
            }
        """)

        self.ruta_txt = ""
        self.ruta_csv = ""

        # Bloque TXT
        self.label_txt = QLabel("😢 Ningún archivo .txt seleccionado")
        self.btn_txt = QPushButton("📁 Seleccionar .txt")
        self.btn_txt.clicked.connect(self.seleccionar_txt)
        box_txt = QFrame()
        layout_txt = QVBoxLayout()
        layout_txt.addWidget(self.btn_txt)
        layout_txt.addWidget(self.label_txt)
        box_txt.setLayout(layout_txt)

        # Bloque CSV
        self.label_csv = QLabel("😢 Ningún archivo .csv seleccionado")
        self.btn_csv = QPushButton("📁 Seleccionar .csv")
        self.btn_csv.clicked.connect(self.seleccionar_csv)
        box_csv = QFrame()
        layout_csv = QVBoxLayout()
        layout_csv.addWidget(self.btn_csv)
        layout_csv.addWidget(self.label_csv)
        box_csv.setLayout(layout_csv)

        # Layout horizontal de bloques
        h_layout = QHBoxLayout()
        h_layout.setSpacing(20)
        h_layout.addWidget(box_txt)
        h_layout.addWidget(box_csv)

        # Botón continuar
        self.btn_continuar = QPushButton("✅ Continuar")
        self.btn_continuar.setEnabled(False)
        self.btn_continuar.clicked.connect(self.continuar)

        # Layout principal
        main_layout = QVBoxLayout()
        main_layout.addLayout(h_layout)
        main_layout.addSpacing(20)
        main_layout.addWidget(self.btn_continuar, alignment=1)  # centrado

        self.setLayout(main_layout)

    def seleccionar_txt(self):
        ruta, _ = QFileDialog.getOpenFileName(
            self, "Seleccionar archivo .txt", "", "Archivos de texto (*.txt)"
        )
        if ruta.endswith(".txt"):
            self.ruta_txt = ruta
            self.label_txt.setText(f"😄 .txt seleccionado:\n{ruta}")
        else:
            self.ruta_txt = ""
            self.label_txt.setText("❌ Solo se acepta archivo .txt")
        self.verificar_continuar()

    def seleccionar_csv(self):
        ruta, _ = QFileDialog.getOpenFileName(
            self, "Seleccionar archivo .csv", "", "Archivos CSV (*.csv)"
        )
        if ruta.endswith(".csv"):
            self.ruta_csv = ruta
            self.label_csv.setText(f"😄 .csv seleccionado:\n{ruta}")
        else:
            self.ruta_csv = ""
            self.label_csv.setText("❌ Solo se acepta archivo .csv")
        self.verificar_continuar()

    def verificar_continuar(self):
        if self.ruta_txt and self.ruta_csv:
            self.btn_continuar.setEnabled(True)
        else:
            self.btn_continuar.setEnabled(False)

    def continuar(self):
        print("Continuando con:")
        print("TXT:", self.ruta_txt)
        print("CSV:", self.ruta_csv)
        # Aquí puedes lanzar DiscoInterfaz o pasarle estas rutas al sistema principal
        self.close()  # opcional: cerrar esta ventana


if __name__ == "__main__":
    app = QApplication(sys.argv)
    ventana = SelectorArchivos()
    ventana.show()
    sys.exit(app.exec())
