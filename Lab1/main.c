#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// user - defined signal handler for alarm .

void alarm_handler(int signo){
    if (signo==SIGALRM){
        printf("Alarm\n");
    }
    //signal for when ctrl+c is pressed, print message
    if (signo==SIGINT){
        printf("CTRL+C pressed!\n");
    }
    //signal for when ctrl+z is pressed, print message and exit 
    if (signo==SIGTSTP){
        printf("CTRL+Z pressed!\n");
        exit(0);

    }
}

int main(void) {
    // register the signal handler
    //connection of the signal
    
    
    if (signal(SIGALRM,alarm_handler)==SIG_ERR) {
        printf(" failed to register alarm handler .");
        exit(1);
    }
    
    if (signal(SIGINT,alarm_handler)==SIG_ERR) {
        printf(" failed to register alarm handler .");
        exit(1);
    }
    
    if (signal(SIGTSTP,alarm_handler)==SIG_ERR) {
        printf(" failed to register alarm handler .");
        exit(1);
    }
   
    while (1) {
        sleep(2); //waits for 2 sec
        //raise alarm
        raise(SIGALRM);
    } 
}