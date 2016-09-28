#ifndef DEBUGGERVIEWDIALOG_H
#define DEBUGGERVIEWDIALOG_H

#include <QDialog>
#include<QTableWidgetItem>
namespace Ui {
class DebuggerViewDialog;
}
namespace ArmMcu {


class DebuggerViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DebuggerViewDialog(QWidget *parent = 0);
    ~DebuggerViewDialog();

private:
    Ui::DebuggerViewDialog *ui;
};
}
#endif // DEBUGGERVIEWDIALOG_H
