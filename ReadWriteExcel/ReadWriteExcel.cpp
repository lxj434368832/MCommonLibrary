#include "ReadWriteExcel.h"
//#include <exception>
#include <QFile>
#include <QDebug>
#include <QAxObject>

ReadWriteExcel::ReadWriteExcel()
{
	m_bOpenSuccess = false;
	m_AxExcel = new QAxObject("Excel.Application"); //连接excel控件
	//m_AxExcel->setProperty("Visible", true);       //设置为不显示窗体	
	//m_AxExcel->dynamicCall("SetVisible(bool Visible)", "true");//设置为不显示窗体
	m_AxExcel->setProperty("DisplayAlerts", false);//不显示任何警告信息，如关闭时的是否保存提示
	m_AxWorkbooks = m_AxExcel->querySubObject("WorkBooks");

	m_AxWorkbooks = nullptr;
	m_AxWorkbook = nullptr;
	m_AxWorksheets = nullptr;
	m_AxWorksheet = nullptr;

}

ReadWriteExcel::~ReadWriteExcel()
{
	if (m_AxWorkbook)
		m_AxWorkbook->dynamicCall("Close(Boolean)", false); //关闭文件
	//关闭excel程序,操作完成后记着关闭，由于是隐藏的看不到，不关闭进程会有很多excel.exe
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
			qDebug() << "获取所有工作表失败！";
			return false;
		}
		m_AxWorksheet = m_AxWorksheets->querySubObject("Item(int)", 1);
		if (nullptr == m_AxWorksheet)
		{
			qDebug() << "获取第一张工作表失败！";
			return false;
		}

		QAxObject *axUsedRange = m_AxWorksheet->querySubObject("UsedRange");
		if (nullptr == axUsedRange)
		{
			qDebug() << "获取使用范围失败！";
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
		qDebug() << QString::fromLocal8Bit("打开文件%1异常！").arg(qstrFileName);
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

		QVariant var = axUsedRange->dynamicCall("Value2()");   //读取区域内所有值
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
		qDebug() << QString::fromLocal8Bit("读取所有数据异常！");
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
		qDebug() << "获取一行数据花费ielapse1:" << ielapse1 << " ielapse2:" << ielapse2;
		return listRow;
	}
	catch (...)
	{
		qDebug() << QString::fromLocal8Bit("读取行数据异常！");
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
