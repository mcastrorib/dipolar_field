import os
import sys
import json
import numpy as np
import matplotlib
from matplotlib.backends.backend_qt5agg import (FigureCanvasQTAgg, NavigationToolbar2QT)
from matplotlib.figure import Figure
from PyQt5 import QtCore, QtGui, QtWidgets 
from scipy import ndimage
from imageViewer import imageViewer
from fieldViewer import fieldViewer
from formViewer import formViewer

class setupTab():
    def __init__(self, _parent, _widget):
        self.parent = _parent
        self.m_widget = _widget

        self.m_viewer = None
        self.m_form = None
        self.m_field = None

        # setting list of widgets
        self.active_widgets = []

        # setting two initial windows
        self.active_widgets.append(QtWidgets.QWidget())
        self.active_widgets.append(QtWidgets.QWidget())
        self.active_widgets.append(QtWidgets.QWidget())

        lay = QtWidgets.QGridLayout(self.m_widget)
        lay.addWidget(self.active_widgets[0], 0, 0)
        lay.addWidget(self.active_widgets[1], 0, 1)
        lay.addWidget(self.active_widgets[2], 0, 2)
        

        # lay = QtWidgets.QGridLayout(self.active_widgets[1])
        # self.active_widgets.append(QtWidgets.QWidget())
        # self.active_widgets.append(QtWidgets.QWidget())
        # lay.addWidget(self.active_widgets[2], 0, 0)
        # lay.addWidget(self.active_widgets[3], 1, 0)

        # setting image_viewer object
        self.m_viewer = imageViewer(self.parent, self.active_widgets[0])
        self.m_form = formViewer(self.parent, self.active_widgets[1])
        self.m_field = fieldViewer(self.parent, self.active_widgets[2])
        return       
