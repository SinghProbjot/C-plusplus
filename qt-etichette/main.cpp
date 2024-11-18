#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

static void m(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}
