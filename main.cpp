#include "mainwindow.h"

#include <QApplication>
#include <QIcon>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QPixmap pixmap("C:/programming/project/weboasis/images/app-development.png");
    a.setWindowIcon(QIcon(pixmap));

    w.show();
    return a.exec();
}
