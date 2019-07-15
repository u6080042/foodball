#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<time.h>
#include<sys/stat.h>
#include<sys/msg.h>
#include<sys/ipc.h>

struct msgn{
    long int msg_type;
    char text[BUFSIZ];
};

int main(int argc,char *argv[]){
    
    int pid,msgID; long int rxcv_msg_type;
    struct msgn a_msg;
    struct msgn b_msg;
    
    
    if(argc!=2){
        
        fprintf(stderr,"User: %s <[1,2]>\n",*argv);exit(EXIT_FAILURE);
    }
    
    msgID = msgget((key_t)6013110, 0666|IPC_CREAT);
    
    if(msgID == -1) {
        fprintf(stderr, "msgget failed! could not create a Message Queue\n");
        exit(EXIT_FAILURE);
    }
    
    argv++;
    
    pid = fork();
    switch (pid){
            
        case -1 : perror("Forking Failed"); exit(EXIT_FAILURE);
        case  0 : while (strncmp(a_msg.text, "end chat", 8)&&strncmp(b_msg.text, "end chat", 8)){
            
            
            
            if(strcmp(*argv,"1")==0){
                
                fflush(stdin);
                rxcv_msg_type = 2;
                
                if(msgrcv(msgID, (void*)&a_msg, BUFSIZ, rxcv_msg_type, 0) == -1) {
                    
                    fprintf(stderr, "msgrcv failed! could not receive a message from the Message Queue\n");
                    exit(EXIT_FAILURE);
                }
                
                setbuf(stdout, NULL);
                printf("User2 => %s",a_msg.text);
            }
            if(strcmp(*argv,"2")==0){
                
                fflush(stdin);
                rxcv_msg_type = 1;
                
                if(msgrcv(msgID, (void*)&b_msg, BUFSIZ, rxcv_msg_type, 0) == -1) {
                    
                    fprintf(stderr, "msgrcv failed! could not receive a message from the Message Queue\n");
                    exit(EXIT_FAILURE);
                }
                
                setbuf(stdout, NULL);
                printf("User1 => %s",b_msg.text);
            }
        }
        default : while (strncmp(b_msg.text, "end chat", 8)&&strncmp(a_msg.text, "end chat", 8)){
            
            if(strcmp(*argv,"1")==0){
                
                fflush(stdin);
                fgets(a_msg.text, BUFSIZ, stdin);
                a_msg.msg_type = 1;
                if(msgsnd(msgID, (void*)&a_msg, BUFSIZ, 0) == -1) {
                    
                    fprintf(stderr, "msgsnd failed! could not send a message to the Message Queue\n");
                    exit(EXIT_FAILURE);
                }
            }
            if(strcmp(*argv,"2")==0){
                
                fflush(stdin);
                
                fgets(b_msg.text, BUFSIZ, stdin);
                b_msg.msg_type = 2;
                if(msgsnd(msgID, (void*)&b_msg, BUFSIZ, 0) == -1) {
                    
                    fprintf(stderr, "msgsnd failed! could not send a message to the Message Queue\n");
                    exit(EXIT_FAILURE);
                }
            }
            
        }
            kill(pid,SIGINT);
    }
    
    exit(EXIT_SUCCESS);
}

