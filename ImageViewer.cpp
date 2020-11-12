#include "ImageViewer.h"
#include<string>
#include<iostream>
#include <qgsmapcanvas.h>
#include <QFileDialog>
#include <QObject>
#include <QDialog>
#include <qgsrasterlayer.h>
#include <QMessageBox>
#include <string.h>
#include "qgsmaptoolidentifyfeature.h"
#include <qgsunittypes.h>
#include <qgslinesymbollayer.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qgsgraduatedsymbolrenderer.h>
#include <qgsrendererrange.h>
#include <qgsrulebasedrenderer.h>
#include <qfiledialog.h>
#include <qgsrenderer.h>
#include <qgstextrenderer.h>
#include <qgsvectorlayer.h>
#include <qgspallabeling.h>
#include <qgssymbol.h>
#include <qgscategorizedsymbolrenderer.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include <qgssymbollayer.h>
#include <qgsvectorlayerlabeling.h>
#include <qgsrenderer.h>
#include <qgsmaplayerstore.h>
#include <qgsmaplayer.h>

#include <qgssinglesymbolrenderer.h>
#include <qgsmarkersymbollayer.h>
#define QGIS_DEV_H
using namespace std;
ImageViewer::ImageViewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->resize(1000, 800);

	// create the menus and then add the actions to them.
	fileMenu = this->menuBar()->addMenu("File");
	openFileAction = new QAction("Open", this);
	
	fileMenu->addAction(openFileAction);
	//this->statusBar();
	// initialize the map canvas
	mapCanvas = new QgsMapCanvas();
	this->setCentralWidget(mapCanvas);
	mapCanvas->setCanvasColor(QColor(255, 255, 255));
	mapCanvas->setVisible(true);
	mapCanvas->enableAntiAliasing(true);
	//QgsMapToolIdentifyFeature * m_identify = new QgsMapToolIdentifyFeature(mapCanvas, m_pLayer);
	this->connect(openFileAction, SIGNAL(triggered(bool)), this, SLOT(on_openFileAction_triggered()));
	this->connect(mapCanvas, SIGNAL(xyCoordinates(QgsPointXY)),
		this, SLOT(Show_Coordinate(QgsPointXY)));
	this->connect(mapCanvas, SIGNAL(scaleChanged(double)),
		this, SLOT(Show_Coordinate(QgsPointXY)));

}

void ImageViewer::on_openFileAction_triggered() {
	QList<QgsMapLayer *> layers;
	addChartlayers();
}


void ImageViewer::addChartlayers()
{
	mapCanvas->freeze(true);//冻结或解冻地图画布对象,frozen (true) or thawed (false). Default is true.

	  //步骤1：打开文件选择对话框
	filename = QFileDialog::getOpenFileName(this, tr("Open S57 file"), "", "*.000");
	if (filename.isNull())//如果未选择文件则返回
	{
		return;
	}
	QStringList temp = filename.split('/');
	QString basename = temp.at(temp.size() - 1);
	//步骤2：创建QgsVectorLayer类
	QgsVectorLayer* layer = new QgsVectorLayer(filename, basename, "ogr");
	if (!layer->isValid())//如果图层不合法
	{
		QMessageBox::critical(this, "error", QString("layer is invalid: \n") + filename);
		return;
	}
	layer->setProviderEncoding("System");//设置图层的编码格式
	QStringList sublayers = layer->dataProvider()->subLayers();//获取用户选择的电子海图所包含的全部子图层的字符串名称
	QStringList sLayerDefs;
	QString composedURI;
	QString layerName;
	QString layerGeometryType;
	int subLayersCount = sublayers.count();
	QgsVectorLayer* layerTemp;
	QgsSymbol* symbol;

	
	//layers.append(Lights(filename));
	//layers.append(Seaare(filename));
	
    // layers.append(Depcnt(filename));
	layers.append(Depare(filename));
	
	//layers.append(Depare(filename));
	layers.append(LndarePolygon(filename));
	layers.append(Lights(filename));

	mapCanvas->setExtent(Seaare(filename)->extent());
	mapCanvas->setLayers(layers);//设置图层集合
	mapCanvas->setVisible(true);
	mapCanvas->freeze(false);//解冻图层
	mapCanvas->zoomScale(600000);
	mapCanvas->refresh(); //刷新
}

