#include "simplereactions.h"
#include <QDebug>


SIMPLEREACTIONS::SIMPLEREACTIONS()
{
    multichain = new MultiChain;
}

QString SIMPLEREACTIONS::name()
{
    return "simplereactions";
}

QString SIMPLEREACTIONS::descr()
{
    return "simplereactions plugin";
}

int SIMPLEREACTIONS::getPriority()
{
    return 0;
}

void SIMPLEREACTIONS::setPriority(int p)
{
    prio = p;
}

QString SIMPLEREACTIONS::error()
{
    return "";
}

QStringList SIMPLEREACTIONS::commands()
{
    QStringList l;
    l << QString::fromUtf8("хуй");
    l << QString::fromUtf8("пизда");
    l << QString::fromUtf8("сковорода");
    return l;
}

QStringList SIMPLEREACTIONS::trigers()
{
    return QStringList();
}

QList<long long> *SIMPLEREACTIONS::getChats()
{
    return 0;
}

void SIMPLEREACTIONS::setChats(QList<long long> *chats)
{

}

void SIMPLEREACTIONS::mewMessage(Message *m)
{
    qDebug() << "simplereactions plugin message->text" << m->text;
}

void SIMPLEREACTIONS::mewCallback(Callback *c)
{
    qDebug() << "simplereactions plugin callback" << c->data;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(CotegSIMPLEREACTIONSPlugIn, SIMPLEREACTIONS)
#endif
