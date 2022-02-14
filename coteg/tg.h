#ifndef TG_H
#define TG_H

#include <QObject>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QTimer>
#include "./libs/basicstruct/basicstruct.h"

class Request: public QObject
{
    Q_OBJECT
public:
    Request(QString url);
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
public slots:
    void answer();
signals:
    void mew(QString s);
private:
    QString url;
};

class TG: public QObject {
    Q_OBJECT
public:
    TG(QString token,QString chatId);
    ~TG();
    void report(QString message,long long answerid = 0);
    void message(QString message,long long answerid = 0, long long chatid = 0);
    void messageEX(QString message,long long answerid = 0, long long chatid = 0);
    void buttons (QString message,QStringList btns,long long chatid = 0,long long answerid = 0);
    void deleteMessage(long long id,long long chatid = 0);
    void setAdminChat(QString chatId);
    void setAdminChat(long long chatId);
public slots:
    void updates();
    void answer();
    void exexit();
    void reqRes(QString result);
signals:
    void mewCallback(Callback *c);
    void mewMessage(Message *m);
    void mewUser(User *u);
    void readyToExit();
private:
    QString token;
    QString chatId;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QTimer *updateTimer;
    long long update_id;
    void req(QString url,bool wait = 0);
    bool firstUpdate;
    bool exexitflg;
};
#endif // TG_H
