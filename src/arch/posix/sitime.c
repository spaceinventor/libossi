#include <time.h>
#include <ossi/sitime.h>

uint64_t clock_get_nsec(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1E9 + ts.tv_nsec;
}

uint64_t clock_get_nsec_from_isr(void) {
    return clock_get_nsec();
}