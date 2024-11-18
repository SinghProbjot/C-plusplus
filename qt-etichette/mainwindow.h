#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tablemodel.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QStringLiteral>
#include <QTableView>
#include <QMainWindow>
#include <QMessageBox>
#include <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tableView_4_clicked(const QModelIndex &index);
    void on_tableView_3_clicked(const QModelIndex &index);

private:
    const QString url = QString(QStringLiteral("http://www.ivl.disco.unimib.it/minisites/cpp/%1"));
    const QString uniFilename = QString("List_of_Universal_artists.txt");
    const QString emiFilename = QString("List_of_EMI_artists.txt");
    const QString itemUrl = QString("https://wikipedia.org");


    Ui::MainWindow *ui;

    TableModel *uniModel;
    TableModel *emiModel;
    QList<Artist> uniArtists;
    QList<Artist> emiArtists;
    //QBarSeries *grafico;


    QNetworkAccessManager netManager;

    void handle_network_data(QNetworkReply *networkReply);
    void fill_list(QList<Artist> &artists, QByteArray response);
    void draw_chart(QChart *chart, QList<Artist> artists);
    void draw_chart2(QChart *chart, QList<Artist> artists);
    void draw_chart(QChart *chart, QList<Artist> artists,  QList<Artist> artists2);
};
#endif // MAINWINDOW_H
