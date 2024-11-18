#ifndef ACTIVITYREPOSITORY_H
#define ACTIVITYREPOSITORY_H

#include "qdatetime.h"
#include <QList>

struct Activity{
    int id;
    QString title;
    QString type;
    QDate date;
    QTime startHour;
    QTime endHour;
    QString recursive;
    int repCount;
    bool repeated = false;

    explicit Activity();

    Activity(const Activity &other) {
        id = other.id;
        title = other.title;
        type = other.type;
        date = other.date;
        startHour = other.startHour;
        endHour = other.endHour;
        recursive = other.recursive;
        repCount = other.repCount;
    }

    Activity(int id, const QString &title, const QString &type, const QDate &date, const QTime &startHour, const QTime &endHour, const QString &recursive);
};

class ActivityRepository {
public:
    ActivityRepository();

    const QList<Activity> &getActivities() const;

    QList<Activity> getActivities(const QDate &date) const;

    Activity getActivity(int id) const;

    QList<QString> addActivity(Activity activity);

    QList<QString> editActivity(Activity &activity);

    void removeActivity(const Activity &activity); //int id

    void refresh();

    QList<QString> checkOverlays(const Activity &activity);

    QList<QString> isValid(Activity a);

private:
    QList<Activity> activities;

    int getIndexFromId(int id);

    QList<QString> addActivity(Activity activity, QList<QString> errors);

    QList<QString> editActivity(Activity &activity, QList<QString> errors);
};

#endif // ACTIVITYREPOSITORY_H
