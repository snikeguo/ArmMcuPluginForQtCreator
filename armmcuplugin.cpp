


#include "armmcuplugin.h"
#include "armmcuconstants.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>
#include <QtPlugin>
#include<QtXml/QDomDocument>
#include<QDir>
#ifdef Q_OS_WIN
    #if _MSC_VER >= 1600
    #pragma execution_character_set("utf-8")
    #endif
#endif

using namespace ArmMcu::Internal;
ArmMcuPlugin::ArmMcuPlugin()
{
    // Create your members
    ipjd=new ImportProjectDialog(Core::ICore::mainWindow());
    proSetDlg=new ProjectOptionDialog(Core::ICore::mainWindow());
    debugview=new DebuggerViewDialog(Core::ICore::mainWindow());
    qRegisterMetaType<ProjectConfig>("ProjectConfig");
    connect(this,SIGNAL(ProjectConfigChange(ProjectConfig&)),proSetDlg,SLOT(onProjectConfigChanged(ProjectConfig&)));
    //proSetDlg->setEnabled(false);
    //debugview->setEnabled(false);
    QDir bin(Core::ICore::libexecPath());
    bin.cdUp();//lib bin path
    bin.cd("lib");
    bin.cd("qtcreator");
    bin.cd("plugins");
    bin.cd("ARMMCU");
    ThisPluginProjectPath=bin.path();
    proSetDlg->SetThisPluginProjectPath(ThisPluginProjectPath);
    ipjd->SetThisPluginProjectPath(ThisPluginProjectPath);

}

ArmMcuPlugin::~ArmMcuPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool ArmMcuPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)


    QAction *action = new QAction(tr("import CubeMx Project"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::IMPORT_PRJ_ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    //cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+Meta+A")));
    connect(action, SIGNAL(triggered()), this, SLOT(importTriggerAction()));

    QAction *ProjectOptionAction=new QAction(tr("Project Option"),this);
    Core::Command *ProjectOptionCmd=Core::ActionManager::registerAction(ProjectOptionAction, Constants::PRJ_OPTION_ACTION_ID,
                                                                        Core::Context(Core::Constants::C_GLOBAL));
    connect(ProjectOptionAction, SIGNAL(triggered()), this, SLOT(ProjectOption()));

    QAction *CmsisRtosDebugerViewAction=new QAction(tr("CMSIS RTOS View "),this);
    Core::Command *CmsisRtosDebugerViewActionCmd=Core::ActionManager::registerAction(CmsisRtosDebugerViewAction, Constants::CMSIS_RTOS_VIEW_ACTION_ID,
                                                                        Core::Context(Core::Constants::C_GLOBAL));
    connect(CmsisRtosDebugerViewAction, SIGNAL(triggered()), this, SLOT(CmsisRtosDebugerViewAction()));


    //void AboutArmMcuAction();
    QAction *aboutAction=new QAction(tr("About This Plugins"),this);
    Core::Command *AboutCmd=Core::ActionManager::registerAction(aboutAction, Constants::ABOUT_ACTION_ID,
                                                                        Core::Context(Core::Constants::C_GLOBAL));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(AboutArmMcuAction()));

    //Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    Core::ActionContainer *menu = Core::ActionManager::createMenu("ArmMcuMenu");
    menu->menu()->setTitle(tr("ArmMcuPlugin"));
    menu->addAction(cmd);
    menu->addAction(ProjectOptionCmd);
    menu->addAction(CmsisRtosDebugerViewActionCmd);
    menu->addAction(AboutCmd);
    Core::ActionManager::actionContainer(Core::Constants::MENU_BAR)->addMenu(menu);

    return true;
}

void ArmMcuPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag ArmMcuPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void ArmMcuPlugin::importTriggerAction()
{
    //using namespace Debugger;
    //QDomDocument qdd;

    ipjd->exec();
    emit this->ProjectConfigChange(ipjd->GetProjectConfig());
}
void ArmMcuPlugin::ProjectOption()
{
    proSetDlg->show();
}
void ArmMcuPlugin::AboutArmMcuAction()
{
    QMessageBox::information(Core::ICore::mainWindow(),tr("about"),tr("这个插件是免费的，如果有问题，请联系 E-mail:snikeguo@foxmail.com"));
}
void ArmMcuPlugin::CmsisRtosDebugerViewAction()
{
    debugview->show();
}
