#include "mainwindow.h"
#include <QtSql>

#include <QApplication>
#include <QMessageBox>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "db");
    QMessageBox msgBox;
    /*QFile file(":/database.db");*/

   // msgBox.setText(QDir::tempPath());
    //msgBox.exec();
    db.setDatabaseName(QDir::tempPath()+ "/calendario_869434.db");
    MainWindow w;
    w.setWindowTitle("Calendario");
    w.show();
    return a.exec();
}
