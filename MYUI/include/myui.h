#ifndef MYUI_H
#define MYUI_H

#include <QtWidgets/QtWidgets>
#include "ui_myui.h"

#include <opencv2/opencv.hpp>
using namespace cv;

#define CONFIGURE_PARAMETER 0

class MYUI : public QDialog
{
	Q_OBJECT

public:
	MYUI(QWidget *parent = 0);
	~MYUI();

	void Initialize();

private slots:
	void on_pushButton_clicked();
	void on_pbTestExcel_clicked();

	void on_pbTextCodec_clicked();

private:
	void TraverseFolder();
	void AddQRCFile();

	void CaptureTimer();

	void testQFile();

private:
	Ui::MYUIClass ui;
	cv::String m_strImageName;

	QStringList m_SLFilePaths;

	cv::VideoCapture* m_pVideoCapture;
	bool m_bOpen;

	QTimer* m_pTimerCap;

};

#endif // MYUI_H
