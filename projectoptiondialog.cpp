#include "projectoptiondialog.h"
#include "ui_projectoptiondialog.h"
#include<QFileDialog>
#include<QDir>
#include<QMessageBox>
#ifdef Q_OS_WIN
    #if _MSC_VER >= 1600
    #pragma execution_character_set("utf-8")
    #endif
#endif
using namespace ArmMcu;
ProjectOptionDialog::ProjectOptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectOptionDialog)
{
    ui->setupUi(this);

    /*!
  add item
*/
    QStringList items;
    items<<tr("Not use FPU")
        <<tr("Soft FPU")
       <<tr("Hard FPU");
    ui->FPUComboBox->addItems(items);

    items.clear();
    items<<tr("None(O0)")
        <<tr("Optimizate(O1)")
       <<tr("Optimizate More(O2)")
      <<tr("Optimizate Most(O3)")
     <<tr("Optimizate Size(Os)")
    <<tr("Optimizate Fast(Ofast)")
    <<tr("Optimizate Debug(Og)");
    ui->OptimizationComboBox->addItems(items);

    items.clear();
    items<<tr("No debug")
        <<tr("minimal information")
       <<tr("default debug")
      <<tr("extra debug");
    ui->DebugComboBox->addItems(items);

    items.clear();
    items<<tr("No warnnings")
        <<tr("All warnnings")
       <<tr("Extra warnnings");
    ui->comOptionWarnningComboBox->addItems(items);

    items.clear();
    items<<tr("C99,C89,ISO9899,1990")
        <<tr("ISO9899:1999 C99,ISO9899:199X,C9X")
       <<tr("ISO9899:2011 C11 C1X")
      <<tr("GNU90,GNU89")
     <<tr("GNU99,GNU9X")
    <<tr("GNU11,GNU1X");
    ui->CStandardComboBox->addItems(items);

    items.clear();
    items<<tr("C++983,C++03")
        <<tr("GNU++98,GNU++03")
       <<tr("C++11,C++0X")
      <<tr("GNU11,GNU0X");
    ui->CppStandardComboBox->addItems(items);

    items.clear();
    items<<tr("No Use C Library")
        <<tr("Use Nano C Library");
    ui->LinkCLibComboBox->addItems(items);
}

ProjectOptionDialog::~ProjectOptionDialog()
{
    delete ui;
}

void ProjectOptionDialog::on_ComIncludeBrowersPushButton_clicked()
{
    QString fn=QFileDialog::getExistingDirectory(this,tr("add include path"),tr(""),QFileDialog::ShowDirsOnly);
    ui->ComIncludeLineEdit->setText(fn);
}

void ProjectOptionDialog::on_ComIncludeAddPushButton_clicked()
{
    QDir d(ui->ComIncludeLineEdit->text());

    qDebug()<<d.absolutePath();
    if(d.exists()==false)
    {
        QMessageBox::warning(this,tr("error"),tr("this string isn't a dir!"));
        return;
    }
    ui->ComIncludeListWidget->addItem(ui->ComIncludeLineEdit->text());

}

