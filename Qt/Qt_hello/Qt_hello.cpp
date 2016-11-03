
#include <QApplication>
#include <QWidget>
#include <QTextCodec>
#include <QPushButton>
#include <QLabel>

int main(int argc, char *argv[])
{
	QApplication app(argc,argv);

	QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
	//********或是GB2312和 utf8编码
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
	QWidget w;
//	w.show();
	QLabel la("hello Qt",&w);
	QPushButton B("����",&w);
//	B.show();
	la.move(150,100);
	w.show();
	
	return app.exec();
}
