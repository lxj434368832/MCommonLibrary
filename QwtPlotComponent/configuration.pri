#-------------------------------------------------
# ��������汾��(��ʱ������)
#-------------------------------------------------
#QTCREATOR_VERSION = 4.7.1

isEqual(QT_MAJOR_VERSION, 5) {
    defineReplace(cleanPath) {
        return($$clean_path($$1))
    }
} else { # qt5
    defineReplace(cleanPath) {
        win32:1 ~= s|\\\\|/|g
        contains(1, ^/.*):pfx = /
        else:pfx =
        segs = $$split(1, /)
        out =
        for(seg, segs) {
            equals(seg, ..):out = $$member(out, 0, -2)
            else:!equals(seg, .):out += $$seg
        }
        return($$join(out, /, $$pfx))
    }
}

#-------------------------------------------------
# ָ����Ŀ��Ŀ¼
#-------------------------------------------------
IDE_SOURCE_TREE = $$PWD

#-------------------------------------------------
# 3rdparth path 3rdparty
#-------------------------------------------------
unix {
#    QT_3RDPARTYPATH = /home/dev_env/3rdParty
    QT_3RDPARTYPATH = $$IDE_SOURCE_TREE/3rdParty
}
win32 {
    QT_3RDPARTYPATH = $$IDE_SOURCE_TREE/3rdParty
}

#-------------------------------------------------
# �ҵ��������·��
#-------------------------------------------------
isEmpty(IDE_ROOT) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    IDE_ROOT = $$cleanPath($$OUT_PWD)
    IDE_ROOT ~= s,$$re_escape($$sub_dir)$,,
}

#-------------------------------------------------
# �����·��
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG(release, debug|release):OUT_DIR_NAME=Release
    CONFIG(debug, debug|release):OUT_DIR_NAME=Debug
}
else{
    CONFIG(release, debug|release):OUT_DIR_NAME=Release4
    CONFIG(debug, debug|release):OUT_DIR_NAME=Debug4
}
# ����windows�µ�vs��mingw����
mingw:exists($$IDE_ROOT/.sdk){
    CONFIG(release, debug|release):OUT_DIR_NAME=ReleaseMingw
    CONFIG(debug, debug|release):OUT_DIR_NAME=DebugMingw
}

IDE_BUILD_TREE = $$IDE_ROOT

IDE_APP_PATH = $$IDE_BUILD_TREE/bin     #�����Ŀ¼
#message($$IDE_APP_PATH)

#-------------------------------------------------
# ָ������Ŀ���ļ���
#-------------------------------------------------
IDE_APP_TARGET   = "MCommonLibrary"

#���򼰲�����Ŀ¼
macx {
    IDE_LIBRARY_PATH = $$IDE_APP_PATH/lib
    IDE_PLUGINS_DEPEND_PATH = $$IDE_APP_PATH
    IDE_PLUGINS_PATH = $$IDE_APP_PATH/bin/plugins
} else {
    IDE_LIBRARY_PATH = $$IDE_APP_PATH/lib
    IDE_PLUGINS_DEPEND_PATH = $$IDE_APP_PATH
    IDE_PLUGINS_PATH = $$IDE_APP_PATH/plugins
}

#open release debug
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
