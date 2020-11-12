#pragma once

#include <QtWidgets/QMainWindow>
#include <qmenu.h>
#include <qaction.h>
#include <qgsscalecombobox.h>
#include <qgsmapcanvas.h>
#include<qgsapplication.h>
#include<qgsproviderregistry.h>
#include<qgsvectorlayer.h>
#include<qgsmapcanvas.h>
#include<QString>
#include<QApplication>
#include<QWidget>
#include<QMouseEvent>
#include<QWheelEvent>
#include <QStringList>
#include <qgsscalecombobox.h>
#include <qmainwindow.h>

#include<QMessageBox>
#include<QObject>
#include <QList>
#include <QFileInfoList>
#include <QDir>
#include <QLibrary>
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QGridLayout>
#include <QFont>
#include <QLineEdit>
#include <QToolButton>
#include <QMargins>

#include <QList>
#include <QDockWidget>
#include <QProgressBar>
#include <QLabel>
#include <QDockWidget>


#include <qgsvectorlayerrenderer.h>
#include <qgsrendercontext.h>
#include "ui_ImageViewer.h"

class ImageViewer : public QMainWindow
{
	Q_OBJECT

public:
	ImageViewer(QWidget *parent = Q_NULLPTR);
protected:
	//void wheelEvent(QWheelEvent *event);
private:
	// create the menus and then add the actions to them.
	Ui::ImageViewerClass ui;
	QMenu *fileMenu;
	QAction *openFileAction;
	//map canvas
	QString filename;
	QgsMapCanvas *mapCanvas;
	QList<QgsMapLayer *> layers;
	//void createStatusBar();
public slots:
	void on_openFileAction_triggered();
	void Show_Coordinate(const QgsPointXY &p);
	void Displayfeatures(const double a);
	QgsVectorLayer * Lights(QString filename);
	QgsVectorLayer * Seaare(QString filename);
	QgsVectorLayer * LndarePolygon(QString filename);
	QgsVectorLayer * Depcnt(QString filename);
	QgsVectorLayer * Depare(QString filename);

	//QgsVectorLayer* LndarePoint(QString filename);
	//void Show(const QgsPointXY &p);
	//
	//void createStatusBar();
public:
	void addVectorLayer();
	void addChartlayers();
	//QgsVectorLayer* lndare(QString filename);
};