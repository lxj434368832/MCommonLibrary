#ifndef PROFILE_H
#define PROFILE_H

#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QTextCodec>

class  MConfigManage : public QObject
{
    Q_OBJECT
public:
    explicit MConfigManage(QString strFilePath, QObject *parent = nullptr);
    ~MConfigManage();

public slots:
    void setValue( const QString & prefix, const QString & key, const QVariant & value );
    void setDefault( const QString & prefix, const QString & key, const QVariant & value );
    QVariant value( const QString & prefix,const QString &keys,const QVariant & defaultValue = QVariant() );

public slots:
    void setUserValue( const QString & prefix, const QString & key, const QVariant & value );
    void setUserDefault( const QString & prefix, const QString & key, const QVariant & value );
    QVariant userValue( const QString & prefix,const QString &keys,const QVariant & defaultValue = QVariant() );

private:
    QSettings           *configSettings;
    QSettings           *iniSettings;

};

#endif // PROFILE_H
