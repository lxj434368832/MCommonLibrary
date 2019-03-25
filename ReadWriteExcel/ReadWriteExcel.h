#ifndef READWRITEEXCEL_H
#define READWRITEEXCEL_H

#include <QList>
#include <QVariant>
#include <QString>

class QAxObject;

class ReadWriteExcel
{
public:
    ReadWriteExcel();
    ~ReadWriteExcel();

    //��excel�ļ�
    bool OpenExcel(const QString qstrFileName);

    bool SetCurrentSheet();

    //��ȡ���е�����
    QList<QList<QVariant>> ReadAllData();

    //��ȡ���ݷ�Χ�ڵ�ĳһ������
    QList<QVariant> ReadRowData(int row);

    //��ȡ���ݷ�Χ�ڵ�ĳһ������
    QVariant ReadCelData(int row, int col);

	/*************************************************************************
	* function:д���ݵ��ļ���
	* param :
	* return:
	*************************************************************************/
    bool WriteCurrentSheet(const QList<QList<QVariant>> contexts);

	/*************************************************************************
	* function:дһ�����ݵ��ļ���
	* param :
	* return:
	*************************************************************************/
	bool WriteRowData(int row, QList<QVariant>);

	/*************************************************************************
	* function:д��Ԫ���ݵ��ļ�
	* param :
	* return:
	*************************************************************************/
	bool WriteCellData(int row, int col);

private:

    QString GetRangeString(int iStartRow, int iStartCol, int iEndRow, int iEndCol);

    /*************************************************************************
    * function:������ת��Ϊexcel����ĸ�к�
    * param data:����0����
    * return:��ĸ�кţ���1->A 26->Z 27 AA
    *************************************************************************/
    QString convertToColName(int number);

private:
	bool			m_bOpenSuccess;		//�ļ��򿪳ɹ���־
    QAxObject       *m_AxExcel;         //excel��������
    QAxObject       * m_AxWorkbooks;    //����������
    QAxObject       * m_AxWorkbook;     //��ǰ�򿪵Ĺ�����
    QAxObject       *m_AxWorksheets;    //���еĹ�����
    QAxObject       *m_AxWorksheet;     //��ǰ����������

    int             m_iStartRow;        //��ʼ����
    int             m_iStartCol;        //��ʼ����
    int             m_iRowCount;		//��Ч������
    int             m_iColCount;        //��Ч������

};

#endif // READWRITEEXCEL_H
