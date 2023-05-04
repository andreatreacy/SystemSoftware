#include<stdio.h>      // for IO
#include<string.h>     //for strlen
#include<sys/socket.h> // for socket
#include<arpa/inet.h>  //for inet_addr
#include<unistd.h>     //for write
#include <pwd.h>       //for struct passwd
#include <grp.h>
#include <sys/types.h>
 
int main(int argc , char *argv[])
{
    int s; // socket descriptor
    int cs; // Client Socket
    int connSize; // Size of struct 
    int READSIZE;  // Size of sockaddr_in for client connection

    struct sockaddr_in server , client;
    char message[500];
    char* token;
    char* message_parts[3];
     
    //Create socket
    s = socket(AF_INET , SOCK_STREAM , 0);
    if (s == -1)
    {
        printf("Could not create socket");
    } else {
    	printf("Socket Successfully Created!!");
    } 

    // set sockaddr_in variables
    server.sin_port = htons( 8082 ); // Set the prot for communication
    server.sin_family = AF_INET; // Use IPV4 protocol
    server.sin_addr.s_addr = INADDR_ANY; 
    // When INADDR_ANY is specified in the bind call, the  socket will  be bound to all local interfaces. 
    
     
    //Bind
    if( bind(s,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("Bind issue!!");
        return 1;
    } else {
    	printf("Bind Complete!!");
    }
     
    //Listen for a conection
    listen(s,3); 
    //Accept and incoming connection
    printf("Waiting for incoming connection from Client>>");
    connSize = sizeof(struct sockaddr_in);
     
    //accept connection from an incoming client
    cs = accept(s, (struct sockaddr *)&client, (socklen_t*)&connSize);
    if (cs < 0)
    {
        perror("Can't establish connection");
        return 1;
    } else {
    	printf("Connection from client accepted!!");
    }
     
    //Receive message from client
    /*while( (READSIZE = recv(cs , message , 1999 , 0)) > 0 )
    {
        //Send the message back to client
        write(cs , message , strlen(message));
    }*/
    
    while(1) 
    {
        memset(message, 0, 500);
        //READSIZE = read(cs,message,500);
        READSIZE = recv(cs , message , 2000 , 0);
        printf("Client said: %s\n", message);
        
        // Split the string using the delimiter "-"
        token = strtok(message, "-");
        
        int i = 0;
        // Iterate through the separated parts
        while (token != NULL && i < 3) {
            message_parts[i] = token;
            token = strtok(NULL, "-");
            i++;
        }

        // Print the separated parts
        for (int j = 0; j < i; j++) {
            printf("message_parts %d: %s\n", j + 1, message_parts[j]);
        }


        if(strcmp(message_parts[1], "distribution") == 0)
        {
            // Check if the file to be moved exists
            if (access(message_parts[0], F_OK) == -1) 
            {
                perror("File does not exist");
            }
            else
            {
                // Get the group entry using group name
                struct group* group = getgrnam("distribution");

                int isMember = 0;
                
                if(group != NULL) 
                {
                    // Iterate over group members and check if the username matches
                    char** member = group->gr_mem;
                    
                    while(*member != NULL) 
                    {
                        if (strcmp(*member, message_parts[2]) == 0) 
                        {
                            printf("%s is a member of the distribution group.\n", message_parts[2]);
                            isMember = 1;
                            break;
                        }
                        member++;
                    }
                    
                    if (*member == NULL) 
                    {
                        printf("%s is not a member of the distribution group.\n", message_parts[2]);
                    }
                } 
                else 
                {
                    printf("Group distribution not found.\n");
                }

                if(isMember == 1)
                {
                    /*
                    // Retrieve the user ID (UID) based on the username in message_parts[2]
                    struct passwd *user_info = getpwnam(message_parts[2]);
                    if (user_info == NULL) {
                        printf("Failed to retrieve user information.\n");
                        return 1;
                    }
                    uid_t user_id = user_info->pw_uid;

                    // Set the owner of the file "t.txt" to the user's UID
                    if (chown(message_parts[0], user_id, -1) == -1) {
                        perror("Failed to change file ownership");
                        return 1;
                    }*/
                    // Retrieve the group ID (GID) based on the username
                    struct passwd *user_info = getpwnam(message_parts[3]);
                    if (user_info == NULL) {
                        printf("Failed to retrieve user information.\n");
                        return 1;
                    }
                    gid_t group_id = user_info->pw_gid;

                    // Set the group ownership of the file "t.txt" to the user's group ID
                    if (chown(message_parts[0], -1, group_id) == -1) {
                        perror("Failed to change file ownership");
                        return 1;
                    }

                    printf("File ownership changed successfully.\n");

                    printf("in if");
                    char *args[] = {"mv", message_parts[0], "distribution/", NULL};
            
                    if(execvp(args[0], args) == -1) 
                    {
                        perror("Error executing mv command");
                        //return 1;
                    }
                }
            }
        }

        //puts(message);
	    write(cs , "What ??" , strlen("What ??"));
    }
 
    if(READSIZE == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(READSIZE == -1)
    {
        perror("read error");
    }
     
    return 0;
}
