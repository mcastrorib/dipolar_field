from PyQt5 import QtCore, QtGui, QtWidgets 
import numpy as np

class formViewer():
    def __init__(self, _parent, _widget):
        self.parent = _parent
        self.m_widget = _widget

        self.pore_color = 0
        self.resolution = None
        self.bc = None
        self.external_field = None
        self.pore_sus = None
        self.matrix_sus = None

        self.m_widget.setGeometry(0,0,100,100)
        self.m_widget.setMinimumSize(QtCore.QSize(100, 100))

        titleFont=QtGui.QFont("Arial",15)
        titleFont.setBold(True)
        headerFont = QtGui.QFont("Arial",12)
        headerFont.setBold(False)

        # these are the app widgets connected to their slot methods
        self.titleLabel = QtWidgets.QLabel('Dipolar Field Setup')
        self.titleLabel.setFont(titleFont)
        self.titleLabel.setAlignment(QtCore.Qt.AlignCenter | QtCore.Qt.AlignVCenter)
        
        # set RW config widgets
        self.imageLabel = QtWidgets.QLabel('Image properties:')
        self.imageLabel.setFont(headerFont)
        self.imgPoreColorLabel = QtWidgets.QLabel('Pore color:')
        self.imgPoreColorLabel.setFont(headerFont)
        self.imgPoreColorLineEdit = QtWidgets.QLineEdit('0')
        self.imgResolutionLabel = QtWidgets.QLabel('Resolution:')
        self.imgResolutionLabel.setFont(headerFont)
        self.imgResolutionLineEdit = QtWidgets.QLineEdit('1.0')
        self.imgResolutionLineEdit.setEnabled(True)
        self.imgUnitBox = QtWidgets.QComboBox()
        self.imgUnitBox.addItems(['um/voxel', 'cm/voxel'])

        self.imgBCLabel = QtWidgets.QLabel('Boundaries:')
        self.imgBCLabel.setFont(headerFont)
        self.imgBCBox = QtWidgets.QComboBox()
        self.imgBCBox.addItems(['periodic', 'volume'])

        self.extFieldLabel = QtWidgets.QLabel('External magnetic field:')
        self.extFieldLabel.setFont(headerFont)
        self.fieldIntensityLabel = QtWidgets.QLabel('Intensity:')
        self.fieldIntensityLabel.setFont(headerFont)
        self.fieldIntensityLineEdit = QtWidgets.QLineEdit('250.0')
        self.fieldIntensityLineEdit.setEnabled(True)        
        self.fieldIntensityUnitBox = QtWidgets.QComboBox()
        self.fieldIntensityUnitBox.addItems(['Tesla', 'Gauss'])

        self.magneticSusLabel = QtWidgets.QLabel('Magnetic susceptibilities (SI)')
        self.magneticSusLabel.setFont(headerFont)

        self.poreSusLabel = QtWidgets.QLabel('Porous phase:')
        self.poreSusLabel.setFont(headerFont)
        self.poreSusLineEdit = QtWidgets.QLineEdit('-7e-7')
        self.poreSusLineEdit.setEnabled(True)
        self.poreSusUnitBox = QtWidgets.QComboBox()
        self.poreSusUnitBox.addItems(['SI', r'emu/cm³'])        

        self.matSusLabel = QtWidgets.QLabel('Matrix phase:')
        self.matSusLabel.setFont(headerFont)
        self.matSusLineEdit = QtWidgets.QLineEdit('5e-5')
        self.matSusLineEdit.setEnabled(True)
        self.matSusUnitBox = QtWidgets.QComboBox()
        self.matSusUnitBox.addItems(['SI', r'emu/cm³']) 

        self.parallelLabel = QtWidgets.QLabel('Parallelization:')
        self.parallelLabel.setFont(headerFont)
        self.parallelBox = QtWidgets.QComboBox()
        self.parallelBox.addItems(['none', 'cpu', 'gpu']) 
        self.parallel_map = {
            'none': 0,
            'cpu': 1,
            'gpu': 2
        }      

        self.runButton = QtWidgets.QPushButton('Generate')
        self.runButton.clicked.connect(self.runAnalysis)

        self.loadButton = QtWidgets.QPushButton('Load')
        self.loadButton.clicked.connect(self.loadField)

        self.dllButton = QtWidgets.QPushButton('DLL')
        self.dllButton.clicked.connect(self.debugDLL)

        
        # set the layouts
        self.mainLayout = QtWidgets.QVBoxLayout(self.m_widget)        
        self.mainLayout.addWidget(self.titleLabel)

        # essentials layout
        essentialsLayout = QtWidgets.QVBoxLayout()

        # field layouts
        layoutImage = QtWidgets.QHBoxLayout()
        layoutImage.addWidget(self.imageLabel)


        layoutImgPoreColor = QtWidgets.QHBoxLayout()
        layoutImgPoreColor.addWidget(self.imgPoreColorLabel)
        layoutImgPoreColor.addWidget(self.imgPoreColorLineEdit)
        layoutImgPoreColor.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))

        layoutImgResolution = QtWidgets.QHBoxLayout()
        layoutImgResolution.addWidget(self.imgResolutionLabel)
        layoutImgResolution.addWidget(self.imgResolutionLineEdit)
        layoutImgResolution.addWidget(self.imgUnitBox)
        layoutImgResolution.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))

        layoutImgBC = QtWidgets.QHBoxLayout()
        layoutImgBC.addWidget(self.imgBCLabel)
        layoutImgBC.addWidget(self.imgBCBox)
        layoutImgBC.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))

        layoutExternalField = QtWidgets.QHBoxLayout()
        layoutExternalField.addWidget(self.extFieldLabel)

        layoutFieldIntensity = QtWidgets.QHBoxLayout()
        layoutFieldIntensity.addWidget(self.fieldIntensityLabel)
        layoutFieldIntensity.addWidget(self.fieldIntensityLineEdit)   
        layoutFieldIntensity.addWidget(self.fieldIntensityUnitBox)
        layoutFieldIntensity.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))

        layoutMagSus = QtWidgets.QHBoxLayout()
        layoutMagSus.addWidget(self.magneticSusLabel)
        
        layoutPoreSus = QtWidgets.QHBoxLayout()
        layoutPoreSus.addWidget(self.poreSusLabel)
        layoutPoreSus.addWidget(self.poreSusLineEdit)   
        layoutPoreSus.addWidget(self.poreSusUnitBox) 
        layoutPoreSus.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))   
        
        layoutMatrixSus = QtWidgets.QHBoxLayout()
        layoutMatrixSus.addWidget(self.matSusLabel)
        layoutMatrixSus.addWidget(self.matSusLineEdit)
        layoutMatrixSus.addWidget(self.matSusUnitBox) 
        layoutMatrixSus.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))  

        layoutParallelization = QtWidgets.QHBoxLayout()
        layoutParallelization.addWidget(self.parallelLabel)
        layoutParallelization.addWidget(self.parallelBox)
        layoutParallelization.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))


        layoutButtons = QtWidgets.QHBoxLayout()
        layoutButtons.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))
        layoutButtons.addWidget(self.runButton)        
        layoutButtons.addWidget(self.loadButton)   
        layoutButtons.addWidget(self.dllButton)        
        layoutButtons.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))
        
        for _ in range(5):      
            essentialsLayout.addWidget(QtWidgets.QLabel(''))


        essentialsLayout.addWidget(QtWidgets.QLabel(''))
        essentialsLayout.addLayout(layoutExternalField)
        essentialsLayout.addLayout(layoutFieldIntensity)
        essentialsLayout.addLayout(layoutMagSus)        
        essentialsLayout.addLayout(layoutPoreSus)
        essentialsLayout.addLayout(layoutMatrixSus)
        essentialsLayout.addWidget(QtWidgets.QLabel(''))
        essentialsLayout.addLayout(layoutImage)
        essentialsLayout.addLayout(layoutImgPoreColor)
        essentialsLayout.addLayout(layoutImgResolution)
        essentialsLayout.addLayout(layoutImgBC)   
        essentialsLayout.addWidget(QtWidgets.QLabel(''))
        essentialsLayout.addLayout(layoutParallelization)             
        essentialsLayout.addWidget(QtWidgets.QLabel(''))
        essentialsLayout.addLayout(layoutButtons)
        
        
        # adding layouts to main
        self.mainLayout.addLayout(essentialsLayout)
        self.mainLayout.setAlignment(QtCore.Qt.AlignTop)  
        return
        

    # @Slot()
    def runAnalysis(self):
        self.runButton.setEnabled(False)
        print('running analysis...')
        try:
            mode = self.parallelBox.currentText()
            acc = self.parallel_map[mode]
            self.readFormData()
            self.parent.run(acc)
        except:
            print("could not load form")

        self.runButton.setEnabled(True)
        return

    def loadField(self):
        self.runButton.setEnabled(False)
        print('running analysis...')
        try:
            self.readFormData(True)
            file = self.openFieldFile()
            if(file):
                self.parent.load(file)
            else:
                print('could not open file')
        except:
            print("could not open file/form")

        self.runButton.setEnabled(True)
        return

    def debugDLL(self):
        try:
            self.parent.debug()
        except:
            print('could not debug dll')
        return

    def readFormData(self, _load=False):
        # Convert to gaussian units
        self.pore_color = int(self.imgPoreColorLineEdit.text())
        self.resolution = float(self.imgResolutionLineEdit.text())
        if(self.imgUnitBox.currentText() == 'um/voxel'):
            if(_load):
                self.resolution *= 1.0e-6
            else:
                self.resolution *= 1.0e-4


        self.bc = self.imgBCBox.currentText()
        
        self.external_field = float(self.fieldIntensityLineEdit.text())
        if(self.fieldIntensityUnitBox.currentText() == 'Tesla'):
            self.external_field *= 1.0e4

        self.pore_sus = float(self.poreSusLineEdit.text())
        if(self.poreSusUnitBox.currentText() == 'SI'):
            self.pore_sus /= 4.0*np.pi 

        self.matrix_sus = float(self.matSusLineEdit.text())
        if(self.matSusUnitBox.currentText() == 'SI'):
            self.matrix_sus /= 4.0*np.pi

        return

    def openFieldFile(self):
        options = QtWidgets.QFileDialog.Options()
        file, _ = QtWidgets.QFileDialog.getOpenFileNames(self.parent, "Open internal field data", "","Binary files (*.bin)", options=options)            
        return file

