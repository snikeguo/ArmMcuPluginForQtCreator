#ifndef PROJECTCONFIGSTRUCT_H
#define PROJECTCONFIGSTRUCT_H
#include<QHash>
#include<QString>
namespace ArmMcu {
struct ProjectConfig
{
    QString ProjectFilePath;//qbs or keil project file path
    QString OutPutPath;
    QString TargetName;//ProjectName
    QString Device; //device name .eg:stm32F103C8T6,
    QString Vendor;
    QString CpuType;
    quint32 clockFrequency;//clock Frequency.eg:stm32f1 clockFrequency can set 72Mhz
    QStringList Defines; //eg:gcc -DXXXXXXXX
    QStringList IncludePath;//eg: gcc -I
    QHash<QString,QStringList> group;//<grout name,c code file  name list>; to see keil ide 's group
    struct McuMemory
    {
        quint32 StartAddr;
        quint32 Size;
        bool IsStartUp=false;
    };
    McuMemory IROM[2];//to see keil ide
    McuMemory IRAM[2];//to see keil ide

    QString LinkScript;
    bool IsCreateHexFile=false;
    bool IsCreateMapFile=false;
    bool IsCreateBinFile=false;
    bool IsCreateAsmFile=false;
    QStringList CxxFlag;
    QString Optimization;
    QStringList CFlag;
    //bool WarningLevel=true;
    QStringList CommonCompilerFlags;
    QStringList LinkerFlags;
    QStringList Fpu;
};
}
#endif // PROJECTCONFIGSTRUCT_H
