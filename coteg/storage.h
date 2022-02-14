#ifndef STORAGE_H
#define STORAGE_H
#include "./libs/basicstruct/basicstruct.h"

class Storage
{
public:
    Storage();
    User *getUser(QString name);
    User *getUser(long long uid);
    Message *getMessage(long long id,long long chat_id);
    void saveMessage(const Message *m);
    QList<Message*> getChain(long long chainId);
    void addToChain(const Message *m,long long chainId);
};

#endif // STORAGE_H
