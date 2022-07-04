#ifndef WIN32_COMMON_H
#define WIN32_COMMON_H

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <iostream>
#include <array>
#include <vector>
#include <map>

#include <windows.h>
#include <tchar.h>
#include <winnt.h>
#include <processthreadsapi.h>
#include <psapi.h>
#include <handleapi.h>
#include <processenv.h>
#include <pdh.h>
//#pragma comment(lib, "pdh.lib")
#include <winuser.h>
#include <tlhelp32.h>
#include <winuser.h>
#include <securitybaseapi.h>
#include <intrin.h>
#include <cstring>
#include <sysinfoapi.h>
#include <um/powerbase.h>
#include <shellapi.h>



//powrbase.h中缺失的结构
typedef struct _PROCESSOR_POWER_INFORMATION {
  ULONG Number;
  ULONG MaxMhz;
  ULONG CurrentMhz;
  ULONG MhzLimit;
  ULONG MaxIdleState;
  ULONG CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;

class win32_common
{
public:
    win32_common();
    ~win32_common();

    bool refresh();
    //process etc.
    const std::map<unsigned long,unsigned long long> &getPid_Thrdcnt() const;
    static bool getProcName(unsigned long pid,std::string &name);
    static bool getProcPath(unsigned long pid,std::string &path);
    static bool getProcCla(unsigned long pid,std::string &cla);
    static bool getProcMem(unsigned long pid,unsigned long long &mem);
    static bool getProcUserName(unsigned long pid,std::string &username);
    static bool killProc(const unsigned long pid);

    //cpu, memory etc.
    unsigned getLogicCpuCnt() const;
    double getCpuLoad(unsigned n) const;
    unsigned long long getProcCnt() const;
    unsigned long long getThrdCnt() const;
    unsigned long long getHandlCnt() const;

    std::string getCpuInfo() const;
    double getCpuClock() const;
    double getCpuBaseClock() const;
    static std::string getTimeSinceStart();
    //to be implemented
    unsigned getCpuSlot() const;
    unsigned getCpuCoreCnt() const;
    std::string getVl() const;
    unsigned long getL1Cache() const;
    unsigned long getL2Cache() const;
    unsigned long getL3Cache() const;

    unsigned long getMemLoad() const;
    double getPhyTot() const;
    double getPhyAvail() const;
    double getPhyUsed() const;
    double getCommTot() const;
    double getSysCache() const;
    double getPaged() const;
    double getNonpaged() const;

    //to be implemented
    unsigned long getMemSpeed() const;
    unsigned getMemUsedSlot() const;
    unsigned getMemTotSlot() const;
    std::string getMemShape() const;
    unsigned long getMemReserved() const;

    //static functions
    static std::string to_str(TCHAR *tc);
    static bool openProc(const char *path,const char *params,bool asAdmin);
    static bool shutdown();
    static bool logoff();
    static bool getShutdownPriv();

private:
    unsigned long long getPageSz();

    //fields
    std::map<unsigned long,unsigned long long> pid_thrd;

    PDH_HQUERY *cpuQueries;
    PDH_HCOUNTER *cpuCounters;
#define GENPATH(X) (L"\\Processor("#X")\\% Processor Time")
    const wchar_t paths[64][32] = {   //supports 64-logical-core cpu
        GENPATH(0),GENPATH(1),GENPATH(2),GENPATH(3),
        GENPATH(4),GENPATH(5),GENPATH(6),GENPATH(7),
        GENPATH(8),GENPATH(9),GENPATH(10),GENPATH(11),
        GENPATH(12),GENPATH(13),GENPATH(14),GENPATH(15),
        GENPATH(16),GENPATH(17),GENPATH(18),GENPATH(19),
        GENPATH(20),GENPATH(21),GENPATH(22),GENPATH(23),
        GENPATH(24),GENPATH(25),GENPATH(26),GENPATH(27),
        GENPATH(28),GENPATH(29),GENPATH(30),GENPATH(31),
        GENPATH(32),GENPATH(33),GENPATH(34),GENPATH(35),
        GENPATH(36),GENPATH(37),GENPATH(38),GENPATH(39),
        GENPATH(40),GENPATH(41),GENPATH(42),GENPATH(43),
        GENPATH(44),GENPATH(45),GENPATH(46),GENPATH(47),
        GENPATH(48),GENPATH(49),GENPATH(50),GENPATH(51),
        GENPATH(52),GENPATH(53),GENPATH(54),GENPATH(55),
        GENPATH(56),GENPATH(57),GENPATH(58),GENPATH(59),
        GENPATH(60),GENPATH(61),GENPATH(62),GENPATH(63)
    };
    unsigned CpuCnt;
    double *CpuLoads;
    unsigned long long ProcCnt;
    unsigned long long ThrdCnt;
    unsigned long long HandlCnt;

    std::string CpuInfo;
    double CpuBaseClock;

    MEMORYSTATUSEX msx;
    unsigned long long PageSize;
    unsigned long MemLoad;
    double PhyTot;
    double PhyAvail;
    double CommTot;
    double SysCache;
    double Paged;
    double NonPaged;
};

inline std::string win32_common::to_str(TCHAR *tc)
{
    std::wstring wstr(tc);
    return std::string(wstr.begin(),wstr.end());
}

//process & thread
inline const std::map<unsigned long,unsigned long long> &win32_common::getPid_Thrdcnt() const
{
    return pid_thrd;
}

//CPU
inline unsigned win32_common::getLogicCpuCnt() const
{
    return CpuCnt;
}
inline double win32_common::getCpuLoad(unsigned n) const
{
    return CpuLoads[n];
}
inline unsigned long long win32_common::getProcCnt() const
{
    return ProcCnt;
}
inline unsigned long long win32_common::getThrdCnt() const
{
    return ThrdCnt;
}
inline unsigned long long win32_common::getHandlCnt() const
{
    return HandlCnt;
}
//memory
inline unsigned long win32_common::getMemLoad() const
{
    return MemLoad;
}
inline double win32_common::getPhyTot() const
{
    return PhyTot;
}
inline double win32_common::getPhyAvail() const
{
    return PhyAvail;
}
inline double win32_common::getPhyUsed() const
{
    return PhyTot - PhyAvail;
}
inline double win32_common::getCommTot() const
{
    return CommTot;
}
inline double win32_common::getSysCache() const
{
    return SysCache;
}
inline double win32_common::getPaged() const
{
    return Paged;
}
inline double win32_common::getNonpaged() const
{
    return NonPaged;
}
inline std::string win32_common::getCpuInfo() const
{
    return CpuInfo;
}
inline double win32_common::getCpuBaseClock() const
{
    return CpuBaseClock;
}

//to be implemented
inline unsigned win32_common::getCpuSlot() const
{
    return 0;
}
inline unsigned win32_common::getCpuCoreCnt() const
{
    return 0;
}
inline std::string win32_common::getVl() const
{
    return "";
}
inline unsigned long win32_common::getL1Cache() const
{
    return 0;
}
inline unsigned long win32_common::getL2Cache() const
{
    return 0;
}
inline unsigned long win32_common::getL3Cache() const
{
    return 0;
}
inline unsigned long win32_common::getMemSpeed() const
{
    return 0;
}
inline unsigned win32_common::getMemUsedSlot() const
{
    return 0;
}
inline unsigned win32_common::getMemTotSlot() const
{
    return 0;
}
inline std::string win32_common::getMemShape() const
{
    return "";
}
inline unsigned long win32_common::getMemReserved() const
{
    return 0;
}

#endif // WIN32_COMMON_H
