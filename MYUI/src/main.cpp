#include "myui.h"
#include <QtWidgets/QtWidgets>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QDir::setCurrent(a.applicationDirPath());

	///test Internationalize
#if CONFIGURE_PARAMETER
	int nRes = system("..\\..\\Internationalize\\run_internationalize.bat");
#endif

	QTranslator trans;
	bool bRes = trans.load("../Internationalize/mx_zh.qm");
	bRes = a.installTranslator(&trans);
	//////////////////////////////////////////////////////////////////////////

	a.setStyleSheet("QLabel\
	{\
	font:bold 18pt '微软雅黑';\
	color:blue;\
	qproperty-alignment: AlignCenter;\
	}");

	MYUI w;
	w.show();

	return a.exec();
}
