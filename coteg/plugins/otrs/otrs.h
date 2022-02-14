#ifndef OTRS_H
#define OTRS_H

#include <QObject>
#include "../../libs/basicstruct/basicstruct.h"
#include "plugin.h"

class OTRS : public QObject, public Plugin{
    Q_OBJECT
    Q_INTERFACES( Plugin )
#if QT_VERSION > 0x050000
    Q_PLUGIN_METADATA(IID "ru.sysadm63.coteg.tgbotplugin/1.0" FILE "otrs.json")
#endif
public:
    OTRS();
    QString name() override;
    QString descr() override;
    int getPriority() override;
    void setPriority(int p) override;
    QString error() override;
    QStringList commands() override;
    QStringList trigers() override;
    QList<long long> *getChats() override;
    void setChats(QList<long long> *chats) override;
    QObject * getObject() override { return this; }
    MultiChain *getChain() override {return multichain;}
public slots:
    void mewMessage(Message *m) override;
    void mewCallback(Callback *c) override;
signals:
    void message(QString);
    void buttons(QString);
private:
    int priority;
    QList<long long> *chats;
    MultiChain * multichain;

};

#endif // OTRS_H
