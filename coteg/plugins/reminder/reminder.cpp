#include "reminder.h"
#include <QDebug>


REMINDER::REMINDER()
{
    multichain = new MultiChain;
}

QString REMINDER::name()
{
    return "reminder";
}

QString REMINDER::descr()
{
    return "reminder plugin";
}

int REMINDER::getPriority()
{
    return 0;
}

void REMINDER::setPriority(int p)
{
    prio = p;
}

QString REMINDER::error()
{
    return "";
}

QStringList REMINDER::commands()
{
    QStringList l;
    l << QString::fromUtf8("Напомни");
    l << QString::fromUtf8("НАПОМНИ");
    l << QString::fromUtf8("напомни");
    return l;
}

QStringList REMINDER::trigers()
{
    return QStringList();
}

QList<long long> *REMINDER::getChats()
{
    return 0;
}

void REMINDER::setChats(QList<long long> *chats)
{

}

void REMINDER::mewMessage(Message *m)
{
    qDebug() << "reminder plugin message->text" << m->text;
}

void REMINDER::mewCallback(Callback *c)
{
    qDebug() << "reminder plugin callback" << c->data;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(CotegREMINDERPlugIn, REMINDER)
#endif
