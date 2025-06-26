import os
import json
from Disco import DISCOLBA
from PyQt6.QtWidgets import (
    QWidget, QVBoxLayout, QHBoxLayout, QComboBox, QSpinBox, QLineEdit,
    QPushButton, QLabel, QMessageBox, QGraphicsScene, QGraphicsView,
    QGraphicsRectItem, QGraphicsTextItem, QGraphicsLineItem, QGraphicsPixmapItem,
    QGroupBox, QSizePolicy, QTableWidget, QTableWidgetItem
)
from PyQt6.QtGui import (
    QColor, QBrush, QPen, QPixmap, QFont, QLinearGradient, QPainter, 
    QFontDatabase, QIcon
)
from PyQt6.QtCore import Qt, QRectF, QPointF

class DiscoInterfaz(QWidget):
    def __init__(self, disco, db):
        super().__init__()
        self.disco = disco
        self.db = db
        self.setWindowTitle("Disco")
        self.setWindowIcon(QIcon(os.path.join(os.path.dirname(__file__), "plato_cd.png")))
        self.setStyleSheet("""
            QWidget { background-color: #2b2b2b; color: #e0e0e0; }
            QPushButton { background-color: #3c3f41; border: 1px solid #555; border-radius: 4px; padding: 5px; }
            QPushButton:hover { background-color: #4e5254; }
            QComboBox, QSpinBox, QLineEdit { background-color: #3c3f41; border: 1px solid #555; border-radius: 3px; padding: 3px; }
            QLabel { color: #bbbbbb; }
            QGroupBox { border: 1px solid #555; border-radius: 5px; margin-top: 10px; padding-top: 15px; }
            QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 3px; }
        """)
        self.num_platos = self.disco.platos
        self.superficies_por_plato = self.disco.superficies_por_plato
        self.max_pistas_por_sup = self.disco.pistas_por_superficie
        self.sectores_por_pista = self.disco.sectores_por_pista
        self.registro_encontrado = None
        self.registro_seleccionado = None
        self.init_ui()
        self.mostrar_diagrama()

    def init_ui(self):
        self.main_layout = QVBoxLayout()
        self.setLayout(self.main_layout)

        # === Grupo de control de visualización del disco ===
        self.control_group = QGroupBox("")
        control_layout = QHBoxLayout()
        self.plato_combo = self.create_combo_box("Plato:", [f"Plato {i}" for i in range(self.num_platos)])
        self.superficie_combo = self.create_combo_box("Superficie:", [f"Superficie {i}" for i in range(self.superficies_por_plato)])
        self.pista_selector = self.create_spin_box("Pista inicial:", 0, self.max_pistas_por_sup - 1)
        self.pistas_a_mostrar = self.create_spin_box("Pistas a mostrar:", 1, self.max_pistas_por_sup, min(3, self.max_pistas_por_sup))
        self.boton_mostrar = QPushButton("Mostrar Diagrama")
        self.boton_mostrar.setStyleSheet("background-color: #4a6ea9;")
        self.boton_mostrar.clicked.connect(self.mostrar_diagrama)

        control_layout.addWidget(self.plato_combo["label"])
        control_layout.addWidget(self.plato_combo["combo"])
        control_layout.addWidget(self.superficie_combo["label"])
        control_layout.addWidget(self.superficie_combo["combo"])
        control_layout.addWidget(self.pista_selector["label"])
        control_layout.addWidget(self.pista_selector["spin"])
        control_layout.addWidget(self.pistas_a_mostrar["label"])
        control_layout.addWidget(self.pistas_a_mostrar["spin"])
        control_layout.addWidget(self.boton_mostrar)

        self.control_group.setLayout(control_layout)
        self.main_layout.addWidget(self.control_group)

        # === Filtro por columna ===
        self.filtro_group = QGroupBox("Filtrar Registros")
        filtro_layout = QHBoxLayout()
        columnas = self.db.tablas['empleados']['columnas']
        self.filtro_columna = QComboBox()
        self.filtro_columna.addItems(columnas)
        self.filtro_valor = QLineEdit()
        self.filtro_valor.setPlaceholderText("Valor a buscar")
        self.boton_filtrar = QPushButton("Aplicar Filtro")
        self.boton_filtrar.clicked.connect(self.aplicar_filtro)

        filtro_layout.addWidget(QLabel("Columna:"))
        filtro_layout.addWidget(self.filtro_columna)
        filtro_layout.addWidget(QLabel("Valor:"))
        filtro_layout.addWidget(self.filtro_valor)
        filtro_layout.addWidget(self.boton_filtrar)

        self.filtro_group.setLayout(filtro_layout)
        self.main_layout.addWidget(self.filtro_group)

        # === Vista del diagrama del disco ===
        self.scene = QGraphicsScene()
        self.view = QGraphicsView(self.scene)
        self.view.setRenderHint(QPainter.RenderHint.Antialiasing)
        self.main_layout.addWidget(self.view)

        # === Tabla de resultados ===
        self.tabla_resultados = QTableWidget()
        self.tabla_resultados.setStyleSheet("background-color: #3c3f41; color: #e0e0e0;")
        self.tabla_resultados.cellClicked.connect(self.seleccionar_fila_resultado)
        self.main_layout.addWidget(self.tabla_resultados)

        # === Botón Ver en Disco ===
        self.boton_ver_en_disco = QPushButton("Ver en Disco")
        self.boton_ver_en_disco.setStyleSheet("background-color: #4a6ea9;")
        self.boton_ver_en_disco.setEnabled(False)
        self.boton_ver_en_disco.clicked.connect(self.ver_en_disco)
        self.main_layout.addWidget(self.boton_ver_en_disco)

    def aplicar_filtro(self):
        col = self.filtro_columna.currentText()
        val = self.filtro_valor.text().strip()
        if not val:
            QMessageBox.warning(self, "Valor requerido", "Ingrese un valor para filtrar.")
            return
        resultados = self.db.buscar_por_campo("empleados", col, val)
        self.mostrar_resultados(resultados)
        self.mostrar_diagrama()

    def seleccionar_fila_resultado(self, fila, columna):
        item = self.tabla_resultados.item(fila, 0)
        if not item:
            return
        self.registro_seleccionado = item.data(Qt.ItemDataRole.UserRole)
        self.boton_ver_en_disco.setEnabled(True)

    def ver_en_disco(self):
        if self.registro_seleccionado:
            registro = self.disco.obtener_registro_formateado(self.registro_seleccionado)
            if not registro or self.registro_seleccionado not in registro:
                QMessageBox.warning(self, "Error", f"No se pudo obtener información del registro con ID {self.registro_seleccionado}.")
                return
            self.registro_encontrado = registro[self.registro_seleccionado]
            self.mostrar_diagrama()

            # Obtener la primera ubicación del registro seleccionado
            if self.registro_encontrado and self.registro_encontrado["ubicaciones"]:
                primer_lba = self.registro_encontrado["ubicaciones"][0][0]
                plato, superficie, pista, sector = self.disco._lba_a_chs(primer_lba)

                # Actualizar combo boxes para ir a la ubicación real
                self.plato_combo["combo"].setCurrentIndex(plato)
                self.superficie_combo["combo"].setCurrentIndex(superficie)
                self.pista_selector["spin"].setValue(pista)

                self.mostrar_diagrama()



    def create_combo_box(self, label_text, items):
        label = QLabel(label_text)
        combo = QComboBox()
        combo.addItems(items)
        return {"label": label, "combo": combo}

    def create_spin_box(self, label_text, min_val, max_val, default_val=None):
        label = QLabel(label_text)
        spin = QSpinBox()
        spin.setRange(min_val, max_val)
        if default_val is not None:
            spin.setValue(default_val)
        return {"label": label, "spin": spin}

    def mostrar_resultados(self, lista_resultados):
        if not lista_resultados:
            self.tabla_resultados.setRowCount(0)
            return

        columnas = list(lista_resultados[0]["registro"].keys())
        self.tabla_resultados.setColumnCount(len(columnas))
        self.tabla_resultados.setHorizontalHeaderLabels(columnas)
        self.tabla_resultados.setRowCount(len(lista_resultados))

        for fila, item in enumerate(lista_resultados):
            registro = item["registro"]
            for col, nombre_columna in enumerate(columnas):
                valor = str(registro[nombre_columna])
                celda = QTableWidgetItem(valor)
                self.tabla_resultados.setItem(fila, col, celda)
            self.tabla_resultados.item(fila, 0).setData(Qt.ItemDataRole.UserRole, item["id"])

    def mostrar_diagrama(self):
        self.scene.clear()

        plato = self.plato_combo["combo"].currentIndex()
        superficie = self.superficie_combo["combo"].currentIndex()
        pista_ini = self.pista_selector["spin"].value()
        num_pistas = self.pistas_a_mostrar["spin"].value()
        pista_fin = min(pista_ini + num_pistas, self.max_pistas_por_sup)

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

        title = QGraphicsTextItem(f"Visualización: Plato {plato} - Superficie {superficie}")
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

            for sec in range(self.sectores_por_pista):
                x_sec = base_x + sec * (sector_size + 5)+20
                sectores_por_plato = self.disco.pistas_por_superficie * self.disco.sectores_por_pista * self.disco.superficies_por_plato
                lba = (
                    plato * sectores_por_plato +
                    superficie * (self.disco.pistas_por_superficie * self.disco.sectores_por_pista) +
                    p * self.disco.sectores_por_pista + sec
                )
                sector_obj = self.disco.sectores[lba]

                # === Color por estado del sector ===
                if not self.registro_encontrado:
                    # Si NO hay registro seleccionado
                    if sector_obj and sector_obj.tamano_ocupado > 0:
                        color = QColor("#a96e4a")  # Naranja - ocupado
                    else:
                        color = QColor("#5d8341")  # Verde - libre
                else:
                    # Si hay registro seleccionado (Ver en Disco activado)
                    ubicaciones = [ubi[0] for ubi in self.registro_encontrado.get("ubicaciones", [])]
                    if lba in ubicaciones:
                        if uso_lba.get(lba, 0) > 1:
                            color = QColor("#c0392b")  # Rojo - compartido
                        else:
                            color = QColor("#4a6ea9")  # Azul - exclusivo del registro
                    else:
                        if sector_obj and sector_obj.tamano_ocupado > 0:
                            color = QColor("#a96e4a")  # Naranja - ocupado (ajeno)
                        else:
                            color = QColor("#5d8341")  # Verde - libre


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


    def get_sector_tooltip(self, plato, superficie, pista, sector, sector_obj):
        tooltip = f"""
        <b>Ubicación:</b><br>
        - Plato: {plato}<br>
        - Superficie: {superficie}<br>
        - Pista: {pista}<br>
        - Sector: {sector}<br><br>
        <b>Estado:</b> {"Ocupado" if hasattr(sector_obj, 'ocupado') and sector_obj.ocupado else "Libre"}<br>
        """
        
        # Manejo seguro del tamaño
        if hasattr(sector_obj, 'tamano'):
            tooltip += f"<b>Tamaño:</b> {sector_obj.tamano} bytes<br>"
        elif hasattr(sector_obj, 'size'):
            tooltip += f"<b>Tamaño:</b> {sector_obj.size} bytes<br>"
        
        # Información de datos si está disponible
        if hasattr(sector_obj, 'datos') and sector_obj.datos:
            tooltip += f"<b>Contenido:</b> {sector_obj.datos[:30]}..."
        
        return tooltip

    def buscar_registro(self):
        reg_id = self.input_busqueda.text().strip()
        if not reg_id:
            QMessageBox.warning(self, "ID requerido", "Por favor ingrese un ID de registro para buscar.")
            return

        registro = self.disco.obtener_registro_formateado(reg_id)
        if registro is None:
            QMessageBox.warning(self, "No encontrado", 
                                f"<b>Registro no encontrado</b><br>El registro con ID {reg_id} no existe en el disco.",
                                QMessageBox.StandardButton.Ok)
            self.registro_encontrado = None
        else:
            self.registro_encontrado = registro[reg_id]
            
            # Crear mensaje con formato HTML para mejor presentación
            message = f"""
            <html>
            <body style='font-family: Arial; font-size: 12px;'>
            <h3 style='color: #4a9ae9;'>Registro {reg_id}</h3>
            <div style='margin-left: 15px;'>
            <p><b>Contenido:</b><br>{self.registro_encontrado['contenido']}</p>
            <p><b>Ubicaciones en disco:</b></p>
            <ul>
            """
            
            for (lba, ini, fin) in self.registro_encontrado["ubicaciones"]:
                plato, sup, pista, sector = self.disco._lba_a_pps(lba)
                message += f"""
                <li>Plato {plato}, Superficie {sup}, Pista {pista}, Sector {sector} 
                (bytes {ini}-{fin})</li>
                """
            
            message += """
            </ul>
            </div>
            </body>
            </html>
            """
            
            msg_box = QMessageBox()
            msg_box.setWindowTitle(f"Registro {reg_id}")
            msg_box.setTextFormat(Qt.TextFormat.RichText)
            msg_box.setText(message)
            msg_box.setIcon(QMessageBox.Icon.Information)
            msg_box.exec()
        self.mostrar_resultados([{
            "registro": json.loads(self.registro_encontrado["contenido"]),
            "id": reg_id,
            "ubicacion_disco": self.registro_encontrado["ubicaciones"]
        }])

        self.mostrar_diagrama()

    