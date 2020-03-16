#include "maincamera.h"

Q_DECLARE_METATYPE(QCameraInfo)

MainCamera::MainCamera()
{
    qRegisterMetaType < cv::Mat > ("cv::Mat");

    //Dua doi tuong vao luong xu ly rieng
    m_colorTracking = new ColorTracking;
    m_colorTracking -> createGUI();

    m_objectDetection = new ObjectDetection;
    m_objectDetection -> createGUI();

    m_processMain = new ProcessCamera();
    m_processMain -> start();

    QObject::connect(m_colorTracking, SIGNAL(signalImageToMainCamera(QImage)), this, SLOT(createCamera(QImage)));
    QObject::connect(m_colorTracking, SIGNAL(signalProcessToTracking()), m_processMain, SLOT(slotOffTrackingToProcess()));
    QObject::connect(m_colorTracking, SIGNAL(signalFrameTrackingToProcess(cv::Mat)), m_processMain, SLOT(slotFrameTrackingToProcess(cv::Mat)));

    QObject::connect(m_processMain, SIGNAL(sendFrameProcessed(QImage)), this, SLOT(createCamera(QImage)));
    QObject::connect(m_processMain, SIGNAL(signalFrameToTracking(cv::Mat)), m_colorTracking, SLOT(run(cv::Mat)));
    QObject::connect(m_processMain, SIGNAL(signalFrameToDetection(cv::Mat)), m_objectDetection, SLOT(run(cv::Mat)));

    //Tao GUI ung dung
    QWidget *window = new QWidget;
    setCentralWidget(window); // Importance
    createThongTin(); //Tao label thong tin
    createChucNang(); //Tao nut chuc nang
    m_labelCamera = new CameraLabel(); //Tao Label chua anh
    QObject::connect(m_labelCamera, SIGNAL(signalPosionCusor(int,int, int, int, int)), this, SLOT(slotSetValueInfo(int, int, int, int, int)));

    //Sap xep bo cuc
    QVBoxLayout *vLayoutChucNang = new QVBoxLayout;
    vLayoutChucNang -> addWidget(m_groupThongTin);
    vLayoutChucNang -> addWidget(m_groupEffect);
    vLayoutChucNang -> addWidget(m_groupDetect);
    vLayoutChucNang -> addWidget(m_onOffCamera);

    QLabel *labelTopCamera = new QLabel; // Lam day 2 vien layout camera tranh error coordinate
    QLabel *labelUnderCamera = new QLabel; // Lam day 2 vien layout camera tranh error coordinate
    QVBoxLayout *vLayoutCamera = new QVBoxLayout;
    vLayoutCamera -> addWidget(labelTopCamera);
    vLayoutCamera -> addWidget(m_labelCamera);
    vLayoutCamera -> addWidget(labelUnderCamera);

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout -> addLayout(vLayoutCamera, Qt::AlignLeft);
    hBoxLayout -> addLayout(vLayoutChucNang);
    window -> setLayout(hBoxLayout);
    this -> setCentralWidget(window); // Importance

    createMenu();
}

MainCamera::~MainCamera()
{
    m_processMain -> stop(); //Stop while loop
    m_processMain -> quit();
    m_processMain -> wait(); //Wait thread project stop
//    m_colorTracking -> ~ColorTracking();

    if (m_processMain -> isFinished())
    { qDebug() << "Thread had finished"; }
    else
    { qDebug() << "Thread not finish"; }

    delete m_colorTracking;
    delete m_objectDetection;
    delete m_processMain;
}

void MainCamera::createCamera(QImage frame)
{
    m_labelCamera -> setPixmap(QPixmap::fromImage(frame));
    m_labelCamera -> setStyleSheet("border: 1px solid black");
    this -> adjustSize();
//    m_labelCamera -> setScaledContents(true);
}

void MainCamera::trackingColor()
{
//    m_colorTracking -> resetValue
    m_colorTracking -> show();
}

