#include "win32_common.h"

//static functions
bool win32_common::openProc(const char *path,const char *params,bool asAdmin)
{
    const char *verb = asAdmin ? "runas" : NULL;
    if ((long long)ShellExecuteA(
                NULL,
                verb,
                path,
                params,
                NULL,   // current working directory
                SW_SHOWNORMAL
                ) <= 32)
        return false;
    return true;
}

bool win32_common::getShutdownPriv()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;

    if (!OpenProcessToken(GetCurrentProcess(),
                          TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return( false );

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
                         &tp.Privileges[0].Luid);
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    tp.PrivilegeCount = 1;

    AdjustTokenPrivileges(hToken, FALSE, &tp, 0,
                          (PTOKEN_PRIVILEGES)NULL, 0);
    if (GetLastError() != ERROR_SUCCESS)
        return false;
    return true;
}

bool win32_common::shutdown()
{
    if (getShutdownPriv() == false)
        return false;

    if (!ExitWindowsEx(EWX_SHUTDOWN,0))
        return false;
    return true;
}

bool win32_common::logoff()
{
    if (getShutdownPriv() == false)
        return false;

    if (!ExitWindowsEx(EWX_LOGOFF,0))
        return false;
    return true;
}

bool win32_common::getProcName(unsigned long pid,std::string &name)
{
    name = std::string("unknown");
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION |
                               PROCESS_VM_READ, //needed for access name
                               FALSE,
                               pid);
    if (hProc == nullptr)
        return false;

    HMODULE hMod;
    unsigned long cbNeeded;
    if (!EnumProcessModules(hProc, &hMod, sizeof(hMod),&cbNeeded))
        return false;
    GetModuleBaseName(hProc, hMod, szProcessName,sizeof(szProcessName)/sizeof(TCHAR));
    CloseHandle(hProc);

    name = to_str(szProcessName);
    return true;
}

bool win32_common::getProcPath(unsigned long pid,std::string &path)
{
    path = std::string("unknown");
    TCHAR szProcessPath[MAX_PATH] = TEXT("<unknown>");
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION |
                               PROCESS_VM_READ, //needed for access name
                               FALSE,
                               pid);
    if (hProc == nullptr)
        return false;

    HMODULE hMod;
    unsigned long cbNeeded;
    if (!EnumProcessModules(hProc, &hMod, sizeof(hMod),&cbNeeded))
        return false;
    GetModuleFileNameEx(hProc, hMod, szProcessPath,sizeof(szProcessPath)/sizeof(TCHAR));
    CloseHandle(hProc);

    path = to_str(szProcessPath);
    return true;
}

bool win32_common::getProcCla(unsigned long pid,std::string &cla)
{
    cla = std::string("unknown");
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION,
                               FALSE,
                               pid);
    if (hProc == nullptr)
        return false;
    DWORD dwClass = GetPriorityClass(hProc);
    CloseHandle(hProc);

    switch (dwClass) {
    case REALTIME_PRIORITY_CLASS:
        cla = std::string("实时");
        break;
    case HIGH_PRIORITY_CLASS:
        cla = std::string("高");
        break;
    case ABOVE_NORMAL_PRIORITY_CLASS:
        cla = std::string("高于正常");
        break;
    case NORMAL_PRIORITY_CLASS:
        cla = std::string("正常");
        break;
    case BELOW_NORMAL_PRIORITY_CLASS:
        cla = std::string("低于正常");
        break;
    case IDLE_PRIORITY_CLASS:
        cla = std::string("低");
        break;
    }
    return true;
}

bool win32_common::getProcMem(unsigned long pid,unsigned long long &mem)
{
    mem = 0;
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION,
                               FALSE,
                               pid);
    if (hProc == nullptr)
        return false;

    PROCESS_MEMORY_COUNTERS pmc;
    unsigned long cb = sizeof(PROCESS_MEMORY_COUNTERS);
    if (!GetProcessMemoryInfo(hProc,&pmc,cb))
        return false;
    CloseHandle(hProc);

    mem = (unsigned long long)((pmc.WorkingSetSize)/1024.0);    //in KB
    return true;
}

bool win32_common::getProcUserName(unsigned long pid,std::string &username)
{
    username = std::string("unknown");
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION,
                               FALSE,
                               pid);
    if (hProc == nullptr)
        return false;

    HANDLE hToken;
    if (!OpenProcessToken(hProc,TOKEN_ALL_ACCESS,&hToken))
        return false;

    CloseHandle(hProc);
    //经过测试，GetTokenInformation总是要求4.5个TOKEN_OWNER大小的空间
    TOKEN_OWNER owner[5];
    DWORD len = 5 * sizeof(owner);
    if (!GetTokenInformation(hToken,TokenOwner,owner,len,&len))
        return false;

    CloseHandle(hToken);
    len = MAX_PATH;
    TCHAR name[len];
    TCHAR domain[len];
    SID_NAME_USE temp;
    if (!LookupAccountSid(NULL,((TOKEN_OWNER *)owner)->Owner,name,&len,domain,&len,&temp))
        return false;

    username = to_str(name);
    return true;
}

bool win32_common::killProc(unsigned long pid)
{
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION |
                               PROCESS_TERMINATE,
                               FALSE,
                               pid);
    if (hProc == nullptr)
        return false;
    bool rtval = TerminateProcess(hProc,-1);
    CloseHandle(hProc);
    return rtval;
}

