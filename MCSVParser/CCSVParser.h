#ifndef MCSVPARSER_H
#define MCSVPARSER_H

#if defined(MCSVPARSER_LIBRARY)
#  define MCSVPARSER_EXPORT  __declspec(dllexport)
#else
#  define MCSVPARSER_EXPORT __declspec(dllimport)
#endif
/*
 * function:    用于读写excel文件，采用CSV格式
 * author:      明巧文
 * corporation: 置辰海信科技有限公司成都分公司
 * datetime:    2016-12-5
*/

#include <QStringList>
#include <QFile>
#include <boost/shared_ptr.hpp>

 class MCSVPARSER_EXPORT CSVParser
{
 public:
    CSVParser(QString filePath);
     ~CSVParser(void);

  public:
     bool            setCSVData(const QStringList& CSVList);
     QStringList     getCSVData();

  private:
     boost::shared_ptr<QFile>    CSVFile_;
 };


MCSVPARSER_EXPORT bool readFromSheet(QString strFileName, QList<QStringList> &listData);
#endif // MCSVPARSER_H
