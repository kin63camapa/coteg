#include "tg.h"
#include <QtDebug>

#include <QtGlobal>
#if QT_VERSION >= 0x050000
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#else
//download it in https://github.com/eteran/qjson4
#include "qjson4/QJsonDocument.h"
#include "qjson4/QJsonArray.h"
#include "qjson4/QJsonObject.h"
#include "qjson4/QJsonParseError.h"
#endif

TG::TG(QString token,QString chatId)
{
    this->token = token;
    this->chatId = chatId;
    update_id = 0;

    firstUpdate =1;
    manager = new QNetworkAccessManager();
    /*reply = manager->get(QNetworkRequest(QUrl(QString::fromUtf8("https://api.telegram.org/bot%1/getUpdates").arg(token))));
    connect( reply, SIGNAL(finished()),this, SLOT(answer()));*/

    updateTimer = new QTimer();
    connect( updateTimer, SIGNAL(timeout()),this, SLOT(updates()));
    updateTimer->start(3000);
}

void TG::updates()
{
    reply = manager->get(QNetworkRequest(QUrl(QString::fromUtf8("https://api.telegram.org/bot%1/getUpdates?offset=%2").arg(token).arg(update_id))));
    connect(reply, SIGNAL(finished()),this, SLOT(answer()));
}

