#include "importprojectdialog.h"
#include "ui_importprojectdialog.h"
#include<QFileDialog>
#include<QFile>
#include<QMessageBox>
#include<QThread>
#ifdef Q_OS_WIN
    #if _MSC_VER >= 1600
    #pragma execution_character_set("utf-8")
    #endif
#endif
using namespace ArmMcu;

ImportProjectDialog::ImportProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportProjectDialog)
{
    ui->setupUi(this);

}

ImportProjectDialog::~ImportProjectDialog()
{
    delete ui;
}

void ImportProjectDialog::on_browersPushButton_clicked()
{
    QString fs= QFileDialog::getOpenFileName(this,tr("open cube file"),tr(""),tr("ProjectFile(*.uvproj *uvprojx *.qbs)"));
    ui->cubeFileLineEdit->setText(fs);
    if(!QFile::exists(fs))
    {
        return;
    }
}

void ImportProjectDialog::on_ProductFilePushButton_clicked()
{

    if(!QFile::exists(ui->cubeFileLineEdit->text()))
    {
        QMessageBox::warning(this,tr("error"),tr("file no exist!"),QMessageBox::StandardButton::Close);
    }
    if(importOk==true)
    {
        QMessageBox::StandardButton sb;
        sb=QMessageBox::question(this,tr("question"),tr("当前已经有了一个工程，请问您要导入新的工程吗？如果导入，当前工程自动退出管理。"),QMessageBox::Yes|QMessageBox::Cancel);
        if(sb==QMessageBox::Cancel)
            return;
        importOk=false;
    }
    QString filepath=ui->cubeFileLineEdit->text();
    QFileInfo ff(filepath);
    if(ff.suffix().toLower()==tr("qbs"))
    {
        ProductQbsFile(filepath);
    }
    else
    {
        ProductKeilFile(filepath);

    }
}

void ImportProjectDialog::on_upButton_clicked()
{
    if(ui->stackedWidget->currentIndex()==0)
        return;
    else
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);

}

