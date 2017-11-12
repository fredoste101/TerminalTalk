#include <iostream>
#include <string>

#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>


#include "sockets.h"


#define BUFFER_SIZE 256


//Clear up the include list vafan


using namespace std;


Socket::Socket(in_port_t p, string address)
{
	port = p;
	Socket::setIp(address);
	addrFamily = AF_INET;
	handle = ::socket(addrFamily, SOCK_STREAM, 0); //Check this! EXEPTIONS!!!
}

void Socket::close()
{
	::close(handle);
}

int Socket::getHandle()
{
	return handle;
}

void Socket::setIp(string address)
{
	ip = address; //Check that address is ok, otherwise, throw excetion i presume?
}

char * Socket::ipToString(unsigned int ip)
{
	unsigned char * bytes = new unsigned char[5];
	bytes[0] = ip & 0xFF;
	bytes[1] = (ip >> 8) & 0xFF;
	bytes[2] = (ip >> 16) & 0xFF;
	bytes[3] = (ip >> 24) & 0xFF;   
	bytes[4] = '\0';
	return (char *)bytes;    
}

SocketConnector::SocketConnector(in_port_t p, string address) : Socket(p, address){};

TCPStream SocketConnector::connect(string conIp, in_port_t conPort)
{
	sockaddr_in con_addr;
	con_addr.sin_family = addrFamily;
	con_addr.sin_port = htons(conPort);
	inet_aton(conIp.c_str(), (in_addr*)&con_addr.sin_addr.s_addr);
	
	if(::connect(handle, (sockaddr*)&con_addr, sizeof(con_addr)) < 0)
	{
		//Throw exception!!
		cout << "Could not connect to server! EXITING!\n";
		exit(1);
	}

	return TCPStream(conPort, conIp, handle);
}




SocketListener::SocketListener(in_port_t p, string address) : Socket(p, address){}

void SocketListener::bind()
{
	sockaddr_in host_addr;
	host_addr.sin_family = addrFamily;
	host_addr.sin_port = htons(port);
	::inet_aton(ip.c_str(), (in_addr*)&host_addr.sin_addr.s_addr);
	if( ::bind(handle, (const sockaddr *)&host_addr, sizeof(host_addr)) < 0)
	{
		//Here is where we throw exception when I am familiar with them
		::printf("\nError while binding!EXITING\n");
		::perror(nullptr);
		::exit(EXIT_FAILURE);
	}
}

void SocketListener::listen()
{
	//Ability to set queue size is missing!
	if(::listen(handle, 10) < 0)
	{
		//Here is where we throw exception when I am familiar with them
		::printf("\nError when listening! EXITING\n");
		::perror(nullptr);
		::exit(EXIT_FAILURE);
	}
}

TCPStream SocketListener::accept()
{
	in_port_t conPort;
	int conHandle;
	sockaddr_in con_addr;
	socklen_t addr_len = sizeof(sockaddr_in);
	
	conHandle = ::accept(handle, (sockaddr *)&con_addr, &addr_len);
	string conIp = ipToString(ntohl(con_addr.sin_addr.s_addr));
	conPort = ntohs(con_addr.sin_port);
	
	if(conHandle < 0)
	{
		//Here is where we throw exception when I am familiar with them
		::printf("\nError when accepting! EXITING\n");
		::perror(nullptr);
		::exit(EXIT_FAILURE);
	}

	return TCPStream( conPort, conIp, conHandle );
}


TCPStream::TCPStream(in_port_t p, string address, int fd)
{
	fileHandle = fd;
	port = p;
	ip = address;
}

void TCPStream::operator<<(string& message)
{
	//Kolla polling osv... Vette fan det här men de ble krångligare än vad jag trodde att det skulle bli.
	//Jävla tid o fatta att connection är slut vafan :'(
	ssize_t bytesSent = send(fileHandle, message.c_str(), (sizeof(char) * (message.length()+1)), 0);

	if(bytesSent < 0 || bytesSent == EPIPE)
	{
		cout << "Connection is down!" << endl;
	}
}

void TCPStream::operator>>(string& message)
{
	char buffer[BUFFER_SIZE];
	ssize_t bytesRead = 0;

	bytesRead = ::read(fileHandle, buffer, (sizeof(char) * BUFFER_SIZE));

	if(bytesRead < 0)
	{
		cout << "Error when reading from socket! EXITING\n";
		exit(EXIT_FAILURE);
	}

	if(bytesRead == 0)
	{
		message.clear();
	}
	else
	{
		message = buffer;
	}
}

ssize_t TCPStream::receive(string& message)
{
	char buffer[BUFFER_SIZE];
	ssize_t bytesRead = 0;

	bytesRead = ::read(fileHandle, buffer, (sizeof(char) * BUFFER_SIZE));

	if(bytesRead < 0)
	{
		//EXCEPTION!!!
		cout << "Error when reading from socket! EXITING\n";
		exit(EXIT_FAILURE);
	}

	if(bytesRead == 0)
	{
		message.clear();
	}
	else
	{
		message = buffer;
	}
	
	return bytesRead;
}

void TCPStream::close()
{
	::close(fileHandle);
}