void ProjectOptionDialog::on_ComDefineAddSymbolsPushButton_clicked()
{
    QString newd=ui->ComDefinedLineEdit->text();
    if((pc.Defines.contains(newd)==false) &&(newd.isEmpty()==false))
    {
        pc.Defines.append(newd);
        ui->ComDefinedListWidget->addItem(newd);
    }
}
void ProjectOptionDialog::onProjectConfigChanged(ProjectConfig &config)
{
    pc=config;
    this->setWindowTitle(tr("ProjectOption ----")+pc.ProjectFilePath);
    ui->ComDefinedListWidget->clear();
    ui->ComIncludeListWidget->clear();

    qDebug()<<tr("onProjectConfigChanged");
    qDebug()<<config. ProjectFilePath;
    qDebug()<<config.TargetName;
    qDebug()<<config.Device;
    qDebug()<<config.Vendor;
    qDebug()<<config.CpuType;
    qDebug()<<config.clockFrequency<<tr("Mhz");
    ui->ComDefinedListWidget->addItems(config.Defines);
    for(int ku=0;ku<config.Defines.count();ku++)
    {
        qDebug()<<config.Defines.at(ku);
    }

    ui->ComIncludeListWidget->addItems(config.IncludePath);
    for(int ku=0;ku<config.IncludePath.count();ku++)
    {
        qDebug()<<config.IncludePath.at(ku);

    }
    ui->ChipShowLabel->setText(pc.Device);
    ui->IROM1AddrlineEdit->setText(tr("0x")+QString::number(pc.IROM[0].StartAddr,16));
    ui->IROM1SizeLineEdit->setText(tr("0x")+QString::number(pc.IROM[0].Size,16));
    ui->IRAM1AddrLineEdit->setText(tr("0x")+QString::number(pc.IRAM[0].StartAddr,16));
    ui->IRAM1SizeLineEdit->setText(tr("0x")+QString::number(pc.IRAM[0].Size,16));
    qDebug()<<hex<<config.IROM[0].StartAddr;
    qDebug()<<hex<<config.IRAM[0].StartAddr;


    QFileInfo fi(pc.ProjectFilePath);
    QDir dt(fi.path());
    dt.cdUp();
    dt.mkdir(tr("ArmMcuForQtCreator"));
    dt.cd(tr("ArmMcuForQtCreator"));
    QFile qbsProject(dt.path()+tr("/")+fi.fileName()+tr(".qbs"));

    qbsProject.open(QIODevice::WriteOnly);
    QDataStream  in(&qbsProject);
    in<<tr("this is e eapfsadf").toUtf8();
    qbsProject.close();

    QFile qbsReadme(dt.path()+tr("/readme.txt"));
    qbsReadme.open(QIODevice::WriteOnly);
    QByteArray a("qbs文件为QTCreator的工程文件，请不要修改！！");
    qbsReadme.write(a.constData());
    qbsReadme.close();

}

