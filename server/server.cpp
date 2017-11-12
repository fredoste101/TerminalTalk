#include <iostream>
#include <string>

#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <thread>

#include "../lib/sockets.h"


#define HOST_PORT 1033

using namespace std;

void connectionLost(int signal)
{
	cout << "Signal caught! Connection lost!" << endl;
	exit(1);	
}



void receiving(TCPStream connection)
{
	string message;
	ssize_t messageLength;
	do
	{
		messageLength = connection.receive(message);
		if(messageLength < 0)
		{
				cout << "ERROR when reading from client! EXITING!" << endl;
				exit(1);
		}

		cout << "\nMessage: "  << message << " length: " << messageLength << endl;
	}while(messageLength > 0);
}

void handleConnection(TCPStream connection)
{
	cout << "\n\nNEW CONNECTION\n\n";

	string message;

	thread receiveThread(receiving, connection);

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
	}
	cout << "You entered: " << message << endl;

	receiveThread.join();

	/*
	string message, reply;
	reply = "Yoo this is the server!";
	ssize_t messageLength;
	
	do
	{
		messageLength = connection.receive(message);
		if(messageLength == 0)
		{
			cout <<  "Empty receive...\n";
		}
		cout << "Message: " << message << " Message length: " << messageLength << endl;
	}while(messageLength > 0);
	*/
	connection.close();
	

	cout << "\nCONNECTION ENDED\n" << endl;

	return;
}


int main()
{
	signal(SIGPIPE, connectionLost);
	cout << "A server for the chat application!\n";
	

	SocketListener serverSocket(HOST_PORT, "192.168.10.195");
	serverSocket.bind();
	serverSocket.listen();
	

	cout << "Now accepting connections...\n";

	

	while(1)
	{
		TCPStream connection = serverSocket.accept();

		thread connectionThread(handleConnection, connection);
		
		connectionThread.join();
	}

	cout << "\nEverything worked!\n";
	serverSocket.close();
}
