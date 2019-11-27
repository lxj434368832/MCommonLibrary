#include "MConfigManage.h"
#include <windows.h>

MConfigManage::MConfigManage(QString strFilePath, QObject *parent) :
    QObject(parent)
{
    configSettings = new QSettings(strFilePath, QSettings::IniFormat);
    configSettings->setIniCodec(QTextCodec::codecForName("GBK"));

    iniSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, QLatin1String("AmbitionCom"), QLatin1String("MainClient"));
    iniSettings->setIniCodec(QTextCodec::codecForName("GBK"));
}

MConfigManage::~MConfigManage()
{
    delete configSettings;
    delete iniSettings;
}

/*-------------------------------------------
 * 设置默认值
---------------------------------------------*/
void MConfigManage::setDefault(const QString & prefix,const QString &key, const QVariant &value)
{
    if(configSettings->value(prefix+"/"+key).toString().isEmpty())
    {
        setValue(prefix, key, value);
    }
}

/*-------------------------------------------
 * 设置配置文件值
---------------------------------------------*/
void MConfigManage::setValue(const QString & prefix,const QString & key, const QVariant & value)
{
    /*
    configSettings->beginGroup(prefix);
    {
        configSettings->setValue(key, value);
    }
    configSettings->endGroup();*/

#if defined(_WIN32)
    ::WritePrivateProfileStringA(prefix.toLocal8Bit().data(), key.toLocal8Bit().data(), value.toString().toLocal8Bit().data(),
                                 configSettings->fileName().toLocal8Bit().data());
#else
    configSettings->setValue(prefix+"/"+key, value );
#endif
}

/*-------------------------------------------
 * 返回值
---------------------------------------------*/
QVariant MConfigManage::value(const QString & prefix,const QString &key, const QVariant &defaultValue)
{
    return configSettings->value(prefix+"/"+key, defaultValue);
}

/*-------------------------------------------
 * 设置Ini默认值
---------------------------------------------*/
void MConfigManage::setUserDefault(const QString & prefix,const QString &key, const QVariant &value)
{
    QString strKey = prefix+"/"+key;
    if(iniSettings->value(strKey).toString().isEmpty())
    {
        iniSettings->setValue(strKey, value);
    }
}

/*-------------------------------------------
 * 设置Ini配置文件值
---------------------------------------------*/
void MConfigManage::setUserValue(const QString & prefix,const QString & key, const QVariant & value)
{
#if defined(_WIN32)
    ::WritePrivateProfileStringA(prefix.toLocal8Bit().data(), key.toLocal8Bit().data(), value.toString().toLocal8Bit().data(),
                                 iniSettings->fileName().toLocal8Bit().data());
#else
    iniSettings->setValue(prefix+"/"+key, value );
#endif
}

/*-------------------------------------------
 * 返回值Ini
---------------------------------------------*/
QVariant MConfigManage::userValue(const QString & prefix,const QString &key, const QVariant &defaultValue)
{
    QVariant qVar;
#if defined(_WIN32)
    char strValue[MAX_PATH];
    ::GetPrivateProfileStringA(prefix.toLocal8Bit().data(), key.toLocal8Bit().data(), defaultValue.toString().toLocal8Bit().data()
                               ,strValue, MAX_PATH, iniSettings->fileName().toLocal8Bit().data());
    qVar = QVariant::fromValue( QString::fromLocal8Bit(strValue));
#else
    qVar = iniSettings->value(prefix+"/"+key, defaultValue);
#endif

    return qVar;
}
