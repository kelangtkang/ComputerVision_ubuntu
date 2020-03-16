#include "maincamera.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainCamera cuaSo;
    cuaSo.setWindowTitle("Computer Vision");
    cuaSo.setWindowIcon(QIcon(":/icon/ComputerVision5.png"));
    cuaSo.resize(640, 480);
    cuaSo.show();

    return app.exec();
}
