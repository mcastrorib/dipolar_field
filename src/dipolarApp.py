import os
import sys
import json
import numpy as np
import matplotlib
from matplotlib.backends.backend_qt5agg import (FigureCanvasQTAgg, NavigationToolbar2QT)
from matplotlib.figure import Figure
from PyQt5 import QtCore, QtGui, QtWidgets 
from scipy import ndimage
from setupTab import setupTab
from imageViewer import imageViewer
from fieldViewer import fieldViewer
from formViewer import formViewer

# Inherit from QDialog
class dipolarApp(QtWidgets.QMainWindow):
    # Override the class constructor
    def __init__(self, app_name='', parent=None):
        super(dipolarApp, self).__init__(parent)

        # Set app major tabs objects as None
        self.m_setup_tab = None

        # Set app title 
        self.setAttribute(QtCore.Qt.WA_DeleteOnClose)
        self.setWindowTitle(app_name) 
        self.setWindowIcon(QtGui.QIcon('icons/app.png')) 
        
        # Set geometry and minimum size
        self.setGeometry(100, 100, 1300, 600) 
        self.setMinimumSize(QtCore.QSize(1200, 500))
        
        # Set app main QWidget 
        self.central_widget = QtWidgets.QWidget()
        self.setCentralWidget(self.central_widget)
        self.m_setup_tab = setupTab(self, self.central_widget)   
        
        # Set file menu  
        self.file_menu = QtWidgets.QMenu('&File', self)
        self.file_menu.addAction('&Open image', self.openImage,
                                QtCore.Qt.CTRL + QtCore.Qt.Key_O)
        self.file_menu.addAction('&Quit', self.fileQuit,
                                 QtCore.Qt.CTRL + QtCore.Qt.Key_Q)
        self.menuBar().addMenu(self.file_menu)

        # Set help menu
        self.help_menu = QtWidgets.QMenu('&Help', self)
        self.help_menu.addAction("About...", self.aboutDlg, QtCore.Qt.CTRL + QtCore.Qt.Key_H)
        self.menuBar().addSeparator()
        self.menuBar().addMenu(self.help_menu)
        return

    # @Slot()
    def closeTab(self, index):
        currentQWidget = self.open_tabs[index]
        currentQWidget.deleteLater()
        self.tabs.removeTab(index)
        self.open_tabs.pop(index)
        name = self.open_tabs_names[index]
        self.open_tabs_names.pop(index)
        
        if(name == "setup"):
            self.m_setup_tab = None
        
        return
   
    
    # @Slot()
    def fileQuit(self):
        if (self.m_setup_tab != None and self.m_setup_tab.m_viewer != None and len(self.m_setup_tab.m_viewer.m_map) > 0):
            self.m_setup_tab.m_viewer.removeTempImages()
        self.close()
        return

    # @Slot()
    def aboutDlg(self):
        sm = """DipolarApp\nVersion 1.0.0\n2020\nLicense GPL 3.0\n\nThe authors and the involved Institutions are not responsible for the use or bad use of the program and their results. The authors have no legal dulty or responsability for any person or company for the direct or indirect damage caused resulting from the use of any information or usage of the program available here. The user is responsible for all and any conclusion made with the program. There is no warranty for the program use. """
        msg = QtWidgets.QMessageBox()
        msg.setText(sm)
        msg.setWindowTitle("About")
        msg.exec_()
        return
    
    # @Slot()
    def openImage(self):
        if(len(self.m_setup_tab.m_viewer.m_map) > 0):
            self.m_setup_tab.m_viewer.clear()        
        self.m_setup_tab.m_viewer.openImage()
        return
    