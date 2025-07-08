import os
from PyQt6.QtWidgets import (
    QWidget, QVBoxLayout, QHBoxLayout, QComboBox, QSpinBox, QLineEdit,
    QPushButton, QLabel, QGraphicsScene, QGraphicsView, QGroupBox, QTableWidget,
    QTableWidgetItem, QMessageBox, QGraphicsPixmapItem, QGraphicsTextItem, 
    QGraphicsLineItem, QGraphicsRectItem, QScrollArea, QFrame, QSizePolicy
)
from PyQt6.QtGui import QPainter, QIcon, QColor, QFont, QPen, QBrush, QLinearGradient, QPixmap
from PyQt6.QtCore import Qt

class DiscoInterfaz(QWidget):
    def __init__(self, disco, db):
        super().__init__()
        self.disco = disco
        self.db = db
      
        self.setWindowIcon(QIcon(os.path.join(os.path.dirname(__file__), "plato_cd.png")))
        
        self.num_platos = self.disco.platos
        self.superficies_por_plato = self.disco.superficies_por_plato
        self.max_pistas_por_sup = self.disco.pistas_por_superficie
        
        self.nombre_tabla_actual = list(self.db.tablas.keys())[0]
        self.registro_encontrado = None
        
        self.init_ui()
        self.mostrar_diagrama()

    def init_ui(self):
        self.main_layout = QVBoxLayout(self)
        self.main_layout.setContentsMargins(5, 5, 5, 5)
        self.main_layout.setSpacing(5)

        controles_group = QGroupBox("")
        controles_layout = QHBoxLayout()

        self.plato_combo = QComboBox()
        self.plato_combo.addItems([f"Plato {i}" for i in range(self.num_platos)])

        self.superficie_combo = QComboBox()
        self.superficie_combo.addItems([f"Superficie {i}" for i in range(self.superficies_por_plato)])

        self.pista_inicial = QSpinBox()
        self.pista_inicial.setRange(0, self.max_pistas_por_sup - 1)

        self.pistas_a_mostrar = QSpinBox()
        self.pistas_a_mostrar.setRange(self.max_pistas_por_sup, self.max_pistas_por_sup)
        self.pistas_a_mostrar.setValue(self.max_pistas_por_sup)
        self.pistas_a_mostrar.setEnabled(False)

        self.boton_mostrar = QPushButton("Mostrar Diagrama")
        self.boton_mostrar.clicked.connect(self.mostrar_diagrama)

        columnas = self.db.tablas[self.nombre_tabla_actual]['columnas']
        self.columnas = columnas  
        self.columnas_filtrables = columnas[1:]  

        self.filtro_columna = QComboBox()
        self.filtro_columna.addItem("")  
        self.filtro_columna.addItems(self.columnas_filtrables)


        self.filtro_valor = QLineEdit()
        self.filtro_valor.setPlaceholderText("Valor a buscar")

        self.boton_filtrar = QPushButton("Buscar")
        self.boton_filtrar.clicked.connect(self.aplicar_filtro)

        controles_layout.addWidget(QLabel("Plato:"))
        controles_layout.addWidget(self.plato_combo)
        controles_layout.addWidget(QLabel("Superficie:"))
        controles_layout.addWidget(self.superficie_combo)
        controles_layout.addWidget(QLabel("Pista inicial:"))
        controles_layout.addWidget(self.pista_inicial)
        controles_layout.addWidget(QLabel("Pistas a mostrar:"))
        controles_layout.addWidget(self.pistas_a_mostrar)
        controles_layout.addWidget(self.boton_mostrar)
        controles_layout.addSpacing(20)
        controles_layout.addWidget(QLabel("Columna:"))
        controles_layout.addWidget(self.filtro_columna)
        controles_layout.addWidget(QLabel("Valor:"))
        controles_layout.addWidget(self.filtro_valor)
        controles_layout.addWidget(self.boton_filtrar)

        controles_group.setLayout(controles_layout)
        self.main_layout.addWidget(controles_group)

        self.scroll_area = QScrollArea()
        self.scroll_area.setWidgetResizable(True)
        self.scroll_area.setFrameShape(QFrame.Shape.NoFrame)

        self.content_widget = QWidget()
        self.content_layout = QVBoxLayout(self.content_widget)
        self.content_layout.setContentsMargins(5, 5, 5, 5)
        self.content_layout.setSpacing(10)

        self.scene = QGraphicsScene()
        self.view = QGraphicsView(self.scene)
        self.view.setRenderHint(QPainter.RenderHint.Antialiasing)
        self.view.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
        self.view.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAsNeeded)
        self.view.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAsNeeded)
        self.content_layout.addWidget(self.view)

        self.resultado_frame = QFrame()
        self.resultado_frame.setStyleSheet("QFrame { border: 1px solid #4a6ea9; border-radius: 6px; background-color: #1e1e1e; }")
        self.resultado_frame.setVisible(False)
        frame_layout = QVBoxLayout(self.resultado_frame)
        frame_layout.setContentsMargins(5, 5, 5, 5)
        frame_layout.setSpacing(5)

        header_layout = QHBoxLayout()
        header_layout.addStretch()
        self.minimizar_btn = QPushButton("–")
        self.minimizar_btn.setFixedSize(24, 24)
        self.minimizar_btn.clicked.connect(self.minimizar_tabla)
        self.cerrar_btn = QPushButton("X")
        self.cerrar_btn.setFixedSize(24, 24)
        self.cerrar_btn.clicked.connect(self.ocultar_tabla)
        header_layout.addWidget(self.minimizar_btn)
        header_layout.addWidget(self.cerrar_btn)
        frame_layout.addLayout(header_layout)

        self.tabla_resultados = QTableWidget()
        self.tabla_resultados.setColumnCount(len(columnas))
        self.tabla_resultados.setHorizontalHeaderLabels(columnas)
        self.tabla_resultados.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAsNeeded)
        self.tabla_resultados.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAsNeeded)
        self.tabla_resultados.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
        self.tabla_resultados.setMinimumHeight(200)
        frame_layout.addWidget(self.tabla_resultados)

        self.content_layout.addWidget(self.resultado_frame)

        self.scroll_area.setWidget(self.content_widget)
        self.main_layout.addWidget(self.scroll_area)

        self.setStyleSheet("""
            QWidget {
                background-color: #2b2b2b;
                color: #e0e0e0;
            }
            QGroupBox {
                border: 1px solid #4a6ea9;
                border-radius: 5px;
                margin-top: 10px;
                padding: 10px;
            }
            QGroupBox::title {
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 5px;
            }
            QComboBox, QLineEdit, QSpinBox {
                background-color: #3c3f41;
                border: 1px solid #666;
                border-radius: 4px;
                padding: 4px;
                color: #ffffff;
            }
            QPushButton {
                background-color: #4a6ea9;
                color: white;
                font-weight: bold;
                border: 1px solid #2e4e74;
                border-radius: 4px;
                padding: 5px;
            }
            QPushButton:hover {
                background-color: #5c80b9;
            }
            QTableWidget {
                background-color: #3c3f41;
                gridline-color: #4a6ea9;
            }
            QHeaderView::section {
                background-color: #4a6ea9;
                color: white;
                padding: 4px;
                border: 1px solid #4a6ea9;
            }
        """)


    def mostrar_diagrama(self):
        self.scene.clear()

        plato = self.plato_combo.currentIndex()
        superficie = self.superficie_combo.currentIndex()

        pista_ini = 0
        pista_fin = self.max_pistas_por_sup
        num_pistas = pista_fin - pista_ini

        cx, cy = 150, 250
        spacing_y = 40
        sector_size = 25

        uso_lba = {}
        for ubicaciones in self.disco.mapa_ubicacion_fisica.values():
            for lba_u, inicio, fin in ubicaciones:
                uso_lba[lba_u] = uso_lba.get(lba_u, 0) + 1

        ruta_imagen = os.path.join(os.path.dirname(__file__), "plato_cd.png")
        pixmap = QPixmap(ruta_imagen)
        if not pixmap.isNull():
            img_item = QGraphicsPixmapItem(pixmap)
            img_item.setOffset(cx - pixmap.width() / 2, cy - pixmap.height() / 2)
            self.scene.addItem(img_item)

        title = QGraphicsTextItem(f"Disco: Plato {plato} - Superficie {superficie}")
        title.setDefaultTextColor(QColor("#4a9ae9"))
        title_font = QFont()
        title_font.setPointSize(12)
        title_font.setBold(True)
        title.setFont(title_font)
        title.setPos(20, 20)
        self.scene.addItem(title)

        base_x = cx + (pixmap.width() if not pixmap.isNull() else 200) / 2 + 50
        base_y = cy - (num_pistas * spacing_y) / 2


        for i, p in enumerate(range(pista_ini, pista_fin)):
            y = base_y + i * spacing_y

            pista_label = QGraphicsTextItem(f"Pista {p}")
            pista_label.setDefaultTextColor(QColor("#e0e0e0"))
            label_font = QFont()
            label_font.setBold(True)
            pista_label.setFont(label_font)
            pista_label.setPos(base_x - 30, y - 25)
            self.scene.addItem(pista_label)

            if i == 0:
                flecha = QGraphicsLineItem(
                    cx + (pixmap.width() if not pixmap.isNull() else 200) / 2,
                    cy, base_x - 10, y + sector_size / 2
                )
                pen = QPen(QColor("#4a9ae9"), 2, Qt.PenStyle.DashLine)
                flecha.setPen(pen)
                self.scene.addItem(flecha)

            for sec in range(self.disco.sectores_por_pista):
                x_sec = base_x + sec * (sector_size + 5) + 20
                sectores_por_plato = self.disco.pistas_por_superficie * self.disco.sectores_por_pista * self.disco.superficies_por_plato
                lba = (
                    plato * sectores_por_plato +
                    superficie * (self.disco.pistas_por_superficie * self.disco.sectores_por_pista) +
                    p * self.disco.sectores_por_pista + sec
                )
                sector_obj = self.disco.sectores[lba]

                if not self.registro_encontrado:
                    color = QColor("#ee8543") if sector_obj and sector_obj.tamano_ocupado > 0 else QColor("#80bb55")
                else:
                    ubicaciones = [ubi[0] for ubi in self.registro_encontrado.get("ubicaciones", [])]
                    if lba in ubicaciones:
                        color = QColor("#e6402d") if uso_lba.get(lba, 0) > 1 else QColor("#4185f3")
                    else:
                        color = QColor("#ee8543") if sector_obj and sector_obj.tamano_ocupado > 0 else QColor("#8cd855")

                rect = QGraphicsRectItem(x_sec, y, sector_size, sector_size)
                gradient = QLinearGradient(x_sec, y, x_sec, y + sector_size)
                gradient.setColorAt(0, color.lighter(120))
                gradient.setColorAt(1, color.darker(120))
                rect.setBrush(QBrush(gradient))
                rect.setPen(QPen(QColor("#333"), 1))

                tooltip = self.get_sector_tooltip(plato, superficie, p, sec, sector_obj)
                if uso_lba.get(lba, 0) > 1:
                    tooltip += "<br><b>Compartido por múltiples registros</b>"
                rect.setToolTip(tooltip)

                self.scene.addItem(rect)

                if sector_size > 20:
                    sector_num = QGraphicsTextItem(str(sec))
                    sector_num.setDefaultTextColor(QColor("#fff"))
                    sector_num.setPos(x_sec + sector_size / 2 - 5, y + sector_size / 2 - 8)
                    self.scene.addItem(sector_num)

        alto_total = num_pistas * spacing_y
        ancho_total = base_x + (self.disco.sectores_por_pista * (sector_size + 5)) + 100
        self.scene.setSceneRect(0, min(0, base_y) - 50, ancho_total, alto_total + 100)




    def aplicar_filtro(self):
        columna = self.filtro_columna.currentText()
        valor = self.filtro_valor.text().strip()

        if columna.lower() == self.db.tablas[self.nombre_tabla_actual]['columnas'][0].lower():
            QMessageBox.warning(self, "No permitido", "No se puede buscar por la columna ID.")
            return

        registros = self.db.buscar_por_campo(self.nombre_tabla_actual, columna, valor)

        if registros:
            dialogo = QWidget(self, flags=Qt.WindowType.Dialog)
            dialogo.setWindowTitle("Resultados de búsqueda")
            dialogo.resize(800, 500)

            layout = QVBoxLayout(dialogo)
            tabla = QTableWidget()
            columnas = self.db.tablas[self.nombre_tabla_actual]['columnas']

            tabla.setColumnCount(len(columnas))
            tabla.setHorizontalHeaderLabels(columnas)
            tabla.setRowCount(len(registros))

            for i, item in enumerate(registros):
                fila = item["registro"]
                for j, valor in enumerate(fila.values()):
                    tabla.setItem(i, j, QTableWidgetItem(str(valor)))

            tabla.resizeColumnsToContents()
            layout.addWidget(tabla)

            boton_ver = QPushButton("Ver en disco")
            boton_ver.setEnabled(False)
            layout.addWidget(boton_ver)

            def cuando_selecciona():
                boton_ver.setEnabled(True)

            tabla.itemSelectionChanged.connect(cuando_selecciona)

            def ver_en_disco():
                fila = tabla.currentRow()
                if fila >= 0:
                    registro = registros[fila]
                    ubicaciones = registro["ubicacion"]
                    primer_lba = ubicaciones[0][0]

                    plato, superficie, pista, _ = self.disco._lba_a_chs(primer_lba)

                    self.plato_combo.setCurrentIndex(plato)
                    self.superficie_combo.setCurrentIndex(superficie)
                    self.pista_inicial.setValue(pista)

                    self.registro_encontrado = {
                        "registro": registro["registro"],
                        "ubicaciones": ubicaciones
                    }

                    self.mostrar_diagrama()
                    dialogo.close()


            boton_ver.clicked.connect(ver_en_disco)
            dialogo.setLayout(layout)
            dialogo.show()
            dialogo.activateWindow()
            dialogo.raise_()
            dialogo.exec = lambda: None 
        else:
            QMessageBox.information(self, "Sin resultados", "No se encontraron coincidencias.")


    def get_sector_tooltip(self, plato, superficie, pista, sector, sector_obj):
        lba = self.disco._chs_a_lba(plato, superficie, pista, sector)

        registros_en_sector = []
        for id_reg, ubicaciones in self.disco.mapa_ubicacion_fisica.items():
            for lba_u, ini, fin in ubicaciones:
                if lba == lba_u:
                    registros_en_sector.append(id_reg)

        tooltip = (
            f"<b>Plato:</b> {plato}<br>"
            f"<b>Superficie:</b> {superficie}<br>"
            f"<b>Pista:</b> {pista}<br>"
            f"<b>Sector:</b> {sector}<br>"
            f"<b>LBA:</b> {lba}<br>"
        )

        if sector_obj is not None and sector_obj.tamano_ocupado > 0:
            tooltip += f"<b>Ocupado:</b> {sector_obj.tamano_ocupado} bytes<br>"
            if registros_en_sector:
                tooltip += "<b>Registros:</b><br>" + "<br>".join(registros_en_sector)
            else:
                tooltip += "<b>Registro no identificado</b>"
        else:
            tooltip += "<b>Sector vacío</b>"

        return tooltip


        return tooltip
    def ocultar_tabla(self):
        self.resultado_frame.setVisible(False)

    def minimizar_tabla(self):
        if self.tabla_resultados.isVisible():
            self.tabla_resultados.setVisible(False)
            self.minimizar_btn.setText("+")
        else:
            self.tabla_resultados.setVisible(True)
            self.minimizar_btn.setText("–")
    