void MainCamera::objectDetection()
{
    m_objectDetection -> show();
}

void MainCamera::slotSetValueInfo(int x, int y, int r, int g, int b)
{
    m_xValue -> setNum(x);
    m_yValue -> setNum(y);
    m_rValue -> setNum(r);
    m_gValue -> setNum(g);
    m_bValue -> setNum(b);
}

void MainCamera::changeNamePushButton()
{
    if(!m_onOffCamera -> text().compare("STOP"))
    {
        m_onOffCamera -> setText("START");
        m_onOffCamera -> setStyleSheet("background-color: rgb(0, 176, 80);");
        QObject::disconnect(m_processMain, SIGNAL(sendFrameProcessed(QImage)), this, SLOT(createCamera(QImage)));
    }
    else
    {
        m_onOffCamera -> setText("STOP");
        m_onOffCamera -> setStyleSheet("background-color: rgb(255, 0, 0);");
        QObject::connect(m_processMain, SIGNAL(sendFrameProcessed(QImage)), this, SLOT(createCamera(QImage)));
    }
}

void MainCamera::createChucNang() //Tao nut chuc nang
{
    m_onOffCamera = new QPushButton("STOP", this);
    m_onOffCamera -> setFont(QFont("Calibri", 12, QFont::Bold));
    m_onOffCamera -> setStyleSheet("background-color: rgb(255, 0, 0);");
    m_onOffCamera -> setCursor(Qt::PointingHandCursor);

    QPushButton *pushOriginalCamera = new QPushButton();
    QPushButton *pushGrey = new QPushButton();
    QPushButton *pushHSV = new QPushButton();
    QPushButton *pushThreshold = new QPushButton();
    QPushButton *pushObjectDetection = new QPushButton();
    QPushButton *pushTrackingColor = new QPushButton();
    QPushButton *pushAmBan = new QPushButton();
    QPushButton *pushNhiPhan = new QPushButton();

    pushOriginalCamera = createButton("Original");
    pushGrey = createButton("Grey");
    pushHSV = createButton("HSV");
    pushThreshold = createButton("Threshold");
    pushObjectDetection = createButton("Detecting");
    pushTrackingColor = createButton("Tracking");
    pushAmBan = createButton("Âm bản");
    pushNhiPhan = createButton("Nhị phân");

    QGridLayout *gridEffect = new QGridLayout;
    gridEffect -> addWidget(pushOriginalCamera, 0, 0);
    gridEffect -> addWidget(pushHSV, 0, 1);
    gridEffect -> addWidget(pushGrey, 1, 0);
    gridEffect -> addWidget(pushAmBan, 1, 1);
    gridEffect -> addWidget(pushThreshold, 2, 0);
    gridEffect -> addWidget(pushNhiPhan, 2, 1);

    QGridLayout *gridDetect = new QGridLayout;
    gridDetect -> addWidget(pushObjectDetection, 0, 0);
    gridDetect -> addWidget(pushTrackingColor, 0, 1);

    m_groupEffect = new QGroupBox("Bộ lọc");
    m_groupEffect -> setLayout(gridEffect);

    m_groupDetect = new QGroupBox("Nhận diện");
    m_groupDetect -> setLayout(gridDetect);

    //Connect PushButton
    QObject::connect(m_onOffCamera, SIGNAL(clicked()), this, SLOT(changeNamePushButton()));
    QObject::connect(pushOriginalCamera, SIGNAL(clicked()), m_processMain, SLOT(setOriginal()));
    QObject::connect(pushGrey, SIGNAL(clicked()), m_processMain, SLOT(setGrey()));
    QObject::connect(pushHSV, SIGNAL(clicked()), m_processMain, SLOT(setHSV()));
    QObject::connect(pushAmBan, SIGNAL(clicked()), m_processMain, SLOT(setNegative()));

//    QObject::connect(pushTrackingColor, SIGNAL(clicked()), m_colorTracking, SLOT(slotResetValue()));

    QObject::connect(pushTrackingColor, SIGNAL(clicked()), this, SLOT(trackingColor()));
    QObject::connect(pushTrackingColor, SIGNAL(clicked()), m_processMain, SLOT(slotOnTrackingToProcess()));
    QObject::connect(pushObjectDetection, SIGNAL(clicked()), this, SLOT(objectDetection()));
}

