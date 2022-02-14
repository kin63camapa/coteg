#include "basicstruct.h"
#include <QDebug>
#include <QString>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/utsname.h>
#endif


User::User()
{
    id = 0;
    is_bot = 0;
    male = 0;
}

Message::Message()
{
    id = 0;
    from = 0;
    chat_id = 0;
    reply_id = 0;
}
Callback::Callback()
{
    id = 0;
    from = 0;
    chat_id = 0;
    chat_in = 0;
    message_id = 0;
}

QString OSINFO::toString(QString format)
{
    if (format.isEmpty())
        return QString("%1.%2").arg(MajorVersion).arg(MinorVersion);
    else
    {
        format = format.replace("%MajorVersion%",QString::number(MajorVersion));
        format = format.replace("%MinorVersion%",QString::number(MinorVersion));
        format = format.replace("%BuildNumber%",QString::number(BuildNumber));
        format = format.replace("%szCSDVersion%",szCSDVersion);
        format = format.replace("%ServicePackMajor%",QString::number(ServicePackMajor));
        format = format.replace("%ServicePackMinor%",QString::number(ServicePackMinor));
        format = format.replace("%SuiteMaskN%",QString::number(SuiteMask));
#ifdef _WIN32
        switch (SuiteMask)
        {
        case VER_SUITE_BACKOFFICE:
            format = format.replace("%SuiteMask%","BackOffice");
            break;
        case VER_SUITE_BLADE:
            format = format.replace("%SuiteMask%","Web Edition");
            break;
        case VER_SUITE_COMPUTE_SERVER:
            format = format.replace("%SuiteMask%","Compute Cluster Edition");
            break;
        case VER_SUITE_DATACENTER:
            if (WinNoarch == W2003) format = format.replace("%SuiteMask%","Datacenter Edition");
            if (WinNoarch == W2KSERVER) format = format.replace("%SuiteMask%","Datacenter Server");
            format = format.replace("%SuiteMask%","Datacenter");
            break;
        case VER_SUITE_ENTERPRISE:
            if (WinNoarch == W2003) format = format.replace("%SuiteMask%","Enterprise Edition");
            if (WinNoarch == W2KSERVER) format = format.replace("%SuiteMask%","Advanced Server");
            format = format.replace("%SuiteMask%","Enterprise");
            break;
        case VER_SUITE_SINGLEUSERTS:
            format = format.replace("%SuiteMask%","");
            break;
        case VER_SUITE_SMALLBUSINESS:
            format = format.replace("%SuiteMask%","Small Business Server");
            break;
        case VER_SUITE_STORAGE_SERVER:
            format = format.replace("%SuiteMask%","Windows Storage Server");
            break;
        case VER_SUITE_TERMINAL:
            format = format.replace("%SuiteMask%","Terminal Services");
            break;
//        case VER_SUITE_WH_SERVER:
//            format = format.replace("%SuiteMask%","Home Server");
//            break;

        }
#endif
        format = format.replace("%SuiteMask%","");

        format = format.replace("%ProductTypeN%",QString::number(ProductType));
        //format = format.replace("%ProductType%",QString(ProductType));
        format = format.replace("%ReservedN%",QString::number(Reserved));
        format = format.replace("%Reserved%","");
        switch (WinNoarch)
        {
        case W95: format = format.replace("%Name%","95"); break;
        case W98: format = format.replace("%Name%","98"); break;
        case WME: format = format.replace("%Name%","ME"); break;
        case W2KSERVER: format = format.replace("%Name%","2000"); break;
        case W2000: format = format.replace("%Name%","2000"); break;
        case WXP: format = format.replace("%Name%","XP"); break;
        case W2003: format = format.replace("%Name%","2003"); break;
        case WVISTA: format = format.replace("%Name%","Vista"); break;
        case W2008: format = format.replace("%Name%","2008"); break;
        case W7: format = format.replace("%Name%","7"); break;
        case W2008R2: format = format.replace("%Name%","2008R2"); break;
        case W8: format = format.replace("%Name%","8"); break;
        case W2012: format = format.replace("%Name%","2012"); break;
        case W8_1: format = format.replace("%Name%","8.1"); break;
        case W2012R2: format = format.replace("%Name%","2012R2"); break;
        case W10P: format = format.replace("%Name%","10"); break;
        case W2014P: format = format.replace("%Name%","Technical Preview"); break;
        case W10: format = format.replace("%Name%","10"); break;
        case W2014: format = format.replace("%Name%","Technical Preview"); break;
        case WINVALID: format = format.replace("%Name%","Unknown"); break;
        default: format = format.replace("%Name%","Unknown"); WinNoarch = WINVALID; break;
        }
        if (is64)
        {
            format = format.replace("%arch%","x64");
            format = format.replace("%Arch%","X64");
            format = format.replace("%Bits%","64");
            format = format.replace("%Is64N%","1");
            format = format.replace("%IS64%","TRUE");
            format = format.replace("%Is64%","True");
            format = format.replace("%is64%","true");
            format = format.replace("%Is32N%","0");
            format = format.replace("%IS32%","FALSE");
            format = format.replace("%Is32%","False");
            format = format.replace("%is32%","false");
        }
        else
        {
            format = format.replace("%arch%","x86");
            format = format.replace("%Arch%","X86");
            format = format.replace("%Bits%","32");
            format = format.replace("%Is64N%","0");
            format = format.replace("%IS64%","FALSE");
            format = format.replace("%Is64%","False");
            format = format.replace("%is64%","false");
            format = format.replace("%Is32N%","1");
            format = format.replace("%IS32%","TRUE");
            format = format.replace("%Is32%","True");
            format = format.replace("%is32%","true");
        }
        if (is64build)
        {
            format = format.replace("%parch%","x64");
            format = format.replace("%pArch%","X64");
            format = format.replace("%pBits%","64");
            format = format.replace("%pIs64N%","1");
            format = format.replace("%pIS64%","TRUE");
            format = format.replace("%pIs64%","True");
            format = format.replace("%pis64%","true");
            format = format.replace("%pIs32N%","0");
            format = format.replace("%pIS32%","FALSE");
            format = format.replace("%pIs32%","False");
            format = format.replace("%pis32%","false");
        }
        else
        {
            format = format.replace("%parch%","x86");
            format = format.replace("%pArch%","X86");
            format = format.replace("%pBits%","32");
            format = format.replace("%pIs64N%","0");
            format = format.replace("%pIS64%","FALSE");
            format = format.replace("%pIs64%","False");
            format = format.replace("%pis64%","false");
            format = format.replace("%pIs32N%","1");
            format = format.replace("%pIS32%","TRUE");
            format = format.replace("%pIs32%","True");
            format = format.replace("%pis32%","true");
        }
        if (isAdmin)
        {
            format = format.replace("%Privileges%","Admin");
            format = format.replace("%isAdminN%","1");
            format = format.replace("%isAdminN%","true");
            format = format.replace("%IsAdminN%","True");
            format = format.replace("%ISAdminN%","TRUE");
        }
        else
        {
            format = format.replace("%Privileges%","User");
            format = format.replace("%isAdminN%","0");
            format = format.replace("%isAdminN%","false");
            format = format.replace("%IsAdminN%","False");
            format = format.replace("%ISAdminN%","FALSE");
        }
        return format;
    }
}

