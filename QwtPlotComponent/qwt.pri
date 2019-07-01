LIB_NAME        = Qwt6.1.3
#LIB_VERSION     = 3.0.0_x64
LIB_VERSION     = 6.1.3

LIB_PATH = D:\Qt\Qt5.6.3\5.6.3\msvc2013

exists( $${LIB_PATH} ) {
    ######################################################################
    # Include library
    ######################################################################
    LIB_HEADERS   = $${LIB_PATH}/include/Qwt
    LIB_LIBS      = $${LIB_PATH}/lib

    CONFIG(debug, debug|release): LIBS += -L$${LIB_LIBS}/ -lqwtd
    CONFIG(release, debug|release): LIBS += -L$${LIB_LIBS}/ -lqwt

    INCLUDEPATH += $${LIB_HEADERS}

#    LIB_install.files += $$LIB_PATH/bin/*
#    LIB_install.path = $${IDE_APP_PATH}/
#    INSTALLS += LIB_install

    DEFINES *= QWT_DLL
}

!exists( $${LIB_PATH} ) {
    warning("Cann't find LIB DIR:")
    warning( $${LIB_PATH})
}
