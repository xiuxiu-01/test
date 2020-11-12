#include "ImageViewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QgsApplication::setPrefixPath("C:/Program Files/QGIS 3.14", true);
    ImageViewer w;
    w.show();
    return a.exec();
}
