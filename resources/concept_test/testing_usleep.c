#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

long get_time_in_microseconds(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000000L + tv.tv_usec);
}

int main(void)
{
    long start, end;
    int sleep_time = 500000; // 500,000 microseconds = 0.5 seconds

    printf("Testing usleep with %d microseconds (0.5 seconds)\n", sleep_time);

    start = get_time_in_microseconds();
    usleep(sleep_time);
    end = get_time_in_microseconds();

    printf("usleep duration: %ld microseconds\n", end - start);

    return 0;
}
