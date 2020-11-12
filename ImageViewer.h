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


class ImageViewer : public QMainWindow
{
	Q_OBJECT

public:
	ImageViewer(QWidget *parent = Q_NULLPTR);

private:
	// create the menus and then add the actions to them.
	QMenu *fileMenu;
	QAction *openFileAction;


	QLabel* m_scaleLabel; // ��״̬������ʾ��scale 1:��
	QgsScaleComboBox* m_scaleEdit; //! ��״̬������ʾ������ֵ
	QProgressBar* m_progressBar;
	QLabel* m_coordsLabel; //! ��״̬����ʾ"Coordinate / Extent"
	QLineEdit* m_coordsEdit; //! ��״̬����ʾ��������
	//map canvas
	QgsMapCanvas *mapCanvas;
	QList<QgsMapLayer *> layers;
	void createStatusBar();
public slots:
	void on_openFileAction_triggered();
	//void wheelEvent(QWheelEvent *event);
	//
	//void createStatusBar();
public:
	void addVectorLayer();
	void addChartlayers();
	
	//void wheelEvent(QWheelEvent*event);
};