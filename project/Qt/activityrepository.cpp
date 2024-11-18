#include "activityrepository.h"
#include <QMessageBox>
#include <QtSql>
#include<QDebug>

Activity::Activity() = default;

Activity::Activity(int id, const QString &title, const QString &type, const QDate &date,
                   const QTime &startHour, const QTime &endHour,
                   const QString &recursive)
    : id(id), title(title), type(type), date(date), startHour(startHour), endHour(endHour),
    recursive(recursive) {}

ActivityRepository::ActivityRepository() : activities() {}

Activity ActivityRepository::getActivity(int id) const {
    return activities[id];
}

QList<QString> ActivityRepository::addActivity(Activity activity) {
    QList<QString> errors = isValid(activity);
    for(int i = 0; i < activities.size(); i++){
        if(activities[i].id != activity.id && activities[i].title == activity.title && activities[i].repeated == false)
        {
           errors.append("Already existing an activity with the same name");
           break;
        }
    }

    if (errors.isEmpty()) {
        Activity tmp(activity);
        for(int i = 1; i < activity.repCount; i++) {
            tmp = Activity(tmp);
            tmp.date = tmp.date.addYears(1);
            errors.append(isValid(tmp));
        }

        if (errors.isEmpty()) {
            activity.repeated = false;
            errors = addActivity(activity, errors);

            tmp = Activity(activity);
            for(int i = 1; i < activity.repCount; i++) {
                tmp = Activity(tmp);
                tmp.date = tmp.date.addYears(1);
                tmp.repeated = true;
                tmp.repCount = activity.repCount;
                errors = addActivity(tmp, errors);
            }
        }

        if (!errors.isEmpty()) {
            QString ers = "";
            for(QString e : errors) {
                ers += "\n" + e;
            }
            errors.clear();
            errors.append(ers);
        }
    }

    return errors;
}

QList<QString> ActivityRepository::addActivity(Activity activity, QList<QString> errors) {

        QSqlDatabase db = QSqlDatabase::database("db");
        if (!db.open()) {
            qDebug("Connessione db fallita");
            errors.append("Connessione db fallita");
            return errors;
        }
        QSqlQuery q("", db);
        q.prepare("INSERT into activities(title,type,date,startHour,endHour,recursive) values (:title, :type, :date, :startHour, :endHour, :recursive)");
        q.bindValue(":title", activity.title);
        q.bindValue(":type", activity.type);
        q.bindValue(":date", activity.date.toString("ddMMyyyy"));
        q.bindValue(":startHour", activity.startHour.toString("HHmm"));
        q.bindValue(":endHour", activity.endHour.toString("HHmm"));
        q.bindValue(":recursive", activity.recursive);

        if(!q.exec())
        {
            qDebug()<< q.lastError().text();
            errors.append(q.lastError().text());
            return errors;
        }

        db.close();

        refresh();

    return errors;
}

