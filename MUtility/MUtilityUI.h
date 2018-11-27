#pragma once
/***************************************************************
 * filename: MUtility.h
 * function: 封装一些通用的UI方法
 * author:   明巧文
 * datetime: 2018-11-27
 ***************************************************************/
#include <QFontMetrics>

namespace MUtilityUI {

/*************************************************************************
* function: 添加空白字符使得字符居中显示
* param metrics:  窗体的metrics
* param txt:    窗体现有的文本
* param:         窗体的宽度
* return txt:      返回添加空白字符的文本
*************************************************************************/
void addBlankForCenterLayout(QFontMetrics &metrics, QString &txt, int widgetWidth);

}

