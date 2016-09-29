DEFINES += ARMMCU_LIBRARY
QT +=core xml

# ArmMcu files
#INCLUDEPATH +=F:/QTPROJECT/stm32plugin/Stm32PluginProject/arm_mcu_plugin/
SOURCES += armmcuplugin.cpp \
    importprojectdialog.cpp \
    projectoptiondialog.cpp \
    debuggerviewdialog.cpp \
    mylistwidget.cpp

HEADERS += armmcuplugin.h \
        armmcu_global.h \
        armmcuconstants.h \
    importprojectdialog.h \
    projectoptiondialog.h \
    debuggerviewdialog.h \
    projectconfigstruct.h \
    mylistwidget.h

# Qt Creator linking
## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=F:/QTPROJECT/stm32plugin/qt-creator-opensource-src-4.0.2

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=F:/QTPROJECT/stm32plugin/QtCreator4_0_2Build/release_MSVC2013_x86

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on OS X
# USE_USER_DESTDIR = yes

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = ArmMcu
QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin  \
    QtSupport   \
    Debugger    \
    ProjectExplorer
##add more plugin,maybe used it!
QTC_PLUGIN_RECOMMENDS += \
    coreplugin  \
    QtSupport   \
    Debugger    \
    ProjectExplorer
# optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)
DEFINES -= QT_NO_CAST_FROM_ASCII
FORMS += \
    importprojectdialog.ui \
    projectoptiondialog.ui \
    debuggerviewdialog.ui

RESOURCES += \
    image.qrc

