#ifndef MEXCEL_H
#define MEXCEL_H

#if defined(MEXCEL_LIBRARY)
#  define MEXCEL_EXPORT __declspec(dllexport)
#else
#  define MEXCEL_EXPORT __declspec(dllimport)
#endif

#include <QList>
#include <QVariant>

class QAxObject;

class MEXCEL_EXPORT MExcel : QObject
{
    Q_OBJECT
public:
    MExcel();
    void create(QString strFileName);
    QVariant readAll();
    bool writeCurrentSheet(const QList<QList<QVariant> > &cells);
private:
    void convertToColName(int data, QString &res);
    QString to26AlphabetString(int data);
    void castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant> > &res);

private:
    QAxObject *m_pApplication;
    QAxObject *m_pWorkBooks;
    QAxObject *m_pWorkBook;
    QAxObject *m_pSheets;
    QAxObject *m_pSheet;

};

#endif // MEXCEL_H
