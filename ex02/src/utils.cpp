#include "utils.hpp"

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__unix__) || defined(__unix) || defined(__APPLE__) || defined(__MACH__)
    #include <unistd.h>
#endif

unsigned int	get_hardware_concurrency(void)
{
#if defined(_WIN32)
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    if (systeminfo.dwNumberOfProcessors > MAX_THREADS)
        return MAX_THREADS;
    return sysinfo.dwNumberOfProcessors;
#elif defined(__unix__) || defined(__unix) || defined(__APPLE__) || defined(__MACH__)
    long nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    return (nprocs > 0) ? static_cast<unsigned int>(nprocs > MAX_THREADS ? MAX_THREADS : nprocs) : 0;
#else
    return 1;
#endif
}
