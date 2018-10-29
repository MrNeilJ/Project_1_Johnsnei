#!/bin/python

#################################################
#   Name: Neil Johnson                          #
#   Due: 10/28/18                               #
#   Assignment: Project 1                       #
#   Details: Create a simple chat client        #
#################################################

from socket import *
import sys
import os
import time


def clear_terminal():
    os.system('cls' if os.name == 'nt' else 'clear')


def animate_connect(port_number):
    print("Trying to connect on port {}".format(port_number), end="")

    for i in range(0, 5):
        time.sleep(1)
        sys.stdout.write(".")

    print("Connected!")
    time.sleep(1)
    print("\nThe server is ready to receive.\n")
    time.sleep(2)


def get_username():
    # Make sure that the username is less than or equal to 10 digits
    user = input("Type in your user (less than or equal to 10 characters. > ")
    while 0 >= len(user) or len(user) > 10:
        if len(user) < 1:
            print("Too short of a user, try again.")
            user = input("> ")
        elif len(user) > 10:
                print("Too long of a user, try again.")
                user = input("> ")
    print("Name approved, hello {}, waiting for other user to connect...".format(user))
    time.sleep(1)

    # Return the appropriate name for the user
    return user


def build_message(user, msg_size):
    # Build the message and make sure it can fit in the allotted size that was asked (500) for our case.
    message = ""
    while 1 > len(message) or len(message) > msg_size:
        message = input("{}> ".format(user))

        if len(message) < 1:
            print("Too short of a message, try again.")
        elif len(message) > msg_size:
            print("Too long of a message, try again.")

    # Return the message to the main statement
    return message


def name_exchange(connection_socket, user):
    # Receive the name from the client
    client_name = connectionSocket.recv(1024).decode()

    # Send the username and encode it for shipping out.
    connection_socket.send(user.encode())

    return client_name


# Create the continuous chatting system between server and client.
def chat_feature(connectionSocket, msg_size, client, user):
    # Clear out terminal to make it look fresh
    clear_terminal()

    # Print out a basic welcome screen once the server is set up
    print("##########################################################################")
    print("#    Welcome to Neil's Project 1 Chat Client!                            ")
    print("#    In here you can chat with other users connected to the same system. ")
    print("#    Current Connected User:                                             ")
    print("#    Server: {}                                                          ".format(user))
    print("#    Client: {}                                                          ".format(client))
    print("##########################################################################")

    while True:
        # Get the message from the client up to a specific amount specified earlier.
        client_msg = connectionSocket.recv(500)[0:-1].decode()

        if client_msg == "":
            print("Terminating connection.")
            break

        print("{}> {}".format(client, client_msg))

        server_msg = build_message(user, msg_size)

        if server_msg == "\quit":
            print("Terminating connection.")
            break

        connectionSocket.send(server_msg.encode())


if __name__ == "__main__":
    # Verify user has given the port as an argument
    if len(sys.argv) != 2:
        print("Incorrect amount of arguments supplied. Re-run and supply the port number you'd like to use.")
        exit(1)
    elif not sys.argv[1].isdigit():
        print("Supplied value is not a number.  Re-run and supply the port number you'd like to use.")
        exit(1)

    # Mimicked this portion from the lecture files to build the connection
    serverPort = sys.argv[1]
    serverSocket = socket(AF_INET, SOCK_STREAM)
    serverSocket.bind(('', int(serverPort)))
    serverSocket.listen(1)

    # Create animation to make connection look fancy on terminal
    animate_connect(serverPort)

    # Get a valid username for the server to use
    username = get_username()

    connectionSocket, addr = serverSocket.accept()

    # Create the chat feature between the user and the server
    chat_feature(connectionSocket, 500, name_exchange(connectionSocket, username), username)

    # Close the connection once done with the client
    connectionSocket.close()

    exit(1)





