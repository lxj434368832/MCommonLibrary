INCLUDEPATH += $$IDE_SOURCE_TREE/LogQt
DEPENDPATH += $$IDE_SOURCE_TREE/LogQt
LIBS *= -L$$IDE_PLUGINS_DEPEND_PATH
LIBS *= -l$$qtLibraryTarget(MLog)

DEFINES += QT_MESSAGELOGCONTEXT #����release�µ�logcontext
#CONFIG += console

message($$LIBS)
