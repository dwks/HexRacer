#include "hrmemainwindow.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	HRMEMainWindow w;
	w.show();
	return a.exec();
}
