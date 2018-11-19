linux*{
ZMQ_Version = 4.1.4-linux
INCLUDEPATH += $$PWD/$$ZMQ_Version/include
LIBS += -L$$PWD/$$ZMQ_Version/lib -lzmq

zmqlibrary.target = $$IDE_APP_PATH/libzmq.so.5
zmqlibrary.commands = $$PATH_PYTHON $$IDE_ROOT/CompileHelper.py COPYDIR $$PWD/$$ZMQ_Version/lib $$IDE_APP_PATH
zmqlibrary.depends = $$IDE_ROOT/CompileHelper.py
QMAKE_EXTRA_TARGETS += zmqlibrary
PRE_TARGETDEPS += $${zmqlibrary.target}
}
else:win32{
ZMQ_Version = 4.0.4_x86
contains(QMAKE_HOST.arch, x86_64):ZMQ_Version = 4.0.4_x64
INCLUDEPATH += $$PWD/$$ZMQ_Version/include
LIBS += -L$$PWD/$$ZMQ_Version/lib -llibzmq-v120-mt-4_0_4

zmqlibrary.target = $$IDE_APP_PATH/libzmq-v120-mt-4_0_4.dll
zmqlibrary.depends = $$PWD/$$ZMQ_Version/bin/libzmq-v120-mt-4_0_4.dll
zmqlibrary.commands = $(COPY_FILE) \"$$replace(zmqlibrary.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(zmqlibrary.target, /, $$QMAKE_DIR_SEP)\"
QMAKE_EXTRA_TARGETS += zmqlibrary
PRE_TARGETDEPS += $${zmqlibrary.target}
}
