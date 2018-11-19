INCLUDEPATH += $$IDE_SOURCE_TREE/MFramelessWidget
DEPENDPATH += $$IDE_SOURCE_TREE/MFramelessWidget
LIBS *= -L$$IDE_PLUGINS_DEPEND_PATH
LIBS *= -l$$qtLibraryTarget(MFramelessWidget)
message($$LIBS)
