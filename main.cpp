#include "ImageViewer.h"
#include <QApplication>
#include <qgsapplication.h>
#include<QMouseEvent>
#include<QWheelEvent>
int main(int argc, char *argv[])
{
	QgsApplication a(argc, argv, true);
	QgsApplication::setPrefixPath("C:\OSGeo4W64\apps\qgis-ltr", true);
	//QgsProviderRegistry::instance("C:\OSGeo4W64\apps\qgis-ltr\plugins");   //���ò��·��
	QgsApplication::initQgis();    //��ʼ��QGISӦ��

	ImageViewer w;    //����һ�����壬������Qt
	w.show();
	return a.exec();   //����QGISӦ�õ���Ϣѭ��
}