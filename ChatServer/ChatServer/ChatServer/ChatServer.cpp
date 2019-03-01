#include "pch.h"
#pragma comment(lib, "ws2_32.lib") // this compiler directive represents the linker to the Microsoft Windows library 
#define _WINSOCK_DEPRECATED_NO_WARNINGS // disabling API-based warnings preventing the code from compiling and running

#include <iostream> // header for the iostream library which handles console in and console out types
#include <winsock.h> // header used for the Windows-adapted sockets library

using namespace std; // defining, most notably, console in and console out commands 

SOCKET socket_connection[10]; // connects up to 10 sockets into an array
int server_counter = 0; // an integer type of variable used to count how many users 
// are actually connected to the chat in real time.

void server_connection(int counter)
{
	char data_buffer[300]; // a buffer of data of char type that can store 300 values

	// Conditional while loop using a boolean variable to keep track of how many users are online

	while (true)
	{
		int tracker = recv(socket_connection[counter], data_buffer, sizeof(data_buffer), 0); // this code lines receives messages from the client

		// below is an if statement that checks if there are any errors related to the sockets

		if (tracker == SOCKET_ERROR)
			continue;

		// the for loop below will check all the online users on the chat 
		// and will ensure the transmission of messages in real time

		for (int i = 0; i < server_counter; i++)
		{
			if (i == counter)
				continue;
			send(socket_connection[i], data_buffer, sizeof(data_buffer), 0);
		}
	}
}

int main() {
	WSAData wsa; // structure type for making the sockets work
	WORD Dll = MAKEWORD(2, 2); // request for the second version of the Winsock library .dll file

	// if statement checking for an error related to the .dll library file
	// in case there is an error, a pop up error message will show and the server will cease to function

	if (WSAStartup(Dll, &wsa) != 0)
	{
		MessageBoxA(0, "Socket connection error!", "The server is no longer functional!", MB_OK | MB_ICONERROR);
		exit(1); // the server will shut down quickly afterwards
	}

	SOCKADDR_IN address; // structure used for the IP internet address
	int sizeofadress = sizeof(address); // checking the size/length of the IP adress
	address.sin_addr.s_addr = inet_addr("127.0.0.1"); // the code for the IP address
	address.sin_port = htons(17337); // code for the port using the htons function which converts values
	// between host and network byte orders
	address.sin_family = AF_INET; // IP address referral for Internet Protocol version 4

	SOCKET server_client = socket(AF_INET, SOCK_STREAM, 0); // this socket will ensure connection between the various IP addresses of the clients
	// and the server proper
	bind(server_client, (SOCKADDR*)&address, sizeof(address)); // this bind function manages to associate the IP adresses to the server
	listen(server_client, SOMAXCONN); // by using the listen function incoming IP adresses will be identified
	// the SOMAXCONN statement is set as a parameter for setting up an indefinite amount of connection requests

	cout << "The server connection is functional. You may now proceed to the chat." << endl;

	SOCKET address_connection; // socket used for connecting 50 users to the chat

	// the following for loop will make the chat hold up to 50 users writing in real time to one another

	for (int i = 0; i < 50; i++)
	{
		address_connection = accept(server_client, (SOCKADDR*)&address, &sizeofadress);
		socket_connection[i] = address_connection;
		server_counter++;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)server_connection, (LPVOID)(i), 0, 0);
	}

	system("pause"); // system call command for pausing the program/application

	return 0; // returns false
}
