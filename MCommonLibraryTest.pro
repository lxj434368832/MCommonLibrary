TEMPLATE = subdirs

SUBDIRS += \
    Test \
#    MExcel \
    QFtp \
    MCSVParser \
    Mqwt \
    MFramelessWidget \
    MTabWidget \
    MFramelessWindow\
    MediaPlayerQT \
    MediaPlayerVLC \
    UtilityWidget

#CONFIG(debug, debug|release) {
#    TARGET = Mqwtd
#}
#CONFIG(release, debug|release) {
#    TARGET = Mqwt
#}
#TARGET = $$qtLibraryTarget(Mqwt)
