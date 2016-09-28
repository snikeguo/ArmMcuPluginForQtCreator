#include "debuggerviewdialog.h"
#include "ui_debuggerviewdialog.h"
#ifdef Q_OS_WIN
    #if _MSC_VER >= 1600
    #pragma execution_character_set("utf-8")
    #endif
#endif
using namespace ArmMcu;
DebuggerViewDialog::DebuggerViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebuggerViewDialog)
{
    ui->setupUi(this);
    //0 Thread id
    //1 Thread name
    //2 Thread state
    //3 Thread priority
    //4 Thread Total stack
    //5 Thread free stack
    //6 Thread Surplus stack
    ui->threadTableWidget->setColumnCount(7);
    QStringList headers;
    headers<<tr("id")
          <<tr("name")
         <<tr("state")
        <<tr("priority")
       <<tr("Total stack")
      <<tr("free stack")
     <<tr("Surplus stack");
    //this is a debug
    ui->threadTableWidget->setHorizontalHeaderLabels(headers);
    int rowIndex = ui->threadTableWidget->rowCount();
    ui->threadTableWidget->setRowCount(rowIndex+1);
    ui->threadTableWidget->setItem(0, 0, new QTableWidgetItem(tr("0")));
    ui->threadTableWidget->setItem(0, 1, new QTableWidgetItem(tr("idle")));
    ui->threadTableWidget->setItem(0, 2, new QTableWidgetItem(tr("R")));
    ui->threadTableWidget->setItem(0, 3, new QTableWidgetItem(tr("255")));
    ui->threadTableWidget->setItem(0, 4, new QTableWidgetItem(tr("128")));
    ui->threadTableWidget->setItem(0, 5, new QTableWidgetItem(tr("112")));
    ui->threadTableWidget->setItem(0, 6, new QTableWidgetItem(tr("16")));
    //debug end
}

DebuggerViewDialog::~DebuggerViewDialog()
{
    delete ui;
}
