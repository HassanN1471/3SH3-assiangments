#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{   
    int fd_child[2];
    int fd_parent[2]; 
    int input, sum=0;
    pid_t pid;
    char arr[100];

    //  initial error check
    if((pipe(fd_child))<0 || (pipe(fd_parent))<0)
    {
        perror("Failed to allocate pipes");
        exit(1);
    }
    
    if((pid=fork()) <0)
    {
        perror("Failed to fork process");
        exit(1);
    }

    //if the pid is zero, then it's the child process
    if(pid == 0)
    {
       //close unneeded descriptors
       close(fd_child[0]);
       close(fd_parent[1]);
        while(1)
        {
            printf("Input: ");
            
            if((input>=-128)&&(input<=127)){
                scanf("%d", &input);
                sprintf(arr,"%d",input);
                //send values to parent
                write(fd_child[1], arr, (strlen(arr)+1));
            }
            else{
                printf("input out of range");
            }
            if(input == -1)
                break;
            
        }
        //receive sum from parent
        read(fd_parent[0], arr, sizeof(arr));
        printf("The sum is: %d\n", atoi(arr));
    }

    //This is the parent process
    else{
        //closing unneeded descriptors
        close(fd_child[1]);
        close(fd_parent[0]);
        
        while(1)
        {
            //read child values
            read(fd_child[0], arr, sizeof(arr));
            if(atoi(arr) == -1)
                break;
            else
            {
                sum += atoi(arr);
            }
        }
        sprintf(arr,"%d",sum);
        //send sum value to child
        write(fd_parent[1], arr, (strlen(arr)+1));
    }

    return 0;
}
