#ifndef PLUGIN_H
#define PLUGIN_H
#include <QtPlugin>
#include "libs/basicstruct/basicstruct.h"
#include <QObject>

class Plugin
{
public:
    virtual QString name()= 0;
    virtual QString descr()= 0;
    virtual int getPriority() = 0;
    virtual void setPriority(int p) = 0;
    virtual QString error() = 0;
    virtual QStringList commands() = 0;
    virtual QStringList trigers() = 0;
    virtual QList<long long> *getChats() = 0;
    virtual void setChats(QList<long long> *chats) = 0;
    virtual QObject * getObject () = 0;
    virtual void mewMessage(Message *m) = 0;
    virtual void mewCallback(Callback *c) = 0;
    virtual MultiChain *getChain() = 0;
signals:
    void message(QString);
    void buttons(QString);
};

Q_DECLARE_INTERFACE( Plugin , "ru.sysadm63.coteg.tgbotplugin/1.0");

#endif // PLUGIN_H
