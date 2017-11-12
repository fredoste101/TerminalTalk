#ifndef SOCKETS_H
#define SOCKETS_H

#define IO_SIGNAL SIGUSR1   /* Signal used to notify I/O completion */


using namespace std;

class TCPStream;

class Socket
{
	protected:
		sa_family_t addrFamily; /* Defined in #include <sys/socket.h> value is most likely AF_INET (IPv4)*/
		in_port_t port;			/* Defined in #include <netinet/in.h>*/
		string ip;				/* Ex: 192.0.0.1 */
		int handle;
	public:
		Socket(in_port_t p, string address);
		
		void close();
		int getHandle();
		void setIp(string address);
		char * ipToString(unsigned int ip);

};

class SocketConnector : public Socket
{
	public:
		SocketConnector(in_port_t p, string address);
		
		TCPStream connect(string ip, in_port_t conPort);
};

class SocketListener : public Socket
{
	public:
		SocketListener(in_port_t p, string address);

		void bind();
		void listen();
		TCPStream accept();

};

class TCPStream
{
	private:
		int fileHandle;
		in_port_t port;
		string ip;
	public:
		TCPStream(in_port_t p, string address, int socketHandle);
		
		void operator<<(string& message);
		void operator>>(string& output);
		ssize_t receive(string& message);
		void close();
		void asyncRead();
		
};


#endif
