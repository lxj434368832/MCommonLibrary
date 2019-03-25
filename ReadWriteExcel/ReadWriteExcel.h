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

    //打开excel文件
    bool OpenExcel(const QString qstrFileName);

    bool SetCurrentSheet();

    //读取所有的数据
    QList<QList<QVariant>> ReadAllData();

    //读取数据范围内的某一行数据
    QList<QVariant> ReadRowData(int row);

    //读取数据范围内的某一行数据
    QVariant ReadCelData(int row, int col);

	/*************************************************************************
	* function:写数据到文件中
	* param :
	* return:
	*************************************************************************/
    bool WriteCurrentSheet(const QList<QList<QVariant>> contexts);

	/*************************************************************************
	* function:写一行数据到文件中
	* param :
	* return:
	*************************************************************************/
	bool WriteRowData(int row, QList<QVariant>);

	/*************************************************************************
	* function:写单元数据到文件
	* param :
	* return:
	*************************************************************************/
	bool WriteCellData(int row, int col);

private:

    QString GetRangeString(int iStartRow, int iStartCol, int iEndRow, int iEndCol);

    /*************************************************************************
    * function:把列数转换为excel的字母列号
    * param data:大于0的数
    * return:字母列号，如1->A 26->Z 27 AA
    *************************************************************************/
    QString convertToColName(int number);

private:
	bool			m_bOpenSuccess;		//文件打开成功标志
    QAxObject       *m_AxExcel;         //excel操作对象
    QAxObject       * m_AxWorkbooks;    //工作簿集合
    QAxObject       * m_AxWorkbook;     //当前打开的工作簿
    QAxObject       *m_AxWorksheets;    //所有的工作表
    QAxObject       *m_AxWorksheet;     //当前操作工作表

    int             m_iStartRow;        //开始行数
    int             m_iStartCol;        //开始列数
    int             m_iRowCount;		//有效的行数
    int             m_iColCount;        //有效的列数

};

#endif // READWRITEEXCEL_H
