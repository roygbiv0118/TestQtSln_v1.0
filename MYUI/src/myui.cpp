#include "myui.h"

#include <QtCore/QtCore>
#include <ActiveQt/QAxObject>

#include <vector>
#include <string>
#include <regex>
using namespace std;

MYUI::MYUI(QWidget *parent)
	: QDialog(parent)
	, m_bOpen(false)
{
	ui.setupUi(this);

	Qt::WindowFlags wf = this->windowFlags();

	ui.label->setText(QString::fromLocal8Bit("open"));

	QString str = QString::fromLocal8Bit("hello");
	ui.lblShow->setText(str);

	ui.label->setStyleSheet("font:bold 20pt 'Wide Latin';qproperty-alignment: AlignCenter;");

	QDir dir;
	QString strPath = dir.currentPath();
	QString strAppPath = QApplication::applicationDirPath();
	dir.setCurrent(strAppPath);

	m_strImageName = "lena.png";

	QString strHtml = QString("<p> <font color=#00F0FF size=12>abcdefg</font> <b><i>zzxxyy</b></i> </p>");
	strHtml += QString::fromLocal8Bit("<p> <font color=#0000FF size=10>aabbccdd</font><u>linelineline</u>abcabcabc</p> \
									  									  						<p><hr width=\"80%\" color=\"blue\" /></p> \
																																													<a><img src=\"???.jpg\" alt=\"test\" /></a>");
	ui.textEdit->setHtml(strHtml);

	Initialize();

	m_pVideoCapture = new VideoCapture;
	m_pVideoCapture->open(0);
	m_bOpen = m_pVideoCapture->isOpened();

	if (m_bOpen)
	{
		m_pTimerCap = new QTimer(this);
		m_pTimerCap->start(1);
		connect(m_pTimerCap, &QTimer::timeout, this, &MYUI::CaptureTimer);
	}
}

MYUI::~MYUI()
{
	if (m_bOpen)
	{
		m_pTimerCap->stop();
		m_bOpen = false;
	}

	if (m_pVideoCapture)
	{
		delete m_pVideoCapture;
		m_pVideoCapture = NULL;
	}
}

void MYUI::Initialize()
{
	///
	testQFile();
	///

	ui.lblTranslate->setText(tr("tr_testing"));

	///test RCC
	TraverseFolder();
	AddQRCFile();

	//QDir::setCurrent;
#if CONFIGURE_PARAMETER
	int nRes = system("..\\Resource\\run_rcc.bat");
#endif

	bool bRes = QResource::registerResource("../Resource/res.rcc");

	QImage image(":/test.jpg");
	QPixmap pixmap(QPixmap::fromImage(image));
	ui.lblImage->setPixmap(pixmap);

	QFile file(":/test.txt");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString strFile;

		while (!file.atEnd())
		{
			QByteArray arr = file.readLine();
			int nSize = arr.size();
			strFile += QString::fromLocal8Bit(arr);
		}
		ui.lblText->setText(strFile);
	}
	//////////////////////////////////////////////////////////////////////////


}

void MYUI::on_pushButton_clicked()
{
	static bool bSwitch = false;
	if (!bSwitch)
	{
		ui.label->setText(QString::fromLocal8Bit("close"));
		bSwitch = true;
		Mat matShow = imread(m_strImageName, IMREAD_COLOR);
		if (!matShow.empty())
		{
			imshow("CVShow", matShow);
		}
	}
	else
	{
		ui.label->setText(QString::fromLocal8Bit("open"));
		bSwitch = false;
		destroyWindow("CVShow");
	}
}

