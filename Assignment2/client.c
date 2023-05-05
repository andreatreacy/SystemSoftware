#include<stdio.h>
#include<string.h>    //for strlen..
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h> //for inet_addr
#include<unistd.h>    //for write
 
int main(int argc , char *argv[])
{
    int SID;
    struct sockaddr_in server;
    char clientMessage[500];
    char serverMessage[500];
    char fileMessage[100];
    char pathMessage[100];
    char mergedMessage[200];
    char fullMessage[200];
     
    //Create socket
    SID = socket(AF_INET , SOCK_STREAM , 0);
    if (SID == -1)
    {
        printf("Error creating socket");
    } {
    	printf("socket created");
    } 
    
    // set sockaddr_in variables
    server.sin_port = htons( 8082 ); // Port to connect on
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP
    server.sin_family = AF_INET; // IPV4 protocol
    
 
    //Connect to server
    if (connect(SID , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("connect failed. Error");
        return 1;
    }
     
    printf("Connected to server ok!!\n");

    // Retrieve the user's name
    char *username = getenv("USER");
    if (username == NULL)
    {
        printf("Unable to retrieve the username.\n");
        return 1;
    }
    printf("Logged in as: %s\n", username);

    printf("Enter the name of the file to be transferred: ");
    scanf("%s", fileMessage);

    printf("Enter the name of the folder to transfer it to: ");
    scanf("%s", pathMessage);

    // Merge the messages with a hyphen
    snprintf(mergedMessage, sizeof(mergedMessage), "%s-%s", fileMessage, pathMessage);

    snprintf(fullMessage, sizeof(fullMessage), "%s-%s", mergedMessage, username);
    printf("full message: %s\n", fullMessage);
     
    //Send some data
    if(send(SID , fullMessage , strlen(fullMessage) , 0) < 0)
    {
        printf("Send failed");
        return 1;
    }

    //keep communicating with server
    int end = 0;
    while(end == 0)
    {  
        //Receive a reply from the server
        if(recv(SID , serverMessage , 500 , 0) < 0)
        {
            printf("IO error");
            //break;
        }
        else
        {
            puts("\nServer sent: ");
            puts(serverMessage);
            end = 1;
        }  
    }
     
    close(SID);
    return 0;
}
