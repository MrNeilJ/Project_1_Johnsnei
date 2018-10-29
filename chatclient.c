#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

#include <arpa/inet.h>>

/*******************************************************************************************************
 * GetString()
 * Details: Gets a string from the user via command line, properly stores and frees the memory for this
 * specific string
 *
 * Popular Online Class CS50 function get string to simplify and safely get a string from the user
 * REF: https://github.com/cs50/libcs50/blob/develop/src/cs50.c
 * *****************************************************************************************************/
string GetString(void)
{
    // Growable buffer for characters
    string buffer = NULL;

    // Capacity of buffer
    size_t capacity = 0;

    // Number of characters actually in buffer
    size_t size = 0;

    // Character read or EOF
    int c;

    // Iteratively get characters from standard input, checking for CR (Mac OS), LF (Linux), and CRLF (Windows)
    while ((c = fgetc(stdin)) != '\r' && c != '\n' && c != EOF)
    {
        // Grow buffer if necessary
        if (size + 1 > capacity)
        {
            // Initialize capacity to 16 (as reasonable for most inputs) and double thereafter
            if (capacity == 0)
            {
                capacity = 16;
            }
            else if (capacity <= (SIZE_MAX / 2))
            {
                capacity *= 2;
            }
            else if (capacity < SIZE_MAX)
            {
                capacity = SIZE_MAX;
            }
            else
            {
                free(buffer);
                return NULL;
            }

            // Extend buffer's capacity
            string temp = realloc(buffer, capacity);
            if (temp == NULL)
            {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }

        // Append current character to buffer
        buffer[size++] = c;
    }

    // Check whether user provided no input
    if (size == 0 && c == EOF)
    {
        return NULL;
    }

    // Check whether user provided too much input (leaving no room for trailing NUL)
    if (size == SIZE_MAX)
    {
        free(buffer);
        return NULL;
    }

    // If last character read was CR, try to read LF as well
    if (c == '\r' && (c = fgetc(stdin)) != '\n')
    {
        // Return NULL if character can't be pushed back onto standard input
        if (c != EOF && ungetc(c, stdin) == EOF)
        {
            free(buffer);
            return NULL;
        }
    }

    // Minimize buffer
    string s = realloc(buffer, size + 1);
    if (s == NULL)
    {
        free(buffer);
        return NULL;
    }

    // Terminate string
    s[size] = '\0';

    // Return string
    return s;
}

struct addrInfo* setAddressInfo(char* address, char* port){
    struct addrinfo hints, *servinfo;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(address, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    return rv;


}


int makeSocket(struct addrInfo* res) {
    int sockfd;

    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
        fprintf(stderr, "Socket could not be made at this time, try again.");
        exit(1);
    }

    return sockfd;
}

int connectSocket(int sockfd, struct addrinfo* res) {
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

    while(True) {
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
    string username = GetString("Type a username (10 characters or less). > ");

    while (strlen(username) < 0 && strlen(username) > 10) {
        if (strlen(username) >  10) {
            username = GetString("Too many characters, try again. > ");
        }
        if (strlen(username) < 0) {
            username = GetString("Too few character, try again. > ");
        }
    }
    // Get the address information for the server.
    struct addrinfo * res = setAddressInfo(argv[1], argv[2]);

    int sockfd = makeSocket(res);

    connectSocket(sockfd, res);

    char severname[10];
    nameExchange(sockfd, username, servername);

    chatWithServer(sockfd, username, servername);

    freeadrinfo(res);





}