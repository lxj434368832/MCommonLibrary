###############################################################################
#  @Description: 查找三方库QWT
#  @Copyright: Copyright (c) 2013
#  @Company: 深圳置辰海信科技有限公司
#  @WebSite: http://www.zhichenhaixin.com/
#  @author 王磊
#
#  @Revision History
#
#  <pre>
#  &lt;Date&gt;, &lt;Who&gt;, &lt;What&gt;
#  &#064;2013-09-12, 王磊, Initial.
#  &#064;2013-10-31, 王磊, Update.
#
###############################################################################

################################################################
# QWT Name and Version
################################################################
QWT_DIR_NAME         = qwt
QWT_VERSION          = 6.1.2

######################################################################
# QWT path
######################################################################
QWT_PATH = $${QT_3RDPARTYPATH}/$${QWT_DIR_NAME}/$${QWT_VERSION}

exists( $${QWT_PATH} ) {
    QWT_HEADERS   = $${QWT_PATH}/include
    QWT_LIBS      = $${QWT_PATH}/lib

    LIBS += -L$${QWT_LIBS}
    LIBS += -lqwt
    INCLUDEPATH += $${QWT_HEADERS}

    qwt_install.files += $$QWT_PATH/lib/*.so*
    qwt_install.path = $${IDE_APP_PATH}

    INSTALLS += qwt_install

    message("=================QWT Finished====================")
}

!exists( $${QWT_PATH} ) {
    warning("Cann't find QWT=======================")
}