void ArmMcu::ProjectOptionDialog::on_ApplyPushButton_clicked()
{
    //!!complier base
    int i=0;
    //ARM IP
    pc.CommonCompilerFlags.clear();

    if(pc.CpuType==tr("\"Cortex-M0\""))
    {
        pc.CommonCompilerFlags.append(CompileAndLinkShareFieldString=tr("-mthumb -mcpu=cortex-m0"));
    }
    else if(pc.CpuType==tr("\"Cortex-M0+\""))
    {
        pc.CommonCompilerFlags.append(CompileAndLinkShareFieldString=tr("-mthumb -mcpu=cortex-m0plus"));
    }
    else if(pc.CpuType==tr("\"Cortex-M1\""))
    {
        pc.CommonCompilerFlags.append(CompileAndLinkShareFieldString=tr("-mthumb -mcpu=cortex-m1"));
    }
    else if(pc.CpuType==tr("\"Cortex-M3\""))
    {
        pc.CommonCompilerFlags.append(CompileAndLinkShareFieldString=tr("-mthumb -mcpu=cortex-m3"));
    }
    else if(pc.CpuType==tr("\"Cortex-M4\""))
    {
        pc.CommonCompilerFlags.append(CompileAndLinkShareFieldString=tr("-mthumb -mcpu=cortex-m4"));
        //FPU
        i=ui->FPUComboBox->currentIndex();
        if(i==0)
        {
            pc.Fpu=tr("");
            CompileAndLinkShareFieldString+=tr(" ");
            CompileAndLinkShareFieldString+=pc.Fpu;
        }
        else if(i==1)
        {
            pc.Fpu=tr("-mfloat-abi=softfp -mfpu=fpv4-sp-d16");
            CompileAndLinkShareFieldString+=tr(" ");
            CompileAndLinkShareFieldString+=pc.Fpu;
        }
        else if(i==2)
        {
            pc.Fpu=tr("-mfloat-abi=hard -mfpu=fpv4-sp-d16");
            CompileAndLinkShareFieldString+=tr(" ");
            CompileAndLinkShareFieldString+=pc.Fpu;
        }
    }
    else if(pc.CpuType==tr("\"Cortex-M7\""))
    {
        pc.CommonCompilerFlags.append(CompileAndLinkShareFieldString=tr("-mthumb -mcpu=cortex-m7"));
        i=ui->FPUComboBox->currentIndex();
        if(i==0)
        {
            pc.Fpu=tr("");
            CompileAndLinkShareFieldString+=tr(" ");
            CompileAndLinkShareFieldString+=pc.Fpu;
        }
        else if(i==1)
        {
            pc.Fpu=tr("-mfloat-abi=softfp -mfpu=fpv4-sp-d16");
            CompileAndLinkShareFieldString+=tr(" ");
            CompileAndLinkShareFieldString+=pc.Fpu;
        }
        else if(i==2)
        {
            pc.Fpu=tr("-mfloat-abi=hard -mfpu=fpv4-sp-d16");
            CompileAndLinkShareFieldString+=tr(" ");
            CompileAndLinkShareFieldString+=pc.Fpu;
        }
    }
    i=ui->OptimizationComboBox->currentIndex();
    if(i==0)
    {
        pc.Optimization=tr("-O0");
    }
    else if(i==1)
    {
        pc.Optimization=tr("-O1");
    }
    else if(i==2)
    {
        pc.Optimization=tr("-O2");
    }
    else if(i==3)
    {
        pc.Optimization=tr("-O3");
    }
    else if(i==4)
    {
        pc.Optimization=tr("-Os");
    }
    else if(i==5)
    {
        pc.Optimization=tr("-Ofast");
    }
    else if(i==6)
    {
        pc.Optimization=tr("-Og");
    }
    //debug
    i=ui->DebugComboBox->currentIndex();
    if(i==0)
    {
       pc.CommonCompilerFlags.append(tr("-g0"));
    }
    else if(i==1)
    {
        pc.CommonCompilerFlags.append(tr("-g1"));
    }
    else if(i==2)
    {
        pc.CommonCompilerFlags.append(tr("-g2"));
    }
    else if(i==3)
    {
        pc.CommonCompilerFlags.append(tr("-g3"));
    }

    //waring
    i=ui->comOptionWarnningComboBox->currentIndex();
    if(i==0)
    {
        pc.CommonCompilerFlags.append(tr("-w"));
    }
    else if(i==1)
    {
        pc.CommonCompilerFlags.append(tr("-Wall"));
    }
    else if(i==2)
    {
        pc.CommonCompilerFlags.append(tr("-Wextra"));
    }

    //include path
    pc.IncludePath.clear();
    for(i=0;i<(ui->ComIncludeListWidget->count());i++)
    {
        pc.IncludePath.append(ui->ComIncludeListWidget->item(i)->text());
    }
    //define
    pc.Defines.clear();
    for(i=0;i<(ui->ComDefinedListWidget->count());i++)
    {
        pc.Defines.append(ui->ComDefinedListWidget->item(i)->text());
    }

    //!complie advance
    //-fno-rtti
    if(ui->_fno_rttiCheckBox->checkState()==Qt::Checked)
    {
        pc.CommonCompilerFlags.append(tr("-fno-rtti"));
    }
    if(ui->_fno_threadsafe_staticsCheckBox->checkState()==Qt::Checked)
    {
        pc.CommonCompilerFlags.append(tr("-fno-threadsafe-statics"));
    }
    if(ui->_fno_exceptionsCheckBox->checkState()==Qt::Checked)
    {
        pc.CommonCompilerFlags.append(tr("-fno-exceptions"));
    }
    if(ui->_fno_enforce_eh_specsCheckBox->checkState()==Qt::Checked)
    {
        pc.CommonCompilerFlags.append(tr("-fno-enforce-eh-specs"));
    }
    if(ui->_fno_optional_diagsCheckBox->checkState()==Qt::Checked)
    {
        pc.CommonCompilerFlags.append(tr("-fno-optional-diags"));
    }
    if(ui->_fvisibility_inlines_hiddenCheckBox->checkState()==Qt::Checked)
    {
        pc.CommonCompilerFlags.append(tr("-fvisibility-inlines-hidden"));
    }

    pc.CFlag.clear();
    i=ui->CStandardComboBox->currentIndex();
    if(i==0)
    {
        pc.CFlag.append(tr("-std=C90"));
    }
    else if(i==1)
    {
        pc.CFlag.append(tr("-std=C99"));
    }
    else if(i==2)
    {
        pc.CFlag.append(tr("-std=C11"));
    }
    else if(i==3)
    {
        pc.CFlag.append(tr("-std=gnu90"));
    }
    else if(i==4)
    {
        pc.CFlag.append(tr("-std=gnu99"));
    }
    else if(i==5)
    {
        pc.CFlag.append(tr("-std=gnu11"));
    }

    pc.CxxFlag.clear();
    i=ui->CppStandardComboBox->currentIndex();
    if(i==0)
    {
        pc.CxxFlag.append(tr("-std=C++98"));
    }
    else if(i==1)
    {
        pc.CxxFlag.append(tr("-std=gnu++98"));
    }
    else if(i==2)
    {
        pc.CxxFlag.append(tr("-std=c++11"));
    }
    else if(i==3)
    {
        pc.CxxFlag.append(tr("-std=gnu++11"));
    }
    QString b;
    for(i=0;i<pc.CommonCompilerFlags.count();i++)
    {
        b+=pc.CommonCompilerFlags.at(i)+tr(" ");
    }
    b+=pc.Optimization+tr(" ");
    b+=pc.Fpu+tr(" ");
    for(i=0;i<pc.CFlag.count();i++)
    {
        b+=pc.CFlag.at(i)+tr(" ");
    }
    for(i=0;i<pc.CxxFlag.count();i++)
    {
        b+=pc.CxxFlag.at(i)+tr(" ");
    }
    ui->ShowComArgTextEdit->setText(b);

    //!linker
    //-Wl,--gc-sections
    pc.LinkerFlags.clear();
    pc.LinkerFlags.append(CompileAndLinkShareFieldString);
    if(ui->GcSectionsCheckBox->checkState()==Qt::Checked)
    {
        pc.LinkerFlags.append(tr("-Wl,--gc-sections"));
    }
    if(ui->NoUseSystemStartUpFilecheckBox->checkState()==Qt::Checked)
    {
        pc.LinkerFlags.append(tr("-nostartfiles"));
    }
    if(ui->EnableLTOCheckBox->checkState()==Qt::Checked)
    {
        pc.LinkerFlags.append(tr("-flto"));
    }
    if(ui->LinkCLibComboBox->currentIndex()==1)
    {
        pc.LinkerFlags.append(tr("-specs=nano.specs"));
    }
    b=tr("");
    for(i=0;i<pc.LinkerFlags.count();i++)
    {
        b+=pc.LinkerFlags.at(i)+tr("  ");
    }
    ui->ShowLinkStringTextEdit->setText(b);
}

void ArmMcu::ProjectOptionDialog::on_LdBrowersPushButton_clicked()
{
    QString fs= QFileDialog::getOpenFileName(this,tr("add ld script file"),tr(""),tr("ProjectFile(*.ld)"));
    ui->LdLineEdit->setText(fs);

}

void ArmMcu::ProjectOptionDialog::on_LdAddPushButton_clicked()
{
    if(ui->LdListWidget->count()>0)
        return;
    QString fs=ui->LdLineEdit->text();
    if(!QFile::exists(fs))
    {
        return;
    }
    ui->LdListWidget->addItem(fs);
    pc.LinkScript=fs;
}
