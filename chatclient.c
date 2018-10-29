#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>

#include <arpa/inet.h>

/*******************************************************************************************************
 * GetString()
 * Details: Gets a string from the user via command line, properly stores and frees the memory for this
 * specific string
 *
 * Popular Online Class CS50 function get string to simplify and safely get a string from the user
 * REF: https://github.com/cs50/libcs50/blob/develop/src/cs50.c
 * *****************************************************************************************************/
void getUser(char * input)
{
    printf("Type in your username > ");
    scanf("%s", input);
}

struct addrinfo* setAddressInfo(char* address, char* port){
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


int makeSocket(struct addrinfo* p) {
    int sockfd;

    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
        fprintf(stderr, "Socket could not be made at this time, try again.");
        exit(1);
    }

    return sockfd;
}

void connectSocket(int sockfd, struct addrinfo* p) {
    int status;
    if ((status = connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)) {
        fprintf(stderr, "Socket could not be connected at this time, try again.");
        exit(1);
    }
}

void nameExchange(int sockfd, char* userName, char* serveName){
    int sentStatus = send(sockfd, userName, strlen(userName), 0);
    int recvStatus = recv(sockfd, serveName, 10, 0);
}

void chatWithServer(int sockfd, char* username, char* servername) {
    char input[500];
    char output[500];

    int total_bytes = 0;
    int status;

    memset(input, 0, sizeof(input));
    memset(output, 0, sizeof(input));

    fgets(input, 500, stdin);

    while(1) {
        printf("%s> ", username);
        fgets(input, 500, stdin);

        if (strcmp(input, "\\quite\n") == 0) {
            break;
        }

        total_bytes = send(sockfd, input, strlen(input), 0);

        if (total_bytes == -1) {
            fprintf(stderr, "Couldn't send information properly, try again");
            exit(1);
        }

        status = recv(sockfd, output, 500, 0);

        if (status == -1) {
            fprintf(stderr, "Couldn't receive the information from the server, try again");
            exit(1);
        }
        else if (status == 0) {
            printf("Connection closed safely by server.");
            break;
        }

        memset(input, 0, sizeof(input));
        memset(output, 0, sizeof(output));

    }

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
        if (strlen(username) >  10) {
            getUser(username);
        }
        if (strlen(username) < 0) {
            getUser(username);
        }
    }
    // Get the address information for the server.
    struct addrinfo * res = setAddressInfo(argv[1], argv[2]);

    int sockfd = makeSocket(res);

    connectSocket(sockfd, res);

    char servername[10];
    nameExchange(sockfd, username, servername);

    chatWithServer(sockfd, username, servername);

    freeaddrinfo(res);






}