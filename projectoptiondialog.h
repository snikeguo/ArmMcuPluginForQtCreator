#ifndef PROJECTOPTIONDIALOG_H
#define PROJECTOPTIONDIALOG_H

#include <QDialog>
#include"projectconfigstruct.h"
#include<QMessageBox>
namespace Ui {
class ProjectOptionDialog;
}
namespace ArmMcu {


class ProjectOptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectOptionDialog(QWidget *parent = 0);
    ~ProjectOptionDialog();

private slots:
    void on_ComIncludeBrowersPushButton_clicked();

    void on_ComIncludeAddPushButton_clicked();

    void on_ComDefineAddSymbolsPushButton_clicked();

    void on_ApplyPushButton_clicked();

    void on_LdBrowersPushButton_clicked();

    void on_LdAddPushButton_clicked();
   // void on_ComIncludeListWidget_customContextMenuRequested(const QPoint &pos);

    void on_OkPushButton_clicked();

private:
    Ui::ProjectOptionDialog *ui;
    ProjectConfig pc;
    QString CompileAndLinkShareFieldString;
    void WriteQbsFile();
public slots:
    void onProjectConfigChanged(ProjectConfig &config);

};
}
#endif // PROJECTOPTIONDIALOG_H
