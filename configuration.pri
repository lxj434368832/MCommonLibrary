#-------------------------------------------------
# 设置软件版本号(暂时不可用)
#-------------------------------------------------
QTCREATOR_VERSION = 4.0.3

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
# 指定项目根目录
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
# 找到主程序根路径
#-------------------------------------------------
isEmpty(IDE_ROOT) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    IDE_ROOT = $$cleanPath($$OUT_PWD)
    IDE_ROOT ~= s,$$re_escape($$sub_dir)$,,
}

#-------------------------------------------------
# 程序根路径
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG(release, debug|release):OUT_DIR_NAME=Release
    CONFIG(debug, debug|release):OUT_DIR_NAME=Debug
}
else{
    CONFIG(release, debug|release):OUT_DIR_NAME=Release4
    CONFIG(debug, debug|release):OUT_DIR_NAME=Debug4
}
# 区分windows下的vs和mingw编译
mingw:exists($$IDE_ROOT/.sdk){
    CONFIG(release, debug|release):OUT_DIR_NAME=ReleaseMingw
    CONFIG(debug, debug|release):OUT_DIR_NAME=DebugMingw
}

IDE_BUILD_TREE = $$IDE_ROOT

IDE_APP_PATH = $$IDE_BUILD_TREE/bin     #程序根目录
#message($$IDE_APP_PATH)

#-------------------------------------------------
# 指定程序目标文件名
#-------------------------------------------------
IDE_APP_TARGET   = "MCommonLibrary"

#程序及插件存放目录
macx {
    IDE_LIBRARY_PATH = $$IDE_APP_PATH/lib
    IDE_PLUGINS_DEPEND_PATH = $$IDE_APP_PATH
    IDE_PLUGINS_PATH = $$IDE_APP_PATH/bin/plugins
} else {
    IDE_LIBRARY_PATH = $$IDE_APP_PATH/lib
    IDE_PLUGINS_DEPEND_PATH = $$IDE_APP_PATH
    IDE_PLUGINS_PATH = $$IDE_APP_PATH/plugins
}
