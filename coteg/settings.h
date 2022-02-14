#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

extern QSettings *AppSettings;
extern void setDefaultConfig(bool replace = true);

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);
signals:
public slots:
};

#endif // SETTINGS_H
