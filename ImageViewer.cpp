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
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qgsvectorlayer.h>
#include <qgssymbol.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include <qgssymbollayer.h>
#include <qgsrenderer.h>
#include <qgsmaplayerstore.h>
#include <qgsmaplayer.h>
#include <qgssinglesymbolrenderer.h>
#define QGIS_DEV_H
using namespace std;
ImageViewer::ImageViewer(QWidget *parent)
	: QMainWindow(parent)
{
	this->resize(600, 400);

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
	this->connect(openFileAction, SIGNAL(triggered(bool)), this, SLOT(on_openFileAction_triggered()));
	


}

void ImageViewer::on_openFileAction_triggered() {
	addChartlayers();
}

void ImageViewer::addVectorLayer()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open shape file"), "", "*.shp");
	QStringList temp = fileName.split('/');
	QString basename = temp.at(temp.size() - 1);
	QgsVectorLayer* vecLayer = new QgsVectorLayer(fileName, basename, "ogr");

	if (!vecLayer->isValid())
	{
		QMessageBox::critical(this, "error", QString("layer is invalid: \n") + fileName);
		return;
	}
	QMessageBox::critical(this, "error", QString(basename) + fileName);
	layers.append(vecLayer); 
	QgsProject::instance()->addMapLayers(layers);
	mapCanvas->setExtent(vecLayer->extent());
	
	mapCanvas->setLayers(layers);
	mapCanvas->refresh();
}

