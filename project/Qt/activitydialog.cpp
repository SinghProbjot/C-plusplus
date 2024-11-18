#include "activitydialog.h"
#include <QMessageBox>
#include "ui_activitydialog.h"
#include <QtSql>
#include <qDebug>

ActivityDialog::ActivityDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActivityDialog)
{
    ui->setupUi(this);

    ui->label_6->hide();
    ui->repeatTimecomboBox->hide();
}

ActivityDialog::ActivityDialog(const Activity &activity, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActivityDialog),
    activity(activity)
{
    ui->setupUi(this);

    ui->titleTextEdit->setText(activity.title);
    ui->typeComboBox->setCurrentIndex(activity.type == "Evento" ? 1 : 0);
    ui->startHourTimeEdit->setTime(activity.startHour);
    ui->endHourTimeEdit->setTime(activity.endHour);
    ui->repeatComboBox->setCurrentIndex(activity.recursive == "Mai" ? 0 : 1);
    ui->label_6->hide();
    ui->repeatTimecomboBox->hide();
}

ActivityDialog::~ActivityDialog()
{
    delete ui;
}


const Activity &ActivityDialog::getActivity() const {
    return activity;
}

void ActivityDialog::on_buttonBox_accepted()
{
    if(ui->endHourTimeEdit->time() == QTime(00,00))
    {
        activity.endHour = QTime(23, 59);
    }
    else
    {
        activity.endHour = ui->endHourTimeEdit->time();
    }
    activity.title = ui->titleTextEdit->toPlainText();
    activity.type = ui->typeComboBox->currentText();
    activity.startHour = ui->startHourTimeEdit->time();
    activity.recursive = ui->repeatComboBox->currentText();
    activity.repCount = ui->repeatTimecomboBox->currentText().toInt();
}


void ActivityDialog::on_repeatComboBox_currentIndexChanged(int index)
{
    if(ui->repeatComboBox->currentIndex() == 1)
        {
            ui->label_6->show();
            ui->repeatTimecomboBox->show();
        }
    else
       {
            ui->label_6->hide();
            ui->repeatTimecomboBox->hide();
       }
}

