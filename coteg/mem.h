#ifndef MEM_H
#define MEM_H

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>

unsigned long long totalMemory()
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullTotalPhys;
}
unsigned long long freeMemory()
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullTotalPageFile - status.ullAvailPageFile;
}
typedef HRESULT (_stdcall *GetProcessMemoryInfo_)(HANDLE,PROCESS_MEMORY_COUNTERS*,DWORD);
unsigned long long procMemory()
{
    PROCESS_MEMORY_COUNTERS_EX pmc;

    HMODULE psapi = LoadLibrary(TEXT("Psapi.dll"));
    if (!psapi)
    {
        return 0;
    }
    GetProcessMemoryInfo_ GetProcessMemoryInfo = (GetProcessMemoryInfo_)GetProcAddress(psapi, "GetProcessMemoryInfo");
    if (!CoInitialize)
    {
        return 0;
    }
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return  pmc.PrivateUsage;
}

#else
#include <unistd.h>
#include "sys/types.h"
#include "sys/sysinfo.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

long long parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    long long i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoll(p);
    return i;
}

unsigned long long totalMemory()
{
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size;
}
unsigned long long freeMemory()
{
    struct sysinfo memInfo;
    sysinfo (&memInfo);
    long long virtualMemUsed = memInfo.totalram - memInfo.freeram;
    //Add other values in next statement to avoid int overflow on right hand side...
    //virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
    virtualMemUsed *= memInfo.mem_unit;
    return virtualMemUsed;
}
unsigned long long procMemory()
{
    FILE* file = fopen("/proc/self/status", "r");
    long long result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result*1024;
}

#endif
#endif // MEM_H