void ImportProjectDialog::on_downButton_clicked()
{
    if(ui->stackedWidget->currentIndex()==(ui->stackedWidget->count()-1))
    {
        QMessageBox::information(this,tr("information"),tr("MCU Project import Success！！"));
        this->hide();
        return;
    }
    else
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);

}
void ImportProjectDialog::ProductQbsFile(QString filepath)
{
    qDebug()<<"this qbs Thread id="<<this->thread()->currentThreadId();
}
void ImportProjectDialog::outString(QString s)
{
    // qDebug()<<s;
}
void ImportProjectDialog::ProductKeilFile(QString filepath)
{
    /*QFile f(filepath);
    if(f.exists()==false)
    {
        outString("文件不存在");
    }
    QDomDocument document;
    QString error;
    int row,column;
    document.setContent(&f,false,&error,&row,&column);
    QDomElement root = document.documentElement();
    QDomNodeList list=root.childNodes();
    int count = list.count();
    KeilVersion kv=NoKeil;
    for(int i=0;i<count-1;i++)
    {
        QDomNode dom_node = list.item(i);

        QDomElement element = dom_node.toElement();

        if(element.nodeName()=="SchemaVersion" && //keil4
                element.text()=="1.1"      )
        {
            outString("this is a keil4.");
            outString( element.nodeName());
            outString( element.text());
            kv=Keil4;
        }
        else if(element.nodeName()=="SchemaVersion" && //keil4
                element.text()=="2.1"      )
        {
            outString("this is a keil5.");
            outString( element.nodeName());
            outString( element.text());
            kv=Keil5;
        }
    }*/

    QFile f(filepath);
    if(f.exists()==false)
    {
        this->outString(tr("file no exit"));
    }
    QDomDocument document;
    QString error;
    int row,column;
    document.setContent(&f,false,&error,&row,&column);
    QDomElement root = document.documentElement();
    QDomNodeList list=root.childNodes();
    int count = list.count();
    config.ProjectFilePath=filepath;
    for(int i=0;i<count;i++)
    {
        QDomNode dom_node = list.item(i);
        QDomElement element = dom_node.toElement();
        if(element.nodeName()==tr("Targets"))
        {
            QDomNodeList Targets=element.childNodes();
            QDomNode Target=Targets.item(0);
            QDomElement TargetName=Target.childNodes().item(0).toElement();
            outString(tr("TargetName=")+TargetName.text());
            config.TargetName=TargetName.text();

            QDomNode TargetOption=Target.childNodes().item(3);
            QDomNode TargetCommonOption=TargetOption.childNodes().item(0);
            QDomNode Device=TargetCommonOption.childNodes().item(0);
            outString(tr("Device=")+Device.toElement().text());
            config.Device=Device.toElement().text();
            QDomNode Vendor=TargetCommonOption.childNodes().item(1);
            outString(tr("Vendor=")+Vendor.toElement().text());
            config.Vendor=Vendor.toElement().text();
            QDomNode Cpu=TargetCommonOption.childNodes().item(2);
            outString(tr("Cpu=")+Cpu.toElement().text());


            QDomNode TargetArmAds=TargetOption.childNodes().item(5);
            QDomNode ArmAdsMisc=TargetArmAds.childNodes().item(0);
            QDomNode CpuType=ArmAdsMisc.childNodes().item(27);
            outString(tr("CpuType=")+CpuType.toElement().text());
            config.CpuType=CpuType.toElement().text();

            QDomNode OnChipMemories=ArmAdsMisc.childNodes().item(27+35);
            QDomNode IRAM=OnChipMemories.childNodes().item(6);
            QDomNode StartAddress=IRAM.childNodes().item(1);
            QDomNode Size=IRAM.childNodes().item(2);
            outString(tr("RAM StartAddress=")+StartAddress.toElement().text());
            outString(tr("RAM Size=")+Size.toElement().text());
            bool oe;
            config.IRAM[0].StartAddr=StartAddress.toElement().text().toULong(&oe,16);
            config.IRAM[0].Size=Size.toElement().text().toUInt(&oe,16);

            QDomNode IROM=OnChipMemories.childNodes().item(7);
            QDomNode ROMStartAddress=IROM.childNodes().item(1);
            QDomNode ROMSize=IROM.childNodes().item(2);
            outString(tr("ROM StartAddress=")+ROMStartAddress.toElement().text());
            outString(tr("ROM Size=")+ROMSize.toElement().text());
            config.IROM[0].StartAddr=ROMStartAddress.toElement().text().toUInt(&oe,16);
            config.IROM[0].Size=ROMSize.toElement().text().toUInt(&oe,16);

            QDomNode Cads=TargetArmAds.childNodes().item(1);
            QDomNode VariousControls=Cads.childNodes().item(15);
            QDomNode Define=VariousControls.childNodes().item(1);
            outString(tr("Define=")+Define.toElement().text());
            config.Defines=DefineProduct( Define.toElement().text());

            QDomNode IncludePath=VariousControls.childNodes().item(3);
            outString(tr("IncludePath=")+IncludePath.toElement().text());
            config.IncludePath=IncludepathProduct(IncludePath.toElement().text());
            //clean space!!!
            for(int c=0;c<config.IncludePath.count();c++)
            {
                for(int g=0;g<config.IncludePath.at(c).size();g++)
                {
                    config.IncludePath.at(c).trimmed();
                }
            }
            QDomNode LDads=TargetArmAds.childNodes().item(3);
            QDomNode TextAddressRange=LDads.childNodes().item(6);
            outString(tr("TextAddressRange=")+TextAddressRange.toElement().text());
            QDomNode DataAddressRange=LDads.childNodes().item(6);
            outString(tr("DataAddressRange=")+DataAddressRange.toElement().text());

            QDomNode Groups=Target.childNodes().item(4);
            int group_count=Groups.childNodes().count();
            for(int j=0;j<group_count;j++)
            {
                QDomNode Group=Groups.childNodes().item(j);
                QDomNode GroupName=Group.childNodes().item(0);
                outString(tr("GroupName=")+GroupName.toElement().text());
                QDomNode Files=Group.childNodes().item(1);

                int files_count=Files.childNodes().count();

                QStringList fi;
                for(int k=0;k<files_count;k++)
                {
                    QDomNode File= Files.childNodes().item(k);
                    QDomNode FileName=File.childNodes().item(0);
                    QDomNode FilePath=File.childNodes().item(2);
                    outString(tr("FileName=")+FileName.toElement().text()+tr("  Path=")+FilePath.toElement().text());
                    fi<<FilePath.toElement().text();
                }
                config.group.insert(GroupName.toElement().text(),fi);
            }
        }
    }

    ui->textEdit->append(tr("Project Path:")+config.ProjectFilePath+tr("\n")+
                         tr("TargetName:")+config.TargetName+tr("\n")+
                         tr("Device:")+config.Device+tr("\n")+
                         tr("Vendor:")+config.Vendor+tr("\n")+
                         tr("CpuType:")+config.CpuType);
    for(int inc=0;inc<config.Defines.count();inc++)
    {
        ui->textEdit->append(tr("Define:")+config.Defines.at(inc));
    }
    for(int inc=0;inc<config.IncludePath.count();inc++)
    {
        ui->textEdit->append(tr("IncludePath:")+config.IncludePath.at(inc));
    }
    for(auto it=config.group.begin();it!=config.group.end();it++)
    {
        ui->textEdit->append(tr("group:")+it.key());
        for(int gc=0;gc<it.value().count();gc++)
        {
            ui->textEdit->append(it.value().at(gc));
        }
    }
    ui->textEdit->append(tr("Producted Keil Project Success!"));
    importOk=true;
}
