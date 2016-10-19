#ifndef NTASK_H
#define NTASK_H

#include <QThread>
#include <QObject>
#include <QSqlDatabase>

class NTask: public QThread
{
    Q_OBJECT
public:
    NTask(QObject *parent =NULL);

public:
    void run();

signals:
    void resultDone();
    void doneItem(QString item);

public:
    QStringList dbList;
    QString dbName;
    QSqlDatabase database;
};

#endif // NTASK_H