QString OSINFO::error(QString er)
{
    if (er.size()) lastError = QString::fromUtf8(er.toLatin1());
    return lastError;
}

#ifdef _WIN32

#pragma comment(lib, "user32.lib")

std::wstring code2text(DWORD errorMessageID)
{
    if(errorMessageID == 0) {
        return std::wstring(); //No error message has been recorded
    }

    LPWSTR messageBuffer = 0;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

    //Copy the error message into a std::string.
    std::wstring message(messageBuffer, size);

    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);

    return message;

}


bool OSINFO::isRunAsAdmin()
{
    BOOL (*CheckTokenMembership)(HANDLE,PSID,PBOOL);

    int ret = -1;
    BOOL fIsRunAsAdmin = FALSE;
    PSID pAdministratorsGroup = NULL;
    HINSTANCE dllHandle = NULL;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if(AllocateAndInitializeSid(
                &NtAuthority,
                2,
                SECURITY_BUILTIN_DOMAIN_RID,
                DOMAIN_ALIAS_RID_ADMINS,
                0, 0, 0, 0, 0, 0,
                &pAdministratorsGroup))
    {
        dllHandle = LoadLibrary(TEXT("Advapi32.dll"));
        if (NULL != dllHandle)
        {
            CheckTokenMembership = (BOOL(*)(HANDLE,PSID,PBOOL))GetProcAddress(dllHandle,"CheckTokenMembership");
            if (NULL != CheckTokenMembership)
            {
                CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin);
                ret = fIsRunAsAdmin;
            }
            FreeLibrary(dllHandle);
        }
        else
        {
            ret = -2;
            qDebug() << "OSINFO::isRunAsAdmin() LoadLibrary(\"Advapi32.dll\") failed: "+QString::fromStdWString(code2text(GetLastError()));
        }
    }
    else
    {
        ret = -3;
        qDebug() << "OSINFO::isRunAsAdmin() AllocateAndInitializeSid() failed: "+QString::fromStdWString(code2text(GetLastError()));
    }
    if (pAdministratorsGroup)
    {
        FreeSid(pAdministratorsGroup);
        pAdministratorsGroup = NULL;
    }
    return ret;
}

