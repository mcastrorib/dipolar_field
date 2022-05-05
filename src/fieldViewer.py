import os
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
        self.buttonSave = QtWidgets.QPushButton('Save')
        self.buttonSave.setMinimumSize(QtCore.QSize(50, 40))
        self.buttonSave.setEnabled(False)
        self.buttonSave.clicked.connect(self.saveFieldData)
        self.vizBox = QtWidgets.QComboBox()
        self.vizBox.addItems(['field', 'distribution'])
        self.vizBox.setEnabled(False)     
        self.buttonViz = QtWidgets.QPushButton('View')
        self.buttonViz.setMinimumSize(QtCore.QSize(50, 40))
        self.buttonViz.setEnabled(False)
        self.buttonViz.clicked.connect(self.changeDataViz)
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
        layoutH3.addWidget(self.vizBox)
        layoutH3.addWidget(self.buttonViz)
        layoutH3.addWidget(self.buttonSave)
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
        self.field_dist = None
    
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
        ax.set_xticks([])
        ax.set_yticks([])    
        img.set_cmap('seismic') 
        self.figure.colorbar(img)
        img.set_clim(self.field_lims)
        ax.figure.canvas.draw()
        # self.buttonPlot.setEnabled(False)  
        return

    def plotDist(self):
        self.figure.clear()
        ax = self.figure.add_subplot(111)
        heights = self.field_dist[0]
        widths = self.field_dist[1][1:] - self.field_dist[1][:-1]
        l_edges = self.field_dist[1][:-1]
        img = ax.bar(l_edges, heights, widths, align='edge')
        ax.set_xlabel('Field Gradient (Gauss/cm)')
        ax.set_ylabel('Volume fraction')
        ax.figure.canvas.draw()
        self.figure.tight_layout()
        return

    # @Slot()
    def changeValue(self, _value):
        current_slice = _value
        self.loadImageData(current_slice, True)
        self.labelSliceId.setText("Slice = "+str(_value+1))      
        return

    # @Slot()
    def slideMoveUp(self):
        if(self.slideBar.value() + 1 < self.field_data.shape[0]):
            self.slideBar.setValue(self.slideBar.value()+1)
        return

    # @Slot()
    def slideMoveDown(self):
        if(self.slideBar.value() > 0):
            self.slideBar.setValue(self.slideBar.value()-1)
        return

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
        return

    def loadDistViz(self):
        self.buttonPlus.setEnabled(False) 
        self.buttonMinus.setEnabled(False) 
        self.slideBar.setMaximum(self.field_data.shape[0]-1)
        self.slideBar.setValue(0)
        self.slideBar.setEnabled(False)
        self.labelSliceId.setText("")
        self.plotDist()
        self.loaded = True
        return        

    # method
    def loadImageData(self, _slice, _updateWindow):
        self.m_data = self.field_data[_slice]
        if _updateWindow:
            self.labelDimensions.setText("[h="+str(self.m_data.shape[0])+",w="+str(self.m_data.shape[1])+"]")
            self.plotImage()
        return

    def saveFieldData(self):
        print("Image to be saved")
        return

    def changeDataViz(self):
        print("Changing dataviz")
        if(self.vizBox.currentText() == 'field'):
            self.loadFieldViz()
        elif(self.vizBox.currentText() == 'distribution'):
            self.loadDistViz()
        return
    

    # Method
    def setFieldData(self, full_img, field):
        self.field_data = field
        self.field_lims = np.array([-1, 1]) * np.abs([field.max(), field.min()]).max()
        self.vizBox.setEnabled(True)
        self.buttonViz.setEnabled(True)
        self.buttonSave.setEnabled(True)
        self.getFieldDist(full_img)
        self.loadFieldViz()
        return

    def getFieldDist(self, full_img):
        # get true points
        pore_voxels = []
        for z in range(full_img.shape[0]):
            for y in range(full_img.shape[1]):
                for x in range(full_img.shape[2]):
                    if(full_img[z,y,x] == 0):
                        pore_voxels.append(self.field_data[z,y,x])


        self.field_dist = []
        bins = 32
        abs_data = np.abs(pore_voxels)
        max_val = abs_data.max()
        min_val = abs_data.min()
        hbins = np.linspace(min_val, max_val, bins)
        dist = np.histogram(abs_data, hbins, density=False)
        self.field_dist.append(1.0 / dist[0].sum() * dist[0])
        self.field_dist.append(dist[1])
        return

        # @Slot()
    def saveFieldData(self):
        db_dir = os.path.join(os.path.dirname(__file__), '..', 'db')
        options = QtWidgets.QFileDialog.Options()
        filename = QtWidgets.QFileDialog.getSaveFileName(self.parent, 'Save Field Data', db_dir, "", options=options)
        os.mkdir(filename[0])
        my_file = filename[0].split(os.sep)[-1]

        field_dims = np.array([self.field_data.shape[2],self.m_data.shape[1],self.m_data.shape[0]],dtype='int32')

        # Save image data in RAW format
        raw_filename = my_file + '.raw'
        with open(os.path.join(filename[0], raw_filename), "bw") as file_raw:
            dimensions.tofile(file_raw)
            self.field_data.tofile(file_raw)

        # Save image data in TXT format
        txt_filename = my_file + '.txt'    
        with open(os.path.join(filename[0], txt_filename), "w") as file_txt:
            dimensions.tofile(file_txt, sep=',')
            file_txt.write('\n')
            self.field_data.tofile(file_txt, sep=',')

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

