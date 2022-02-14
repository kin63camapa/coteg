#include <malloc.h>
#include <QDir>
#include <QtDebug>
#include <QProcess>
#include <QDateTime>
#include <QPluginLoader>
#include "mem.h"
#include "settings.h"
#include "mew.h"
#if QT_VERSION > 0x060000
#define fromTime_t fromSecsSinceEpoch
#endif
#if QT_VERSION < 0x050000
#include <QHostInfo>
#endif

Mew::Mew(QCoreApplication * a)
{
    uptimer = new QElapsedTimer();
    uptimer->start();
    this->a = a;
    verbose = 1;
    if (AppSettings->contains("DebugMode")) verbose = AppSettings->value("DebugMode").toBool();
    token = AppSettings->value("BotToken").toString();
    me = token.mid(0,token.indexOf(":")).toLongLong();
    if (AppSettings->contains("AdminChatId"))
    {
        chatId = AppSettings->value("AdminChatId").toString();
        tg = new TG(token,chatId);
        connect(tg,SIGNAL(mewUser(User*)),this,SLOT(updateUser(User*)));
        connect(tg,SIGNAL(mewMessage(Message*)),this,SLOT(addMessage(Message*)));
        connect(tg,SIGNAL(mewCallback(Callback*)),this,SLOT(addcall(Callback*)));
    }
    else
    {
        chatId = "";
        tg = new TG(token,QString());
        connect(tg,SIGNAL(mewMessage(Message*)),this,SLOT(WaitID(Message*)));
    }

    QDir pluginsDir( "./plugins" );
    foreach( const QString& pluginName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(pluginName));
        if( loader.load() )
        {
            if( Plugin* plugin = qobject_cast<Plugin*>(loader.instance()))
            {
                qDebug() << "Loaded plugin" << plugin->name();
                if (plugin->name() == "otrs")
                {
                    plugin->setPriority(0);
                    QList<long long> *chats = new QList<long long>;
                    foreach (QString s, AppSettings->value("OtrsChatIds").toStringList()) {
                        chats->append(s.toLongLong());
                    }
                    /*if (chats->isEmpty())
                    {
                        delete chats;
                        chats = 0;
                    }*///no need because otrs not must work in all chats
                    plugin->setChats(chats);
                }
                plugins.append(plugin);
            }
            //loader.unload();
        } else {
            qDebug() << "Failed to load :"+loader.errorString();
        }
    }
    reportSystemInfo("*Start MEW!*");
}

void Mew::WaitID(Message *m)
{
    if (m->text.contains(token))
    {
        chatId = QString::number(m->chat_id);
        tg->setAdminChat(chatId);
        QString rep = QString::fromUtf8("Этот чат выбран в качестве админского!\n id чата: %1\n").arg(chatId);
        if (!AppSettings->isWritable())
        {
            rep+=QString::fromUtf8("Нет возможности сохранить конфигурацию на сервере, проверьте что боту разрешено создавать или изменять файл %1 и перезапустите.\n").arg(AppSettings->fileName());
        }
        else
        {
            AppSettings->setValue("AdminChatId",chatId);
            AppSettings->sync();
            rep+=QString::fromUtf8("Конфигурация сохранена в %1\n").arg(AppSettings->fileName());
        }
        disconnect(tg,SIGNAL(mewMessage(Message*)),this,SLOT(WaitID(Message*)));
        connect(tg,SIGNAL(mewUser(User*)),this,SLOT(updateUser(User*)));
        connect(tg,SIGNAL(mewMessage(Message*)),this,SLOT(addMessage(Message*)));
        connect(tg,SIGNAL(mewCallback(Callback*)),this,SLOT(addcall(Callback*)));
        tg->report(rep);
        reportSystemInfo("*Start MEW!*");
    }
}