QList<QString> ActivityRepository::editActivity(Activity &activity){
    QList<QString> errors = isValid(activity);
    Activity real;
    QMessageBox msg;
    QList<int> idx;
    QList<QDate> dates;
    int rep = 0, index;
    //cerco la prima attivita della catena

     for(int i = 0; i< activities.size();i++) {
            if(activities[i].id == activity.id){
                real = Activity(activities[i]);
                rep = activities[i].repCount;
                index = i;
                break;
            }
     }
     //salvo gli id
     for(int i = 0; i< activities.size();i++) {
            if(activities[i].title == real.title && activities[i].type == real.type && activities[i].startHour == real.startHour && activities[i].endHour == real.endHour){
             idx.append(activities[i].id);
             dates.append(activities[i].date);
            }
     }
     //checked
    if (errors.isEmpty()){
        if(real.recursive == "Mai")
        {
            errors = editActivity(activity,errors);

            if(activity.recursive != "Mai")
            {

                //controllo se posso editare
                activities[idx.at(0)].repeated = false;
                Activity tmp(activity);
                for(int i = 1; i < activity.repCount; i++) {
                    tmp = Activity(tmp);
                    tmp.date = tmp.date.addYears(1);
                    errors.append(isValid(tmp));
                }
                 //edito
                    tmp = Activity(activity);
                    for(int i = 1; i < activity.repCount; i++) {
                        tmp = Activity(tmp);
                        tmp.date = tmp.date.addYears(1);
                        tmp.repeated = true;
                        errors = addActivity(tmp, errors);
                    }
            }
        }

        else{
                if(activity.recursive !="Mai"){

                    if(real.repCount != activity.repCount)
                        {
                           int todelete = idx.size() - activity.repCount;
                           QSqlDatabase db = QSqlDatabase::database("db");
                           if (!db.open()) {
                               qDebug("Connessione db fallita");
                               errors.append("Connessione db fallita");
                               return errors;
                           }
                           QSqlQuery q("", db);
                           for(int i = idx.size() - 1; i >= (idx.size()- todelete); i--)
                           {
                               //elimino

                               q.prepare("DELETE FROM activities WHERE id = :id");
                               q.bindValue(":id", idx.at(i));
                               if(!q.exec())
                               {
                                   errors.append(q.lastError().text());
                                   db.close();
                                   return errors;
                               }
                           }
                           refresh();
                        }
                            else{
                                    //controllo se posso editare
                                    Activity tmp(real);
                                   for(int i = 1; i < rep; i++) {
                                      // tmp = Activity(real);
                                       tmp.startHour = activity.startHour;
                                       tmp.endHour = activity.endHour;
                                       tmp.date = dates.at(0);
                                       errors.append(isValid(tmp));
                                       tmp.date = tmp.date.addYears(1);
                                   }
                                    if (errors.isEmpty())
                                    {
                                        for (int i = 0;i< idx.size();i++) {
                                            activity.id = idx.at(i);
                                            activity.date = dates.at(i);
                                            errors = editActivity(activity,errors);
                                        }
                                    }
                                }


                    }
                else {
                        errors = editActivity(activity,errors);
                        QSqlDatabase db = QSqlDatabase::database("db");

                        if (!db.open()) {
                            qDebug("Connessione db fallita");
                            errors.append("Connessione db fallita");
                            return errors;
                        }
                        QSqlQuery q("", db);
                        //elimino
                        for(int i=1;i<idx.size();i++){
                            q.prepare("DELETE FROM activities WHERE id = :id");
                            q.bindValue(":id", idx.at(i));
                            if(!q.exec())
                            {
                                errors.append(q.lastError().text());
                                db.close();
                                return errors;
                            }
                        }
                        refresh();
                    }
            }
    }
    if (!errors.isEmpty()) {
            QString ers = "";
            for(const QString &e : errors) {
                ers += "\n" + e;
            }
            errors.clear();
            errors.append(ers);
        }
    return errors;
}


QList<QString> ActivityRepository::editActivity(Activity &activity, QList<QString> errors) {
    QSqlDatabase db = QSqlDatabase::database("db");
    if (!db.open()) {
        qDebug("Connessione db fallita");
        errors.append("Connessione db fallita");
        return errors;
    }
    QSqlQuery q("", db);
    q.prepare("UPDATE activities SET title =:title, type = :type, date = :date, startHour = :startHour, endHour = :endHour, recursive = :recursive WHERE id = :id");
    q.bindValue(":id", activity.id);
    q.bindValue(":title", activity.title);
    q.bindValue(":type", activity.type);
    q.bindValue(":date", activity.date.toString("ddMMyyyy"));
    q.bindValue(":startHour", activity.startHour.toString("HHmm"));
    q.bindValue(":endHour", activity.endHour.toString("HHmm"));
    q.bindValue(":recursive", activity.recursive);

    if(!q.exec())
    {
        qDebug()<< q.lastError().text();
        return errors;
    }

    db.close();

    refresh();
   return errors;
}


