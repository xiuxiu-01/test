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
	mapCanvas->freeze(true);//�����ⶳ��ͼ��������,frozen (true) or thawed (false). Default is true.

	  //����1�����ļ�ѡ��Ի���
	QString filename = QFileDialog::getOpenFileName(this, tr("Open S57 file"), "", "*.000");
	if (filename.isNull())//���δѡ���ļ��򷵻�
	{
		return;
	}
	QStringList temp = filename.split('/');
	QString basename = temp.at(temp.size() - 1);
	//����2������QgsVectorLayer��
	QgsVectorLayer* layer = new QgsVectorLayer(filename, basename, "ogr");
	if (!layer->isValid())//���ͼ�㲻�Ϸ�
	{
		QMessageBox::critical(this, "error", QString("layer is invalid: \n") + filename);
		return;
	}
	layer->setProviderEncoding("System");//����ͼ��ı����ʽ
	QStringList sublayers = layer->dataProvider()->subLayers();//��ȡ�û�ѡ��ĵ��Ӻ�ͼ��������ȫ����ͼ����ַ�������
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
			//layerGeometryType = "Polygon";//ͼ������
			//composedURI += "|geometrytype=Polygon";
			layerTemp = new QgsVectorLayer(composedURI, layerName, "ogr");
			//if (!layerTemp->isValid())//���ͼ�㲻�Ϸ�
			//{
				//QMessageBox::critical(this, "error", QString(composedURI) + layerName);
			//}

			layers.append(layerTemp);
			//QgsProject::instance()->addMapLayer(layerTemp);
			if (sLayerDefs[2] == "!!LNDARE!!")//LNDARE���ҵ�*.000�����У��Ҵ�����ʾ���Ǹ���ͼ���ͼ������
			{
				mapCanvas->setExtent(layerTemp->extent());
			}
			//mapCanvas->setExtent(layerTemp->extent());
			mapCanvas->setLayers(layers);//����ͼ�㼯��
		}
		mapCanvas->freeze(false);//�ⶳͼ��
		mapCanvas->refresh(); //ˢ��
		//double dCurrScale = mapCanvas->scale();
	//	QString sCurrScale = this->m_scaleEdit->toString(dCurrScale);
		//this->m_scaleEdit->setScaleString(sCurrScale);
	//mapCanvas->setVisible(true);//�����Ƿ�ɼ�
	//mapCanvas->freeze(false);//�ⶳͼ��
	//double dCurrScale = mapCanvas->scale();
	//QMessageBox::critical(this, "error", QString("layer is invalid: \n") + sLayerDefs[1]);
	/*composedURI = filename + "|layerid=" + sLayerDefs[0];
	layerName = basename + "@LNDARE";
	layerGeometryType = "Polygon";//ͼ������
	QgsVectorLayer* layerTemp;
	composedURI += "|geometrytype=Polygon";
	layerTemp = new QgsVectorLayer(composedURI, layerName, "ogr");
	//if (!layerTemp->isValid())//���ͼ�㲻�Ϸ�
	//{
		//QMessageBox::critical(this, "error", QString("layer is invalid: \n") + filename);
	//}
	layers.append(layerTemp);
	QgsProject::instance()->addMapLayers(layers);
	mapCanvas->setExtent(layerTemp->extent());
	mapCanvas->setLayers(layers);//����ͼ�㼯��
	mapCanvas->setVisible(true);//�����Ƿ�ɼ�
//	mapCanvas->freeze(false);//�ⶳͼ��
	mapCanvas->refresh();//ˢ��
			//layerTemp->setProviderEncoding("System");//����ͼ��ı����ʽ
	

	
		//layers.append(layer);
		/*else
		{
			//QStringList sEncodings = QgsVectorDataProvider::availableEncodings();
			layer->setProviderEncoding("System");//����ͼ��ı����ʽ
			QStringList sublayers = layer->dataProvider()->subLayers();//��ȡ�û�ѡ��ĵ��Ӻ�ͼ��������ȫ����ͼ����ַ�������
			int subLayersCount = sublayers.count();// If the newly created layer has more than 1 layer of data available, we show the sublayers selection dialog so the user can select the sublayers to actually load.
			if (subLayersCount >= 1)
			{
				for (int i = 0; i < subLayersCount; i++)
				{
					//ע�����ʸ�����ݣ���������ӵ�������
					QStringList sLayerDefs = sublayers[i].split(':');//.000�ļ��е���ͼ����ַ������ƽṹ����0:DSID:Unknown:None��1:BUAARE:15:Point��
					QString composedURI = filename + "|layerid=" + sLayerDefs[0];
					QString layerName = basename + "@" + sLayerDefs[1];

					QString layerGeometryType = sLayerDefs[3];//ͼ������

					QgsVectorLayer* layerTemp;
					if (!layerGeometryType.isEmpty() && layerGeometryType == "Polygon" && sLayerDefs[1] == "LNDARE")//LNDARE���ҵ�*.000�����У��Ҵ�����ʾ���Ǹ���ͼ���ͼ������
					{
						composedURI += "|geometrytype=" + layerGeometryType;//���ú�ͼ��ͼ�����ȫ�ַ�������
						layerTemp = new QgsVectorLayer(composedURI, layerName, "ogr");

						if (!layerTemp->isValid())//���ͼ�㲻�Ϸ�
						{
							QMessageBox::critical(this, "error", QString("layer is invalid: \n") + filename);
							return;
						}
						QgsProject::instance()->addMapLayer(layerTemp);
						//���ø�ͼ��ĵ�һ��Ⱦ���
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
		//����3����ͼ�㼯����ӵ�������
		mapCanvas->setLayers(layers);//����ͼ�㼯��
		mapCanvas->setVisible(true);//�����Ƿ�ɼ�
		mapCanvas->freeze(false);//�ⶳͼ��
		mapCanvas->refresh();//ˢ��
		//��״̬�����õ�ͼ�ı�����
		//double dCurrScale = mapCanvas->scale();
		//QString sCurrScale = this->m_scaleEdit->toString(dCurrScale);
		//this->m_scaleEdit->setScaleString(sCurrScale);*/
	//}
}
/*
void ImageViewer::createStatusBar()
{

	//! ���������ʾ��ǩ
	//! ��ӽ�����

	QFont myFont("Arial", 9);
	statusBar()->setFont(myFont);

	//! ���������ʾ��ǩ
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


	//! �����߱�ǩ
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