void TG::answer()
{
    qDebug() << "answer";
    if (reply->error())
    {
        qDebug() << "Error" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QJsonParseError e;
    QJsonDocument d = QJsonDocument::fromJson(reply->readAll(), &e);


    if(!d.isNull() && e.error == QJsonParseError::NoError)
    {
        //qDebug() << "void TG::updates() is array : " << d.isArray() ;
        if (!d.isObject())
        {
            qDebug() << "void TG::updates() Invalid assets index JSON: Root should be an array.";
            return;
        }
        if(!d.object().find("ok").value().toBool())
        {
            qDebug() << "void TG::updates() No ok response";
            qDebug() << d.toJson();
            return;
        }
        QJsonArray data = d.object().find("result").value().toArray();
        foreach (QJsonValue update, data) {
            foreach (QString key, update.toObject().keys())
            {
                QJsonValue value = update.toObject();
                if (key == "message")
                {
                    Message *m = new Message();
                    foreach (QString mkey, value.toObject().find(key)->toObject().keys()) {
                        //QJsonObject mvalue = value.toObject().find(key)->toObject().find(mkey)->toObject();
                        if (mkey == "message_id") m->id = (long long)(value.toObject().find(key)->toObject().find(mkey)->toDouble());
                        if (mkey == "text") m->text = value.toObject().find(key)->toObject().find(mkey)->toString();
                        if (mkey == "chat") m->chat_id = (long long)(value.toObject().find(key)->toObject().find(mkey)->toObject().find("id")->toDouble());
                        if (mkey == "from")
                        {
                            User *u = new User();
                            u->id = m->from = (long long)(value.toObject().find(key)->toObject().find(mkey)->toObject().find("id")->toDouble());
                            foreach (QString ukey, value.toObject().find(key)->toObject().find(mkey)->toObject().keys())
                            {
                                if (ukey == "username") u->username = value.toObject().find(key)->toObject().find(mkey)->toObject().find(ukey)->toString();
                                if (ukey == "first_name") u->first_name = value.toObject().find(key)->toObject().find(mkey)->toObject().find(ukey)->toString();
                                if (ukey == "last_name") u->last_name = value.toObject().find(key)->toObject().find(mkey)->toObject().find(ukey)->toString();
                                if (ukey == "language_code") u->language_code = value.toObject().find(key)->toObject().find(mkey)->toObject().find(ukey)->toString();
                                if (ukey == "is_bot") u->is_bot = value.toObject().find(key)->toObject().find(mkey)->toObject().find(ukey)->toBool();
                            }
                            emit mewUser(u);
                        }
                        if (mkey == "reply_to_message")
                        {
                            Message *rm = new Message();
                            foreach (QString rmkey, value.toObject().find(key)->toObject().find(mkey)->toObject().keys())
                            {
                                //QJsonObject rmvalue = value.toObject().find(key)->toObject().find(mkey)->toObject().find(rmkey)->toObject();
                                if (rmkey == "message_id") m->reply_id = rm->id = (long long)(value.toObject().find(key)->toObject().find(mkey)->toObject().find(rmkey)->toDouble());
                                if (rmkey == "text") rm->text = value.toObject().find(key)->toObject().find(mkey)->toObject().find(rmkey)->toString();
                                if (rmkey == "chat") rm->chat_id = (long long)(value.toObject().find(key)->toObject().find(mkey)->toObject().find(rmkey)->toObject().find("id")->toDouble());
                                if (rmkey == "from")
                                {
                                    User *ru = new User();
                                    ru->id = rm->from = (long long)(value.toObject().find(key)->toObject().find(mkey)->toObject().find(rmkey)->toObject().find("id")->toDouble());
                                    foreach (QString ukey, value.toObject().find(key)->toObject().find(mkey)->toObject().find(rmkey)->toObject().keys())
                                    {
                                        if (ukey == "username") ru->username = value.toObject().find(key)->toObject().find(mkey)->toObject().find(rmkey)->toObject().find(ukey)->toString();
                                        if (ukey == "first_name") ru->first_name = value.toObject().find(key)->toObject().find(mkey)->toObject().find(rmkey)->toObject().find(ukey)->toString();
                                        if (ukey == "last_name") ru->last_name = value.toObject().find(key)->toObject().find(mkey)->toObject().find(rmkey)->toObject().find(ukey)->toString();
                                        if (ukey == "language_code") ru->language_code = value.toObject().find(key)->toObject().find(mkey)->toObject().find(rmkey)->toObject().find(ukey)->toString();
                                        if (ukey == "is_bot") ru->is_bot = value.toObject().find(key)->toObject().find(mkey)->toObject().find(rmkey)->toObject().find(ukey)->toBool();
                                    }
                                    emit mewUser(ru);
                                }
                            }
                            if (!rm->text.isEmpty() && !firstUpdate) emit mewMessage(rm);
                        }
                    }
                    if (!m->text.isEmpty() && !firstUpdate) emit mewMessage(m);
                }
                if (key == "callback_query")
                {
                    Callback *callback = new Callback();
                    foreach (QString ckey, value.toObject().find(key)->toObject().keys()) {
                        QJsonObject mvalue = value.toObject().find(key)->toObject().find(ckey)->toObject();
                        if (ckey == "id") callback->id = value.toObject().find(key)->toObject().find(ckey)->toString().toLongLong();
                        if (ckey == "data") callback->data = value.toObject().find(key)->toObject().find(ckey)->toString();
                        if (ckey == "chat_instance") callback->chat_in = value.toObject().find(key)->toObject().find(ckey)->toString().toLongLong();
                        if (ckey == "from")
                        {
                            User *u = new User();
                            u->id = callback->from = (long long)(value.toObject().find(key)->toObject().find(ckey)->toObject().find("id")->toDouble());
                            foreach (QString ukey, value.toObject().find(key)->toObject().find(ckey)->toObject().keys())
                            {
                                if (ukey == "username") u->username = value.toObject().find(key)->toObject().find(ckey)->toObject().find(ukey)->toString();
                                if (ukey == "first_name") u->first_name = value.toObject().find(key)->toObject().find(ckey)->toObject().find(ukey)->toString();
                                if (ukey == "last_name") u->last_name = value.toObject().find(key)->toObject().find(ckey)->toObject().find(ukey)->toString();
                                if (ukey == "language_code") u->language_code = value.toObject().find(key)->toObject().find(ckey)->toObject().find(ukey)->toString();
                                if (ukey == "is_bot") u->is_bot = value.toObject().find(key)->toObject().find(ckey)->toObject().find(ukey)->toBool();
                            }
                            emit mewUser(u);
                        }
                        if (ckey == "message")
                        {
                            foreach (QString rmkey, value.toObject().find(key)->toObject().find(ckey)->toObject().keys()) {
                                QJsonObject rmvalue = value.toObject().find(key)->toObject().find(ckey)->toObject().find(rmkey)->toObject();
                                if (rmkey == "message_id") callback->message_id = (long long)(value.toObject().find(key)->toObject().find(ckey)->toObject().find(rmkey)->toDouble());
                                if (rmkey == "chat") callback->chat_id = (long long)(value.toObject().find(key)->toObject().find(ckey)->toObject().find(rmkey)->toObject().find("id")->toDouble());
                            }
                        }
                    }
                    if (!callback->data.isEmpty() && !firstUpdate) emit mewCallback(callback);
                }
                if (key == "update_id") update_id = (long long)(value.toObject().find("update_id")->toDouble());
                //else qDebug() << "Undefined key" << key << value << "\n";
            }
        }
    } else {
        qDebug() << e.errorString();
    }
    reply->deleteLater();
    if (firstUpdate) firstUpdate = 0;
}

void TG::exexit()
{
    emit readyToExit();
}

void TG::deleteMessage(long long id,long long chatid)
{
    if (chatid == 0) chatid = chatId.toLongLong();
    req(QString::fromUtf8("https://api.telegram.org/bot%1/deleteMessage?chat_id=%2&message_id=%3").arg(token).arg(chatid).arg(id));
}

void TG::setAdminChat(QString chatId)
{
    this->chatId = chatId;
}

void TG::setAdminChat(long long chatId)
{
    this->chatId = QString::number(chatId);
}

void TG::message(QString message, long long answerid, long long chatid)
{
    if (chatid == 0) chatid = chatId.toLongLong();
    QString url = QString::fromUtf8("https://api.telegram.org/bot%1/sendMessage?chat_id=%2&text=%3").arg(token).arg(chatid).arg(message);
    if (answerid)url+=QString::fromUtf8("&reply_to_message_id=%1").arg(answerid);
    req(url);
}

void TG::messageEX(QString message, long long answerid, long long chatid)
{
    if (chatid == 0) chatid = chatId.toLongLong();
    QString url = QString::fromUtf8("https://api.telegram.org/bot%1/sendMessage?chat_id=%2&text=%3").arg(token).arg(chatid).arg(message);
    if (answerid)url+=QString::fromUtf8("&reply_to_message_id=%1").arg(answerid);
    req(url,true);
}

void TG::buttons(QString message, QStringList btns, long long chatid, long long answerid)
{
    if (chatid == 0) chatid = chatId.toLongLong();
    QString url = QString::fromUtf8("https://api.telegram.org/bot%1/sendMessage?chat_id=%2&text=%3").arg(token).arg(chatid).arg(message);
    if (answerid) url+=QString::fromUtf8("&reply_to_message_id=%1").arg(answerid);
    if (!btns.isEmpty())
    {
        url+= "&reply_markup={\"inline_keyboard\":[[";
        foreach (QString button, btns) {
            url+=QString("{\"text\": \"%1\", \"callback_data\": \"%2\"},").arg(button).arg(button);
        }
        url.resize(url.size()-1);
        url+= "]]}";
    }

    req(url);
}

void TG::report(QString message, long long answerid)
{
    TG::message(message,answerid,chatId.toLongLong());
}

void TG::req(QString url, bool wait)
{
    Request *req = new Request(url);
    if (wait) connect(req,SIGNAL(mew(QString)),this,SLOT(exexit()));
    connect(req,SIGNAL(mew(QString)),this,SLOT(reqRes(QString)));
    connect(req,SIGNAL(mew(QString)),req,SLOT(deleteLater()));
}

void TG::reqRes(QString result)
{
    if (result != "ok") qDebug() << result;
    if (result.contains("retry_after"))
    {
        result = result.replace("retry_after ","");
        int wait = result.mid(0,result.indexOf(" ")).toInt();
        QString url = result.mid(result.indexOf(" ")+1,result.size());
        qDebug() << "need to wait" << wait << url;
    }
}

TG::~TG() {

}

Request::Request(QString url)
{
    this->url = url;
    manager = new QNetworkAccessManager(this);
    reply = manager->get(QNetworkRequest(QUrl(url)));
    if (!reply) emit mew(QString::fromUtf8("Пиздец"));
    connect(reply,SIGNAL(finished()),this,SLOT(answer()));
}

void Request::answer()
{
    /*
    if (reply->error())
    {
        emit mew(reply->errorString());
        reply->deleteLater();
        manager->deleteLater();
        return;
    }
    */
    QString re = "";
    if (reply->error())
    {
        re = reply->errorString();
        re+=": ";
        re = QString::fromUtf8(QByteArray::fromPercentEncoding(re.toLatin1()));
    }
    QJsonParseError e;
    QJsonDocument d = QJsonDocument::fromJson(reply->readAll(), &e);


    if(!d.isNull() && e.error == QJsonParseError::NoError)
    {
        if (!d.isObject())
        {
            emit mew(re+"Invalid assets index JSON: Root should be an array.");
            return;
        }
        if(d.object().find("ok").value().toBool())
        {
            emit mew("ok");
        }else
        {
            if(d.object().find("ok")->isNull() || d.object().find("ok")->isUndefined()) emit mew("No find key \"ok\" in answer. A u realy connect to telegramm?");
            else
            {
                if(d.object().find("retry_after")->toInt())
                {
                    emit mew(re+QString("retry_after %1 %2").arg(d.object().find("retry_after")->toInt()).arg(url));
                }
                if(!(d.object().find("description")->isNull() || d.object().find("description")->isUndefined()))
                {
                    emit mew(re+d.object().find("description")->toString());
                }
                emit mew(re+"Any other error "+d.object().find("error_code")->toString());
            }
        }
    }else
    {
        emit mew(re+e.errorString());
    }
    reply->deleteLater();
    manager->deleteLater();
}