void ActivityRepository::refresh() {
    QSqlDatabase db = QSqlDatabase::database("db");
    if (!db.open()) {
        qDebug("database error");
        return;
    }
    QSqlQuery q("", db);
    if(!q.exec("SELECT * FROM activities ORDER BY date,startHour,type"))
    {
        qDebug()<<q.lastError().text();
        if(!q.exec("CREATE TABLE activities(id INTEGER PRIMARY KEY AUTOINCREMENT, title varchar(255),type varchar(255),date varchar(255),startHour varchar(255), endHour varchar(255), recursive varchar(255) )"))
        {
            qDebug()<<q.lastError().text();
            return;
        }
        return;
    }
    activities.clear();
    while(q.next())
    {
        Activity a;
        a.id = q.value("id").toInt();
        a.title = q.value("title").toString();
        a.type = q.value("type").toString();
        a.date = QDate::fromString( q.value("date").toString(), "ddMMyyyy");
        a.startHour = QTime::fromString( q.value("startHour").toString(), "HHmm");
        a.endHour = QTime::fromString( q.value("endHour").toString(), "HHmm");
        a.recursive = q.value("recursive").toString();
        activities.append(a);
    }
    db.close();
}

const QList<Activity> &ActivityRepository::getActivities() const {
    return activities;
}

QList<Activity> ActivityRepository::getActivities(const QDate &date) const {
    QList<Activity> _activities;

    for (const Activity &activity : activities) {
        if (activity.date == date) {
            _activities.append(activity);
        }
    }
    return _activities;
}

void ActivityRepository::removeActivity(const Activity &activity) {
    QSqlDatabase db = QSqlDatabase::database("db");
    if (!db.open()) {
        qDebug("Database error.'");
        return;
    }
    if(activity.recursive == "Mai"){
        QSqlQuery q("", db);
        q.prepare("DELETE FROM activities WHERE id = :id");
        q.bindValue(":id", activity.id);
        if(!q.exec())
        {
            qDebug()<<q.lastError().text();
            db.close();
            return;
        }
    }

    else{
        QSqlQuery q("", db);
        q.prepare("DELETE FROM activities WHERE title = :title AND startHour = :startHour AND type = :type");
        q.bindValue(":title", activity.title);
        q.bindValue(":startHour", activity.startHour.toString("HHmm"));
        q.bindValue(":type", activity.type);
        if(!q.exec())
        {
            qDebug()<<q.lastError().text();
            db.close();
            return;
        }
    }
    db.close();
    refresh();
}


QList<QString> ActivityRepository::checkOverlays(const Activity &activity) {
    QList<QString> errors;

    if(activity.type != "Attivita")
        return errors;

    QString msg = "";

    for (int i = 0; i < activities.size(); i++) {
        if (activities[i].id == activity.id)
            continue;
        if (activities[i].title == activity.title && activities[i].repeated)
            continue;
        if(activities[i].type == "Attivita" && activity.date == activities[i].date) {
            if(
                (activity.startHour >= activities[i].startHour && activity.startHour < activities[i].endHour) ||
                    (activity.endHour > activities[i].startHour && activity.endHour <= activities[i].endHour) ||
                    (activity.endHour > activities[i].startHour && activity.endHour <= activities[i].endHour) ||
                    (activities[i].startHour >= activity.startHour && activities[i].startHour < activity.endHour) ||
                    (activities[i].endHour > activity.startHour && activities[i].endHour <= activity.endHour) ||
                    (activity.startHour <= activities[i].startHour && activity.endHour >= activities[i].endHour)
            ) {
                msg += "--> " + activities[i].title + " ";
                msg += "from " + activities[i].startHour.toString("HH:mm") +
                        " to " + activities[i].endHour.toString("HH:mm") +
                        " on date " + activities[i].date.toString("dd/MM/yyyy");
                msg += "\n";
            }
        }
    }

    if (msg != "")
        errors.append("ALREADY EXISTING ACTIVITIES IN THIS TIME RANGE! \n " + msg);

    return errors;
}


QList<QString> ActivityRepository::isValid(Activity a) {
    QList<QString> errors;

    if(a.title.trimmed() == "")
        errors.append("Title cannot be empty.");

    if(a.endHour < a.startHour)
        errors.append("Start hour can't be grater than end hour.");


    errors.append(checkOverlays(a));

     return errors;
}
