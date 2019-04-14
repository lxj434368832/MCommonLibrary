#include "MConfigManage.h"

MConfigManage::MConfigManage(QString strFilePath, QObject *parent) :
    QObject(parent)
{
    configSettings = new QSettings(strFilePath, QSettings::IniFormat);
    configSettings->setIniCodec(QTextCodec::codecForName("GBK"));

    iniSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, QLatin1String("CommonLibrary"), QLatin1String("Config"));
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
    configSettings->beginGroup(prefix);
    if(configSettings->value(key).toString().isEmpty())
    {
        configSettings->setValue(key, value);
    }
    configSettings->endGroup();
}

/*-------------------------------------------
 * 设置配置文件值
---------------------------------------------*/
void MConfigManage::setValue(const QString & prefix,const QString & key, const QVariant & value)
{
    configSettings->beginGroup(prefix);
    {
        configSettings->setValue(key, value);
    }
    configSettings->endGroup();
}

/*-------------------------------------------
 * 返回值
---------------------------------------------*/
QVariant MConfigManage::value(const QString & prefix,const QString &keys, const QVariant &defaultValue)
{
//    QVariant values;
//    configSettings->beginGroup(prefix);
//    {
//        values =  configSettings->value( keys,defaultValue);
//    }
//    configSettings->endGroup();
    return configSettings->value(prefix+"/"+keys, defaultValue);;
}

/*-------------------------------------------
 * 设置Ini默认值
---------------------------------------------*/
void MConfigManage::setUserDefault(const QString & prefix,const QString &key, const QVariant &value)
{
    iniSettings->beginGroup(prefix);
    if(iniSettings->value(key).toString().isEmpty())
    {
        iniSettings->setValue(key, value);
    }
    iniSettings->endGroup();
}
/*-------------------------------------------
 * 设置Ini配置文件值
---------------------------------------------*/
void MConfigManage::setUserValue(const QString & prefix,const QString & key, const QVariant & value)
{
    iniSettings->beginGroup(prefix);
    {
        iniSettings->setValue(key, value);
    }
    iniSettings->endGroup();
}
/*-------------------------------------------
 * 返回值Ini
---------------------------------------------*/
QVariant MConfigManage::userValue(const QString & prefix,const QString &keys, const QVariant &defaultValue)
{
    QVariant values;
    iniSettings->beginGroup(prefix);
    {
        values =  iniSettings->value( keys,defaultValue);
    }
    iniSettings->endGroup();
    return values;
}
