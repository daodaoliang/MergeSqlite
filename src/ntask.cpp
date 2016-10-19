#include "ntask.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>

NTask::NTask(QObject *parent):
    QThread(parent)
{
}

void NTask::run()
{
    sleep(2);
    foreach(QString itemPath, dbList){
        emit doneItem(tr("正在合并:%1").arg(itemPath));
        database = QSqlDatabase::addDatabase("QSQLITE");
        QStringList macList;
        QString dbFilePath = itemPath;
        database.setDatabaseName(dbFilePath);
        qDebug()<<dbFilePath;
        if(database.open()){
            QSqlQuery tempQuery(database);
            tempQuery.exec(tr("select mac from macdata;"));
            while (tempQuery.next()) {
                macList.append(tempQuery.value(0).toString().trimmed());
            }
        }
        qDebug()<<database.lastError().text();
        database.close();
        // 合并进数据库
        database.setDatabaseName(dbName);
        if(database.open()){
            foreach (QString macItem, macList) {
                QSqlQuery tempQuery(database);
                qDebug()<<tempQuery.exec(tr("insert into macdata (`mac`) values ('%1');").arg(macItem));
            }
        }
        database.close();
        emit doneItem(tr("合并完成:%1").arg(itemPath));
    }
    emit resultDone();
}
