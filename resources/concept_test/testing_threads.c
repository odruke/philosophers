#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


void    *print_msg(void *arg)
{
    char    *args;
    int     i;

    args = (char *)arg;
    i = 0;
    while (i < 4)
    {
        printf("%s: message n %d\n", args, i + 1);
        i++;
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t thread1;
    pthread_t thread2;
    
    pthread_create(&thread1, NULL, print_msg, "THREAD 1");
    pthread_create(&thread2, NULL, print_msg, "THREAD 2");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

}