double win32_common::getCpuClock() const
{
    PROCESSOR_POWER_INFORMATION info[CpuCnt];
    if (CallNtPowerInformation((POWER_INFORMATION_LEVEL)11,nullptr,0,info,CpuCnt * sizeof(PROCESSOR_POWER_INFORMATION)))
        std::cerr << "error get power info. error code: " << GetLastError() << std::endl;
    double avg(0);
    for (decltype(CpuCnt) i = 0; i < CpuCnt; ++i)
        avg += info[i].CurrentMhz;
    avg /= CpuCnt;
    avg /= 1024;    //GHz
    return avg;
}

std::string win32_common::getTimeSinceStart()
{
    auto ms = GetTickCount();
    ms /= 1000;
    decltype (ms) s = ms % 60;
    ms /= 60;
    decltype (ms) m = ms % 60;
    ms /= 60;
    decltype (ms) h = ms % 24;
    ms /= 24;
    decltype (ms) d = ms;
    char buf[13];
    sprintf(buf,"%lu:%02lu:%02lu:%02lu",d,h,m,s);
    return std::string(buf);
}

unsigned long long win32_common::getPageSz()
{
    PERFORMANCE_INFORMATION perfInfo;
    DWORD cb = sizeof(perfInfo);
    msx.dwLength = sizeof(msx);
    if (!GetPerformanceInfo(&perfInfo,cb))
        exit(-1);
    return perfInfo.PageSize;
}

win32_common::win32_common():PageSize(getPageSz())
{
    //get number of logical CPUs
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    CpuCnt = sysInfo.dwNumberOfProcessors;

    //cpu brand
    std::array<int, 4> integerBuffer = {};
    constexpr size_t sizeofIntegerBuffer = sizeof(int) * integerBuffer.size();
    std::array<char, 64> charBuffer = {};
    constexpr std::array<unsigned long long, 3> functionIds = {
        0x8000'0002,    // Manufacturer
        0x8000'0003,    // Model
        0x8000'0004     // Clockspeed
    };

    std::string parts[3];
    int i(0);
    for (int id : functionIds)
    {
        // Get the data for the current ID.
        __cpuid(integerBuffer.data(), id);
        // Copy the raw data from the integer buffer into the character buffer
        std::memcpy(charBuffer.data(), integerBuffer.data(), sizeofIntegerBuffer);
        // Copy that data into a std::string
        parts[i++] += std::string(charBuffer.data());
    }
    CpuInfo = parts[0] + parts[1] + parts[2];

    CpuBaseClock = (double) stold(parts[2]);

    //allocate mem
    CpuLoads = new double[CpuCnt + 1];
    cpuQueries = new PDH_HQUERY[CpuCnt + 1];
    cpuCounters = new PDH_HCOUNTER[CpuCnt + 1];

    //pdh info initialization
    for (unsigned i = 0; i < CpuCnt; ++i) {
        if (PdhOpenQuery(NULL, NULL, &cpuQueries[i]) != ERROR_SUCCESS)
            exit(-1);
        PdhAddCounter(cpuQueries[i], paths[i], NULL, &cpuCounters[i]);
    }
    //_Total CPU load
    if (PdhOpenQuery(NULL, NULL, &cpuQueries[CpuCnt]) != ERROR_SUCCESS)
        exit(-1);
    PdhAddCounter(cpuQueries[CpuCnt], L"\\Processor(_Total)\\% Processor Time",
                  NULL, &cpuCounters[CpuCnt]);

    //first collect
    for (unsigned i = 0; i <= CpuCnt; ++i)
        PdhCollectQueryData(cpuQueries[i]);

    //初始化后填入有效数据
    refresh();
}

bool win32_common::refresh()
{
    //do with process & thread
    pid_thrd.clear();

    HANDLE hProcSnap;
    PROCESSENTRY32 pe32;

    hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcSnap == INVALID_HANDLE_VALUE)
        return false;

    pe32.dwSize = sizeof(PROCESSENTRY32);
    bool retval = Process32First(hProcSnap, &pe32);
    while (retval) {
        pid_thrd[pe32.th32ProcessID] = pe32.cntThreads;
        retval = Process32Next(hProcSnap, &pe32);
    }
    CloseHandle(hProcSnap);

    //cpu loads
    PDH_FMT_COUNTERVALUE counterVals[CpuCnt +1];
    //collect and format
    for (unsigned i = 0; i <= CpuCnt; ++i) {
        PdhCollectQueryData(cpuQueries[i]);
        PdhGetFormattedCounterValue(cpuCounters[i], PDH_FMT_DOUBLE,
                                    NULL, &counterVals[i]);
    }

    //store cpu loads
    for (unsigned i = 0; i <= CpuCnt; ++i)
        CpuLoads[i] = counterVals[i].doubleValue;

    //other cpu info
    PERFORMACE_INFORMATION perinfo;
    DWORD cb = sizeof(perinfo);
    if (!GetPerformanceInfo(&perinfo,cb))
        return false;

    ProcCnt = perinfo.ProcessCount;
    ThrdCnt = perinfo.ThreadCount;
    HandlCnt = perinfo.HandleCount;

    //mem info (in KB)
    PhyTot = perinfo.PhysicalTotal * PageSize / 1024;
    PhyAvail = perinfo.PhysicalAvailable * PageSize / 1024;
    CommTot = perinfo.CommitTotal * PageSize / 1024;
    SysCache = perinfo.SystemCache * PageSize / 1024;
    Paged = perinfo.KernelPaged * PageSize / 1024;
    NonPaged = perinfo.KernelNonpaged * PageSize / 1024;

    GlobalMemoryStatusEx(&msx);
    MemLoad = msx.dwMemoryLoad;

    return true;
}

win32_common::~win32_common()
{
    for (unsigned i = 0; i <= CpuCnt; ++i)
        PdhCloseQuery(cpuQueries[i]);
    delete[] cpuQueries;
    delete[] cpuCounters;
    delete[] CpuLoads;
}
