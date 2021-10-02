// Chat Application.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include <iostream>
#include <string>
#include <WS2tcpip.h> //windows sockets uses this api to access the network
//it includes winsock network
#pragma comment (lib, "ws2_32.lib") //tells the linker to add the 'libname' library to the list
//of library dependencies as if you had added it in the project properties at Linker -> 
//Input-> Additional dependencies
using namespace::std;
void main()
{
	printf("gi");
	cout << "Hello World  !!\n";
	//initialize winsock = windows socket
	WSADATA wsData;
	//The MAKEWORD(2,2) parameter of WSAStartup makes a request for version 2.2
	//of Winsock on the system, and sets the passed version as the highest version
	//of Windows Sockets support that the caller can use.
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsData);

	if (wsok != 0)
	{
		cerr << "cant initialize winsock ! Quitting" << endl;
		return;
	}


	//create a socket one thta we are binding to
	//creating a socket has a domain IPv4 protocol or IPv6 protocol
	//The IPv6 is the sixth revision to the Internet Protocol and sucessor to IPv4
	//It provides unique addresses necessary for internet-enabled devices to communicate
	//It also asks for a socket type: the communication type 
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	//AF_INET is used to specify the IPv4 address family.
	//	SOCK_STREAM is used to specify a stream socket.
	//IPPROTO_TCP is used to specify the TCP protocol 

	if (listening == INVALID_SOCKET)
	{
		cerr << "Cant create a socket ! Quitting" << endl;
		return;
	}
	//bind the socket to an ip address and port
	//link the ip address and the port to the socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(22);//htons= host to network shorts use port 5  4000
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	//we are binding the port 54000 to any address
	bind(listening, (sockaddr*)&hint, sizeof(hint));


	//tell the winsock socket is for listening
	listen(listening, SOMAXCONN);
	//somaxconn=maximum connections we can have is large

	//declare fd_set
	fd_set master;
	FD_ZERO(&master);

	FD_SET(listening, &master);// add the listening socket to the set

	while (true)
	{
		fd_set copy = master;
		//select function determines the status of one or more sockets, waiting if necessary , to perform I/O
		//readfds [in ,out]: An optional pointer to set a sockets to be checked for readability
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		//readfds optional pointer to a set of sockets to be checked for readability

		//sockets are gonna be inside the copy

		//server can accept the incoming message from client and recieve the message
		for (int i = 0; i < socketCount; i++)
		{
			//accept the message
			//recieve the message
			SOCKET sock = copy.fd_array[i];
			if (sock == listening)
			{
				//accept a new connection
				SOCKET client = accept(listening, nullptr, nullptr);
				//add the new connection to the list of connected clients
				FD_SET(client, &master);//add it to the fs set

				//send a welcome message to teh connected client
				string welcomeMsg = "Welcome to the Awesome Chat Server!";
				send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
				//TODO: broadcast we have a new connection
			}
			else
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				//accept a new message

				int bytesIn = recv(sock, buf, 4096, 0);
				if(bytesIn <= 0 )
				{
					//Drop the client
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else
				{
					//Send message to to other clients, and definately NOT the listening client
					for (int i = 0; i < master.fd_count; i++)
					{
						SOCKET outSock = master.fd_array[i];
						if (outSock != listening && outSock != sock)
						{
							send(outSock, buf, bytesIn, 0);
						}
					}
				}
			}

		}
	}




	//shutdown winsock
	WSACleanup();

	system("pause");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging me nu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
