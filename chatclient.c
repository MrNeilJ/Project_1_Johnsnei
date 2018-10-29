#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAXDATASIZE 500

void getUser(char *input) {
    printf("Type in your username > ");
    scanf("%s", input);
}

/* Got the addrinfo struct required information from the Linux  Man Pages
 Not sure if they are the same, but the seem to match up for what I needed.

 Migrated the first  IF statement from Beej.us layout into a more modular layout
 */
struct addrinfo *setAddressInfo(char *address, char *port) {
    struct addrinfo hints, *servinfo;
    int rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(address, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }
    return servinfo;
}

/*
 * Simplified the socket process found in Beej.us since I do not need to loop
 * through and find the compatible socket as I am hard coding that in essentially
 * using the command line when starting the application.
 *
 * Pulled that loop out and made it into a function for more modularity
 */
int makeSocket(struct addrinfo *p) {
    int sockfd;

    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
        fprintf(stderr, "Socket could not be made at this time, try again.");
        exit(1);
    }
    return sockfd;
}

/*
 * Pulled out  third if statement from the Beej.us program example and migrated that into a function
 * to help validate that the application properly connected to the socket we were trying to connect to.
 */
void connectSocket(int sockfd, struct addrinfo *p) {
    int status;
    if ((status = connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)) {
        fprintf(stderr, "Socket could not be connected at this time, try again.");
        exit(1);
    }
}

/*
 * Removed the sending an receiving section and made it more modular and easier to read.
 * Pulled concept partially from both the example from Beej.us from the client and the server
 * side to understand how to send and receive the information
 */
void nameExchange(int sockfd, char *userName, char *serveName) {
    int sentStatus = send(sockfd, userName, strlen(userName), 0);
    int recvStatus = recv(sockfd, serveName, 10, 0);
}

/*
 * Bread and butter on how this works.  Found some examples online from Geeks for Geeks
 * about client / server examples in C.
 *
 * Pulled the concepts half from the client example of Beej.us and the other half from
 * the server side.  To create the sense of communication back and forth
 */
void chatWithServer(int sockfd, char *username, char *servername) {
    char input[MAXDATASIZE];
    char output[MAXDATASIZE];

    int total_bytes = 0;
    int status;

    memset(input, 0, sizeof(input));
    memset(output, 0, sizeof(input));

    fgets(input, MAXDATASIZE, stdin);

    while (1) {
        printf("%s> ", username);
        fgets(input, MAXDATASIZE, stdin);

        if (strcmp(input, "\\quit\n") == 0) {
            break;
        }

        if (strlen(input) > MAXDATASIZE - 1) {
            // Re did so it shouldn't ever proc this.  But still good to have..?
            printf("Too big, try again");
        } else {
            // Send input to server
            total_bytes = send(sockfd, input, strlen(input), 0);

            // Check status of sending to server
            if (total_bytes == -1) {
                fprintf(stderr, "Couldn't send information properly, try again");
                exit(1);
            }

            // Receive information from user
            status = recv(sockfd, output, MAXDATASIZE - 1, 0);

            // Check status of receiving from server
            if (status == -1) {
                fprintf(stderr, "Couldn't receive the information from the server, try again");
                exit(1);
            } else if (status == 0) {
                printf("Connection closed safely by server.");
                break;
            }
                // Print out response
            else {
                printf("%s> %s\n", servername, output);
            }
            // Clear out buffer for standard input so we don't get erroneous values
            setbuf(stdin, NULL);
        }

        memset(input, 0, sizeof(input));
        memset(output, 0, sizeof(output));

    }
    // Close socket once done communicating
    close(sockfd);
    printf("Closing Connection");
}


int main(int argc, char *argv[]) {
    // Validate that we have the correct number of arguments before proceeding with the rest of the setup
    if (argc != 3) {
        fprintf(stderr, "Not the correct amount of arguments supplied.  Re-run the application");
    }
    char username[10];

    getUser(username);

    while (strlen(username) < 0 || strlen(username) > 10) {
        if (strlen(username) > 10) {
            getUser(username);
        }
        if (strlen(username) < 0) {
            getUser(username);
        }
    }
    // Get the address information for the server.
    struct addrinfo *res = setAddressInfo(argv[1], argv[2]);

    int sockfd = makeSocket(res);

    connectSocket(sockfd, res);

    char servername[10];
    nameExchange(sockfd, username, servername);

    // Clear out terminal to make it look fresh
    system("clear");

    // Print out a basic welcome screen once the server is set up
    printf("##########################################################################\n");
    printf("#    Welcome to Neil's Project 1 Chat Client!                             \n");
    printf("#    In here you can chat with other users connected to the same system.  \n");
    printf("#    Current Connected User:                                              \n");
    printf("#    Server: %s                                                           \n", servername);
    printf("#    Client: %s                                                           \n", username);
    printf("##########################################################################\n");

    chatWithServer(sockfd, username, servername);

    freeaddrinfo(res);


}