void MYUI::on_pbTestExcel_clicked()
{
	ui.pbTestExcel->setText("TestExcel");
	system("taskkill /F /IM EXCEL.EXE");

	if (0)///Primary
	{
		QAxObject excel("Excel.Application");
		excel.setProperty("Visible", false);
		QAxObject *work_books = excel.querySubObject("WorkBooks");
		work_books->dynamicCall("Open (const QString&)", "test.xlsx");
		QVariant title_value = excel.property("Caption");  //???????
		qDebug() << QString("excel title : ") << title_value;
		QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
		QAxObject *work_sheets = work_book->querySubObject("Sheets");  //Sheets??????WorkSheets

		int sheet_count = work_sheets->property("Count").toInt();  //????????????
		qDebug() << QString("sheet count : ") << sheet_count;
		for (int i = 1; i <= sheet_count; i++)
		{
			QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", i);  //Sheets(int)??????Worksheets(int)
			QString work_sheet_name = work_sheet->property("Name").toString();  //?????????????
			QString message = QString("sheet ") + QString::number(i, 10) + QString(" name");
			qDebug() << message << work_sheet_name;
		}
		if (sheet_count > 0)
		{
			QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 3);
			QAxObject *used_range = work_sheet->querySubObject("UsedRange");
			QAxObject *rows = used_range->querySubObject("Rows");
			QAxObject *columns = used_range->querySubObject("Columns");
			int row_start = used_range->property("Row").toInt();  //????????   
			int column_start = used_range->property("Column").toInt();  //????????
			int row_count = rows->property("Count").toInt();  //???????
			int column_count = columns->property("Count").toInt();  //???????
			for (int i = row_start; i <= row_count; i++)
			{
				for (int j = column_start; j <= column_count; j++)
				{
					QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", i, j);
					QVariant cell_value = cell->property("Value");  //????????????
					QString message = QString("row-") + QString::number(i, 10) + QString("-column-") + QString::number(j, 10) + QString(":");
					qDebug() << message << cell_value;
				}
			}
		}
		work_book->dynamicCall("Close(Boolean)", false);  //??????
		excel.dynamicCall("Quit(void)");  //???
	}

	////////////////////////////////////////////////////////////////////////
	if (0)///Advanced
	{
		QAxObject excel("Excel.Application");
		excel.setProperty("Visible", false);
		QAxObject *work_books = excel.querySubObject("WorkBooks");
		work_books->dynamicCall("Open(const QString&)", "test1.xlsx");
		excel.setProperty("Caption", "Qt Excel");
		QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
		QAxObject *work_sheets = work_book->querySubObject("Sheets");  //Sheets??????WorkSheets

		//????????????????????
		QAxObject *first_sheet = work_sheets->querySubObject("Item(int)", 1);
		first_sheet->dynamicCall("delete");

		//???????????????????????
		int sheet_count = work_sheets->property("Count").toInt();  //????????????
		QAxObject *last_sheet = work_sheets->querySubObject("Item(int)", sheet_count);
		QAxObject *work_sheet = work_sheets->querySubObject("Add(QVariant)", last_sheet->asVariant());
		last_sheet->dynamicCall("Move(QVariant)", work_sheet->asVariant());

		work_sheet->setProperty("Name", "Qt Sheet");  //??????????????

		//???????????2????2????
		QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", 2, 2);
		cell->setProperty("Value", "Java C++ C# PHP Perl Python Delphi Ruby");  //?????????
		cell->setProperty("RowHeight", 50);  //????????????
		cell->setProperty("ColumnWidth", 30);  //????????????
		cell->setProperty("HorizontalAlignment", -4108); //?????xlLeft????-4131  ??????xlCenter????-4108  ?????xlRight????-4152
		cell->setProperty("VerticalAlignment", -4108);  //?????xlTop??-4160 ??????xlCenter????-4108  ?????xlBottom????-4107
		cell->setProperty("WrapText", true);  //??????????????
		//cell->dynamicCall("ClearContents()");  //???????????

		QAxObject* interior = cell->querySubObject("Interior");
		interior->setProperty("Color", QColor(0, 255, 0));   //??????????????????

		QAxObject* border = cell->querySubObject("Borders");
		border->setProperty("Color", QColor(0, 0, 255));   //??????????????????

		QAxObject *font = cell->querySubObject("Font");  //????????????
		font->setProperty("Name", QStringLiteral("???????"));  //????????????
		font->setProperty("Bold", true);  //??????????????
		font->setProperty("Size", 20);  //???????????????
		font->setProperty("Italic", true);  //????????????????
		font->setProperty("Underline", 2);  //?????????????
		font->setProperty("Color", QColor(255, 0, 0));  //??????????????????????

		//?????????????????????????5????3??-??8????5????
		QAxObject *cell_5_6 = work_sheet->querySubObject("Cells(int,int)", 5, 3);
		cell_5_6->setProperty("Value", "Java");  //?????????
		QAxObject *cell_8_5 = work_sheet->querySubObject("Cells(int,int)", 8, 5);
		cell_8_5->setProperty("Value", "C++");

		QString merge_cell;
		merge_cell.append(QChar(3 - 1 + 'A'));  //?????
		merge_cell.append(QString::number(5));  //?????
		merge_cell.append(":");
		merge_cell.append(QChar(5 - 1 + 'A'));  //?????
		merge_cell.append(QString::number(8));  //?????
		QAxObject *merge_range = work_sheet->querySubObject("Range(const QString&)", merge_cell);
		merge_range->setProperty("HorizontalAlignment", -4108);
		merge_range->setProperty("VerticalAlignment", -4108);
		merge_range->setProperty("WrapText", true);
		merge_range->setProperty("MergeCells", true);  //????????
		//merge_range->setProperty("MergeCells", false);  //???????

		work_book->dynamicCall("Save()");  //??????????????test???????test2???????????????????? 
		//work_book->dynamicCall("SaveAs(const QString&)", m_strAppPath + "/test2.xlsx");  //???????????? 
		work_book->dynamicCall("Close(Boolean)", false);  //??????
		excel.dynamicCall("Quit(void)");  //???
	}

	//////////////////////////////////////////////////////////////////////////
	if (1) ///Draw
	{
		QString strAppPath = QApplication::applicationDirPath();

		QAxObject excel("Excel.Application");
		excel.setProperty("Visible", true);
		QAxObject *work_books = excel.querySubObject("WorkBooks");
		work_books->dynamicCall("Open(const QString&)", strAppPath + "/../Resource/test.xlsx");///??????
		excel.setProperty("Caption", "Qt Excel");
		QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
		QAxObject *work_sheets = work_book->querySubObject("Sheets");  //Sheets??????WorkSheets

		//???????????????????????
		int sheet_count = work_sheets->property("Count").toInt();  //????????????
		QAxObject *last_sheet = work_sheets->querySubObject("Item(int)", sheet_count);
		QAxObject *work_sheet = work_sheets->querySubObject("Add(QVariant)", last_sheet->asVariant());
		last_sheet->dynamicCall("Move(QVariant)", work_sheet->asVariant());

		work_sheet->setProperty("Name", "Draw Sheet");  //??????????????

		QImage img("../Resource/test.png");
		QSize size = img.size();

		int nPerLine = img.bytesPerLine();
		int nDep = img.depth();
		QImage::Format format = img.format();
		const uchar* pBuffer = img.bits();

		//?????????
		for (int h = 1; h <= size.height(); h++)
		{
			unsigned int nHindex = (h - 1)*nPerLine;

			for (int w = 1; w <= size.width(); w++)
			{
				unsigned int nWindex = (w - 1) * 4;

				QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", h, w);
				cell->setProperty("RowHeight", 10);  //????????????
				cell->setProperty("ColumnWidth", 1);  //????????????

				QAxObject* interior = cell->querySubObject("Interior");
				interior->setProperty("Color", QColor(pBuffer[nHindex + nWindex + 2], pBuffer[nHindex + nWindex + 1], pBuffer[nHindex + nWindex]));   //??????????????????
			}
		}


		//work_book->dynamicCall("Save()");  //??????????????test???????test2???????????????????? 
		//work_book->dynamicCall("SaveAs(const QString&)",strAppPath + "/test2.xlsx");  //???????????? 
		//work_book->dynamicCall("Close(Boolean)", false);  //??????
		//excel.dynamicCall("Quit(void)");  //???

		ui.pbTestExcel->setText("Ok");
	}

}

