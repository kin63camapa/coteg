#ifndef basicstruct_H
#define basicstruct_H

#include <QString>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QtCore/QVariant>

struct User
{
    User();
    long long id;
    bool is_bot;
    QString first_name;
    QString last_name;
    QString username;
    QString language_code;
    int male;
    QStringList alowedCommands;
    QStringList rejectedCommands;
};

struct Message
{
    Message();
    long long reply_id;
    long long id;
    long long from;
    long long chat_id;
    QString text;
};

struct Callback
{
    Callback();
    long long id;
    long long from;
    long long chat_id;
    long long chat_in;
    long long message_id;
    QString data;
};

class Chain : public QList<Message*>
{
public:
    Chain();
    long long CID;
    long long firstMID;
    QMap<QString,QVariant> *data;
};

class MultiChain : public QList<Chain*>
{
    Chain *find(long long CID,long long MID);
};

Q_DECLARE_METATYPE(Chain);
Q_DECLARE_METATYPE(MultiChain);


extern struct OSINFO
{
   enum WIN
   {
       W95X86     = 4000,
       W98X86     = 4100,
       WMEX86     = 4900,
       W2000X86   = 5000,
       W2000X64   = 5001,//а такие бывают?
       W2000IA64  = 5002,
       W2KSERVERX86 = 5010,
       W2KSERVERX64 = 5011,
       W2KSERVERIA64 = 5012,
       WXPX86     = 5100,
       WXPX64     = 5101,
       WXPIA64    = 5102,
       W2003X86   = 5210,
       W2003X64   = 5211,
       W2003IA64  = 5212,
       WVISTAX86  = 6000,
       WVISTAX64  = 6001,
       W2008X86   = 6010,
       W2008X64   = 6011,
       W7X86      = 6100,
       W7X64      = 6101,
       W2008R2X64 = 6111,
       W8X86      = 6200,
       W8X64      = 6201,
       W2012X64   = 6211,
       W8_1X86    = 6300,
       W8_1X64    = 6301,
       W2012R2X64 = 6311,
       W10X86P    = 6400,
       W10X64P    = 6401,
       W2014X64P  = 6411,
       W10X86     = 10000,
       W10X64     = 10001,
       W2014X64   = 10011,
       WINVALIDX
   } Win;
   enum WINNOARCH
   {
       W95        = 4000,
       W98        = 4100,
       WME        = 4900,
       W2000      = 5000, //lol
       W2KSERVER  = 5010,
       WXP        = 5100,
       W2003      = 5210,
       WVISTA     = 6000,
       W2008      = 6010,
       W7         = 6100,
       W2008R2    = 6111,
       W8         = 6200,
       W2012      = 6211,
       W8_1       = 6300,
       W2012R2    = 6311,
       W10P       = 6400,
       W2014P     = 6411,
       W10        = 10000,
       W2014      = 10011,
       WINVALID
   } WinNoarch;
   bool isServer;
   bool isWine;
   bool isWindows;
   bool is64;
   bool is64build;
   bool isAdmin;
   unsigned long MajorVersion;
   unsigned long MinorVersion;
   unsigned long BuildNumber;
   unsigned long PlatformId;
   QString szCSDVersion;
   unsigned short ServicePackMajor;
   unsigned short ServicePackMinor;
   unsigned short SuiteMask;
   unsigned char ProductType;
   unsigned char Reserved;
   QString toString(QString format = "");
   QString error(QString er = "");
   QString wineVer;
   bool init();
private:
   QString lastError;
   bool isRunAsAdmin();
   bool isWow64();
} OSinfo;

#endif
