#include "MCSVParser.h"
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>

CSVParser::CSVParser(QString filePath)
{
    CSVFile_ = boost::shared_ptr<QFile>(new QFile(filePath));
    CSVFile_->open(QIODevice::ReadWrite);
}

CSVParser::~CSVParser(void)
{
    CSVFile_->close();
}

// 注意， QStringList 的参数必须符合csv格式： 逗号分隔，\n结尾
bool CSVParser::setCSVData(const QStringList& CSVList)
{
    if (CSVList.empty())
        return false;

    if (!CSVFile_->isOpen())
        return false;

    if (!CSVFile_->resize(0))
        return false;

    Q_FOREACH(QString str, CSVList)
    {
        CSVFile_->write(str.toUtf8());
    }
    CSVFile_->flush();
    return true;
}

QStringList CSVParser::getCSVData()
{
    QStringList CSVList;
    CSVList.clear();

    QTextStream stream(CSVFile_.get());

    while (!stream.atEnd())
    {
        CSVList.push_back(stream.readLine());
    }

    return CSVList;

}

bool writeToSheet(QString strFileName, QList<QStringList> &listData)
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
        QStringList row = listData[i];
        int iColCount = row.size();

        for(int j = 0; j < iColCount; j++)
        {
            out<< row.at(j)<< ",";//写入文件
        }
        out<<"\n";
    }

    file.close();
    return true;
}

bool readFromSheet(QString strFileName, QList<QStringList> &listData)
{
    QFile file(strFileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning()<<"Open file to read failed!";
        return false;
    }

    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("GB18030"));

    int iRowCount = listData.size();
    for(int i = 0; i < iRowCount; i++)
    {
        QStringList row = listData[i];
        int iColCount = row.size();

        for(int j = 0; j < iColCount; j++)
        {
            out<< row.at(j)<< ",";//写入文件
        }
        out<<"\n";
    }

    file.close();
    return true;
}
