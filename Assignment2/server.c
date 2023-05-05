#include<stdio.h>      // for IO
#include<string.h>     //for strlen
#include<sys/socket.h> // for socket
#include<arpa/inet.h>  //for inet_addr
#include<unistd.h>     //for write
#include <pwd.h>       //for struct passwd
#include <grp.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h> // include pthread library

// Function signature for connection_handler
void *connection_handler(void *socket_desc);

// mutex lock
pthread_mutex_t lock_x; 
 
int main(int argc , char *argv[])
{
    int s; // socket descriptor
    int cs; // Client Socket
    int connSize; // Size of struct 
    struct sockaddr_in server , client;

    // thread
    pthread_t thread_id;

    // create the lock
    pthread_mutex_init(&lock_x, NULL);
     
    //Create socket
    s = socket(AF_INET , SOCK_STREAM , 0);
    if (s == -1)
    {
        printf("Could not create socket");
    } 
    else 
    {
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
     
    // Accept and handle incoming connections
    while((cs = accept(s, (struct sockaddr *)&client, (socklen_t *)&connSize)) > 0)
    {
        printf("Connection from client accepted!!\n");

        // Create a new thread for each client connection
        int *new_sock = malloc(1);
        *new_sock = cs;
        if (pthread_create(&thread_id, NULL, connection_handler, (void *)new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        // Detach the thread to allow it to run independently
        pthread_detach(thread_id);
    }
	
	if (cs < 0)
    {
        perror("Can't establish connection");
        return 1;
    }
     
    return 0;
}



// Function to handle client connection
void *connection_handler(void *socket_desc)
{
    // use the mutex lock
    pthread_mutex_lock(&lock_x);

    // Get the socket descriptor
    int cs = *(int *)socket_desc;
    int READSIZE;
    char message[500];
    char *token;
    char *message_parts[3];

    // Receive message from client
    while((READSIZE = recv(cs, message, 2000, 0)) > 0)
    {
        printf("Client said: %s\n", message);
        //sleep(10);  // Wait for 15 seconds

        // Split the string using the delimiter "-"
        token = strtok(message, "-");

        int i = 0;
        // Iterate through the separated parts
        while (token != NULL && i < 3)
        {
            message_parts[i] = token;
            token = strtok(NULL, "-");
            i++;
        }

        // Print the separated parts
        for (int j = 0; j < i; j++)
        {
            printf("message_parts %d: %s\n", j + 1, message_parts[j]);
        }

        if(strcmp(message_parts[1], "distribution") == 0)
        {
            // Check if the file to be moved exists
            if (access(message_parts[0], F_OK) == -1) 
            {
                perror("File does not exist");
                char response[100];
                int length;
                length = snprintf(response, sizeof(response), "%s does not exist", message_parts[0]);
                write(cs, response, length);
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
                        char response[100];
                        int length;
                        length = snprintf(response, sizeof(response), "%s is not a member of the manufacturing group.\n", message_parts[2]);
                        write(cs, response, length);
                    }
                } 
                else 
                {
                    printf("Group distribution not found.\n");
                    char response[100];
                    int length;
                    length = snprintf(response, sizeof(response), "Group distribution not found\n");
                    write(cs, response, length);
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

                    /*
                    printf("in ismember.\n");
                    // Retrieve the group ID (GID) based on the username
                    struct passwd *user_info = getpwnam(message_parts[2]);
                    if (user_info == NULL) {
                        printf("Failed to retrieve user information.\n");
                        return 1;
                    }
                    printf("getting group id.\n");
                    gid_t group_id = user_info->pw_gid;

                    // Set the group ownership of the file "t.txt" to the user's group ID
                    if (chown(message_parts[0], -1, group_id) == -1) {
                        perror("Failed to change file ownership");
                        return 1;
                    }

                    printf("File ownership changed successfully.\n");
                    */
                    

                    // Create a command that moves the file using system()
                    char command[256];
                    snprintf(command, sizeof(command), "mv %s %s", message_parts[0], "distribution");

                    // Execute the command
                    int result = system(command);
                    if (result == -1) {
                        printf("Error executing the command.\n");
                        char response[100];
                        int length;
                        length = snprintf(response, sizeof(response), "Error executing the move command file.\n");
                        write(cs, response, length);
                    }

                    // Check the exit status of the command
                    if (WIFEXITED(result) && WEXITSTATUS(result) == 0) 
                    {
                        printf("File moved successfully.\n");
                        char response[100];
                        int length;
                        length = snprintf(response, sizeof(response), "%s was transferred to the distribution folder", message_parts[0]);
                        write(cs, response, length);
                    } 
                    else 
                    {
                        printf("Failed to move the file.\n");
                        char response[100];
                        int length;
                        length = snprintf(response, sizeof(response), "Error moving the %s file.\n", message_parts[0]);
                        write(cs, response, length);
                    }
                }
            }
        }
        else if(strcmp(message_parts[1], "manufacturing") == 0)
        {
            // Check if the file to be moved exists
            if (access(message_parts[0], F_OK) == -1) 
            {
                perror("File does not exist");
                char response[100];
                int length;
                length = snprintf(response, sizeof(response), "%s does not exist", message_parts[0]);
                write(cs, response, length);
            }
            else
            {
                // Get the group entry using group name
                struct group* group = getgrnam("manufacturing");

                int isMember = 0;
                
                if(group != NULL) 
                {
                    // Iterate over group members and check if the username matches
                    char** member = group->gr_mem;
                    
                    while(*member != NULL) 
                    {
                        if(strcmp(*member, message_parts[2]) == 0) 
                        {
                            printf("%s is a member of the manufacturing group.\n", message_parts[2]);
                            isMember = 1;
                            break;
                        }
                        member++;
                    }
                    
                    if(*member == NULL) 
                    {
                        printf("%s is not a member of the manufacturing group.\n", message_parts[2]);
                        char response[100];  // Adjust the size as per your requirements
                        int length;
                        length = snprintf(response, sizeof(response), "%s is not a member of the manufacturing group.\n", message_parts[2]);
                        write(cs, response, length);
                    }
                } 
                else 
                {
                    printf("Group manufacturing not found.\n");
                    char response[100];
                    int length;
                    length = snprintf(response, sizeof(response), "Group manufacturing not found\n");
                    write(cs, response, length);
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

                    /*
                    printf("in ismember.\n");
                    // Retrieve the group ID (GID) based on the username
                    struct passwd *user_info = getpwnam(message_parts[2]);
                    if (user_info == NULL) {
                        printf("Failed to retrieve user information.\n");
                        return 1;
                    }
                    printf("getting group id.\n");
                    gid_t group_id = user_info->pw_gid;

                    // Set the group ownership of the file "t.txt" to the user's group ID
                    if (chown(message_parts[0], -1, group_id) == -1) {
                        perror("Failed to change file ownership");
                        return 1;
                    }

                    printf("File ownership changed successfully.\n");
                    */
                    

                    // Create a command that moves the file using system()
                    char command[256];
                    snprintf(command, sizeof(command), "mv %s %s", message_parts[0], "manufacturing");

                    // Execute the command
                    int result = system(command);
                    if (result == -1) 
                    {
                        printf("Error executing the command.\n");
                        char response[100];
                        int length;
                        length = snprintf(response, sizeof(response), "Error executing the move command file.\n");
                        write(cs, response, length);
                    }

                    // Check the exit status of the command
                    if (WIFEXITED(result) && WEXITSTATUS(result) == 0) 
                    {
                        printf("File moved successfully.\n");
                        char response[100];
                        int length;
                        length = snprintf(response, sizeof(response), "%s was transferred to the manufacturing folder", message_parts[0]);
                        write(cs, response, length);
                    } 
                    else 
                    {
                        printf("Failed to move the file.\n");
                        char response[100];
                        int length;
                        length = snprintf(response, sizeof(response), "Error moving the %s file.\n", message_parts[0]);
                        write(cs, response, length);
                    }
                }
            }
        }
    }

    if (READSIZE == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (READSIZE == -1)
    {
        perror("read error");
        char response[100];
        int length;
        length = snprintf(response, sizeof(response), "Read error.\n");
        write(cs, response, length);
    }

    // release the lock
    pthread_mutex_unlock(&lock_x);

    // Free the socket descriptor
    free(socket_desc);

    // Exit the thread
    pthread_exit(NULL);
}