void Mew::reportSystemInfo(QString additionalMessage, long long chatid, long long ansver)
{
    OSINFO OSinfo;
    bool altinfo = OSinfo.init();
    QString plgs = "";
    foreach (Plugin * p, plugins) {
       plgs+=p->name()+", ";
    }
    if (plgs.size() > 2) plgs.resize(plgs.size()-2);
    if (chatid == 0) chatid = chatId.toLongLong();
    if (!additionalMessage.isEmpty()) additionalMessage+=" \n";
    additionalMessage += "`Process ID         : "+QString::number(QCoreApplication::applicationPid())+"\n";
#if QT_VERSION < 0x050000
    if(altinfo)
        additionalMessage += "Executable         : "+QCoreApplication::applicationFilePath()+OSinfo.toString(" %pArch%\n");
    else
        additionalMessage += "Executable         : "+QCoreApplication::applicationFilePath()+"\n";

#else
    additionalMessage += "Executable         : "+QCoreApplication::applicationFilePath()+" "+QSysInfo::buildCpuArchitecture()+"\n";
#endif
    additionalMessage += "Process uptime     : "+QString::number(uptimer->elapsed()/1000/86400)+"d "+QDateTime::fromTime_t(uptimer->elapsed()/1000).toUTC().toString("hh:mm:ss")+"\n";
    if (plugins.size())
        additionalMessage += "Plugins loaded     : "+plgs+"\n";
    else
        additionalMessage += "No plugins loaded!\n";
    additionalMessage += "QT version         : "+QString(QT_VERSION_STR)+"\n";
    if(altinfo)
    {
        QString infoText;
        if (OSinfo.isWine)
        {
            QTextStream(&infoText) << "Wine Ver:"+OSinfo.wineVer+QString::fromUtf8(" предоставляет API ");
        }
        else
        {
            if (OSinfo.isWindows) QTextStream(&infoText) << QString::fromUtf8("Windows ");
            else QTextStream(&infoText) << OSinfo.toString(QString::fromUtf8("%OSName% "));
        }
        if (OSinfo.isServer)
            QTextStream(&infoText) << "Server ";
        QTextStream(&infoText) << OSinfo.toString(QString::fromUtf8("%Name%"));
        //QTextStream(&infoText) << OSinfo.szCSDVersion;
        if (OSinfo.ServicePackMajor || OSinfo.ServicePackMinor)
            QTextStream(&infoText) << OSinfo.toString(" Service Pack %ServicePackMajor%.%ServicePackMinor%");
        QTextStream(&infoText) << OSinfo.toString(QString::fromUtf8( " %Bits% разрядная build %MajorVersion%.%MinorVersion%.%BuildNumber%"));
        QTextStream(&infoText) << OSinfo.toString(" %SuiteMask%");
        additionalMessage += "Server info        : "+infoText+"\n";
    }
#if QT_VERSION < 0x050000
    additionalMessage += "Server hostname    : "+QHostInfo::localHostName()+"\n";
#else
    if (!altinfo) additionalMessage += "Server info        : "+QSysInfo::productType()+" "+QSysInfo::currentCpuArchitecture()+" version "+QSysInfo::productVersion()+"\n";
    additionalMessage += "Server hostname    : "+QSysInfo::machineHostName()+"\n";
#endif
    additionalMessage += "Server time        : "+QDateTime::currentDateTime().toString()+"\n";
    additionalMessage += "Server total memory: "+QString::number((totalMemory()-1)/1024/1024)+"MB\n";
    additionalMessage += "Server memory free : "+QString::number((freeMemory()-1)/1024/1024)+"MB\n";
    additionalMessage += "Process use memory : "+QString::number(int((procMemory()+0.001)/1024.0/1024.0))+"MB`&parse_mode=Markdown";
    tg->message(additionalMessage,ansver,chatid);
}

void Mew::shutdown()
{
    //exit(0);
    a->quit();
}

void Mew::updateUser(User *user)
{
    bool newu = 1;
    for (int i = 0; i < users.size(); ++i)
    {
        if (users.at(i)->id == user->id)
        {
            newu = 0;
            //update here
        }
    }
    if (newu)
    {
        users.append(user);
        QString rep = QString::fromUtf8("Я вижу нового пользователя:\nid : %1 ").arg(user->id);
        if (!user->username.isEmpty()) rep += "\n username: "+user->username;
        if (!user->first_name.isEmpty()) rep += "\n first_name: "+user->first_name;
        if (!user->last_name.isEmpty()) rep += "\n last_name: "+user->last_name;
        if (!user->language_code.isEmpty()) rep += "\n language_code: "+user->language_code;
        if (user->male == 0) rep += "\n male: unknown";
        if (user->male > 0) rep += "\n male: male";
        if (user->male < 0) rep += "\n male: fimale";
        if (verbose) tg->report(rep);
        qDebug() << rep;
    } else delete user;
}

