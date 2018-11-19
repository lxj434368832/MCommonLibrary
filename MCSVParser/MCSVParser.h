#ifndef MCSVPARSER_H
#define MCSVPARSER_H

#if defined(MCSVPARSER_LIBRARY)
#  define MCSVPARSER_EXPORT  Q_DECL_EXPORT
#else
#  define MCSVPARSER_EXPORT  Q_DECL_IMPORT
#endif
/*
 * function:    用于读写excel文件，采用CSV格式
 * author:      明巧文
 * corporation: 置辰海信科技有限公司成都分公司
 * datetime:    2016-12-5
*/

#include <QStringList>

/*****************************************************************
 * function:    将表格所有的数据一次写入到excel文件中，效率比较高
 * strFileName: 需要写入的文件名
 * list:        需要写入的所有行数据，每个QStringList是一行的数据，列表头插入到第一行中，然后是每行的数据
 * return:      操作成功返回true, 否则返回false
*****************************************************************/
MCSVPARSER_EXPORT bool writeToSheet(QString strFileName, const QList<QStringList> &listData, QChar cSplit = ',');

/*****************************************************************
 * function:    从excel文件中读取所有表格的数据，效率比较高
 * strFileName: 需要读取的文件名
 * list:        需要读取的所有数据，每个QStringList是一行的数据，列表头在第一行中，然后是每行的数据
 * return:      操作成功返回true, 否则返回false
*****************************************************************/
MCSVPARSER_EXPORT bool readFromSheet(QString strFileName, QList<QStringList> &listData, QChar cSplit = ',');


#endif // MCSVPARSER_H
