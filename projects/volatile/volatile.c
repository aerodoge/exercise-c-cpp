#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

volatile int flag = 1;

void* fun_thread(void* arg) {
    sleep(2);
    flag = 0;
    return NULL;
}

int main(void) {
    pthread_t t1;
    if (pthread_create(&t1, NULL, fun_thread,NULL)) {
        perror("pthread_create");
        return -1;
    }

    while (flag == 1) ;
    pthread_join(t1, NULL);
    printf("end main\n");

    return 0;
}