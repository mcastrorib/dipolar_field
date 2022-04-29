from PyQt5 import QtCore, QtGui, QtWidgets 

class formViewer():
    def __init__(self, _parent, _widget):
        self.parent = _parent
        self.m_widget = _widget

        self.resolution = None
        self.field = None
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
        self.imgResolutionLabel = QtWidgets.QLabel('Resolution:')
        self.imgResolutionLabel.setFont(headerFont)
        self.imgResolutionLineEdit = QtWidgets.QLineEdit('')
        self.imgResolutionLineEdit.setEnabled(True)
        self.imgUnitLabel = QtWidgets.QLabel('um/voxel')

        self.extFieldLabel = QtWidgets.QLabel('External magnetic field (T):')
        self.extFieldLabel.setFont(headerFont)
        self.fieldIntensityLabel = QtWidgets.QLabel('Intensity:')
        self.fieldIntensityLabel.setFont(headerFont)
        self.fieldIntensityLineEdit = QtWidgets.QLineEdit('')
        self.fieldIntensityLineEdit.setEnabled(True)

        self.magneticSusLabel = QtWidgets.QLabel('Magnetic susceptibilities (SI)')
        self.magneticSusLabel.setFont(headerFont)

        self.poreSusLabel = QtWidgets.QLabel('Porous phase:')
        self.poreSusLabel.setFont(headerFont)
        self.poreSusLineEdit = QtWidgets.QLineEdit('')
        self.poreSusLineEdit.setEnabled(True)

        self.matSusLabel = QtWidgets.QLabel('Matrix phase:')
        self.matSusLabel.setFont(headerFont)
        self.matSusLineEdit = QtWidgets.QLineEdit('')
        self.matSusLineEdit.setEnabled(True)

        self.runButton = QtWidgets.QPushButton('Generate')
        self.runButton.clicked.connect(self.runAnalysis)

        
        # set the layouts
        self.mainLayout = QtWidgets.QVBoxLayout(self.m_widget)        
        self.mainLayout.addWidget(self.titleLabel)

        # essentials layout
        essentialsLayout = QtWidgets.QVBoxLayout()

        # field layouts
        layoutImage = QtWidgets.QHBoxLayout()
        layoutImage.addWidget(self.imageLabel)

        layoutImgResolution = QtWidgets.QHBoxLayout()
        layoutImgResolution.addWidget(self.imgResolutionLabel)
        layoutImgResolution.addWidget(self.imgResolutionLineEdit)
        layoutImgResolution.addWidget(self.imgUnitLabel)
        layoutImgResolution.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))

        layoutExternalField = QtWidgets.QHBoxLayout()
        layoutExternalField.addWidget(self.extFieldLabel)

        layoutFieldIntensity = QtWidgets.QHBoxLayout()
        layoutFieldIntensity.addWidget(self.fieldIntensityLabel)
        layoutFieldIntensity.addWidget(self.fieldIntensityLineEdit)
        layoutFieldIntensity.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))

        layoutMagSus = QtWidgets.QHBoxLayout()
        layoutMagSus.addWidget(self.magneticSusLabel)
        
        layoutPoreSus = QtWidgets.QHBoxLayout()
        layoutPoreSus.addWidget(self.poreSusLabel)
        layoutPoreSus.addWidget(self.poreSusLineEdit) 
        layoutPoreSus.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))   
        
        layoutMatrixSus = QtWidgets.QHBoxLayout()
        layoutMatrixSus.addWidget(self.matSusLabel)
        layoutMatrixSus.addWidget(self.matSusLineEdit)
        layoutMatrixSus.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))   

        layoutRunButton = QtWidgets.QHBoxLayout()
        layoutRunButton.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))
        layoutRunButton.addWidget(self.runButton)        
        layoutRunButton.addItem(QtWidgets.QSpacerItem(15, 15, QtWidgets.QSizePolicy.MinimumExpanding))
        
        for _ in range(5):      
            essentialsLayout.addWidget(QtWidgets.QLabel(''))

        essentialsLayout.addLayout(layoutImage)
        essentialsLayout.addLayout(layoutImgResolution)
        essentialsLayout.addWidget(QtWidgets.QLabel(''))
        essentialsLayout.addLayout(layoutExternalField)
        essentialsLayout.addLayout(layoutFieldIntensity)
        essentialsLayout.addWidget(QtWidgets.QLabel(''))
        essentialsLayout.addLayout(layoutMagSus)        
        essentialsLayout.addLayout(layoutPoreSus)
        essentialsLayout.addLayout(layoutMatrixSus)
        
        for _ in range(5):      
            essentialsLayout.addWidget(QtWidgets.QLabel(''))
        
        essentialsLayout.addLayout(layoutRunButton)
        
        
        # adding layouts to main
        self.mainLayout.addLayout(essentialsLayout)
        self.mainLayout.setAlignment(QtCore.Qt.AlignTop)  
        return
        

    # @Slot()
    def runAnalysis(self):
        print('running analysis...')
        try:
            self.resolution = float(self.imgResolutionLineEdit.text())
            self.field = float(self.fieldIntensityLineEdit.text())
            self.pore_sus = float(self.poreSusLineEdit.text())
            self.matrix_sus = float(self.matSusLineEdit.text())
            self.parent.run()
        except:
            print("could not load form")
        return

