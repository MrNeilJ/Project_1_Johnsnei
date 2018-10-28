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
    sys.stdout.write("Trying to connect on port {}".format(port_number))

    for i in range(0, 5):
        time.sleep(1)
        sys.stdout.write(".")

    sys.stdout.write("Connected!")
    time.sleep(2)
    print("The server is ready to receive")
    time.sleep(5)
    clear_terminal()


def get_username():
    # Make sure that the username is less than or equal to 10 digits
    username = ""
    username = input("Type in your username (less than or equal to 10 characters.\n> ")
    while 0 >= len(username) > 10:
        if len(username) < 1:
            print("Too short of a username, try again.")
            username = input("> ")
        elif len(username) > 10:
                print("Too long of a username, try again.")
                username = input("> ")

    return username


# Create the continuous chatting system between server and client.
def chat(test):
    """
    Welcome to Neil's Project 1 Chat Client!
    In here you can chat with other users connected to the same system.

    Have Fun!
    """

    while True:






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
    serverSocket.bind(('', serverPort))
    serverSocket.listen(1)

    # Create animation to make connection look fancy on terminal
    animate_connect(serverPort)

    # Get a valid username for the server to use
    username = get_username()

    while 1:
        connectionSocket, addr = serverSocket.accept()

        sentence = connectionSocket.recv(1024)
        capitalizedSentence = sentence.upper()
        connectionSocket.send(capitalizedSentence)
        connectionSocket.close()





