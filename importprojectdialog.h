#ifndef IMPORTPROJECTDIALOG_H
#define IMPORTPROJECTDIALOG_H

#include <QDialog>
#include"projectconfigstruct.h"
#include<QtXml/QDomDocument>
#include"projectconfigstruct.h"
namespace Ui {
class ImportProjectDialog;
}

namespace ArmMcu
{

class ImportProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportProjectDialog(QWidget *parent = 0);
    ~ImportProjectDialog();
    inline ProjectConfig GetProjectConfig()
    {
        if(importOk==true)
        return this->config;
        else
        {
            ProjectConfig c;
            return c;
        }
    }
private slots:
    void on_browersPushButton_clicked();

    void on_ProductFilePushButton_clicked();

    void on_upButton_clicked();

    void on_downButton_clicked();

private:
    Ui::ImportProjectDialog *ui;
    ProjectConfig config;
    void ProductQbsFile(QString filepath);
    void ProductKeilFile(QString filepath);
    bool importOk=false;
    void outString(QString s);
    inline QStringList DefineProduct(QString dstr)
    {
        int i=0,j=0;
        QStringList r;
        char t[255];
        memset(t,0,sizeof(t));
        std::string dstrs=dstr.toStdString();
        for(;i<dstr.length();i++)
        {
            if(dstrs.at(i)==',')
            {
                j=0;
                r<<tr(t);
                memset(t,0,sizeof(t));

            }
            else
                t[j++]=dstrs.at(i);
        }
        r<<tr(t);
        return r;
    }
    inline QStringList IncludepathProduct(QString dstr)
    {
        int i=0,j=0;
        QStringList r;
        char t[255];
        memset(t,0,sizeof(t));
        std::string dstrs=dstr.toStdString();
        for(;i<dstr.length();i++)
        {
            if(dstrs.at(i)==';')
            {
                j=0;
                r<<tr(t);
                memset(t,0,sizeof(t));

            }
            else
                t[j++]=dstrs.at(i);
        }
        r<<tr(t);
        return r;
    }
};
}
#endif // IMPORTPROJECTDIALOG_H
