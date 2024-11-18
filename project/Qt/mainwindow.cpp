#include "mainwindow.h"
#include "activitydialog.h"
#include "qmessagebox.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , repository(new ActivityRepository)
    , filter(QDate::currentDate())
{
    ui->setupUi(this);

    ui->calendarWidget->setSelectedDate(filter);
    repository->refresh();
    refreshTableView();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if (index.column() != 1) {
        return;
    }
}

//static void m(QString msg)
//{
//    QMessageBox msgBox;
//    msgBox.setText(msg);
//    msgBox.exec();
//}

void MainWindow::on_addButton_clicked()
{
    QMessageBox msg;
    if(filter < QDate::currentDate())
    {
        msg.setText("ACTIVITIES CAN NOT BE ADDED IN THE PAST");
        msg.exec();
    }
    else
    {
        ActivityDialog activityDialog;
        activityDialog.setWindowTitle("Add activity");
        do {
            if(activityDialog.exec() != QDialog::Accepted)
                break;

            Activity a = activityDialog.getActivity();
            a.date = filter;
            QList<QString> messages = repository->addActivity(a);
            if (!messages.isEmpty()) {
                msg.setText(messages.first());
                msg.exec();
                continue;
            }

            refreshTableView();

            break;
        } while(true);
    }
}


void MainWindow::on_editButton_clicked()
{
    const QModelIndexList indexes = ui->tableView->selectionModel()->selectedIndexes();
    if (indexes.isEmpty()) {
        return;
    }
    QModelIndex idx = indexes.first();
    ActivityDialog activityDialog(activityModel->getActivity(idx.row()));
    activityDialog.setWindowTitle("Edit activity");
    do {
        if(activityDialog.exec() != QDialog::Accepted)
            break;

        Activity a = activityDialog.getActivity();
        a.date = filter;
        QList<QString> messages = repository->editActivity(a);
        if (!messages.isEmpty()) {
            QMessageBox msg;
            msg.setText(messages.first());
            msg.exec();
            continue;
        }

        refreshTableView();

        break;
    } while(true);
}


void MainWindow::on_deleteButton_clicked()
{
    QMessageBox msgBox;
    const QModelIndexList indexes = ui->tableView->selectionModel()->selectedIndexes();
    if (indexes.isEmpty()) {
        return;
    }

    for (const QModelIndex &index : indexes) {
        repository->removeActivity(activityModel->getActivity(index.row()));  //  getActivity(index.row()).id);
    }

    refreshTableView();
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    filter = date;
    refreshTableView();
}

void MainWindow::refreshTableView()
{
    activityModel = new TableModel(repository->getActivities(filter), this);
    ui->tableView->setModel(activityModel);
}


