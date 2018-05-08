#include <QApplication>
#include <QMetaType>
#include "../Headers/MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	qRegisterMetaType<uint32_t>("uint32_t");
	qRegisterMetaType<MyBlock>("MyBlock");

	MainWindow w;
	w.show();

	return a.exec();
}
