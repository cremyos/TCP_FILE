/*
 ************************************************
 *Name : TCP_Server.c                           *
 *Date : 2016-01-19                             *
 *Author : sniper                               *
 *Aim : Server get the file from the Client.    *
 ************************************************
 */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>



int socketfd_connect = 0;
pthread_t ntid;
int len = 0;


#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512


void *print_message(void *arg)
{
        /*
         *define the buffer to receive the file
         */
        int pthread_socket_connect;
        int i=0;
        int write_length;
        FILE *fp;
        char buffer[BUFFER_SIZE];
        int length = 0;

        bzero(buffer,BUFFER_SIZE);
        pthread_socket_connect = socketfd_connect;
        pthread_detach(ntid);

        /*
         *receive the file's name
         */
        char filename[1000];

        memset(filename,'\0',sizeof(filename));
        len=recv(socketfd_connect,filename,100,0);
        if(len<0){
                printf("receive error!\n");
                exit(1);
        }
        else
                printf("%d receive message is %s\n",__LINE__,filename);


        /*
         *Receive the file
         */
        fp = fopen(filename,"w");
        if(NULL == fp )
        {
                printf("File:\t Can Not Open To Write\n");
                exit(1);
        }


        while( length = recv(pthread_socket_connect,buffer,BUFFER_SIZE,0))
        {
                if(length < 0)
                {
                    printf("Recieve Data From Client Failed!\n");
                    break;
                }

                /*
                 *write the buffer to the file
                 */
                write_length = fwrite(buffer,sizeof(char),length,fp);
                if (write_length<length)
                {
                    printf("File:file Write Failed\n");
                    break;
                }
                bzero(buffer,BUFFER_SIZE);
        }
        printf("Recieve File: %s  From Client Finished\n",filename);

        fclose(fp);

        /******************************************************************/


        pthread_exit(0);
}


int main(int argc,char *argv[])
{
        int socketfd_listen;
        struct sockaddr_in server_addr,client_addr;
        unsigned short portnum = 0x22B8;
        int err;

        /*
         *Create the socket
         */
        if((socketfd_listen=socket(AF_INET,SOCK_STREAM,0))<0)
        {
                printf("Socket create error! \n");
                exit(1);
        }

        /*
         *set the address format
         */
        bzero(&server_addr,sizeof(struct sockaddr_in));
        server_addr.sin_family=AF_INET;
        server_addr.sin_addr.s_addr=inet_addr("192.168.0.120");
        server_addr.sin_port=htons(portnum);

        if(bind(socketfd_listen,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))<0)
        {
                printf("bind failure!\n");
                return -1;
        }
        printf("bind Success!\n");

        if(listen(socketfd_listen,5)<0)
        {
                printf("Listen failure!\n");
                return -1;
        }

        len=sizeof(struct sockaddr);

        /*
         *Using the loop to send and receive the message.
         */
        while(1)
        {
                socketfd_connect = accept(socketfd_listen, (struct sockaddr *)(&client_addr), &len);

                if(socketfd_connect<0)
                {
                        printf("accept fail !\n");
                        return -1;
                }

                err = pthread_create(&ntid,NULL,print_message,NULL);
                if(err!=0)
                        printf("can't create pthread!\n");
        }

        close(socketfd_listen);
        close(socketfd_connect);
        return 0;
}

