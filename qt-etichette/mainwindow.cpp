#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&this->netManager, &QNetworkAccessManager::finished, this, &MainWindow::handle_network_data);
    this->netManager.get(QNetworkRequest(this->url.arg(this->uniFilename)));
    this->netManager.get(QNetworkRequest(this->url.arg(this->emiFilename)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handle_network_data(QNetworkReply *networkReply)
{
    QUrl url = networkReply->url();
    if (networkReply->error() == QNetworkReply::NoError) {

        QByteArray response(networkReply->readAll());

        if (url.fileName() == this->uniFilename) {
            fill_list(this->uniArtists, response);

            uniModel = new TableModel(uniArtists, this);
            ui->tableView_3->setModel(uniModel);

            QChart *chart = new QChart();
            chart->legend()->setVisible(false);
            chart->setTitle("Artisti Universal");
            draw_chart(chart, uniArtists);
            this->ui->graphicsView_2->setChart(chart);
            this->ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);
        } else if (url.fileName() == this->emiFilename) {
            fill_list(this->emiArtists, response);

            emiModel = new TableModel(emiArtists, this);
            ui->tableView_4->setModel(emiModel);

            QChart *chart = new QChart();
            chart->legend()->setVisible(false);
            chart->setTitle("Artisti EMI");
            draw_chart(chart, emiArtists);
            this->ui->graphicsView_3->setChart(chart);
            this->ui->graphicsView_3->setRenderHint(QPainter::Antialiasing);
        } else {
            throw new std::exception;
        }

        if (!uniArtists.isEmpty() && !emiArtists.isEmpty()) {
            QChart *chart = new QChart();
            chart->legend()->setVisible(false);
            chart->setTitle("Artisti per etichetta");
            draw_chart(chart, uniArtists, emiArtists);
            this->ui->graphicsView->setChart(chart);
            this->ui->graphicsView->setRenderHint(QPainter::Antialiasing);
        }
    }

    networkReply->deleteLater();
}


void MainWindow::draw_chart(QChart *chart, QList<Artist> artists)
{
    QBarSet *set0 = new QBarSet("artists"); // Artista per ogni lettera
    QStringList categories; // Lista per legenda asse X

    for (int i = 0; i <= 'Z'-'A'; i++) {
        QChar currentLetter;
        currentLetter.setCell(((QChar)'A').unicode() + i);

        categories << QString(currentLetter);
        *set0 << 0;
    }

    for (int i = 0; i < artists.size(); i++) {
        if (!artists.at(i).name.isEmpty()) {
            QChar charFound = artists.at(i).name.at(0);

            if (charFound >= 'A' && charFound <= 'Z') {
                int index = (int)(charFound.unicode() - 'A');
                set0->replace(index, set0->at(index ) + 1);
            }
        }
    }

    int max = 0;
    for (int i = 0; i < set0->count(); i++) {
        max = (set0->at(i) > max) ? set0->at(i) : max;
    }

    QBarSeries *series = new QBarSeries(); // Contiene i set
    series->append(set0);
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTruncateLabels(false);
    //axisX->setRange("A", "Z");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, max);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
}

void MainWindow::draw_chart(QChart *chart, QList<Artist> artists, QList<Artist> artists2)
{
    QBarSet *set0 = new QBarSet("artists"); // Artista per ogni lettera
    QStringList categories; // Lista per legenda asse X
    categories<<"Universal"<<"EMI";
    *set0 << artists.size() << artists2.size();

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTruncateLabels(false);
    //axisX->setRange("A", "Z");

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, std::max(artists.size() , artists2.size()));

    QBarSeries *series = new QBarSeries(); // Contiene i set
    series->append(set0);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->setAnimationOptions(QChart::SeriesAnimations);
}

void MainWindow::fill_list(QList<Artist> &artists, QByteArray response)
{
    artists.clear();

    QTextStream in(&response);
    while(!in.atEnd()){
        QString line = in.readLine();
        QStringList row = line.split(' ');

        Artist a;
        a.url = row.at(0);
        a.name = row.at(1);
       // a.name.remove("_");

        if (a.name.at(0) == '_')//tolgo underscore iniziali e finali
            a.name = a.name.right(a.name.length()-1);
        if (a.name.at(a.name.length()-1) == '_')
            a.name = a.name.left(a.name.length()-1);

       if(a.name.contains("_"))//underscore tra nome e cognome
           a.name = a.name.replace("_", " ");

        if (a.url.startsWith('"')) {
           a.url = a.url.last(a.url.length()-1);
        }

        if (a.url.endsWith('"')) {
           a.url = a.url.first(a.url.length()-1);
        }
        a.url = QUrl::fromPercentEncoding(a.url.toUtf8());
        if(a.url.isEmpty())
            a.url = "Link non disponibile";

        artists.append(a);
    }
}

void MainWindow::on_tableView_4_clicked(const QModelIndex &index)
{
    if (index.column() != 1) {
        return;
    }

    Artist artist = emiArtists.at(index.row());
    QDesktopServices::openUrl(QUrl(this->itemUrl + artist.url));
}

void MainWindow::on_tableView_3_clicked(const QModelIndex &index)
{
    if (index.column() != 1) {
        return;
    }

    Artist artist = uniArtists.at(index.row());
    QDesktopServices::openUrl(QUrl(this->itemUrl + artist.url));
}
