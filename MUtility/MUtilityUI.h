#pragma once
/***************************************************************
 * filename: MUtility.h
 * function: ��װһЩͨ�õ�UI����
 * author:   ������
 * datetime: 2018-11-27
 ***************************************************************/
#include <QFontMetrics>

namespace MUtilityUI {

/*************************************************************************
* function: ��ӿհ��ַ�ʹ���ַ�������ʾ
* param metrics:  �����metrics
* param txt:    �������е��ı�
* param:         ����Ŀ��
* return txt:      ������ӿհ��ַ����ı�
*************************************************************************/
void addBlankForCenterLayout(QFontMetrics &metrics, QString &txt, int widgetWidth);

}

