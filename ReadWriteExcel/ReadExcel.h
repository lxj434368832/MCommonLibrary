#ifndef READEXCEL_H
#define READEXCEL_H

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <QAxObject>
#include <QString>
#include <QFile>


class Position
{
public:
    Position()
    {
        m_row = 0;
        m_col = 0;
    }

    Position(int row, int col)
    {
        m_row = row;
        m_col = col;
    }

public:
    int m_row;
    int m_col;

public:
    bool operator<(const Position & ct) const   // ����const�Ǳ���ġ�
    {
        if (m_row < ct.m_row)
        {
            return true;
        }
        else if (m_row == ct.m_row)
        {
            if ( m_col < ct.m_col )
            {
                return true;
            }
        }
        return false;
    }
};

class ReadExcel
{
public:
    ReadExcel();
    ~ReadExcel();

public:
    //��excel�ļ�
    bool openExcel(const QString& filename);
    //��ȡ ָ����Ԫ�������
    std::string getCellData(const int& row, const int& col);
    //��ȡ ����������
    void getInfo(int& row, int& col) const;

private:
    void getALLfromExcel();

private:
    int m_row;    //��
    int m_col;    //��
    QString m_filename;
    QAxObject* m_excel;
    std::map<Position, std::string> m_mapdata;
    Position p;
};

#endif // READEXCEL_H
