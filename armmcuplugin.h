#pragma once

#include "armmcu_global.h"

#include <extensionsystem/iplugin.h>
#include"importprojectdialog.h"
#include"projectoptiondialog.h"
#include"debuggerviewdialog.h"

#include"projectconfigstruct.h"
namespace ArmMcu {



namespace Internal {

class ArmMcuPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "ArmMcu.json")

public:
    ArmMcuPlugin();
    ~ArmMcuPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private slots:
    void importTriggerAction();
    void ProjectOption();
    void AboutArmMcuAction();
    void CmsisRtosDebugerViewAction();
private:
    ImportProjectDialog *ipjd;
    ProjectOptionDialog *proSetDlg;
    DebuggerViewDialog *debugview;
    ProjectConfig m_ProjectConfig;
signals:
    void ProjectConfigChange(ProjectConfig &p);
};

} // namespace Internal
} // namespace ArmMcu
