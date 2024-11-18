#ifndef ACTIVITYDIALOG_H
#define ACTIVITYDIALOG_H

#include "activityrepository.h"
#include <QDialog>

namespace Ui {
class ActivityDialog;
}

class ActivityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ActivityDialog(QWidget *parent = nullptr);
    ActivityDialog(const Activity &activity, QWidget *parent = nullptr);
    ~ActivityDialog();

    const Activity &getActivity() const;

private slots:
    void on_buttonBox_accepted();

    void on_repeatComboBox_currentIndexChanged(int index);

private:
    Ui::ActivityDialog *ui;

    Activity activity;
};

#endif // ACTIVITYDIALOG_H
