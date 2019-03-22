#include "ReadWriteExcel.h"
//#include <exception>
#include <QFile>
#include <QDebug>
#include <QAxObject>

ReadWriteExcel::ReadWriteExcel()
{
	m_bOpenSuccess = false;
	m_AxExcel = new QAxObject("Excel.Application"); //����excel�ؼ�
	//m_AxExcel->setProperty("Visible", true);       //����Ϊ����ʾ����	
	//m_AxExcel->dynamicCall("SetVisible(bool Visible)", "true");//����Ϊ����ʾ����
	m_AxExcel->setProperty("DisplayAlerts", false);//����ʾ�κξ�����Ϣ����ر�ʱ���Ƿ񱣴���ʾ
	m_AxWorkbooks = m_AxExcel->querySubObject("WorkBooks");

	m_AxWorkbooks = nullptr;
	m_AxWorkbook = nullptr;
	m_AxWorksheets = nullptr;
	m_AxWorksheet = nullptr;

}

ReadWriteExcel::~ReadWriteExcel()
{
	if (m_AxWorkbook)
		m_AxWorkbook->dynamicCall("Close(Boolean)", false); //�ر��ļ�
	//�ر�excel����,������ɺ���Źرգ����������صĿ����������رս��̻��кܶ�excel.exe
	m_AxExcel->dynamicCall("Quit(void)");

	if (m_AxExcel)
	{
		delete m_AxExcel;
		m_AxExcel = nullptr;
	}
}

bool ReadWriteExcel::OpenExcel(const QString qstrFileName)
{
	if (false == QFile::exists(qstrFileName))
	{
		return false;
	}

	try {
		m_AxWorkbooks->dynamicCall("Open(const QString&)", qstrFileName);
		m_AxWorkbook = m_AxExcel->querySubObject("ActiveWorkBook");

		m_AxWorksheets = m_AxWorkbook->querySubObject("Sheets");
		if (nullptr == m_AxWorksheets)
		{
			qDebug() << "��ȡ���й�����ʧ�ܣ�";
			return false;
		}
		m_AxWorksheet = m_AxWorksheets->querySubObject("Item(int)", 1);
		if (nullptr == m_AxWorksheet)
		{
			qDebug() << "��ȡ��һ�Ź�����ʧ�ܣ�";
			return false;
		}

		QAxObject *axUsedRange = m_AxWorksheet->querySubObject("UsedRange");
		if (nullptr == axUsedRange)
		{
			qDebug() << "��ȡʹ�÷�Χʧ�ܣ�";
			return false;
		}

		m_iStartRow = axUsedRange->property("Row").toInt();
		m_iStartCol = axUsedRange->property("Column").toInt();

		QAxObject *axRows = axUsedRange->querySubObject("Rows");
		m_iRowCount = axRows->property("Count").toInt();
		QAxObject *AxColumns = axUsedRange->querySubObject("Columns");
		m_iColCount = AxColumns->property("Count").toInt();

		delete axUsedRange;
		m_bOpenSuccess = true;
		return true;
	}
	catch (...)
	{
		qDebug() << QString::fromLocal8Bit("���ļ�%1�쳣��").arg(qstrFileName);
		return false;
    }
}

bool ReadWriteExcel::SetCurrentSheet()
{
return true;
}

QList<QList<QVariant> > ReadWriteExcel::ReadAllData()
{
	QList<QList<QVariant> > context;
	if (false == m_bOpenSuccess)  return context;

	try {
		QAxObject *axUsedRange = m_AxWorksheet->querySubObject("UsedRange");
		if (nullptr == axUsedRange || axUsedRange->isNull())
			return context;

		QVariant var = axUsedRange->dynamicCall("Value2()");   //��ȡ����������ֵ
		delete axUsedRange;

		QList<QVariant> rows = var.toList();
		for (QVariant row : rows)
		{
			context.append(row.toList());
		}

		return context;
	}
	catch (...)
	{
		qDebug() << QString::fromLocal8Bit("��ȡ���������쳣��");
		return context;
	}
}

#include <QElapsedTimer>
QList<QVariant> ReadWriteExcel::ReadRowData(int row)
{
	QList<QVariant> listRow;
	QAxObject* axCell;
	if (false == m_bOpenSuccess || row >= m_iRowCount) return listRow;

	try {
		int iRow = row + m_iStartRow;
		QElapsedTimer elap;
		elap.start();
        QAxObject *axRange = m_AxWorksheet->querySubObject("Range(QVariant, QVariant)","A3", "H3");
        QVariant var = axRange->dynamicCall("Value2()");
		auto rows = var.toList();
		listRow = rows.at(0).toList();

		qint64 ielapse1 = elap.elapsed();

		elap.restart();
		for (int col = 1; col <= m_iColCount; col++)
		{
			axCell = m_AxWorksheet->querySubObject("Cells(int,int)", iRow, col);
			listRow.append(axCell->dynamicCall("Value2()"));
		}
		qint64 ielapse2 = elap.elapsed();
		qDebug() << "��ȡһ�����ݻ���ielapse1:" << ielapse1 << " ielapse2:" << ielapse2;
		return listRow;
	}
	catch (...)
	{
		qDebug() << QString::fromLocal8Bit("��ȡ�������쳣��");
		return listRow;
	}
}

QVariant ReadWriteExcel::ReadCelData(int row, int col)
{
	QVariant var;
	if (false == m_bOpenSuccess || row >= m_iRowCount || col >= m_iColCount) return var;

	try
	{
		int iRow = m_iStartRow + row;
		int iCol = m_iStartCol + col;
		QAxObject *axCell = m_AxWorksheet->querySubObject("Cells(int,int)", iRow, iCol);
		var = axCell->dynamicCall("Value2()");
		return var;
	}
	catch (...)
	{

	}
	return var;
}


bool ReadWriteExcel::WriteCurrentSheet(const QList<QList<QVariant> > rows)
{
    if(rows.size() <= 0)
        return false;
    if(nullptr == m_AxWorksheet || m_AxWorksheet->isNull())
        return false;

    int iRowCount = rows.size();
    int iColCount = rows.at(0).size();

    QString rangStr;
    convertToColName(iColCount, rangStr);
    rangStr += QString::number(iRowCount);
    rangStr = "A1:" + rangStr;
    qDebug()<<rangStr;

    QAxObject *axRange = m_AxWorksheet->querySubObject("Range(const QString&)", rangStr);
//    QAxObject *axRange = m_AxWorksheet->querySubObject("Range(QVariant, QVariant)","A1", rangStr);
    if(NULL == axRange || axRange->isNull())
        return false;

    QVariant var = castListListVariant2Variant(rows);
    bool bRet = axRange->setProperty("Value", var);
    delete axRange;

    return bRet;
}

bool ReadWriteExcel::WriteRowData(int row, QList<QVariant>)
{
	return true;

}

bool ReadWriteExcel::WriteCellData(int row, int col)
{

	return true;
}