void Mew::addMessage(Message *m)
{
    bool newm = 1;
    for (int i = 0; i < messagers.size(); ++i)
    {
        if (messagers.at(i)->id == m->id && messagers.at(i)->chat_id == m->chat_id) newm = 0;
    }
    if (newm)
    {
        messagers.append(m);
        emit newMessage(*m);
        if (verbose) tg->report(QString::fromUtf8("Я вижу новое сообщение: %1 ").arg(m->text.replace("#","%23")), m->id);
        foreach (Plugin* p, plugins) {
            foreach (QString command, p->commands()) {
                if (m->text.mid(0,command.size()) == command){
                    if (p->getChats() == 0 || p->getChats()->contains(m->chat_id) || ((m->chat_id == AppSettings->value("AdminChatId").toString().toLongLong()) && m->chat_id != 0))
                        p->mewMessage(m);
                }
            }
        }
        foreach (Plugin* p, plugins) {
            foreach (QString command, p->trigers()) {
                if (m->text.mid(0,command.size()) == command){
                    if (p->getChats() == 0 || p->getChats()->contains(m->chat_id) || ((m->chat_id == AppSettings->value("AdminChatId").toString().toLongLong()) && m->chat_id != 0))
                    {
                        Message * am = getMessage(m->reply_id);
                        if ((am && am->from == me) || (getUser(me) && m->text.contains("@"+getUser(me)->username)))
                        {
                            p->mewMessage(m);
                        }
                    }
                }
            }
        }
        foreach (Plugin* p, plugins) {
            foreach (QString command, p->trigers()) {
                if (m->text.mid(0,command.size()) == command){
                    if (p->getChats() == 0 || p->getChats()->contains(m->chat_id) || ((m->chat_id == AppSettings->value("AdminChatId").toString().toLongLong()) && m->chat_id != 0))
                    {
                        Message * am = getMessage(m->reply_id);
                        if ((am && am->from == me) || (getUser(me) && m->text.contains("@"+getUser(me)->username)))
                        {
                            p->mewMessage(m);
                        }
                    }
                }
            }
        }


        if (m->text.contains(QString::fromUtf8("info")) || m->text.contains(QString::fromUtf8("инфо")))
        {
            Message * am = getMessage(m->reply_id);
            if (am || m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot"))){
                if ( m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")) || am->from == me )
                {
                    reportSystemInfo();
                }
            }
        }


        if (m->text.contains(QString::fromUtf8("молчи")) || m->text.contains(QString::fromUtf8("тих")))
        {
            Message * am = getMessage(m->reply_id);
            if (am || m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot"))){
                if ( m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")) || am->from == me )
                {
                    if (verbose)
                    {
                        verbose = 0;
                        tg->report(QString::fromUtf8("Молчу-молчу"),m->id);
                    } else
                    {
                        tg->report(QString::fromUtf8("Я и так молчал!"),m->id);
                    }
                }
            }
        }
        if (m->text.contains(QString::fromUtf8("о видишь")) || m->text.contains(QString::fromUtf8("овори!")))
        {
            Message * am = getMessage(m->reply_id);
            if (am || m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")))
            {
                if ( m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")) || am->from == me )
                {
                    if (!verbose)
                    {
                        verbose = 1;
                        tg->report(QString::fromUtf8("Включен подробный режим, теперь я буду сообщать обо всем, что вижу!"),m->id);
                    } else
                    {
                        tg->report(QString::fromUtf8("Подробный режим уже вкючен!"),m->id);
                    }
                }
            }
        }
        if (m->text.contains(QString::fromUtf8("о помнишь")))
        {
            Message * am = getMessage(m->reply_id);
            if (am || m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")))
            {
                if ( m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")) || am->from == me )
                {
                    unsigned long long s = 0;
                    foreach (User *u, users) {
                        s+=64*3;
                        s+=u->username.size()+u->first_name.size()+u->last_name.size();
                    }
                    foreach (Message *m, messagers) {
                        s+=64*5;
                        s+=m->text.size();
                    }
                    tg->report(QString::fromUtf8("Я помню %1 сообщений от %2 пользователей, объемом %3 байт.").arg(messagers.size()).arg(users.size()).arg(s));
                    //tg->report(QString::fromUtf8("%1 байт размер основного класса\n%2 байт размер класса bot api\n%3 размер qapp").arg(malloc_usable_size(this)).arg(malloc_usable_size(tg)).arg(malloc_usable_size(a)));
                }
            }
        }
        if (m->text.contains(QString::fromUtf8("пынх")) || m->text.contains(QString::fromUtf8("ping")) || m->text.contains(QString::fromUtf8("пинг")))
        {
            Message * am = getMessage(m->reply_id);
            if (am || m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")))
            {
                if ( m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")) || am->from == me )
                {
                    if (QDateTime::currentDateTime().time().second()%2) tg->report(QString::fromUtf8("Понг!"),m->id);
                    else tg->report(QString::fromUtf8("Я жив, да, спасибо что спросили..."),m->id);
                }
            }
        }
        if (m->text.contains(QString::fromUtf8("удоли")) || m->text.contains(QString::fromUtf8("удали")))
        {
            Message * am = getMessage(m->reply_id);
            if (am || m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")))
            {
                if ( m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")) || am->from == me )
                {
                    tg->deleteMessage(m->id);
                    tg->deleteMessage(am->id);
                }
            }
        }
        if (m->text.contains(QString::fromUtf8("нопки")) || m->text.contains(QString::fromUtf8("нопок")))
        {
            Message * am = getMessage(m->reply_id);
            if (am || m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")))
            {
                if ( m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")) || am->from == me )
                {
                    QStringList btn;
                    btn << "Button1" << QString::fromUtf8("Кнопка 2") << QString::fromUtf8("Кнопка 3");
                    tg->buttons(QString::fromUtf8("Кнопочный тест"),btn,0,m->id);
                }
            }
        }
        /*if (m->text.contains(QString::fromUtf8("рестарт")) || m->text.contains(QString::fromUtf8("перезапустись")) || m->text.contains(QString::fromUtf8("reboot")) || m->text.contains(QString::fromUtf8("restart")) || m->text.contains(QString::fromUtf8("ребут")))
        {
            Message * am = getMessage(m->reply_id);
            if (am || m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")))
            {
                if ( m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")) || am->from == me )
                {
                    tg->messageEX("><",m->id);
                    tg->deleteMessage(m->id);
                    QProcess::startDetached(QCoreApplication::applicationFilePath());
                    connect(tg,SIGNAL(readyToExit()),this,SLOT(shutdown()));

                }
            }
        }
        if (m->text.contains(QString::fromUtf8("stop!"))  || m->text.contains(QString::fromUtf8("exit!")) || m->text.contains(QString::fromUtf8("стоп!")) || m->text.contains(QString::fromUtf8("выход!")))
        {
            Message * am = getMessage(m->reply_id);
            if (am || m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")))
            {
                if ( m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")) || am->from == me )
                {
                    tg->messageEX(QString::fromUtf8("Какая жаль! Пока-пока"),m->id);
                    connect(tg,SIGNAL(readyToExit()),this,SLOT(shutdown()));
                }
            }
        }*/
        if (m->text.contains(QString::fromUtf8("time"))  || m->text.contains(QString::fromUtf8("время")) || m->text.contains(QString::fromUtf8("времени")))
        {
            Message * am = getMessage(m->reply_id);
            if (am || m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")))
            {
                if ( m->text.contains(QString::fromUtf8("kin63camapa_otrs_bot")) || am->from == me )
                {
                    if (QDateTime::currentDateTime().time().second()%2) tg->report(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"),m->id);
                    else tg->report(QString::fromUtf8("Откуда я знаю, у меня лапки..."),m->id);
                }
            }
        }
    }else delete m;
}

void Mew::addcall(Callback *c)
{
    bool newc = 1;
    for (int i = 0; i < calls.size(); ++i)
    {
        if (calls.at(i) == c->id) newc = 0;
    }
    if (newc)
    {
        calls.append(c->id);
        tg->deleteMessage(c->message_id);
        if (verbose) tg->report(QString::fromUtf8("Нажата кнопка %1 юзером %2").arg(c->data).arg(getUser(c->from)->username));
    }
}

User* Mew::getUser(long long id)
{
    for (int i = 0; i < users.size(); ++i)
    {
        if (users.at(i)->id == id)
        {
            return users.at(i);
        }
    }
    return 0;
}

Message *Mew::getMessage(long long id)
{
    for (int i = 0; i < messagers.size(); ++i)
    {
        if (messagers.at(i)->id == id)
        {
            return messagers.at(i);
        }
    }
    return 0;
}

Mew::~Mew() {

}
