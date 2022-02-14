#include <QDebug>
#include "otrs.h"



OTRS::OTRS()
{
    multichain = new MultiChain;
    chats = 0;
    priority = 0;
}

QString OTRS::name()
{
    return "otrs";
}

QString OTRS::descr()
{
    return "otrs plugin";
}

int OTRS::getPriority()
{
    return 0;
}

void OTRS::setPriority(int p)
{
    priority = p;
}

QString OTRS::error()
{
    return "";
}

QStringList OTRS::commands()
{
    QStringList l;
    l << QString::fromUtf8("#заявка");
    l << QString::fromUtf8("#Заявка");
    l << QString::fromUtf8("#ЗАЯВКА");
    return l;
}

QStringList OTRS::trigers()
{
    return QStringList();
}

QList<long long> *OTRS::getChats()
{
    return chats;
}

void OTRS::setChats(QList<long long> *chats)
{
    this->chats = chats;
}

void OTRS::mewMessage(Message *m)
{
    qDebug() << "otrs plugin message->text" << m->text;
}

void OTRS::mewCallback(Callback *c)
{
    qDebug() << "otrs plugin callback" << c->data;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(CotegOTRSPlugIn, OTRS)
#endif
