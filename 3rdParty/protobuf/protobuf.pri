PB_Version = 2.6.1-msys2
INCLUDEPATH += $$PWD/$$PB_Version/include
LIBS += -L$$PWD/$$PB_Version/Release -llibprotobuf

ProtoBuf_install.files += $$PWD/$$PB_Version/Release/libprotobuf-9.dll
ProtoBuf_install.path = $${IDE_APP_PATH}/
INSTALLS += ProtoBuf_install