void MYUI::on_pbTextCodec_clicked()
{
	QString str11 = QString::fromLocal8Bit("你好");

	QTextCodec* pCodec = QTextCodec::codecForLocale();
	QByteArray aName = pCodec->name();
	QChar a0 = aName[0];
	QChar a1 = aName[1];
	QChar a2 = aName[2];

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	QByteArray aData("11");

	QString str(QString::fromLocal8Bit("11"));

	QByteArray aRes = QCryptographicHash::hash(str.toLocal8Bit(), QCryptographicHash::Md5);
	QByteArray aRes1 = aRes.toHex();

	QString strTemp(aRes);
	QByteArray aResHex = strTemp.toLocal8Bit().toHex();

	QLatin1String lstr = QLatin1String("zap1219");
	QByteArray bstr = "zap1219";
	QString sstr = bstr;
}

void MYUI::TraverseFolder()
{
	QString strFolder("../Resource");
	//判断路径是否存在
	QDir dir(strFolder);
	if (!dir.exists())
	{
		return;
	}

	//获取所选文件类型过滤器
	QStringList filters;
	filters << QString("*.txt") << QString("*.jpg") << QString("*.png") << QString("*.xlsx");

	//定义迭代器并设置过滤器
	QDirIterator dir_iterator(strFolder,
		filters,
		QDir::Files | QDir::NoSymLinks,
		QDirIterator::Subdirectories);
	while (dir_iterator.hasNext())
	{
		dir_iterator.next();
		QFileInfo file_info = dir_iterator.fileInfo();
		QString absolute_file_path = file_info.absoluteFilePath();
		QString file_path = file_info.filePath();
		m_SLFilePaths.append(file_path);
	}
}

