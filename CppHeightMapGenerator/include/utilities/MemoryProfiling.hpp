#pragma once
#include<Windows.h>
#include<Psapi.h>

// Use to convert bytes to MB
//#define DIV 1048576

// Use to convert bytes to MB
//#define DIV 1024

// Specify the width of the field in which to print the numbers. 
// The asterisk in the format specifier "%*I64d" takes an integer 
// argument and uses it to pad and right justify the number.

//#define WIDTH 7

class MemoryProfiler
{
public:
	MemoryProfiler() {};
	~MemoryProfiler() {};

	static PROCESS_MEMORY_COUNTERS_EX GetMemoryUsage() {
        PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        
        return pmc;
	};
    static void PrintMemoryInfo(DWORD processID)
    {
        PROCESS_MEMORY_COUNTERS pmc;

        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        {
            printf("\tPageFaultCount: 0x%08X\n", pmc.PageFaultCount);
            printf("\tPeakWorkingSetSize: 0x%08X\n",
                pmc.PeakWorkingSetSize);
            printf("\tWorkingSetSize: 0x%08X\n", pmc.WorkingSetSize);
            printf("\tQuotaPeakPagedPoolUsage: 0x%08X\n",
                pmc.QuotaPeakPagedPoolUsage);
            printf("\tQuotaPagedPoolUsage: 0x%08X\n",
                pmc.QuotaPagedPoolUsage);
            printf("\tQuotaPeakNonPagedPoolUsage: 0x%08X\n",
                pmc.QuotaPeakNonPagedPoolUsage);
            printf("\tQuotaNonPagedPoolUsage: 0x%08X\n",
                pmc.QuotaNonPagedPoolUsage);
            printf("\tPagefileUsage: 0x%08X\n", pmc.PagefileUsage);
            printf("\tPeakPagefileUsage: 0x%08X\n",
                pmc.PeakPagefileUsage);
        }
    };

private:

};
