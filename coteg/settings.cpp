#include "settings.h"
#include <QStringList>

QSettings *AppSettings;

void setDefaultConfig(bool replace)
{
    if (replace) AppSettings->clear();
    if (!AppSettings->contains("DebugMode"))
        AppSettings->setValue("DebugMode",bool(true));
    if (!AppSettings->contains("BotToken"))
        AppSettings->setValue("BotToken",QString("0000000000:AABBccddEEffgghhIIKKLLmmnnooPPqq321"));

    if (!AppSettings->contains("AdminChatId"))
        AppSettings->setValue("AdminChatId",QString("-1000000000000"));
    if (!AppSettings->contains("OtrsChatIds"))
    {
        QStringList l;
        l << "-1000000000000";
        l << "-1000000000001";
        AppSettings->setValue("OtrsChatIds",l);
    }
/*
    AppSettings->beginGroup("Test");
    if (!AppSettings->contains("Test"))
        AppSettings->setValue("Test",QString("test"));
    AppSettings->endGroup();
*/
}

Settings::Settings(QObject *parent) :
    QObject(parent)
{
}