void MYUI::AddQRCFile()
{
	QFile file("../Resource/res.qrc");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QIODevice::OpenMode om = file.openMode();

	QTextStream out(&file);
	out << "<RCC>" << "\n" << "\t" << "<qresource>";
	for (const QString &strFilePath : m_SLFilePaths)
	{
		///RegEx
		string strFile("this subject has a submarine as a subsequence"), strKey;
		const regex sear("(\\b)(sub)([^ ]*)");
		smatch res;

		vector<string> vecKey;
		while (regex_search(strFile, res, sear))
		{
			for (auto x : res)
			{
				strKey = x;
				vecKey.push_back(strKey);
			}
			strFile = res.suffix().str();
		}
		///

		const std::regex pattern("\\b\\w+\\.\\w+$");

		string strNames = strFilePath.toStdString(), strName;
		std::smatch result;
		bool match = std::regex_search(strNames, result, pattern);
		if (match)
		{
			for (size_t i = 0; i < result.size(); ++i)
			{
				strName = result[i];
			}
		}

		out << "\n\t\t" << "<file alias=\"" << QString::fromStdString(strName) << "\">" << strFilePath << "</file>";
	}
	out << "\n\t" << "</qresource>" << "\n" << "</RCC>";

	file.close();
}

void MYUI::CaptureTimer()
{
	if (!m_bOpen) return;
	Mat frame;

	*m_pVideoCapture >> frame;

	imshow("Video", frame);
}

void MYUI::testQFile()
{
	QFile file(QStringLiteral("../Resource/test.dat"));
	file.open(QIODevice::WriteOnly | QIODevice::Truncate);
	QDataStream dout(&file);
	QString str = "123abc";
	short n = 123;
	dout << str << n;
	file.close();

	///
	QFile fileRead(QStringLiteral("../Resource/test.dat"));
	fileRead.open(QIODevice::ReadOnly);
	QDataStream din(&fileRead);
	QString strIn;
	short nIn;
	din >> strIn >> nIn;
	file.close();


}