bool OSINFO::isWow64()
{
#ifdef WIN64
    return is64build = true;
#else
    is64build = false;
    BOOL bIsWow64 = FALSE;
    typedef BOOL (APIENTRY *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fnIsWow64Process;
    HMODULE module = GetModuleHandle(TEXT("kernel32.dll"));
    if(NULL == module)
    {
        error("Can not GetModuleHandle from kernel32.dll\nProbably, system is X86!\nCoder id Invalid!");
        return 0;
    }
    fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(module, "IsWow64Process");
    if(fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
        {
            error("API fnIsWow64Process() is FAIL!\nProbably, system is X86!\nCoder id Invalid!");
            return FALSE;
        }
    }
    else
    {
        error("Can not find API fnIsWow64Process() in kernel32.dll\nProbably, system is X86!\nCoder id Invalid!");
        return FALSE;
    }
    return bIsWow64 != FALSE;
#endif
}

bool OSINFO::init()
{
    isWindows = 1;
    typedef struct _RTL_OSVERSIONINFOW {
      ULONG  dwOSVersionInfoSize;
      ULONG  dwMajorVersion;
      ULONG  dwMinorVersion;
      ULONG  dwBuildNumber;
      ULONG  dwPlatformId;
      WCHAR  szCSDVersion[128];
    } *PRTL_OSVERSIONINFOW;

    struct _RTL_OSVERSIONINFOEXW {
      ULONG  dwOSVersionInfoSize;
      ULONG  dwMajorVersion;
      ULONG  dwMinorVersion;
      ULONG  dwBuildNumber;
      ULONG  dwPlatformId;
      WCHAR  szCSDVersion[128];
      USHORT  wServicePackMajor;
      USHORT  wServicePackMinor;
      USHORT  wSuiteMask;
      UCHAR  wProductType;
      UCHAR  wReserved;
    };

    BOOL (*RtlGetVersion)(PRTL_OSVERSIONINFOW);

    error("Пипец какая странная ошибка");
    MajorVersion = 0;
    isServer = 0;
    isWine = 0;
    _RTL_OSVERSIONINFOEXW osver;
    ZeroMemory(&osver, sizeof(_RTL_OSVERSIONINFOEXW));
    osver.dwOSVersionInfoSize = sizeof(_RTL_OSVERSIONINFOEXW);

    HINSTANCE dllHandle;
    if ((dllHandle = LoadLibrary(TEXT("ntdll.dll"))))
    {
        static const char (*pwine_get_version)(void);
        pwine_get_version = (const char (*)())GetProcAddress(dllHandle, "wine_get_version");
        if(pwine_get_version)
        {
            isWine = true;
            wineVer = pwine_get_version();
        }
        if ((RtlGetVersion = (BOOL(*)(PRTL_OSVERSIONINFOW))GetProcAddress(dllHandle,"RtlGetVersion")))
        {
            if(RtlGetVersion((_RTL_OSVERSIONINFOW *) &osver))
            {
                osver.dwOSVersionInfoSize = sizeof(_RTL_OSVERSIONINFOW);
                if(RtlGetVersion((_RTL_OSVERSIONINFOW *) &osver))
                    qDebug() << "OSINFO::init() RtlGetVersion is fail!";
                else
                    qDebug() << "OSINFO::init() RtlGetVersion is fail for _RTL_OSVERSIONINFOEXW will use _RTL_OSVERSIONINFOW";
            }
            else
            {
                /*qDebug() << "RtlGetVersion is OK"*/;
            }
        }else
        {
            qDebug() << "OSINFO::init() Can not find API RtlGetVersion() in ntdll.dll";
        }
        FreeLibrary(dllHandle);
    }else
    {
        qDebug() << "OSINFO::init() LoadLibrary \"ntdll.dll\" is fail!";
    }
    if ( osver.wProductType == VER_NT_SERVER )
    {
        qDebug() << "OSINFO::init() Probably server";
        isServer = true;
    }
    lastError.clear();
    is64 = isWow64();
    if (lastError.size()) qDebug() << "OSINFO::init()"+lastError;
    isAdmin = isRunAsAdmin();
    MajorVersion = osver.dwMajorVersion;
    MinorVersion = osver.dwMinorVersion;
    BuildNumber = osver.dwBuildNumber;
    //unsigned long  dwOSVersionInfoSize;
    PlatformId = osver.dwPlatformId;
    szCSDVersion = QString::fromWCharArray(osver.szCSDVersion);
    ServicePackMajor = osver.wServicePackMajor;
    ServicePackMinor = osver.wServicePackMinor;;
    SuiteMask =  osver.wSuiteMask;
    ProductType = osver.wProductType;
    Reserved = osver.wReserved;

    int tmp;

    int win;
    win=MajorVersion*1000+MinorVersion*100+int(isServer)*10+int(is64);
    Win=static_cast<WIN>(win);
    tmp=win;
    if (is64) win--;
    if (isServer && MajorVersion == 6 && MinorVersion) win++;
    WinNoarch=static_cast<WINNOARCH>(win);

    qDebug() << "OSINFO::init()"+QString("WIN: %1\nWINNOARCH %2").arg(tmp).arg(win);

    return MajorVersion;
}
#else

bool OSINFO::isRunAsAdmin()
{
    return !geteuid();
}

bool OSINFO::isWow64()
{
    return false;
}

bool OSINFO::init()
{
    isWindows = 0;
    return 0;
}
#endif

Chain::Chain()
{
    data = new QMap<QString,QVariant>;
    CID = 0;
    firstMID = 0;
}

Chain *MultiChain::find(long long CID, long long MID)
{
    foreach (Chain *ch, *this) {
        if (ch->CID == CID)
        {
            foreach (Message*m, *ch) {
                if (m->id == MID) return ch;
            }
        }
    }
    return 0;
}
