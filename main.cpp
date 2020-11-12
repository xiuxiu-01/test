#include "ImageViewer.h"
#include <QApplication>
#include <qgsapplication.h>
#include<QMouseEvent>
#include<QWheelEvent>
int main(int argc, char *argv[])
{
	QgsApplication a(argc, argv, true);
	QgsApplication::setPrefixPath("C:\OSGeo4W64\apps\qgis-ltr", true);
	//QgsProviderRegistry::instance("C:\OSGeo4W64\apps\qgis-ltr\plugins");   //设置插件路径
	QgsApplication::initQgis();    //初始化QGIS应用

	ImageViewer w;    //创建一个窗体，类似于Qt
	w.show();
	return a.exec();   //进入QGIS应用的消息循环
}