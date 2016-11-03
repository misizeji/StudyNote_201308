
#include <QApplication>
#include <QWidget>
#include <QTextCodec>
#include <QPushButton>
#include <QLabel>

int main(int argc, char *argv[])
{
	QApplication app(argc,argv);

	QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
	//********ÊàñÊòØGB2312Âíå utf8ÁºñÁ†Å
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
	QWidget w;
//	w.show();
	QLabel la("hello Qt",&w);
	QPushButton B("∫Ù∫Ù",&w);
//	B.show();
	la.move(150,100);
	w.show();
	
	return app.exec();
}