void MainCamera::createThongTin() //Tao Label chua thong tin
{
    QFont fontThongTin("Calibri", 12);
    m_xValue = new QLabel();
    m_xValue -> setFont(fontThongTin);
    m_yValue = new QLabel();
    m_yValue -> setFont(fontThongTin);
    m_rValue = new QLabel();
    m_rValue -> setFont(fontThongTin);
    m_rValue -> setStyleSheet("QLabel {color: red;}");
    m_gValue = new QLabel();
    m_gValue -> setFont(fontThongTin);
    m_gValue -> setStyleSheet("QLabel {color: green;}");
    m_bValue = new QLabel();
    m_bValue -> setFont(fontThongTin);
    m_bValue -> setStyleSheet("QLabel {color: blue;}");

    QLabel *x = new QLabel("X         :", this);
    x -> setFont(fontThongTin);
    QLabel *y = new QLabel("Y         :", this);
    y -> setFont(fontThongTin);
    QLabel *r = new QLabel("R         :", this);
    r -> setFont(fontThongTin);
    r -> setStyleSheet("QLabel {color: red;}");
    QLabel *g = new QLabel("G         :", this);
    g -> setFont(fontThongTin);
    g -> setStyleSheet("QLabel {color: green;}");
    QLabel *b = new QLabel("B         :", this);
    b -> setFont(fontThongTin);
    b -> setStyleSheet("QLabel {color: blue;}");

    QGridLayout *gridToaDo = new QGridLayout;
    gridToaDo -> addWidget(x, 0, 0);
    gridToaDo -> addWidget(y, 1, 0);
    gridToaDo -> addWidget(m_xValue, 0, 1);
    gridToaDo -> addWidget(m_yValue, 1, 1);
    QGroupBox *groupToaDo = new QGroupBox("Tọa độ");
    groupToaDo -> setLayout(gridToaDo);

    QGridLayout *gridRGBValue = new QGridLayout;
    gridRGBValue -> addWidget(r, 0, 0);
    gridRGBValue -> addWidget(g, 1, 0);
    gridRGBValue -> addWidget(b, 2, 0);
    gridRGBValue -> addWidget(m_rValue, 0, 1);
    gridRGBValue -> addWidget(m_gValue, 1, 1);
    gridRGBValue -> addWidget(m_bValue, 2, 1);
    QGroupBox *groupRGBValue = new QGroupBox("RGB Value");
    groupRGBValue -> setLayout(gridRGBValue);

    QVBoxLayout *vBoxThongTin = new QVBoxLayout;
    vBoxThongTin -> addWidget(groupToaDo);
    vBoxThongTin -> addWidget(groupRGBValue);
    m_groupThongTin = new QGroupBox("Thông tin");
    m_groupThongTin -> setLayout(vBoxThongTin);
}

