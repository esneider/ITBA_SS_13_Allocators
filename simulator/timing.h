#ifdef __APPLE__

#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>

#define TIME_T uint64_t

static inline TIME_T get_time(void) {

    return mach_absolute_time();
}

static inline double elapsed_time(TIME_T start, TIME_T end) {

    static mach_timebase_info_data_t tinfo;

    if (tinfo.denom == 0) {
        (void) mach_timebase_info(&tinfo);
    }

    return ((end - start) * tinfo.numer / tinfo.denom) / 1000.0;
}

#else /* __APPLE__ */

#include <sys/time.h>

#define TIME_T struct timeval

static inline TIME_T get_time(void) {

    TIME_T ret;
    gettimeofday(&ret, NULL);
    return ret;
}

static inline double elapsed_time(TIME_T start, TIME_T end) {

    return end.tv_usec - start.tv_usec;
}

#endif /* __APPLE__ */
