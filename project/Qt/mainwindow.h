#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "activityrepository.h"
#include "tablemodel.h"
#include <QMainWindow>

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
    void on_tableView_clicked(const QModelIndex &index);

    void on_addButton_clicked();

    void on_editButton_clicked();

    void on_deleteButton_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void on_calendarWidget_currentPageChanged(int year, int month);

    void on_calendarWidget_selectionChanged();

private:
    Ui::MainWindow *ui;

    TableModel *activityModel;

    ActivityRepository *repository;

    QDate filter;

    void refreshTableView();
    void filterTableView(QString date);
};
#endif // MAINWINDOW_H