void MainCamera::createMenu() //Tao MenuBar
{
    //Tao Action
    m_settings = new QAction("Settings", this);
    m_settings -> setFont(QFont("Calibri", 12));

    m_exit = new QAction("Exit", this);
    m_exit -> setFont(QFont("Calibri", 12));

    //Tao group action resolution
    QActionGroup *resolutionGroup = new QActionGroup(this);
    resolutionGroup -> setExclusive(true);

    m_1080x720 = new QAction("1280 x 720", resolutionGroup);
    m_1080x720 -> setFont(QFont("Calibri", 12));
    m_1080x720 -> setCheckable(true);
    m_1080x720 -> setData(1);

    m_800x600 = new QAction("800 x 600", resolutionGroup);
    m_800x600 -> setFont(QFont("Calibri", 12));
    m_800x600 -> setCheckable(true);
    m_800x600 -> setData(3);

    m_720x480 = new QAction("720 x 480", resolutionGroup);
    m_720x480 -> setFont(QFont("Calibri", 12));
    m_720x480 -> setCheckable(true);
    m_720x480 -> setData(2);

    m_640x480 = new QAction("640 x 480", resolutionGroup);
    m_640x480 -> setFont(QFont("Calibri", 12));
    m_640x480 -> setCheckable(true);
    m_640x480 -> setChecked(true);
    m_640x480 -> setData(4);

    m_320x240 = new QAction("320 x 240", resolutionGroup);
    m_320x240 -> setFont(QFont("Calibri", 12));
    m_320x240 -> setCheckable(true);
    m_320x240 -> setData(5);

    //Tao Menu bar
    QMenu *file = new QMenu;
    file = menuBar() -> addMenu("&File");
    file -> addAction(m_settings);
    file -> addAction(m_exit);

    //Tao Menu bar chon camera devices
    QMenu *devices = new QMenu;
    devices = menuBar() -> addMenu("&Devices");
    QActionGroup *videoDevicesGroup = new QActionGroup(this);
    videoDevicesGroup -> setExclusive(true);

    //Tao Menu bar resolution
    QMenu *resolution = new QMenu;
    resolution = menuBar() -> addMenu("&Resolution");

    resolution -> addAction(m_1080x720);
    resolution -> addAction(m_800x600);
    resolution -> addAction(m_720x480);
    resolution -> addAction(m_640x480);
    resolution -> addAction(m_320x240);

//    QMenu *menuColorDialog = new QMenu;
//    QColorDialog *colorDialog = new QColorDialog;
//    menuColorDialog -> addMenu("&Dialog");
//    menuColorDialog -> addAction(colorDialog);

    int i = 0;
    const QList <QCameraInfo> listOfCameras = QCameraInfo::availableCameras(); //Danh sách camera đầu vào (Qstring)
    foreach (const QCameraInfo &cameraInfo, listOfCameras) //Vong lap for-each tao danh sach camera
    {
        QAction *camera_i = new QAction(cameraInfo.description(), videoDevicesGroup);
        camera_i -> setCheckable(true);
        camera_i -> setData(i);
        i = i + 2; //i++ vs Window and (i + 2) vs Ubuntu
        if (cameraInfo == QCameraInfo::defaultCamera())
            camera_i -> setChecked(true);
        devices -> addAction(camera_i);
    }

    QObject::connect(this, SIGNAL(signalDeviceID(int)), m_processMain, SLOT(slotChangeCamera(int)));
    QObject::connect(videoDevicesGroup, SIGNAL(triggered(QAction*)), SLOT(updateCameraDevice(QAction*)));
    QObject::connect(resolutionGroup, SIGNAL(triggered(QAction*)), SLOT(slotSetResolution(QAction*)));
    QObject::connect(this, SIGNAL(signalResolution(int)), m_processMain, SLOT(slotSetResolution(int)));
    QObject::connect(m_exit, SIGNAL(triggered()), qApp, SLOT(quit()));
    QObject::connect(this, SIGNAL(signalStartCamera()), m_processMain, SLOT(slotStartCamera()));
}

void MainCamera::updateCameraDevice(QAction *action)
{
    int deviceID = (qvariant_cast <int> (action -> data()));
    emit signalDeviceID(deviceID);
    emit signalStartCamera();
    m_processMain -> start();
}

void MainCamera::slotSetResolution(QAction *action)
{
    int resolution = (qvariant_cast <int> (action -> data()));
    emit signalResolution(resolution);
    emit signalStartCamera();
    m_processMain -> start();
}

QPushButton* MainCamera::createButton(const QString nameButton)
{
    QPushButton *button = new QPushButton(nameButton, this);
    button -> setFixedSize(QSize(75, 22));
    button -> setFont(QFont("Calibri", 10));
    button -> setCursor(Qt::PointingHandCursor);
    return button;
}
