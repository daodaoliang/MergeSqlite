#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <qdebug.h>
#include "ntask.h"
#include "nbasewaitdialog.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->textBrowser->insertPlainText(tr("请点击左边按钮选择所有的数据库文件;\r\n"));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(
                this,
                "选择所有的需要合并的文件",
                "./",
                "数据库文件 (*.db)");
    ui->listWidget->clear();
    foreach (QString itemName, files) {
        ui->listWidget->addItem(itemName);
    }
    ui->textBrowser->insertPlainText(tr("你已经选择了%1个数据库文件;\r\n").arg(files.size()));
    ui->textBrowser->insertPlainText(tr("请输入合并后的数据库的数据库名字(例如0030021610132);\r\n"));
}

void Widget::on_pushButton_2_clicked()
{
    if(ui->listWidget->count()<=0){
        ui->textBrowser->insertPlainText(tr("请先选择待合并的数据库文件;\r\n"));
        return;
    }
    NTask *taskThread = new NTask(nullptr);
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    if(ui->lineEdit->text().isEmpty()){
        ui->textBrowser->insertPlainText(tr("请输入合并后的数据库的数据库名字(例如0030021610132);\r\n"));
        taskThread->deleteLater();
        return;
    } else {
        QFile::remove(QApplication::applicationDirPath() + "/" + ui->lineEdit->text() + ".db");
        database.setDatabaseName(QApplication::applicationDirPath() + "/" + ui->lineEdit->text() + ".db");
        if(database.open()){
            QString createSql = tr("CREATE TABLE macdata(ID INTEGER PRIMARY KEY AUTOINCREMENT,mac text not null);");
            QSqlQuery tempQuery(database);
            tempQuery.exec(createSql);
        } else {
            ui->textBrowser->insertPlainText(database.lastError().text());
        }
        ui->textBrowser->insertPlainText(database.lastError().text());
        database.close();
        ui->textBrowser->insertPlainText(tr("创建合并数据库成功; \r\n"));
    }
    connect(taskThread,SIGNAL(doneItem(QString)),ui->textBrowser,SLOT(insertPlainText(QString)));
    while (ui->listWidget->count()>0) {
        // 任务分配
        QString taskName = ui->listWidget->takeItem(ui->listWidget->count()-1)->text();
        taskThread->dbList.append(taskName);
        taskThread->dbName = QApplication::applicationDirPath() + "/" + ui->lineEdit->text() + ".db";
    }
    taskThread->start();
    NBaseWaitDialog * waitDialog = new NBaseWaitDialog(this, NBaseWaitDialog::ANDROIDSTYLE);
    waitDialog->showWaitDialog();
    connect(taskThread,SIGNAL(resultDone()),waitDialog,SLOT(deleteLater()));
    connect(waitDialog,SIGNAL(destroyed(QObject*)),taskThread,SLOT(deleteLater()));
}