void ImageViewer::addChartlayers()
{
	mapCanvas->freeze(true);//冻结或解冻地图画布对象,frozen (true) or thawed (false). Default is true.

	  //步骤1：打开文件选择对话框
	QString filename = QFileDialog::getOpenFileName(this, tr("Open S57 file"), "", "*.000");
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
		for (int i = 0; i < subLayersCount; i++)
		{
			sLayerDefs = sublayers[i].split(':');
			
			composedURI = filename + "|layerid="+QString::number(i);
			layerName = basename;
			//layerGeometryType = "Polygon";//图层类型
			//composedURI += "|geometrytype=Polygon";
			layerTemp = new QgsVectorLayer(composedURI, layerName, "ogr");
			//if (!layerTemp->isValid())//如果图层不合法
			//{
				//QMessageBox::critical(this, "error", QString(composedURI) + layerName);
			//}

			layers.append(layerTemp);
			//QgsProject::instance()->addMapLayer(layerTemp);
			if (sLayerDefs[2] == "!!LNDARE!!")//LNDARE是我的*.000数据中，我打算显示的那个子图层的图层名称
			{
				mapCanvas->setExtent(layerTemp->extent());
			}
			//mapCanvas->setExtent(layerTemp->extent());
			mapCanvas->setLayers(layers);//设置图层集合
		}
		mapCanvas->freeze(false);//解冻图层
		mapCanvas->refresh(); //刷新
		//double dCurrScale = mapCanvas->scale();
	//	QString sCurrScale = this->m_scaleEdit->toString(dCurrScale);
		//this->m_scaleEdit->setScaleString(sCurrScale);
	//mapCanvas->setVisible(true);//设置是否可见
	//mapCanvas->freeze(false);//解冻图层
	//double dCurrScale = mapCanvas->scale();
	//QMessageBox::critical(this, "error", QString("layer is invalid: \n") + sLayerDefs[1]);
	/*composedURI = filename + "|layerid=" + sLayerDefs[0];
	layerName = basename + "@LNDARE";
	layerGeometryType = "Polygon";//图层类型
	QgsVectorLayer* layerTemp;
	composedURI += "|geometrytype=Polygon";
	layerTemp = new QgsVectorLayer(composedURI, layerName, "ogr");
	//if (!layerTemp->isValid())//如果图层不合法
	//{
		//QMessageBox::critical(this, "error", QString("layer is invalid: \n") + filename);
	//}
	layers.append(layerTemp);
	QgsProject::instance()->addMapLayers(layers);
	mapCanvas->setExtent(layerTemp->extent());
	mapCanvas->setLayers(layers);//设置图层集合
	mapCanvas->setVisible(true);//设置是否可见
//	mapCanvas->freeze(false);//解冻图层
	mapCanvas->refresh();//刷新
			//layerTemp->setProviderEncoding("System");//设置图层的编码格式
	

	
		//layers.append(layer);
		/*else
		{
			//QStringList sEncodings = QgsVectorDataProvider::availableEncodings();
			layer->setProviderEncoding("System");//设置图层的编码格式
			QStringList sublayers = layer->dataProvider()->subLayers();//获取用户选择的电子海图所包含的全部子图层的字符串名称
			int subLayersCount = sublayers.count();// If the newly created layer has more than 1 layer of data available, we show the sublayers selection dialog so the user can select the sublayers to actually load.
			if (subLayersCount >= 1)
			{
				for (int i = 0; i < subLayersCount; i++)
				{
					//注册添加矢量数据，并个并添加到画布中
					QStringList sLayerDefs = sublayers[i].split(':');//.000文件中的子图层的字符串名称结构：如0:DSID:Unknown:None和1:BUAARE:15:Point等
					QString composedURI = filename + "|layerid=" + sLayerDefs[0];
					QString layerName = basename + "@" + sLayerDefs[1];

					QString layerGeometryType = sLayerDefs[3];//图层类型

					QgsVectorLayer* layerTemp;
					if (!layerGeometryType.isEmpty() && layerGeometryType == "Polygon" && sLayerDefs[1] == "LNDARE")//LNDARE是我的*.000数据中，我打算显示的那个子图层的图层名称
					{
						composedURI += "|geometrytype=" + layerGeometryType;//设置海图子图层的完全字符串名称
						layerTemp = new QgsVectorLayer(composedURI, layerName, "ogr");

						if (!layerTemp->isValid())//如果图层不合法
						{
							QMessageBox::critical(this, "error", QString("layer is invalid: \n") + filename);
							return;
						}
						QgsProject::instance()->addMapLayer(layerTemp);
						//设置该图层的单一渲染风格
						QgsSymbol* symbol = QgsSymbol::defaultSymbol(layerTemp->geometryType());//QGis::GeometryType::Polygon
						symbol->setColor(QColor(100, 100, 160, 255));
						layerTemp->setRenderer(new QgsSingleSymbolRenderer(symbol));
						//layers << layerTemp;
						layers.append(layerTemp);
						mapCanvas->setLayers(layers);
						mapCanvas->refresh();
						continue;
					}
					else
					{
						continue;
					}
				}
				delete layer;// The first layer loaded is not useful in that case.
			}
			else
			{
				QString msg = tr("%1 doesn't have any layers").arg(basename);
				QMessageBox::critical(this, "Invalid Data Source", msg);
				delete layer;
			}
		}
		//步骤3：将图层集合添加到画布中
		mapCanvas->setLayers(layers);//设置图层集合
		mapCanvas->setVisible(true);//设置是否可见
		mapCanvas->freeze(false);//解冻图层
		mapCanvas->refresh();//刷新
		//在状态条设置地图的比例尺
		//double dCurrScale = mapCanvas->scale();
		//QString sCurrScale = this->m_scaleEdit->toString(dCurrScale);
		//this->m_scaleEdit->setScaleString(sCurrScale);*/
	//}
}
/*
void ImageViewer::createStatusBar()
{

	//! 添加坐标显示标签
	//! 添加进度条

	QFont myFont("Arial", 9);
	statusBar()->setFont(myFont);

	//! 添加坐标显示标签
	m_coordsLabel = new QLabel(QString(), statusBar());
	m_coordsLabel->setObjectName("m_coordsLabel");
	m_coordsLabel->setFont(myFont);
	m_coordsLabel->setMinimumWidth(10);
	m_coordsLabel->setMargin(3);
	m_coordsLabel->setAlignment(Qt::AlignCenter);
	m_coordsLabel->setFrameStyle(QFrame::NoFrame);
	m_coordsLabel->setText(tr("Coordinate:"));
	m_coordsLabel->setToolTip(tr("Current map coordinate"));
	statusBar()->addPermanentWidget(m_coordsLabel, 0);


	//! 比例尺标签
	m_scaleLabel = new QLabel(QString(), statusBar());
	m_scaleLabel->setObjectName("m_scaleLabel");
	m_scaleLabel->setFont(myFont);
	m_scaleLabel->setMinimumWidth(10);
	m_scaleLabel->setMargin(3);
	m_scaleLabel->setAlignment(Qt::AlignCenter);
	m_scaleLabel->setFrameStyle(QFrame::NoFrame);
	m_scaleLabel->setText(tr("Scale"));
	m_scaleLabel->setToolTip(tr("Current map scale"));
	statusBar()->addPermanentWidget(m_scaleLabel, 0);

	m_scaleEdit = new QgsScaleComboBox(statusBar());
	m_scaleEdit->setObjectName("m_scaleEdit");
	m_scaleEdit->setFont(myFont);
	m_scaleEdit->setMinimumWidth(10);
	m_scaleEdit->setMaximumWidth(100);
	m_scaleEdit->setContentsMargins(0, 0, 0, 0);
	m_scaleEdit->setToolTip(tr("Current map scale (formatted as x:y)"));
	statusBar()->addPermanentWidget(m_scaleEdit, 0);
	connect(m_scaleEdit, SIGNAL(scaleChanged()), this, SLOT(userScale()));

}
*/