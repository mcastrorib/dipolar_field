import os
import sys
import time
import multiprocessing as mp
import json
import numpy as np
import matplotlib
from matplotlib.backends.backend_qt5agg import (FigureCanvasQTAgg, NavigationToolbar2QT)
from matplotlib.figure import Figure
from PyQt5 import QtCore, QtGui, QtWidgets 
from pydll import *
from scipy import ndimage
from setupTab import setupTab
from imageViewer import imageViewer
from fieldViewer import fieldViewer
from formViewer import formViewer

dipolar_sum_core_dll = ez_load_dynamic_library(os.path.join(os.path.dirname(__file__), 'bin'), 'dipolar_sum_core')
dipolar_sum_core_dll.call_dipolar_sum.restype = c_void_p
dipolar_sum_core_dll.call_field_gradient.restype = c_void_p


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
        self.setWindowIcon(QtGui.QIcon(os.path.join(os.path.dirname(__file__), '..', 'icons', 'app.png'))) 
        
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

    # Method
    def run(self):
        try:
            self.m_setup_tab.m_viewer.getFullData()

            full_data = self.m_setup_tab.m_viewer.full_data
            resolution = self.m_setup_tab.m_form.resolution
            bc = self.m_setup_tab.m_form.bc
            bc_flag = None
            external_field = self.m_setup_tab.m_form.external_field
            pore_sus = self.m_setup_tab.m_form.pore_sus
            matrix_sus = self.m_setup_tab.m_form.matrix_sus
            fd_shape = self.m_setup_tab.m_viewer.full_data.shape

            self.internal_field = np.zeros(fd_shape)
            self.internal_grads = np.zeros(fd_shape)

            if(bc == 'volume'):
                bc_flag = False
            elif(bc == 'periodic'):
                bc_flag = True
        except:
            print('image not loaded')
            return

        try:
            print("Cpp::computing_field")
            ct = time.time()
            dipolar_sum_core_dll.call_dipolar_sum(
                ctypes.c_double(resolution), ctypes.c_bool(bc_flag), 
                ctypes.c_double(external_field), ctypes.c_double(pore_sus), ctypes.c_double(matrix_sus),
                ctypes.c_int32(fd_shape[2]), ctypes.c_int32(fd_shape[1]), ctypes.c_int32(fd_shape[0]), 
                ct_array_ptr(full_data), ct_array_ptr(self.internal_field))
            cpp_time = time.time() - ct
            print("Process took", cpp_time, "seconds")
        except:
            print('could not compute magnetic field')
            return

        try:
            print("Cpp::computing_grads")
            ct = time.time()
            dipolar_sum_core_dll.call_field_gradient(
                ctypes.c_double(resolution), ctypes.c_bool(bc_flag), 
                ctypes.c_int32(fd_shape[2]), ctypes.c_int32(fd_shape[1]), ctypes.c_int32(fd_shape[0]), 
                ct_array_ptr(full_data), ct_array_ptr(self.internal_field), ct_array_ptr(self.internal_grads))
            cpp_time = time.time() - ct
            print("Process took", cpp_time, "seconds")
        except:
            print("could not compute field gradient")
            return

        try:
            self.m_setup_tab.m_field.setFieldData(full_data, 1e-4*self.internal_field, 1e-2*self.internal_grads)
        except:
            print("could not show results")
            return

        return

    # Method
    def load(self, _file):
        try:
            self.m_setup_tab.m_viewer.getFullData()

            full_data = self.m_setup_tab.m_viewer.full_data
            resolution = self.m_setup_tab.m_form.resolution
            bc = self.m_setup_tab.m_form.bc
            bc_flag = None
            fd_shape = self.m_setup_tab.m_viewer.full_data.shape

            if(bc == 'volume'):
                bc_flag = False
            elif(bc == 'periodic'):
                bc_flag = True
        except:
            print('image not loaded')
            return

        try:
            print("Py::loading_field")
            ct = time.time()
            print(resolution)
            self.internal_field = np.fromfile(_file[0], dtype='float64')
            self.internal_field = self.internal_field.reshape(fd_shape)
            cpp_time = time.time() - ct
            print("Process took", cpp_time, "seconds")
        except:
            print('could not load magnetic field')
            return

        # Compute internal field gradients
        self.internal_grads = np.zeros(fd_shape)
        try:
            print("Cpp::computing_grads")
            ct = time.time()
            dipolar_sum_core_dll.call_field_gradient(
                ctypes.c_double(resolution), ctypes.c_bool(bc_flag), 
                ctypes.c_int32(fd_shape[2]), ctypes.c_int32(fd_shape[1]), ctypes.c_int32(fd_shape[0]), 
                ct_array_ptr(full_data), ct_array_ptr(self.internal_field), ct_array_ptr(self.internal_grads))
            cpp_time = time.time() - ct
            print("Process took", cpp_time, "seconds")
        except:
            print("could not compute field gradient")
            return

        try:
            self.m_setup_tab.m_field.setFieldData(full_data, self.internal_field, self.internal_grads)
        except:
            print("could not show results")
            return

        return


    # Method
    def analysis(self, data: np.ndarray, res_field: np.ndarray, resolution: float, external_field: float, pore_sus: float, matrix_sus: float):
        if(data.shape == res_field.shape and len(data.shape) == 3):
            sus_contrast = matrix_sus - pore_sus
            m_factor = (4.0/3.0) * np.pi * ((0.5*resolution)**3.0) * sus_contrast * external_field 
            dim_z = data.shape[0]
            dim_y = data.shape[1]
            dim_x = data.shape[2]

            for z in range(dim_z):
                print('k-slice ', z)
                for y in range(dim_y):
                    for x in range(dim_x):
                        if(data[z, y, x] == 0):
                            point = resolution * np.array([x, y, z])
                            res_field[z, y, x] = self.dipolarSum(data, point, m_factor, resolution)
        return

    # Method 
    def dipolarSum(self, data: np.ndarray, point: np.ndarray, m_factor: float, resolution: float) -> float:
        dipsum = 0.0
        ek = np.array([0,0,1])
        dim_z = data.shape[0]
        dim_y = data.shape[1]
        dim_x = data.shape[2]
       
        for z in range(dim_z):
            for y in range(dim_y):
                for x in range(dim_x):
                    if(data[z, y, x] > 0):
                        current = resolution * np.array([x,y,z])
                        dist = current - point
                        norm_dist = np.linalg.norm(dist)
                        unit_dist = dist / norm_dist
                        costheta = np.dot(unit_dist, ek)
                        Biz = m_factor * (3.0 * costheta**2 - 1.0) / norm_dist**3
                        dipsum += Biz * norm_dist

        return dipsum

    
    def check_arrays_sizes(v1, v2):
        if(v1.size != v2.size):
            raise ValueError