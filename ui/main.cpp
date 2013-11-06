#include <qapplication.h>
#include "mainwindow.h"

int main( int argc, char **argv )
{

	if(argc != 2){
		printf("usage: simuGui <file-name>\n");
		return -1;
	}
    QApplication a( argc, argv );

    MainWindow w(argv[1]);
    w.show();

    return a.exec();
}
