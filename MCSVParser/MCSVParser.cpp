#include "MCSVParser.h"
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>

bool writeToSheet(QString strFileName, const QList<QStringList> &listData, QChar cSplit)
{
    QFile file(strFileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning()<<"Open file to write failed!";
        return false;
    }

    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("GB18030"));

    int iRowCount = listData.size();
    for(int i = 0; i < iRowCount; i++)
    {
        QStringList row = listData.at(i);
        int iColCount = row.size();

        for(int j = 0; j < iColCount; j++)
        {
            if(j < iColCount-1)
            {
                out<< row.at(j)<< cSplit;  //非最后一个写入分隔符
            }
            else
            {
                out<< row.at(j)<<'\n';  //最后一个写入换行符
            }
        }
    }

    file.close();
    return true;
}

bool readFromSheet(QString strFileName, QList<QStringList> &listData, QChar cSplit)
{
    listData.clear();
    QFile file(strFileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning()<<"Open file to read failed!";
        return false;
    }

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("GB18030"));

    QString strLine;
    QStringList listLine;

    while(false == in.atEnd())
    {
        strLine = in.readLine();
        listLine = strLine.split(cSplit);
        listData.push_back(listLine);
    }

    file.close();
    return true;
}
