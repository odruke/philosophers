#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int count = 0;
// pthread_mutex_t count_mutex;

void    *increment(void *arg)
{

    (void)arg;
    int i;

    i = 0;
    while(i < 1000000)
    {
        i++;
        // pthread_mutex_lock(&count_mutex);
        count++;
        // pthread_mutex_unlock(&count_mutex);
    }
    return NULL;
}

int main()
{
    pthread_t thread1;
    pthread_t thread2;
    
    // pthread_mutex_init(&count_mutex, NULL);

    pthread_create(&thread1, NULL, increment, NULL);
    pthread_create(&thread2, NULL, increment, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // pthread_mutex_destroy(&count_mutex);

    printf("count: %i\n", count);
}