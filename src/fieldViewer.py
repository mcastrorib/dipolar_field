import numpy as np
import matplotlib
from matplotlib.backends.backend_qt5agg import (FigureCanvasQTAgg, NavigationToolbar2QT)
from matplotlib.figure import Figure
from PyQt5 import QtCore, QtGui, QtWidgets 
from scipy import ndimage

class fieldViewer():
    def __init__(self, _parent, _widget):
        self.parent = _parent
        self.m_widget = _widget
        self.filepaths = None
        self.loaded = False

        self.m_widget.setMinimumSize(QtCore.QSize(350, 350))

        # a figure instance to plot on
        self.figure = Figure(figsize=(3,4))

        # this is the Canvas widget that displays the 'figure'
        self.canvas = FigureCanvasQTAgg(self.figure)
        # self.figure.set_cmap('viridis') # 'gray' # magma, seismic

        # this is the Navigation widget - toolbar for image
        self.toolbar = NavigationToolbar2QT(self.canvas, self.parent)

        # these are the app widgets connected to their slot methods
        titleFont=QtGui.QFont("Arial",15)
        titleFont.setBold(True)
        self.titleLabel = QtWidgets.QLabel('Internal field')
        self.titleLabel.setFont(titleFont)
        self.titleLabel.setAlignment(QtCore.Qt.AlignCenter | QtCore.Qt.AlignVCenter)        
        self.slideBar = QtWidgets.QSlider(QtCore.Qt.Horizontal)
        self.slideBar.setMinimum(0)
        self.slideBar.setTickPosition(QtWidgets.QSlider.TicksBothSides)
        self.slideBar.setTickInterval(1)        
        self.slideBar.setSingleStep(1)
        self.slideBar.setEnabled(False)
        self.slideBar.valueChanged[int].connect(self.changeValue)
        self.buttonPlus = QtWidgets.QPushButton('+')
        self.buttonPlus.setMaximumSize(QtCore.QSize(25, 30))
        self.buttonPlus.setEnabled(False)
        self.buttonPlus.clicked.connect(self.slideMoveUp)
        self.buttonMinus = QtWidgets.QPushButton('-')
        self.buttonMinus.setMaximumSize(QtCore.QSize(25, 30))
        self.buttonMinus.setEnabled(False) 
        self.buttonMinus.clicked.connect(self.slideMoveDown)        
        self.buttonLoad = QtWidgets.QPushButton('Save')
        self.buttonLoad.setMinimumSize(QtCore.QSize(50, 40))
        self.buttonLoad.setEnabled(True)
        self.buttonLoad.clicked.connect(self.saveFieldData)     
        self.labelDimensions = QtWidgets.QLabel('[h=0,w=0]')
        self.labelSliceId = QtWidgets.QLabel('Slice = 0')
        self.labelSliceId.setAlignment(QtCore.Qt.AlignRight | QtCore.Qt.AlignVCenter)

        # set the layouts
        mainLayout = QtWidgets.QVBoxLayout(self.m_widget)
        mainLayout.addWidget(self.titleLabel)
        mainLayout.addWidget(self.toolbar)
        layoutH2 = QtWidgets.QHBoxLayout()
        layoutH3 = QtWidgets.QHBoxLayout()
        layoutH2.addWidget(self.buttonMinus)        
        layoutH2.addWidget(self.slideBar)        
        layoutH2.addWidget(self.buttonPlus)  
        layoutH3.addWidget(self.labelDimensions)
        layoutH3.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))
        layoutH3.addWidget(self.buttonLoad)
        layoutH3.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))
        layoutH3.addWidget(self.labelSliceId)
        mainLayout.addWidget(self.canvas, QtWidgets.QSizePolicy.MinimumExpanding)
        mainLayout.addLayout(layoutH2)
        mainLayout.addLayout(layoutH3)   
        mainLayout.setAlignment(QtCore.Qt.AlignTop)   

        # initialize the main image data
        self.m_data = None # numpy array
        self.m_image = None # QImage object
        self.field_data = None # numpy array
        self.field_lims = None
    
    def clear(self):
        self.__del__()

    def __del__(self):
        # remove temporary data: 
        self.m_data = None
        self.m_image = None

    # @Slot()
    def plotImage(self):
        self.figure.clear()
        ax = self.figure.add_subplot(111)
        img = ax.imshow(self.m_data)     
        img.set_cmap('seismic') 
        self.figure.colorbar(img)
        img.set_clim(self.field_lims)
        ax.figure.canvas.draw()
        # self.buttonPlot.setEnabled(False)  

    # @Slot()
    def changeValue(self, _value):
        current_slice = _value
        self.loadImageData(current_slice, True)
        self.labelSliceId.setText("Slice = "+str(_value+1))      

    # @Slot()
    def slideMoveUp(self):
        if(self.slideBar.value() + 1 < self.field_data.shape[0]):
            self.slideBar.setValue(self.slideBar.value()+1)

    # @Slot()
    def slideMoveDown(self):
        if(self.slideBar.value() > 0):
            self.slideBar.setValue(self.slideBar.value()-1)
    
    # @Slot()
    def loadFieldViz(self):
        self.loadImageData(0, True)
        self.buttonPlus.setEnabled(True) 
        self.buttonMinus.setEnabled(True) 
        self.slideBar.setMaximum(self.field_data.shape[0]-1)
        self.slideBar.setValue(0)
        self.slideBar.setEnabled(True)
        self.labelSliceId.setText("Slice = 1")
        self.loaded = True
    
    # method
    def loadImageData(self, _slice, _updateWindow):
        self.m_data = self.field_data[_slice]
        if _updateWindow:
            self.labelDimensions.setText("[h="+str(self.m_data.shape[0])+",w="+str(self.m_data.shape[1])+"]")
            self.plotImage()

    def saveFieldData(self):
        print("Image to be saved")
        return
    

    # Method
    def setFieldData(self, field):
        self.field_data = field
        self.field_lims = np.array([-1, 1]) * np.abs([field.max(), field.min()]).max()
        self.loadFieldViz()
        return

# This function was adapted from (https://github.com/Entscheider/SeamEater/blob/master/gui/QtTool.py)
# Project: SeamEater; Author: Entscheider; File: QtTool.py; GNU General Public License v3.0 
# Original function name: qimage2numpy(qimg)
# We consider just 8 bits images and convert to single depth:
def convertQImageToNumpy(_qimg):
    h = _qimg.height()
    w = _qimg.width()
    ow = _qimg.bytesPerLine() * 8 // _qimg.depth()
    d = 0
    if _qimg.format() in (QtGui.QImage.Format_ARGB32_Premultiplied,
                        QtGui.QImage.Format_ARGB32,
                        QtGui.QImage.Format_RGB32):
        d = 4 # formats: 6, 5, 4.
    elif _qimg.format() in (QtGui.QImage.Format_Indexed8,
                            QtGui.QImage.Format_Grayscale8):
        d = 1 # formats: 3, 24.
    else:
        raise ValueError(".ERROR: Unsupported QImage format!")
    buf = _qimg.bits().asstring(_qimg.byteCount())
    res = np.frombuffer(buf, 'uint8')
    res = res.reshape((h,ow,d)).copy()
    if w != ow:
        res = res[:,:w] 
    if d >= 3:
        res = res[:,:,0].copy()
    else:
        res = res[:,:,0] 
    return res

