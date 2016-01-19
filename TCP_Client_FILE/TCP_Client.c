/*
 *************************************************
 *Name : send_file.c                       *
 *Date : 2015-04-15                        *
 *Author : Sniper                          *
 *Aim : Client send the file to the Server.*
 *************************************************
 */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#define BUFFER_SIZE                1024  
#define FILE_NAME_MAX_SIZE         512  

int main(int argc,char *argv[])
{
	int socketfd;
	struct sockaddr_in s_add,c_add;
	unsigned short portnum = 0x22B8;
	int len;
	char buffer[BUFFER_SIZE];  
	FILE *fp ;
	int file_block_length = 0;  
	
	
	
	/*
	 *Create the socket
	 */
	if((socketfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("Socket create error! \n");
		exit(1);
	}
	
	/*
	 *set the address format
	 */	
	bzero(&s_add,sizeof(struct sockaddr_in));
	s_add.sin_family = AF_INET;
	/*
	 *change the VM address
	 */
	s_add.sin_addr.s_addr = inet_addr("192.168.0.120");//change the string to 32-bit internet byte.
	s_add.sin_port=htons(portnum);
	
	if (connect(socketfd,(struct sockaddr *)(&s_add),sizeof(struct sockaddr))<0)
	{
		printf("Connect failure!\n");
		return -1;
	}
	else
		printf("Connect Success!\n");
	
	/*
	 *Send the file
	 */	
	char filepath[1000];
	printf("please input the file's path : ");
	scanf("%s",filepath);
	fp = fopen(filepath, "r");  
	
	char filename[1000];
	memset(filename,'\0',sizeof(filename));
	int i=0,k=0;
	for(i=strlen(filepath);i>=0;i--)
	{
		if(filepath[i]!='/')	
		{
			k++;
		}
		else 
			break;	
	}
	strcpy(filename,filepath+(strlen(filepath)-k)+1);
	printf("filename is %s \n",filename);
	
	/*
	 *Send the file name to Server.
	 */
	len=send(socketfd,filename,strlen(filename),0);
	if(len<0)  
        {  
            printf("send error!\n");  
            exit(1);  
        }
	else   
        {  
            printf("Send succeed send : %s \n",filename);  
        }  

	printf("**************************************\n");
	
	sleep(3);

	/*
	 *Send the file to Server.
	 */ 
	if (fp == NULL)  
	{  
		printf("File: file Not Found!\n");  
	}  
	else  
	{  
		bzero(buffer, BUFFER_SIZE);  		
		while( (file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)  
		{  
		    printf("file_block_length = %d\n", file_block_length);  

			/*
			 *Send the string to the buffer  
			 */
		    if (send(socketfd, buffer, file_block_length, 0) < 0)  
		    {  
		        printf("Send File:test Failed!\n");  
		        break;  
		    }  

		    bzero(buffer, sizeof(buffer));  
		}  
		fclose(fp);  
		printf("File:file Transfer Finished!\n");  
	}
	close(socketfd);  
	
	return 0;
}
