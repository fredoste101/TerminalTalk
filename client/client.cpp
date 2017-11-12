#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <thread>
#include "terminal.h"

#include "../lib/sockets.h"

#define SERVER_PORT 1033
#define CLIENT_PORT 1032
#define BUFFER_SIZE 256;


using namespace std;

ClientGUI * clientInterface;

void connectionLost(int signal)
{
	clientInterface->printSendMessage("Signal caught! connection Lost!");
	delete clientInterface;
	//cout << "Signal caught! Connection lost!" << endl;
	exit(1);	
}


void receiving(TCPStream connection)
{
	string message;
	ssize_t bytesReceived;
	do
	{
		bytesReceived = connection.receive(message);
		if(bytesReceived < 0)
		{
				cout << "ERROR when reading from server! EXITING!" << endl;
				exit(1);
		}

		clientInterface->printReceivedMessage(message);
		
		//cout << "\nMessage: "  << message << " length: " << bytesReceived << endl;
	}while(bytesReceived > 0);
}

//ClientGUI * clientInterface;

int main()
{
	signal(SIGPIPE, connectionLost);
	
	clientInterface = new ClientGUI();
	
		//cout << "A client for the chat application!\n";

	int sd, connected;
	string message;

	SocketConnector conSock(CLIENT_PORT, "192.168.10.195");

	TCPStream connection = conSock.connect("192.168.10.195", SERVER_PORT);

		//cout << "\nConnected!\n" << endl;

	std::thread receiveThread(receiving, connection);
	
	while(1)
	{
		clientInterface->getInput(message);
		connection << message;
		clientInterface->printSendMessage(message);
		message.clear();
	}
	/*
	while(1)
	{
		
		try
		{
			getline(cin, message);
			connection << message;
			
		}
		catch(int e)
		{
			cout << "Excepion!\n";
			exit(1);
		}
	}*/
	
	delete clientInterface;
	receiveThread.join();
	

	cout << "\nEverything worked!\n";

	close(sd);
}
