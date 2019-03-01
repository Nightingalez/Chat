#include "pch.h"
#pragma comment(lib, "ws2_32.lib")	// this compiler directive represents the linker to the Microsoft Windows library for sockets
#define _WINSOCK_DEPRECATED_NO_WARNINGS	// disabling API-based warnings preventing the code from compiling and running

// the following headers for the libraries to be included for the code of the application

#include <iostream> // header for the iostream library which handles console in and console out types
#include <stdio.h> // header for the stdio.h library which uses most basic 
#include <string.h> // header used for the strings library which handles char arrays
#include <winsock.h> // header used for the Windows-adapted sockets library

using namespace std; // defining, most notably, console in and console out commands

SOCKET server; // socket variable used later for connecting with the server

char data_buffer[300]; // char array variable for a data buffer of up to 300 values
char* user = new char[30]; // char array variable for the name of the user
char entry[300];  // char array variable for the total amount of characters in one chat entry/message

void messaging() {
	recv(server, data_buffer, sizeof(data_buffer), 0); // getting the first message from server
	cout << "Latest received message: " << data_buffer << endl; // printing the first message from the server on screen

	// Conditional loop for receiving all incoming messages from the server

	while (true) {
		recv(server, data_buffer, sizeof(data_buffer), 0);	// receives all new messages stemming from the server
		cout << "Latest received message: " << data_buffer << endl; // prints out/shows all new messages on screen
	}
}

int main() {
	WSAData wsaData; // structure type for making the sockets work
	WORD DllVersion = MAKEWORD(2, 2); // request for the second version of the Winsock library .dll file

	// The following if statement checks for errors 
	// while using the WS2_32.dll library file for Windows sockets

	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(0, "Socket error!", "The program will shut down.", MB_OK | MB_ICONERROR); //Shows a window error message
		exit(TRUE);	// exits the client window shortly afterwards
	}

	SOCKADDR_IN address; // structure used for the IP internet address
	int sizeofaddress = sizeof(address); // checking the size/length of the IP adress
	address.sin_addr.s_addr = inet_addr("127.0.0.1"); // the code for the IP address
	address.sin_port = htons(17337); // the code for the port using the htons function which converts values
	// between and network orders
	address.sin_family = AF_INET;	// IP address referral for Internet Protocol version 4
	server = socket(AF_INET, SOCK_STREAM, 0); // initializes the socket with transmission control protocol

	// if statement showing a pop error window in case of connection failure

	if (connect(server, (SOCKADDR*)&address, sizeofaddress) != 0) {
		MessageBoxA(0, "Connection error.", "Please restart the application.", MB_OK | MB_ICONERROR);
		return 0; // returns false							
	}
	cout << "MEA3 PCSS mini-project: Chat application (February, 2019)" << endl; // the title of the program
	cout << "Please type your username: "; // username creation
	cin >> user; // input command for the username
	system("cls"); // clears the text in the console window in order to make the transition for the actual chatting stage
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)messaging, 0, 0, 0); // ensures multithreading for messaging using the messaging function

	while (true)
	{
		Sleep(20); // the chat application sleeps for 20 miliseconds before the transition to the chatting phase
		cout << "New message :";	// asks the user to write up a new message
		cin.getline(entry, sizeof(entry));	// keyboard input for messaging (also checking for the total character length of the message)
		memset(data_buffer, 0, sizeof(data_buffer)); // empties the data buffer
		strcat_s(data_buffer, entry); // Sets written content of the message after the colon

	 // The if statement from below prevents a compilation bug regarding whitespace 
		// in the first character of the message.

		if (entry[0] == 0)
		{
			continue; // the continue statement makes the loop run continously
		}

		send(server, data_buffer, sizeof(data_buffer), 0);	// sends the latest written message to the server
		Sleep(10);	// makes the client window sleep for 10 miliseconds after sending the latest message to the server
	}

	return 0; // returns false
}