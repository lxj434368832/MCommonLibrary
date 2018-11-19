VLC_NAME        = vlc
#VLC_VERSION     = 3.0.0_x64
VLC_VERSION     = 3.0.4

VLC_PATH = $$QT_3RDPARTYPATH/$$VLC_NAME/$$VLC_VERSION


exists( $${VLC_PATH} ) {
    ######################################################################
    # Include library
    ######################################################################
    VLC_HEADERS   = $${VLC_PATH}/include
    VLC_LIBS      = $${VLC_PATH}/lib

    LIBS += -L$${VLC_LIBS}/ -llibvlc \
            -L$${VLC_LIBS}/ -llibvlccore

    INCLUDEPATH += $${VLC_HEADERS}

    VLC_install.files += $$VLC_PATH/bin/*
    VLC_install.path = $${IDE_APP_PATH}/
    INSTALLS += VLC_install
}

!exists( $${VLC_PATH} ) {
    warning("Cann't find VLC DIR =======================")
}