void ImageViewer::Show_Coordinate(const QgsPointXY &p)
{
	QStatusBar* Main_Status_Bar = this->statusBar();
	Main_Status_Bar->showMessage(QString("Coordinate: x=%1,y=%2").arg(p.x()).arg(p.y()) + "       " + QString::number(mapCanvas->scale() / 100) + "m:1cm");
}
/*void ImageViewer::wheelEvent(QWheelEvent *event) {
	double curScale = mapCanvas->scale();//获取地图当前比例尺单位厘米
	int spli = 0;
	if (curScale >100 && curScale < 1300000)
		spli = 1;

	if (event->delta() > 0) {
		if (spli != 0) {
			mapCanvas->zoomScale(curScale + 100000);
		}
	}
	else {
		if (spli != 0)
			mapCanvas->zoomScale(curScale -100000);
	}
}*/
void ImageViewer::Displayfeatures(const double a) {

	if (mapCanvas->scale() < 15000) {

	}

}
QgsVectorLayer* ImageViewer::Lights( QString filename){
	QString composedURI = filename + "|layername=LIGHTS";
	composedURI += "|geometrytype=Point";//设置海图子图层的完全字符串名称
	QString layerName = "LndarePoint";
	QgsVectorLayer *layerTemp = new QgsVectorLayer(composedURI, layerName, "ogr");
	QgsSvgMarkerSymbolLayer* svgMarker = new QgsSvgMarkerSymbolLayer("C:/Users/VULCAN/source/repos/QtWidgetsApplication5/QtWidgetsApplication5/x64/Release/markicons/first.svg",5,2);
	QgsSymbolLayerList symList;
	QgsMarkerSymbol* markSym;
	QgsSingleSymbolRenderer* symRenderer;
	//svgMarker->setPath("D:/Program Files/QGIS 3.14/markicons/Activity-Diving-Scuba-Flag.svg");
	
	svgMarker->setSize(10);
	svgMarker->setSizeUnit(QgsUnitTypes::RenderUnit(0));
	svgMarker->setEnabled(true);
	svgMarker->setFillColor(QColor(0, 200, 255, 255));
	symList.append(svgMarker);
	markSym = new QgsMarkerSymbol(symList);
	symRenderer = new QgsSingleSymbolRenderer(markSym);
	layerTemp->setRenderer(symRenderer);
	//layerTemp->setScaleBasedVisibility(true);
	//layerTemp->setMinimumScale(1000);
	layerTemp->triggerRepaint();
	//layerTemp->setMinimumScale(600000);
	//layers.append(layerTemp);
	return layerTemp;
}
QgsVectorLayer* ImageViewer::Seaare(QString filename) {
	QString composedURI = filename + "|layername=SEAARE";
	//composedURI += "|geometrytype=Point";//设置海图子图层的完全字符串名称
	QString layerName = "Seaare";
	QgsVectorLayer *layerTemp = new QgsVectorLayer(composedURI, layerName, "ogr");
	QgsSymbol* symbol = QgsSymbol::defaultSymbol(layerTemp->geometryType());
	symbol->setColor(QColor(0, 150, 255, 255));
	layerTemp->setRenderer(new QgsSingleSymbolRenderer(symbol));
	return layerTemp;
}
QgsVectorLayer* ImageViewer::LndarePolygon(QString filename) {
	QString composedURI = filename + "|layername=LNDARE";
	composedURI += "|geometrytype=Polygon";//设置海图子图层的完全字符串名称
	QString layerName = "LndarePolygon";
	QgsVectorLayer *layerTemp = new QgsVectorLayer(composedURI, layerName, "ogr");
	QgsSymbol* symbol = QgsSymbol::defaultSymbol(layerTemp->geometryType());
	symbol->setColor(QColor(222, 184, 135));
	QgsUnitTypes::RenderUnit kk= symbol->outputUnit();
	symbol->setOutputUnit(QgsUnitTypes::RenderUnit(0));
	layerTemp->setRenderer(new QgsSingleSymbolRenderer(symbol));
	return layerTemp;
}
QgsVectorLayer* ImageViewer::Depcnt(QString filename) {
QString composedURI = filename + "|layername=DEPCNT";
	QString layerName = "Depcnt";
	QgsVectorLayer *layerTemp = new QgsVectorLayer(composedURI, layerName, "ogr");
	QgsSymbol* symbol = QgsSymbol::defaultSymbol(layerTemp->geometryType());
	
	symbol->setColor(QColor(0, 200, 255,255));
	QVariant value = "0";
	QVariant value1 = "5";
	QVariant value2 = "10";
	QVariant value3 = "20";
	QgsSymbol* sym = QgsSymbol::defaultSymbol(layerTemp->geometryType());
	QList<QgsRendererCategory> categories;
	sym->setColor(QColor(30, 144, 255, 255));
	QgsRendererCategory category = QgsRendererCategory(value, sym, "0");
	categories.append(category);
	QgsSymbol* sym1 = QgsSymbol::defaultSymbol(layerTemp->geometryType());
	sym1->setColor(QColor(135, 206, 250, 255));
	QgsRendererCategory  category1 = QgsRendererCategory(value1, sym1, "5");
	categories.append(category1);
	QgsSymbol* sym2 = QgsSymbol::defaultSymbol(layerTemp->geometryType());
	sym2->setColor(QColor(173, 216, 230));
	QgsRendererCategory category2 = QgsRendererCategory(value2, sym2, "10");
	categories.append(category2);
	QgsSymbol* sym3 = QgsSymbol::defaultSymbol(layerTemp->geometryType());
	sym3->setColor(QColor(173, 216, 230));
	QgsRendererCategory category3 = QgsRendererCategory(value3, sym3, "20");
	categories.append(category3);
	QgsFeatureRenderer *fr = new QgsCategorizedSymbolRenderer("VALDCO", categories);

	//layerTemp->setRenderer(new QgsSingleSymbolRenderer(symbol));
	QgsTextFormat * textFormat = new QgsTextFormat;
	textFormat->setFont(QFont("Arial", 12));
	textFormat->setSize(8);
	textFormat->setColor(QColor(0, 200, 255, 255));
	//textFormat->set    Buffer(*bufSettings);//设置文本缓冲区
	QgsPalLayerSettings *lSettings = new QgsPalLayerSettings;
	lSettings->setFormat(*textFormat);//设置标签文本格式
	lSettings->fieldName = "VALDCO";//要显示的字段名
	lSettings->placement = QgsPalLayerSettings::Placement::Line;//设置标签样式
	//lSettings->placement = QgsPalLayerSettings::Placement::Line;
	lSettings->placementFlags = QgsPalLayerSettings::LinePlacementFlags::OnLine;
	QgsVectorLayerSimpleLabeling * LSsettings = new QgsVectorLayerSimpleLabeling(*lSettings);
	layerTemp->setRenderer(new QgsSingleSymbolRenderer(symbol));
	layerTemp->setScaleBasedVisibility(true);
	layerTemp->setLabelsEnabled(true);//该图层启用label标签
	layerTemp->setLabeling(LSsettings);//设置标签配置
	layerTemp->triggerRepaint();
	//layerTemp->setMinimumScale(800000);
	return layerTemp;
}
QgsVectorLayer* ImageViewer::Depare(QString filename) {
	QString composedURI = filename + "|layername=DEPARE";
	//composedURI += "|geometrytype=Polygon";
	QString layerName = "Depare2";
	QgsVectorLayer *layerTemp = new QgsVectorLayer(composedURI, layerName, "ogr");

	/*QgsFeatureIterator featureIt = layerTemp->getFeatures();
	featureIt.rewind();
	QgsFeatureIds ids;
	QgsFeature f;
	 QgsFeatureIds ids2;	*/
	     QVariant value0 = "-5";
         QVariant value="0";
		 QVariant value1= "5";
		 QVariant value2= "10";
		 QVariant value3 = "20";
		 QVariant value4 = "30";
		 QVariant value5 = "40";
		 QVariant value6 = "50";
		// QgsMarkerSymbol* markSym;
		 QgsSymbol* sym = QgsSymbol::defaultSymbol(layerTemp->geometryType());
		         QList<QgsRendererCategory> categories;
				 sym->setColor(QColor(30, 144, 255, 255));
				 sym->Fill;
				 QgsRendererCategory category = QgsRendererCategory(value, sym, "0");
				 categories.append(category);
				 QgsSymbol* sym1 = QgsSymbol::defaultSymbol(layerTemp->geometryType());
				 sym1->setColor(QColor(135, 206, 250, 255));
				 QgsRendererCategory  category1 = QgsRendererCategory(value1, sym1, "5");
				 categories.append(category1);
				 QgsSymbol* sym2 = QgsSymbol::defaultSymbol(layerTemp->geometryType());
				 sym2->setColor(QColor(173, 216, 230));
				 QgsRendererCategory category2 = QgsRendererCategory(value2, sym2, "10");
				 categories.append(category2);
				 QgsSymbol* sym3 = QgsSymbol::defaultSymbol(layerTemp->geometryType());
				 sym3->setColor(QColor(173, 216, 230));
				 QgsRendererCategory category3 = QgsRendererCategory(value3, sym3, "20");
				 categories.append(category3);
				 QgsSymbol* sym4 = QgsSymbol::defaultSymbol(layerTemp->geometryType());
				 sym4->setColor(QColor(173, 216, 230));
				 QgsRendererCategory category4 = QgsRendererCategory(value4, sym4, "30");
				 categories.append(category4);
				 QgsSymbol* sym5 = QgsSymbol::defaultSymbol(layerTemp->geometryType());
				 sym5->setColor(QColor(173, 216, 230));
				 QgsRendererCategory category5 = QgsRendererCategory(value5, sym5, "40");
				 categories.append(category5);
				 QgsSymbol* sym6 = QgsSymbol::defaultSymbol(layerTemp->geometryType());
				 sym6->setColor(QColor(173, 216, 230));
				 QgsRendererCategory category6 = QgsRendererCategory(value6, sym6, "50");
				 categories.append(category6);
				 QgsSymbol* sym0 = QgsSymbol::defaultSymbol(layerTemp->geometryType());
				 sym0->setColor(QColor(32, 178, 170));
				 QgsRendererCategory category0 = QgsRendererCategory(value0, sym0, "-5");
				 categories.append(category0);
				
		         QgsFeatureRenderer *fr = new QgsCategorizedSymbolRenderer("DRVAL1", categories);	
	             layerTemp->setRenderer(fr);	
	             return layerTemp;
}
