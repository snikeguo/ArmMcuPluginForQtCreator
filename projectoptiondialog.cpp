#include "projectoptiondialog.h"
#include "ui_projectoptiondialog.h"
#include<QFileDialog>
#include<QDir>
#include<QMessageBox>
#include"mylistwidget.h"
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


   /* QFileInfo fi(pc.ProjectFilePath);
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
    qbsReadme.close();*/

}

void ArmMcu::ProjectOptionDialog::on_ApplyPushButton_clicked()
{
    QDir ch(ui->OutPutPathLineEdit->text());
    if(ch.exists()==false)
    {
        QMessageBox::warning(this,tr("Output path is not path!"),tr("error"));
        return;
    }
    pc.OutPutPath=ui->OutPutPathLineEdit->text();
    pc.TargetName=ui->OutPutProductNameLineEdit->text()==tr("")?pc.Device:ui->OutPutProductNameLineEdit->text();

    //!!complier base
    int i=0;
    //ARM IP
    pc.CommonCompilerFlags.clear();
    pc.Fpu.clear();
    pc.CommonCompilerFlags.append("-mthumb");
    if(pc.CpuType==tr("\"Cortex-M0\""))
    {
        pc.CommonCompilerFlags.append(tr("-mcpu=cortex-m0"));
    }
    else if(pc.CpuType==tr("\"Cortex-M0+\""))
    {
        pc.CommonCompilerFlags.append(tr("-mcpu=cortex-m0plus"));
    }
    else if(pc.CpuType==tr("\"Cortex-M1\""))
    {
        pc.CommonCompilerFlags.append(tr("-mcpu=cortex-m1"));
    }
    else if(pc.CpuType==tr("\"Cortex-M3\""))
    {
        pc.CommonCompilerFlags.append(tr("-mcpu=cortex-m3"));
    }
    else if(pc.CpuType==tr("\"Cortex-M4\""))
    {
        pc.CommonCompilerFlags.append(tr("-mcpu=cortex-m4"));
        //FPU
        i=ui->FPUComboBox->currentIndex();
        if(i==0)
        {
            //pc.Fpu=tr("");
            //CompileAndLinkShareFieldString+=tr(" ");
            //CompileAndLinkShareFieldString.;
        }
        else if(i==1)
        {
            //pc.Fpu=tr("-mfloat-abi=softfp -mfpu=fpv4-sp-d16");
            pc.Fpu.append("-mfloat-abi=softfp");
            pc.Fpu.append("-mfpu=fpv4-sp-d16");
            //CompileAndLinkShareFieldString+=tr(" ");
            //CompileAndLinkShareFieldString+=pc.Fpu;
        }
        else if(i==2)
        {
            //pc.Fpu=tr("-mfloat-abi=hard -mfpu=fpv4-sp-d16");
            pc.Fpu.append("-mfloat-abi=hard");
            pc.Fpu.append("-mfpu=fpv4-sp-d16");
            //CompileAndLinkShareFieldString+=tr(" ");
            //CompileAndLinkShareFieldString+=pc.Fpu;
        }
    }
    else if(pc.CpuType==tr("\"Cortex-M7\""))
    {
        pc.CommonCompilerFlags.append(tr("-mthumb -mcpu=cortex-m7"));
        i=ui->FPUComboBox->currentIndex();
        if(i==0)
        {
            //pc.Fpu=tr("");
            //CompileAndLinkShareFieldString+=tr(" ");
            //CompileAndLinkShareFieldString+=pc.Fpu;
        }
        else if(i==1)
        {
            pc.Fpu.append("-mfloat-abi=softfp");
            pc.Fpu.append("-mfpu=fpv4-sp-d16");
            //CompileAndLinkShareFieldString+=tr(" ");
            //CompileAndLinkShareFieldString+=pc.Fpu;
        }
        else if(i==2)
        {
            pc.Fpu.append("-mfloat-abi=hard");
            pc.Fpu.append("-mfpu=fpv4-sp-d16");
            //CompileAndLinkShareFieldString+=tr(" ");
            //CompileAndLinkShareFieldString+=pc.Fpu;
        }
    }
    CompileAndLinkShareFieldString.clear();
    for(i=0;i<pc.CommonCompilerFlags.count();i++)
    {
        CompileAndLinkShareFieldString.append(pc.CommonCompilerFlags.at(i));
    }
    for(i=0;i<pc.Fpu.count();i++)
    {
        CompileAndLinkShareFieldString.append(pc.Fpu.at(i));
    }
    i=ui->OptimizationComboBox->currentIndex();
    if(i==0)
    {
        pc.Optimization=tr("-O0");
        pc.CommonCompilerFlags.append(tr("-O0"));

    }
    else if(i==1)
    {
        pc.Optimization=tr("-O1");
        pc.CommonCompilerFlags.append(tr("-O1"));
    }
    else if(i==2)
    {
        pc.Optimization=tr("-O2");
        pc.CommonCompilerFlags.append(tr("-O2"));
    }
    else if(i==3)
    {
        pc.Optimization=tr("-O3");
        pc.CommonCompilerFlags.append(tr("-O3"));
    }
    else if(i==4)
    {
        pc.Optimization=tr("-Os");
        pc.CommonCompilerFlags.append(tr("-Os"));
    }
    else if(i==5)
    {
        pc.Optimization=tr("-Ofast");
        pc.CommonCompilerFlags.append(tr("-Ofast"));
    }
    else if(i==6)
    {
        pc.Optimization=tr("-Og");
        pc.CommonCompilerFlags.append(tr("-Og"));
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
        pc.CFlag.append(tr("-std=c90"));
    }
    else if(i==1)
    {
        pc.CFlag.append(tr("-std=c99"));
    }
    else if(i==2)
    {
        pc.CFlag.append(tr("-std=c11"));
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
        pc.CxxFlag.append(tr("-std=c++98"));
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
    for(i=0;i<pc.Fpu.count();i++)
    {
       b+=pc.Fpu.at(i);
       b+=" ";
    }

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
    for(i=0;i<CompileAndLinkShareFieldString.count();i++)
    pc.LinkerFlags.append(CompileAndLinkShareFieldString.at(i));
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
    if(ui->LinkCLibComboBox->currentIndex()==0)
    {
        pc.LinkerFlags.append(tr("-specs=nosys.specs"));
    }
    else  if(ui->LinkCLibComboBox->currentIndex()==1)
    {
        pc.LinkerFlags.append(tr("-specs=nano.specs"));
    }
    b=tr("");
    for(i=0;i<pc.LinkerFlags.count();i++)
    {
        b+=pc.LinkerFlags.at(i)+tr("  ");
    }
    ui->ShowLinkStringTextEdit->setText(b);
    if(ui->MapFileCheckBox->checkState()==Qt::Checked)
    {
        pc.IsCreateMapFile=true;
        pc.LinkerFlags.append("-Wl,-Map="+pc.OutPutPath+"/"+pc.TargetName+".map");
    }
    else
    {
        pc.IsCreateMapFile=false;
    }
    if(ui->HexFileCheckBox->checkState()==Qt::Checked)
        pc.IsCreateHexFile=true;
    else
        pc.IsCreateHexFile=false;
    if(ui->BinFileCheckBox->checkState()==Qt::Checked)
        pc.IsCreateBinFile=true;
    else
        pc.IsCreateBinFile=false;
    if(ui->AsmFileCheckBox->checkState()==Qt::Checked)
        pc.IsCreateAsmFile=true;
    else
        pc.IsCreateAsmFile=false;



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


void ArmMcu::ProjectOptionDialog::on_OkPushButton_clicked()
{
    this->on_ApplyPushButton_clicked();
    WriteQbsFile();
    this->hide();
}
bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if(!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
            if(!copyDirectoryFiles(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()),
                coverFileIfExist))
                return false;
        }
        else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }

            /// 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()))){
                    return false;
            }
        }
    }
    return true;
}
void ArmMcu::ProjectOptionDialog::WriteQbsFile()
{
    QFileInfo mdk(pc.ProjectFilePath);
    QDir srcP=mdk.absoluteDir();
    srcP.cdUp();
    QDir qbsPath(srcP.path()+tr("/ArmMcuForQtCreator"));
    if(qbsPath.exists()==false)
    {
        //create qbs path
        //srcP.mkdir(tr("ArmMcuForQtCreator"));
        ::copyDirectoryFiles(ThisPluginProjectPath+"/ArmMcuForQtCreator",qbsPath.path(),true);
    }
    QFileInfo openQbs(srcP.path()+"/"+pc.TargetName+".qbs");
    if(openQbs.exists()==false)
        QFile::copy(ThisPluginProjectPath+"/open.qbs",srcP.path()+"/"+pc.TargetName+".qbs");


    QFile prjqbs;
    prjqbs.setFileName(qbsPath.path()+tr("/prj.qbs"));
    if(prjqbs.exists()==true)
        prjqbs.remove();//delete
    prjqbs.open(QIODevice::WriteOnly);

    QFile ProjectTemplate(ThisPluginProjectPath+tr("/ProjectTemplate.qbs"));
    ProjectTemplate.open(QIODevice::ReadOnly);
    QByteArray pt=ProjectTemplate.readAll();
    ProjectTemplate.close();
    QString ptstring(pt);

    QString res=pc.TargetName;
    ptstring=ptstring.replace(tr("##__ProjectName__##"),"\""+res+"\"");

    res=pc.IsCreateAsmFile==true?tr("true"):tr("false");
    ptstring=ptstring.replace(tr("##__IsCreateAsmFile__##"),res);

    res=pc.IsCreateBinFile==true?tr("true"):tr("false");
    ptstring=ptstring.replace(tr("##__IsCreateBinFile__##"),res);

    res=pc.IsCreateHexFile==true?tr("true"):tr("false");
    ptstring=ptstring.replace(tr("##__IsCreateHexFile__##"),res);

    res=pc.IsCreateMapFile==true?tr("true"):tr("false");
    ptstring=ptstring.replace(tr("##__IsCreateMapFile__##"),res);

    res.clear();
    res+="[";
    for(int i=0;i<pc.Defines.count();i++)
    {
        res+="\"";
        res+=pc.Defines.at(i);
        res+="\",";
    }
    res+="]";
    ptstring=ptstring.replace(tr("##__Defines__##"),res);

    res.clear();
    res+="[";
    for(int i=0;i<pc.IncludePath.count();i++)
    {
        res+="\"";
        res+=pc.IncludePath.at(i);
        res+="\",";
    }
    res+="]";
    ptstring=ptstring.replace(tr("##__IncludePaths__##"),res);


    res.clear();
    res+="[";
    for(int i=0;i<pc.CxxFlag.count();i++)
    {
        res+="\"";
        res+=pc.CxxFlag.at(i);
        res+="\",";
    }
    res+="]";
    ptstring=ptstring.replace(tr("##__CxxFlag__##"),res);


    res.clear();
    res+="[";
    for(int i=0;i<pc.CFlag.count();i++)
    {
        res+="\"";
        res+=pc.CFlag.at(i);
        res+="\",";
    }
    res+="]";
    ptstring=ptstring.replace(tr("##__CFlag__##"),res);
    ptstring=ptstring.replace(tr("##__WarningLevel__##"),"\"all\"");

    res.clear();
    res+="[";
    res+="\"";
    res+=pc.LinkScript;
    res+="\",";
    res+="]";
    ptstring=ptstring.replace(tr("##__LinkerScript__##"),res);


    res.clear();
    res+="[";
    for(int i=0;i<pc.CommonCompilerFlags.count();i++)
    {
        res+="\"";
        res+=pc.CommonCompilerFlags.at(i);
        res+="\",";
    }
    //res.clear();
    for(int i=0;i<pc.Fpu.count();i++)
    {
        res+="\"";
        res+=pc.Fpu.at(i);
        res+="\",";
    }
    res+="]";
    ptstring=ptstring.replace(tr("##__CommonCompilerFlags__##"),res);

    res.clear();
    res+="[";
    for(int i=0;i<pc.LinkerFlags.count();i++)
    {
        res+="\"";
        res+=pc.LinkerFlags.at(i);
        res+="\",";
    }
    res+="]";
    ptstring=ptstring.replace(tr("##__LinkerFlags__##"),res);


    ptstring=ptstring.replace(tr("##__OutputFilePath__##"),tr("\"")
                              +pc.OutPutPath+tr("\""));
    res.clear();

    prjqbs.write(ptstring.toStdString().data());
    prjqbs.close();


}

void ArmMcu::ProjectOptionDialog::on_OutPutPathBrowersPushButton_clicked()
{
    QString fn=QFileDialog::getExistingDirectory(this,tr("打开路径"),tr(""),QFileDialog::ShowDirsOnly);
    ui->OutPutPathLineEdit->setText(fn);
}
