#ifndef MEW_H
#define MEW_H

#include <QCoreApplication>
#include <QObject>
#include <QList>
#include <QElapsedTimer>
#include "./libs/basicstruct/basicstruct.h"
#include "tg.h"
#include "plugin.h"

class Mew: public QObject {
    Q_OBJECT
public:
    Mew(QCoreApplication * a);
    ~Mew();
public slots:
    void updateUser(User *user);
    void addMessage(Message *m);
    void addcall(Callback *c);
    void WaitID(Message *m);
    void reportSystemInfo(QString additionalMessage = "",long long chatid = 0,long long ansver = 0);
    void shutdown();
signals:
    void newCallback(Callback *);
    void newMessage(Message);
private:
    long long me;
    bool verbose;
    TG *tg;
    QList<User*> users;
    QList<Message*> messagers;
    QList<long long> calls;
    User* getUser(long long id);
    Message *getMessage(long long id);
    QString token;
    QString chatId;
    QCoreApplication * a;
    QElapsedTimer *uptimer;
    QList <Plugin*> plugins;
};

#endif // MEW_H
