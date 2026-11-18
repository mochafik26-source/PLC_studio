

import sys

from PySide6.QtWidgets import (
    QApplication,
    QWidget,
    QVBoxLayout,
    QHBoxLayout,
    QTableWidget,
    QTableWidgetItem,
    QPushButton,
    QLabel,
    QHeaderView,
    QMessageBox,
    QSizePolicy,
)
from PySide6.QtCore import Qt


# =========================
# Global widgets
# =========================

table = None
footer = None
status = None
window = None


# =========================
# Variable operations
# =========================

def update_footer():
    footer.setText(f"{table.rowCount()} variables")


def add_variable():

    row = table.rowCount()
    table.insertRow(row)

    values = [
        str(row + 1),
        "New_Variable",
        "BOOL",
        "M0.0",
        "FALSE"
    ]

    for column, value in enumerate(values):

        item = QTableWidgetItem(value)

        if column == 0:
            item.setFlags(
                item.flags() & ~Qt.ItemFlag.ItemIsEditable
            )

        table.setItem(row, column, item)

    table.editItem(table.item(row, 1))
    update_footer()


def delete_variable():

    row = table.currentRow()

    if row < 0:
        QMessageBox.warning(
            window,
            "Warning",
            "Select a variable first."
        )
        return

    table.removeRow(row)

    # Recalculate IDs
    for index in range(table.rowCount()):
        table.item(index, 0).setText(str(index + 1))

    update_footer()


def load_variables():

    variables = [
        ("1", "Motor", "BOOL", "Q0.0", "FALSE"),
        ("2", "Temperature", "INT", "IW0", "25"),
        ("3", "Counter", "INT", "MW10", "0"),
        ("4", "Pressure", "REAL", "MD20", "1.5"),
        ("5", "Emergency_Stop", "BOOL", "I0.0", "TRUE"),
    ]

    table.setRowCount(0)

    for variable in variables:

        row = table.rowCount()
        table.insertRow(row)

        for column, value in enumerate(variable):

            item = QTableWidgetItem(value)

            if column == 0:
                item.setFlags(
                    item.flags() & ~Qt.ItemFlag.ItemIsEditable
                )

            table.setItem(row, column, item)

    update_footer()


# =========================
# Application
# =========================

app = QApplication(sys.argv)

window = QWidget()
window.setWindowTitle("PLC Studio | Variables")
window.resize(1100, 650)


# =========================
# Main horizontal layout
# =========================

main_layout = QHBoxLayout(window)
main_layout.setContentsMargins(0, 0, 0, 0)
main_layout.setSpacing(0)


# =========================
# Sidebar
# =========================

sidebar = QWidget()
sidebar.setObjectName("sidebar")
sidebar.setFixedWidth(220)

sidebar.setSizePolicy(
    QSizePolicy.Policy.Fixed,
    QSizePolicy.Policy.Expanding
)

main_layout.addWidget(sidebar)


# =========================
# Main content
# =========================

content = QWidget()
content.setObjectName("content")

content_layout = QVBoxLayout(content)
content_layout.setContentsMargins(25, 25, 25, 20)
content_layout.setSpacing(15)

main_layout.addWidget(content, 1)


# =========================
# Header
# =========================

header = QHBoxLayout()

title = QLabel("PLC Variables")
title.setObjectName("title")

status = QLabel("● Offline")
status.setObjectName("status")

header.addWidget(title)
header.addStretch()
header.addWidget(status)

content_layout.addLayout(header)


# =========================
# Toolbar
# =========================

toolbar = QHBoxLayout()

add_button = QPushButton("+ Add Variable")
delete_button = QPushButton("Delete Selected")
refresh_button = QPushButton("Refresh")

add_button.clicked.connect(add_variable)
delete_button.clicked.connect(delete_variable)
refresh_button.clicked.connect(load_variables)

toolbar.addWidget(add_button)
toolbar.addWidget(delete_button)
toolbar.addWidget(refresh_button)
toolbar.addStretch()

content_layout.addLayout(toolbar)


# =========================
# Variables table
# =========================

table = QTableWidget(0, 5)

table.setHorizontalHeaderLabels([
    "ID",
    "Name",
    "Data Type",
    "Address",
    "Value"
])

table.horizontalHeader().setSectionResizeMode(
    QHeaderView.ResizeMode.Stretch
)

table.setSelectionBehavior(
    QTableWidget.SelectionBehavior.SelectRows
)

table.setSelectionMode(
    QTableWidget.SelectionMode.SingleSelection
)

table.setAlternatingRowColors(True)
table.setShowGrid(False)
table.verticalHeader().setVisible(False)

content_layout.addWidget(table, 1)


# =========================
# Footer
# =========================

footer = QLabel("0 variables")
footer.setObjectName("footer")

content_layout.addWidget(footer)


# =========================
# Styling
# =========================

window.setStyleSheet("""

    QWidget {
        background-color: #17191f;
        color: #d8dee9;
        font-family: JetBrains Mono;
        font-size: 12px;
    }

    QWidget#sidebar {
        background-color: #181818;
        border-right: 1px solid #303744;
    }

    QWidget#content {
        background-color: #17191f;
    }

    QLabel#title {
        font-size: 22px;
        font-weight: bold;
        color: #eceff4;
    }

    QLabel#status {
        color: #e06c75;
    }

    QLabel#footer {
        color: #7f8795;
    }

    QPushButton {
        background-color: #252a34;
        border: 1px solid #383f4b;
        padding: 9px 15px;
        border-radius: 5px;
    }

    QPushButton:hover {
        background-color: #343c49;
    }

    QTableWidget {
        background-color: #1e222b;
        alternate-background-color: #242a35;
        border: 1px solid #343b48;
        border-radius: 6px;
        selection-background-color: #344c68;
    }

    QHeaderView::section {
        background-color: #252b36;
        color: #9daec4;
        padding: 10px;
        border: none;
        font-weight: bold;
    }

    QTableWidget::item {
        padding: 8px;
        border-bottom: 1px solid #303744;
    }

""")


# =========================
# Run
# =========================

load_variables()

window.show()

sys.exit(app.exec())

