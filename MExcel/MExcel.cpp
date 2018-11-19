#include "MExcel.h"
#include <QAxObject>
#include <QDebug>

MExcel::MExcel()
{
}

void MExcel::create(QString strFileName)
{
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible",true);
    QAxObject *workbooks = excel.querySubObject("WorkBooks");
    workbooks->dynamicCall("Open (const QString&)",QString("c:/a.xls"));
    QAxObject *workbook = excel.querySubObject("ActiveWorkBook");   //获取活动工作簿,刚打开的即为活动工作薄
    QAxObject *worksheets = workbook->querySubObject("WorkSheets"); //获取所有的工作表
    QAxObject *workSheet = workbook->querySubObject("WorkSheet(int)", 1); // 获取第一个工作表
    QAxObject *cell = workSheet->querySubObject("Cells(int,int)",1,1);    //获取cell(1,1)对象
    cell->dynamicCall("Value",QString("value的值"));                       //设置单元的值
    QString strCellValue = cell->dynamicCall("Value2()").toString();      //获取单元的值

}

QVariant MExcel::readAll()
{
    QVariant var;
    if (this->m_pSheet != NULL && ! this->m_pSheet->isNull())
    {
        QAxObject *usedRange = this->m_pSheet->querySubObject("UsedRange");
        if(NULL == usedRange || usedRange->isNull())
        {
            return var;
        }
        var = usedRange->dynamicCall("Value");
        delete usedRange;
    }
    return var;
}
///
/// \brief 把QVariant转为QList<QList<QVariant> >
/// \param var
/// \param res
///
void MExcel::castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant> > &res)
{
    QVariantList varRows = var.toList();
    if(varRows.isEmpty())
    {
        return;
    }

    const int rowCount = varRows.size();
    QVariantList rowData;
    for(int i=0;i<rowCount; ++i)
    {
        rowData = varRows[i].toList();      //每一个varRows[i]是QVariant
        res.push_back(rowData);
    }
}

void castListListVariant2Variant(const QList<QList<QVariant> > &res, QVariant &var)
{
    QVariantList varList;
    int iRowCount = res.size();
    for(int i = 0; i < iRowCount; i++)
    {
        varList.push_back(res.at(i));   //将每一个QList<QVariant>转换成QVariant
    }

    var = QVariant(varList);
}

///
/// \brief 写入一个表格内容
/// \param cells
/// \return 成功写入返回true
/// \see readAllSheet
///
bool MExcel::writeCurrentSheet(const QList<QList<QVariant> > &cells)
{
    if(cells.size() <= 0)
        return false;
    if(NULL == this->m_pSheet || this->m_pSheet->isNull())
        return false;
    int row = cells.size();
    int col = cells.at(0).size();
    QString rangStr;
    convertToColName(col,rangStr);
    rangStr += QString::number(row);
    rangStr = "A1:" + rangStr;
    qDebug()<<rangStr;
    QAxObject *range = this->m_pSheet->querySubObject("Range(const QString&)",rangStr);
    if(NULL == range || range->isNull())
    {
        return false;
    }
    bool succ = false;
    QVariant var;
    castListListVariant2Variant(cells,var);
    succ = range->setProperty("Value", var);
    delete range;
    return succ;
}

///
/// \brief 把列数转换为excel的字母列号
/// \param data 大于0的数
/// \return 字母列号，如1->A 26->Z 27 AA
///
void MExcel::convertToColName(int data, QString &res)
{
    Q_ASSERT(data>0 && data<65535);
    int tempData = data / 26;
    if(tempData > 0)
    {
        int mode = data % 26;
        convertToColName(mode,res);
        convertToColName(tempData,res);
    }
    else
    {
        res=(to26AlphabetString(data)+res);
    }
}
//
// \brief 数字转换为26字母
//
// 1->A 26->Z
// \param data
// \return
//
QString MExcel::to26AlphabetString(int data)
{
    QChar ch = data + 0x40;//A对应0x41
    return QString(ch);
}
