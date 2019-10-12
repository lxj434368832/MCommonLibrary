#ifndef MSETTINGS_H
#define MSETTINGS_H
#include <QString>
#include <QVariant>

class QSettings;

class Settings
{
public:
    Settings(const QString & fileName);
    ~Settings();
    void setValue ( const QString & key, const QVariant & value );
    QVariant value ( const QString & key, const QVariant & defaultValue = QVariant() ) const;
private:
    QSettings *d;
};

#endif // MSETTINGS_H
