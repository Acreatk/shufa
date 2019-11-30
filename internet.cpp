#include "internet.h"
#include "stdafx.h"
//#include <windows.h>
//#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#if (_WIN32_WINNT == 0x0500)
#include <tpipv6.h>
#endif
// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define WIN32_LEAN_AND_MEAN
#define STRICMP _stricmp
#define DEFAULT_SERVER     NULL // Will use the loopback interface
#define DEFAULT_FAMILY     PF_UNSPEC    // Accept either IPv4 or IPv6
#define DEFAULT_SOCKTYPE   SOCK_DGRAM  // TCP
#define DEFAULT_EXTRA      0    // Number of "extra" bytes to send
#define UNKNOWN_NAME "<unknown>"
#define DEFAULT_PORT       "6600"       // Arbitrary, albiet a historical test port
#define BUFFER_SIZE        64   // Set very small for demonstration purposes
// Needed for the Windows 2000 IPv6 Tech Preview.

using namespace std;
//client
char Buffer[BUFFER_SIZE], AddrName[NI_MAXHOST];
char *Server = DEFAULT_SERVER;
int Family = DEFAULT_FAMILY;
int SocketType = DEFAULT_SOCKTYPE;
char *Port = DEFAULT_PORT;
WSADATA wsaData;
int i, RetVal, AddrLen, AmountToSend;
int ExtraBytes = DEFAULT_EXTRA;
unsigned int Iteration, MaxIterations = 3;
BOOL RunForever = FALSE;
ADDRINFO Hints, *AddrInfo, *AI;
SOCKET ConnSocket = INVALID_SOCKET;
struct sockaddr_storage Addr;
//server
char Hostname[NI_MAXHOST];
char *Address = NULL;
int  NumSocks, FromLen, AmountRead;
SOCKADDR_STORAGE From;
SOCKET ServSock[FD_SETSIZE];
fd_set SockSet;
//int AddrLen;
//char AddrName[1000];
char port[10] = "6600";
//#########client#########
void Usage(char *ProgName)
{
	fprintf(stderr, "\nSimple socket sample client program.\n");//print the error to screen
	fprintf(stderr,
		"\n%s [-s server] [-f family] [-t transport] [-p port] [-b bytes] [-n number]\n\n",
		ProgName);
	fprintf(stderr, "  server\tServer name or IP address.  (default: %s)\n",
		(DEFAULT_SERVER == NULL) ? "loopback address" : DEFAULT_SERVER);
	fprintf(stderr,
		"  family\tOne of PF_INET, PF_INET6 or PF_UNSPEC.  (default: %s)\n",
		(DEFAULT_FAMILY ==
			PF_UNSPEC) ? "PF_UNSPEC" : ((DEFAULT_FAMILY ==
				PF_INET) ? "PF_INET" : "PF_INET6"));
	fprintf(stderr, "  transport\tEither TCP or UDP.  (default: %s)\n",
		(DEFAULT_SOCKTYPE == SOCK_STREAM) ? "TCP" : "UDP");
	fprintf(stderr, "  port\t\tPort on which to connect.  (default: %s)\n",
		DEFAULT_PORT);
	fprintf(stderr, "  bytes\t\tBytes of extra data to send.  (default: %d)\n",
		DEFAULT_EXTRA);
	fprintf(stderr, "  number\tNumber of sends to perform.  (default: 1)\n");
	fprintf(stderr, "  (-n by itself makes client run in an infinite loop,");
	fprintf(stderr, " Hit Ctrl-C to terminate)\n");

	WSACleanup();
	exit(1);
}

LPTSTR PrintError(int ErrorCode)
{
	static TCHAR Message[1024];


	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_MAX_WIDTH_MASK,
		NULL, ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		Message, 1024, NULL);
	return Message;
}

int ReceiveAndPrint(SOCKET ConnSocket, char *Buffer, int BufLen)
{
	int AmountRead;

	AmountRead = recv(ConnSocket, Buffer, BufLen, 0);
	if (AmountRead == SOCKET_ERROR) {
		fprintf(stderr, "recv() failed with error %d: %s\n",
			WSAGetLastError(), PrintError(WSAGetLastError()));
		closesocket(ConnSocket);
		WSACleanup();
		exit(1);
	}
	//
	// We are not likely to see this with UDP, since there is no
	// 'connection' established. 
	//
	if (AmountRead == 0) {
		printf("Server closed connection\n");
		closesocket(ConnSocket);
		WSACleanup();
		exit(0);
	}

	printf("Received %d bytes from server: [%.*s]\n",
		AmountRead, AmountRead, Buffer);

	return AmountRead;
}
//int main(int argc, char **argv)
int esta_client()
{

	//output test
	/*
	cout << "argc: " << argc << endl;
	cout << "**argv: " << **argv << endl;
	cout <<
	"*argv: " << *argv << endl;
	cout << "argv: " << argv << endl;
	cout << "argv[0]: " << argv[0] << endl;
	//cout << "argc[1]" << argv[1] << endl;
	*/

	//Usage("-f");

	std::cout << "Please input IP address:  ";
	char temp[50], p[10], port[10];
	std::cin >> temp;
	Server = temp;

	//std::cout << "Please input destnation Port:¡¡";
	//std::cin >> port;
	//Port = port;
	Port = "6600";

	//std::cout << "Chose a protocal :UDP or TCP" << endl;
	//std::cin >> p;
	//if (!STRICMP(p, "UDP")) { SocketType = SOCK_DGRAM; }
	SocketType = SOCK_DGRAM;

	/*
	if (argc > 1) {
	for (i = 1; i < argc; i++) {
	if (((argv[i][0] == '-') || (argv[i][0] == '/')) &&
	(argv[i][1] != 0) && (argv[i][2] == 0)) {  //-?0  /?0
	switch (tolower(argv[i][1])) { //tolower() letters from uppercase to lowercase
	case 'f':
	if (!argv[i + 1])
	Usage(argv[0]);
	if (!STRICMP(argv[i + 1], "PF_INET"))//STRICMP compare two array
	Family = PF_INET;
	else if (!STRICMP(argv[i + 1], "AF_INET"))
	Family = PF_INET;
	else if (!STRICMP(argv[i + 1], "PF_INET6"))
	Family = PF_INET6;
	else if (!STRICMP(argv[i + 1], "AF_INET6"))
	Family = PF_INET6;
	else if (!STRICMP(argv[i + 1], "PF_UNSPEC"))//accept ipv4 and ipv6
	Family = PF_UNSPEC;
	else if (!STRICMP(argv[i + 1], "AF_UNSPEC"))
	Family = PF_UNSPEC;
	else
	Usage(argv[0]);
	i++;
	break;

	case 't':
	if (!argv[i + 1])
	Usage(argv[0]);
	if (!STRICMP(argv[i + 1], "TCP"))
	SocketType = SOCK_STREAM;
	else if (!STRICMP(argv[i + 1], "UDP"))
	SocketType = SOCK_DGRAM;
	else
	Usage(argv[0]);
	i++;
	break;

	case 's':
	if (argv[i + 1]) {
	if (argv[i + 1][0] != '-') {
	Server = argv[++i];
	break;
	}
	}
	Usage(argv[0]);
	break;

	case 'p':
	if (argv[i + 1]) {
	if (argv[i + 1][0] != '-') {
	Port = argv[++i];
	break;
	}
	}
	Usage(argv[0]);
	break;

	case 'b':
	if (argv[i + 1]) {
	if (argv[i + 1][0] != '-') {
	ExtraBytes = atoi(argv[++i]);
	if (ExtraBytes >
	sizeof(Buffer) -
	sizeof("Message #4294967295"))
	Usage(argv[0]);
	break;
	}
	}
	Usage(argv[0]);
	break;

	case 'n':
	if (argv[i + 1]) {
	if (argv[i + 1][0] != '-') {
	MaxIterations = atoi(argv[++i]);
	break;
	}
	}
	RunForever = TRUE;
	break;

	default:
	Usage(argv[0]);
	break;
	}
	}
	else
	Usage(argv[0]);
	}
	}
	*/
	//getchar();
	// Ask for Winsock version 2.2.
	if ((RetVal = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		// wVersionRequested:The high-order byte specifies the minor version number; the low-order byte specifies the major version number.
		//lpWSAData [out]: A pointer to the WSADATA data structure that is to receive details of the Windows Sockets implementation.
		fprintf(stderr, "WSAStartup failed with error %d: %s\n",
			RetVal, PrintError(RetVal));
		WSACleanup();
		return -1;
	}
	//getchar();
	//cout << "*wsaData" << *wsaData << endl;

	// By not setting the AI_PASSIVE flag in the hints to getaddrinfo, we're
	// indicating that we intend to use the resulting address(es) to connect
	// to a service.  This means that when the Server parameter is NULL,
	// getaddrinfo will return one entry per allowed protocol family
	// containing the loopback address for that family.
	//

	memset(&Hints, 0, sizeof(Hints));//memset :reset memory
	Hints.ai_family = Family;
	Hints.ai_socktype = SocketType;
	RetVal = getaddrinfo(Server, Port, &Hints, &AddrInfo);//retval:return val
	if (RetVal != 0) {
		fprintf(stderr,
			"Cannot resolve address [%s] and port [%s], error %d: %s\n",
			Server, Port, RetVal, gai_strerror(RetVal));
		WSACleanup();
		return -1;
	}

	//
	// Try each address getaddrinfo returned, until we find one to which
	// we can successfully connect.
	//
	for (AI = AddrInfo; AI != NULL; AI = AI->ai_next) {

		// Open a socket with the correct address family for this address.

		ConnSocket = socket(AI->ai_family, AI->ai_socktype, AI->ai_protocol);

		//**** DEBUG
		//printf("socket call with family: %d socktype: %d, protocol: %d\n",
		//	AI->ai_family, AI->ai_socktype, AI->ai_protocol);
		if (ConnSocket == INVALID_SOCKET)
			printf("socket call failed with %d\n", WSAGetLastError());
		//**** DEBUG END

		if (ConnSocket == INVALID_SOCKET) {
			fprintf(stderr, "Error Opening socket, error %d: %s\n",
				WSAGetLastError(), PrintError(WSAGetLastError()));
			continue;
		}
		//
		// Notice that nothing in this code is specific to whether we 
		// are using UDP or TCP.
		//
		// When connect() is called on a datagram socket, it does not 
		// actually establish the connection as a stream (TCP) socket
		// would. Instead, TCP/IP establishes the remote half of the
		// (LocalIPAddress, LocalPort, RemoteIP, RemotePort) mapping.
		// This enables us to use send() and recv() on datagram sockets,
		// instead of recvfrom() and sendto().
		//

		printf("Attempting to connect to: %s\n", Server ? Server : "localhost");
		//std::cout << "HERE" << AI->ai_addr << endl;
		if (connect(ConnSocket, AI->ai_addr, (int)AI->ai_addrlen) != SOCKET_ERROR)
			break;

		i = WSAGetLastError();
		if (getnameinfo(AI->ai_addr, (int)AI->ai_addrlen, AddrName,
			sizeof(AddrName), NULL, 0, NI_NUMERICHOST) != 0)
			strcpy_s(AddrName, sizeof(AddrName), UNKNOWN_NAME);
		fprintf(stderr, "connect() to %s failed with error %d: %s\n",
			AddrName, i, PrintError(i));
		closesocket(ConnSocket);
	}

	if (AI == NULL) {
		fprintf(stderr, "Fatal error: unable to connect to the server.\n");
		WSACleanup();
		return -1;
	}
	//
	// This demonstrates how to determine to where a socket is connected.
	//
	AddrLen = sizeof(Addr);
	if (getpeername(ConnSocket, (LPSOCKADDR)& Addr, (int *)&AddrLen) == SOCKET_ERROR) {
		fprintf(stderr, "getpeername() failed with error %d: %s\n",
			WSAGetLastError(), PrintError(WSAGetLastError()));
	}
	else {
		if (getnameinfo((LPSOCKADDR)& Addr, AddrLen, AddrName,
			sizeof(AddrName), NULL, 0, NI_NUMERICHOST) != 0)
			strcpy_s(AddrName, sizeof(AddrName), UNKNOWN_NAME);

		printf("Connected to %s, port %d, protocol %s, protocol family %s\n",
			AddrName, ntohs(SS_PORT(&Addr)),
			(AI->ai_socktype == SOCK_STREAM) ? "TCP" : "UDP",
			(AI->ai_family == PF_INET) ? "PF_INET" : "PF_INET6");


	}


	// We are done with the address info chain, so we can free it.
	freeaddrinfo(AddrInfo);

	//
	// Find out what local address and port the system picked for us.
	//
	AddrLen = sizeof(Addr);
	if (getsockname(ConnSocket, (LPSOCKADDR)& Addr, &AddrLen) == SOCKET_ERROR) {
		fprintf(stderr, "getsockname() failed with error %d: %s\n",
			WSAGetLastError(), PrintError(WSAGetLastError()));
	}
	else {
		if (getnameinfo((LPSOCKADDR)& Addr, AddrLen, AddrName,
			sizeof(AddrName), NULL, 0, NI_NUMERICHOST) != 0)
			strcpy_s(AddrName, sizeof(AddrName), UNKNOWN_NAME);
		printf("Using local address %s, port %d\n",
			AddrName, ntohs(SS_PORT(&Addr)));
	}
}

int sendto_s(char data[BUFFER_SIZE], int AmountToSend) {
	//
	// Send and receive in a loop for the requested number of iterations.
	//
	//for (Iteration = 0; RunForever || Iteration < MaxIterations; Iteration++) {

	// Compose a message to send.
	/*
	AmountToSend =
	sprintf_s(Buffer, sizeof(Buffer), "Message-x #%u", Iteration + 1);

	for (i = 0; i < ExtraBytes; i++) {
	Buffer[AmountToSend++] = (char)((i & 0x3f) + 0x20);
	}

	getchar();
	*/
	// Send the message.  Since we are using a blocking socket, this
	// call shouldn't return until it's able to send the entire amount.
	
	RetVal = send(ConnSocket, data, AmountToSend, 0);
	if (RetVal == SOCKET_ERROR) {
		fprintf(stderr, "send() failed with error %d: %s\n",
			WSAGetLastError(), PrintError(WSAGetLastError()));
		WSACleanup();
		return -1;
	}
	//printf("Sent %d bytes of data: [%.*s]\n",//* is used to control the width of the string
	//	RetVal, AmountToSend, AmountToSend, Buffer);

	// Clear buffer just to prove we're really receiving something.
	memset(Buffer, 0, sizeof(Buffer));

	// Receive and print server's reply.
	//ReceiveAndPrint(ConnSocket, Buffer, sizeof(Buffer));
	//ReceiveAndPrint(ConnSocket, Buffer, sizeof(Buffer));
	//}

	// Tell system we're done sending.
	//printf("Done sending\n");
	//getchar();
	//shutdown(ConnSocket, SD_SEND);

	//
	// Since TCP does not preserve message boundaries, there may still
	// be more data arriving from the server.  So we continue to receive
	// data until the server closes the connection.
	//




	return 0;
}

int refrom_S(char data[BUFFER_SIZE])
{
	if (SocketType == SOCK_STREAM)
		while (ReceiveAndPrint(ConnSocket, Buffer, sizeof(Buffer)) != 0);
	return 0;
}

void close_c()
{
	closesocket(ConnSocket);
	WSACleanup();
}

int esta_server()
{

	SocketType = SOCK_DGRAM;
	//std::cout << "Input the listening Port: ";
	//std::cin >> port;
	Port = port;
	// Ask for Winsock version 2.2.
	// Ask for Winsock version 2.2.
	if ((RetVal = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		fprintf(stderr, "WSAStartup failed with error %d: %s\n",
			RetVal, PrintError(RetVal));
		WSACleanup();
		return -1;
	}

	if (Port == NULL) {
		//Usage(argv[0]);
	}
	//
	// By setting the AI_PASSIVE flag in the hints to getaddrinfo, we're
	// indicating that we intend to use the resulting address(es) to bind
	// to a socket(s) for accepting incoming connections.  This means that
	// when the Address parameter is NULL, getaddrinfo will return one
	// entry per allowed protocol family containing the unspecified address
	// for that family.
	//
	memset(&Hints, 0, sizeof(Hints));
	Hints.ai_family = Family;
	Hints.ai_socktype = SocketType;
	Hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE;
	RetVal = getaddrinfo(Address, Port, &Hints, &AddrInfo);
	if (RetVal != 0) {
		fprintf(stderr, "getaddrinfo failed with error %d: %s\n",
			RetVal, gai_strerror(RetVal));
		WSACleanup();
		return -1;
	}
	//
	// For each address getaddrinfo returned, we create a new socket,
	// bind that address to it, and create a queue to listen on.
	//
	for (i = 0, AI = AddrInfo; AI != NULL; AI = AI->ai_next) {

		// Highly unlikely, but check anyway.
		if (i == FD_SETSIZE) {
			printf("getaddrinfo returned more addresses than we could use.\n");
			break;
		}
		// This example only supports PF_INET and PF_INET6.
		if ((AI->ai_family != PF_INET) && (AI->ai_family != PF_INET6))
			continue;

		// Open a socket with the correct address family for this address.
		ServSock[i] = socket(AI->ai_family, AI->ai_socktype, AI->ai_protocol);
		if (ServSock[i] == INVALID_SOCKET) {
			fprintf(stderr, "socket() failed with error %d: %s\n",
				WSAGetLastError(), PrintError(WSAGetLastError()));
			continue;
		}

		if ((AI->ai_family == PF_INET6) &&
			IN6_IS_ADDR_LINKLOCAL((IN6_ADDR *)INETADDR_ADDRESS(AI->ai_addr)) &&
			(((SOCKADDR_IN6 *)(AI->ai_addr))->sin6_scope_id == 0)
			) {
			fprintf(stderr,
				"IPv6 link local addresses should specify a scope ID!\n");
		}
		//
		// bind() associates a local address and port combination
		// with the socket just created. This is most useful when
		// the application is a server that has a well-known port
		// that clients know about in advance.
		//
		if (bind(ServSock[i], AI->ai_addr, (int)AI->ai_addrlen) == SOCKET_ERROR) {
			fprintf(stderr, "bind() failed with error %d: %s\n",
				WSAGetLastError(), PrintError(WSAGetLastError()));
			closesocket(ServSock[i]);
			continue;
		}
		//
		// So far, everything we did was applicable to TCP as well as UDP.
		// However, there are certain fundamental differences between stream
		// protocols such as TCP and datagram protocols such as UDP.
		//
		// Only connection orientated sockets, for example those of type
		// SOCK_STREAM, can listen() for incoming connections.
		//
		if (SocketType == SOCK_STREAM) {
			if (listen(ServSock[i], 5) == SOCKET_ERROR) {
				fprintf(stderr, "listen() failed with error %d: %s\n",
					WSAGetLastError(), PrintError(WSAGetLastError()));
				closesocket(ServSock[i]);
				continue;
			}
		}

		printf("'Listening' on port %s, protocol %s, protocol family %s\n",
			Port, (SocketType == SOCK_STREAM) ? "TCP" : "UDP",
			(AI->ai_family == PF_INET) ? "PF_INET" : "PF_INET6");
		i++;
	}

	freeaddrinfo(AddrInfo);

	if (i == 0) {
		fprintf(stderr, "Fatal error: unable to serve on any address.\n");
		WSACleanup();
		return -1;
	}
	NumSocks = i;

	//
	// We now put the server into an eternal loop,
	// serving requests as they arrive.
	//
	FD_ZERO(&SockSet);
	/*
	while (1)
	{
		FromLen = sizeof(From);
		std::cout << "1" << std::endl;
		//
		// For connection orientated protocols, we will handle the
		// packets received from a connection collectively.  For datagram
		// protocols, we have to handle each datagram individually.
		//

		//
		// Check to see if we have any sockets remaining to be served
		// from previous time through this loop.  If not, call select()
		// to wait for a connection request or a datagram to arrive.
		//
		for (i = 0; i < NumSocks; i++) {
			if (FD_ISSET(ServSock[i], &SockSet))//check ServSock[i] is or not a element of SockSet collection
				break;
		}
		std::cout << "2" << std::endl;
		if (i == NumSocks) {
			for (i = 0; i < NumSocks; i++)
				FD_SET(ServSock[i], &SockSet);//add ServSock[i]
			if (select(NumSocks, &SockSet, 0, 0, 0) == SOCKET_ERROR) {
				fprintf(stderr, "select() failed with error %d: %s\n",
					WSAGetLastError(), PrintError(WSAGetLastError()));
				WSACleanup();
				return -1;
			}
		}
		std::cout << "3" << std::endl;
		for (i = 0; i < NumSocks; i++) {
			if (FD_ISSET(ServSock[i], &SockSet)) {
				FD_CLR(ServSock[i], &SockSet);
				break;
			}
		}
		std::cout << "4" << std::endl;
		if (SocketType == SOCK_STREAM) {
			SOCKET ConnSock;

			//
			// Since this socket was returned by the select(), we know we
			// have a connection waiting and that this accept() won't block.
			//
			ConnSock = accept(ServSock[i], (LPSOCKADDR)& From, &FromLen);
			if (ConnSock == INVALID_SOCKET) {
				fprintf(stderr, "accept() failed with error %d: %s\n",
					WSAGetLastError(), PrintError(WSAGetLastError()));
				WSACleanup();
				return -1;
			}
			if (getnameinfo((LPSOCKADDR)& From, FromLen, Hostname,
				sizeof(Hostname), NULL, 0, NI_NUMERICHOST) != 0)
				strcpy_s(Hostname, NI_MAXHOST, "<unknown>");
			printf("\nAccepted connection from %s\n", Hostname);
			//****
			//AddrLen = sizeof(Addr);
			//if (getpeername(ConnSock, (LPSOCKADDR)& Addr, (int *)&AddrLen) == SOCKET_ERROR) {
			//	fprintf(stderr, "getpeername() failed with error %d: %s\n",
			//		WSAGetLastError(), PrintError(WSAGetLastError()));
			//}
			//else {
			//	if (getnameinfo((LPSOCKADDR)& Addr, AddrLen, AddrName,
			//		sizeof(AddrName), NULL, 0, NI_NUMERICHOST) != 0)
			//		strcpy_s(AddrName, sizeof(AddrName), UNKNOWN_NAME);
			//
			printf("Connected to %s, port %d",
				Hostname, ntohs(SS_PORT(&From)));


			//}
			//****

			//
			// This sample server only handles connections sequentially.
			// To handle multiple connections simultaneously, a server
			// would likely want to launch another thread or process at this
			// point to handle each individual connection.  Alternatively,
			// it could keep a socket per connection and use select()
			// on the fd_set to determine which to read from next.
			//
			// Here we just loop until this connection terminates.
			//

			while (1) {

				//
				// We now read in data from the client.  Because TCP
				// does NOT maintain message boundaries, we may recv()
				// the client's data grouped differently than it was
				// sent.  Since all this server does is echo the data it
				// receives back to the client, we don't need to concern
				// ourselves about message boundaries.  But it does mean
				// that the message data we print for a particular recv()
				// below may contain more or less data than was contained
				// in a particular client send().
				//

				AmountRead = recv(ConnSock, Buffer, sizeof(Buffer), 0);



				if (AmountRead == SOCKET_ERROR) {
					fprintf(stderr, "recv() failed with error %d: %s\n",
						WSAGetLastError(), PrintError(WSAGetLastError()));
					closesocket(ConnSock);
					break;
				}
				if (AmountRead == 0) {
					printf("Client closed connection\n");
					closesocket(ConnSock);
					break;
				}

				printf("Received %d bytes from client: [%.*s]\n",
					AmountRead, AmountRead, Buffer);
				printf("Echoing same data back to client\n");

				RetVal = send(ConnSock, Buffer, AmountRead, 0);
				if (RetVal == SOCKET_ERROR) {
					fprintf(stderr, "send() failed: error %d: %s\n",
						WSAGetLastError(), PrintError(WSAGetLastError()));
					closesocket(ConnSock);
					break;
				}
			}

		}
		else {

			//
			// Since UDP maintains message boundaries, the amount of data
			// we get from a recvfrom() should match exactly the amount of
			// data the client sent in the corresponding sendto().
			//
			memset(Buffer, 0, sizeof(Buffer));
			AmountRead = recvfrom(ServSock[i], Buffer, sizeof(Buffer), 0,
				(LPSOCKADDR)& From, &FromLen);
			if (AmountRead == SOCKET_ERROR) {
				fprintf(stderr, "recvfrom() failed with error %d: %s\n",
					WSAGetLastError(), PrintError(WSAGetLastError()));
				closesocket(ServSock[i]);
				break;
			}
			std::cout << "5" << std::endl;
			if (AmountRead == 0) {
				// This should never happen on an unconnected socket, but...
				printf("recvfrom() returned zero, aborting\n");
				closesocket(ServSock[i]);
				break;
			}
			std::cout << "6" << std::endl;

			RetVal = getnameinfo((LPSOCKADDR)& From, FromLen, Hostname,
				sizeof(Hostname), NULL, 0, NI_NUMERICHOST);
			std::cout << "7" << std::endl;
			if (RetVal != 0) {
				fprintf(stderr, "getnameinfo() failed with error %d: %s\n",
					RetVal, PrintError(RetVal));
				strcpy_s(Hostname, NI_MAXHOST, "<unknown>");
			}
			std::cout << "8" << std::endl;
			//printf("Received a %d byte datagram from %s : %s: [%.*s]\n",
			//	AmountRead, Hostname, ntohs(SS_PORT(&From)), AmountRead, Buffer);
			std::cout << Buffer << std::endl;
			//RetVal = sendto(ServSock[i], Buffer, AmountRead, 0,
			//	(LPSOCKADDR)& From, FromLen);

			//if (RetVal == SOCKET_ERROR) {
			//	fprintf(stderr, "send() failed with error %d: %s\n",
			//		WSAGetLastError(), PrintError(WSAGetLastError()));
			//}

		}

	}
	*/
	
	return 0;
}

int get_data(char data[BUFFER_SIZE])
{

	
		FromLen = sizeof(From);
		//
		// For connection orientated protocols, we will handle the
		// packets received from a connection collectively.  For datagram
		// protocols, we have to handle each datagram individually.
		//

		//
		// Check to see if we have any sockets remaining to be served
		// from previous time through this loop.  If not, call select()
		// to wait for a connection request or a datagram to arrive.
		//
		for (i = 0; i < NumSocks; i++) {
			if (FD_ISSET(ServSock[i], &SockSet))//check ServSock[i] is or not a element of SockSet collection
				break;
		}

		if (i == NumSocks) {
			for (i = 0; i < NumSocks; i++)
				FD_SET(ServSock[i], &SockSet);//add ServSock[i]
			if (select(NumSocks, &SockSet, 0, 0, 0) == SOCKET_ERROR) {
				fprintf(stderr, "select() failed with error %d: %s\n",
					WSAGetLastError(), PrintError(WSAGetLastError()));
				WSACleanup();
				return -1;
			}
		}
		for (i = 0; i < NumSocks; i++) {
			if (FD_ISSET(ServSock[i], &SockSet)) {
				FD_CLR(ServSock[i], &SockSet);
				break;
			}
		}
		if (SocketType == SOCK_STREAM) {
			SOCKET ConnSock;

			//
			// Since this socket was returned by the select(), we know we
			// have a connection waiting and that this accept() won't block.
			//
			ConnSock = accept(ServSock[i], (LPSOCKADDR)& From, &FromLen);
			if (ConnSock == INVALID_SOCKET) {
				fprintf(stderr, "accept() failed with error %d: %s\n",
					WSAGetLastError(), PrintError(WSAGetLastError()));
				WSACleanup();
				return -1;
			}
			if (getnameinfo((LPSOCKADDR)& From, FromLen, Hostname,
				sizeof(Hostname), NULL, 0, NI_NUMERICHOST) != 0)
				strcpy_s(Hostname, NI_MAXHOST, "<unknown>");
			printf("\nAccepted connection from %s\n", Hostname);
			//****
			//AddrLen = sizeof(Addr);
			//if (getpeername(ConnSock, (LPSOCKADDR)& Addr, (int *)&AddrLen) == SOCKET_ERROR) {
			//	fprintf(stderr, "getpeername() failed with error %d: %s\n",
			//		WSAGetLastError(), PrintError(WSAGetLastError()));
			//}
			//else {
			//	if (getnameinfo((LPSOCKADDR)& Addr, AddrLen, AddrName,
			//		sizeof(AddrName), NULL, 0, NI_NUMERICHOST) != 0)
			//		strcpy_s(AddrName, sizeof(AddrName), UNKNOWN_NAME);
			//
			printf("Connected to %s, port %d",
				Hostname, ntohs(SS_PORT(&From)));


			//}
			//****

			//
			// This sample server only handles connections sequentially.
			// To handle multiple connections simultaneously, a server
			// would likely want to launch another thread or process at this
			// point to handle each individual connection.  Alternatively,
			// it could keep a socket per connection and use select()
			// on the fd_set to determine which to read from next.
			//
			// Here we just loop until this connection terminates.
			//

			while (1) {

				//
				// We now read in data from the client.  Because TCP
				// does NOT maintain message boundaries, we may recv()
				// the client's data grouped differently than it was
				// sent.  Since all this server does is echo the data it
				// receives back to the client, we don't need to concern
				// ourselves about message boundaries.  But it does mean
				// that the message data we print for a particular recv()
				// below may contain more or less data than was contained
				// in a particular client send().
				//

				AmountRead = recv(ConnSock, Buffer, sizeof(Buffer), 0);



				if (AmountRead == SOCKET_ERROR) {
					fprintf(stderr, "recv() failed with error %d: %s\n",
						WSAGetLastError(), PrintError(WSAGetLastError()));
					closesocket(ConnSock);
					break;
				}
				if (AmountRead == 0) {
					printf("Client closed connection\n");
					closesocket(ConnSock);
					break;
				}

				printf("Received %d bytes from client: [%.*s]\n",
					AmountRead, AmountRead, Buffer);
				printf("Echoing same data back to client\n");

				RetVal = send(ConnSock, Buffer, AmountRead, 0);
				if (RetVal == SOCKET_ERROR) {
					fprintf(stderr, "send() failed: error %d: %s\n",
						WSAGetLastError(), PrintError(WSAGetLastError()));
					closesocket(ConnSock);
					break;
				}
			}

		}
		else {

			//
			// Since UDP maintains message boundaries, the amount of data
			// we get from a recvfrom() should match exactly the amount of
			// data the client sent in the corresponding sendto().
			//
			memset(Buffer, 0, sizeof(Buffer));
			AmountRead = recvfrom(ServSock[i], Buffer, sizeof(Buffer), 0,
				(LPSOCKADDR)& From, &FromLen);
			if (AmountRead == SOCKET_ERROR) {
				fprintf(stderr, "recvfrom() failed with error %d: %s\n",
					WSAGetLastError(), PrintError(WSAGetLastError()));
				closesocket(ServSock[i]);
				//break;
				return 0;
			}
			if (AmountRead == 0) {
				// This should never happen on an unconnected socket, but...
				printf("recvfrom() returned zero, aborting\n");
				closesocket(ServSock[i]);
				//break;
				return 0;
			}
			RetVal = getnameinfo((LPSOCKADDR)& From, FromLen, Hostname,
				sizeof(Hostname), NULL, 0, NI_NUMERICHOST);
			if (RetVal != 0) {
				fprintf(stderr, "getnameinfo() failed with error %d: %s\n",
					RetVal, PrintError(RetVal));
				strcpy_s(Hostname, NI_MAXHOST, "<unknown>");
			}
			//printf("Received a %d byte datagram from %s : %s: [%.*s]\n",
			//	AmountRead, Hostname, ntohs(SS_PORT(&From)), AmountRead, Buffer);
			//std::cout << Buffer << std::endl;
			//data = Buffer;
			memcpy(data, Buffer,BUFFER_SIZE);
			//RetVal = sendto(ServSock[i], Buffer, AmountRead, 0,
			//	(LPSOCKADDR)& From, FromLen);

			//if (RetVal == SOCKET_ERROR) {
			//	fprintf(stderr, "send() failed with error %d: %s\n",
			//		WSAGetLastError(), PrintError(WSAGetLastError()));
			//}

		}

	
	
	/*
	FromLen = sizeof(From);
	std::cout << "1" << std::endl;
	//
	// For connection orientated protocols, we will handle the
	// packets received from a connection collectively.  For datagram
	// protocols, we have to handle each datagram individually.
	//

	//
	// Check to see if we have any sockets remaining to be served
	// from previous time through this loop.  If not, call select()
	// to wait for a connection request or a datagram to arrive.
	//
	for (i = 0; i < NumSocks; i++) {
		if (FD_ISSET(ServSock[i], &SockSet))//check ServSock[i] is or not a element of SockSet collection
			break;
	}
	std::cout << "2" << std::endl;
	if (i == NumSocks) {
		for (i = 0; i < NumSocks; i++)
			FD_SET(ServSock[i], &SockSet);//add ServSock[i]
		if (select(NumSocks, &SockSet, 0, 0, 0) == SOCKET_ERROR) {
			fprintf(stderr, "select() failed with error %d: %s\n",
				WSAGetLastError(), PrintError(WSAGetLastError()));
			WSACleanup();
			return -1;
		}
	}
	std::cout << "3" << std::endl;
	for (i = 0; i < NumSocks; i++) {
		if (FD_ISSET(ServSock[i], &SockSet)) {
			FD_CLR(ServSock[i], &SockSet);
			break;
		}
	}
	std::cout << "4" << std::endl;
	if (SocketType == SOCK_STREAM) {
		SOCKET ConnSock;

		//
		// Since this socket was returned by the select(), we know we
		// have a connection waiting and that this accept() won't block.
		//
		ConnSock = accept(ServSock[i], (LPSOCKADDR)& From, &FromLen);
		if (ConnSock == INVALID_SOCKET) {
			fprintf(stderr, "accept() failed with error %d: %s\n",
				WSAGetLastError(), PrintError(WSAGetLastError()));
			WSACleanup();
			return -1;
		}
		if (getnameinfo((LPSOCKADDR)& From, FromLen, Hostname,
			sizeof(Hostname), NULL, 0, NI_NUMERICHOST) != 0)
			strcpy_s(Hostname, NI_MAXHOST, "<unknown>");
		printf("\nAccepted connection from %s\n", Hostname);
		//****
		//AddrLen = sizeof(Addr);
		//if (getpeername(ConnSock, (LPSOCKADDR)& Addr, (int *)&AddrLen) == SOCKET_ERROR) {
		//	fprintf(stderr, "getpeername() failed with error %d: %s\n",
		//		WSAGetLastError(), PrintError(WSAGetLastError()));
		//}
		//else {
		//	if (getnameinfo((LPSOCKADDR)& Addr, AddrLen, AddrName,
		//		sizeof(AddrName), NULL, 0, NI_NUMERICHOST) != 0)
		//		strcpy_s(AddrName, sizeof(AddrName), UNKNOWN_NAME);
		//
		printf("Connected to %s, port %d",
			Hostname, ntohs(SS_PORT(&From)));


		//}
		//****

		//
		// This sample server only handles connections sequentially.
		// To handle multiple connections simultaneously, a server
		// would likely want to launch another thread or process at this
		// point to handle each individual connection.  Alternatively,
		// it could keep a socket per connection and use select()
		// on the fd_set to determine which to read from next.
		//
		// Here we just loop until this connection terminates.
		//

		while (1) {

			//
			// We now read in data from the client.  Because TCP
			// does NOT maintain message boundaries, we may recv()
			// the client's data grouped differently than it was
			// sent.  Since all this server does is echo the data it
			// receives back to the client, we don't need to concern
			// ourselves about message boundaries.  But it does mean
			// that the message data we print for a particular recv()
			// below may contain more or less data than was contained
			// in a particular client send().
			//

			AmountRead = recv(ConnSock, Buffer, sizeof(Buffer), 0);



			if (AmountRead == SOCKET_ERROR) {
				fprintf(stderr, "recv() failed with error %d: %s\n",
					WSAGetLastError(), PrintError(WSAGetLastError()));
				closesocket(ConnSock);
				break;
			}
			if (AmountRead == 0) {
				printf("Client closed connection\n");
				closesocket(ConnSock);
				break;
			}

			printf("Received %d bytes from client: [%.*s]\n",
				AmountRead, AmountRead, Buffer);
			printf("Echoing same data back to client\n");

			RetVal = send(ConnSock, Buffer, AmountRead, 0);
			if (RetVal == SOCKET_ERROR) {
				fprintf(stderr, "send() failed: error %d: %s\n",
					WSAGetLastError(), PrintError(WSAGetLastError()));
				closesocket(ConnSock);
				break;
			}
		}

	}
	else {

		//
		// Since UDP maintains message boundaries, the amount of data
		// we get from a recvfrom() should match exactly the amount of
		// data the client sent in the corresponding sendto().
		//
		AmountRead = recvfrom(ServSock[i], data, sizeof(data), 0,
			(LPSOCKADDR)& From, &FromLen);
		if (AmountRead == SOCKET_ERROR) {
			fprintf(stderr, "recvfrom() failed with error %d: %s\n",
				WSAGetLastError(), PrintError(WSAGetLastError()));
			closesocket(ServSock[i]);
			return 0;
		}
		std::cout << "5" << std::endl;
		if (AmountRead == 0) {
			// This should never happen on an unconnected socket, but...
			printf("recvfrom() returned zero, aborting\n");
			closesocket(ServSock[i]);
			return 0;
		}
		std::cout << "6" << std::endl;

		RetVal = getnameinfo((LPSOCKADDR)& From, FromLen, Hostname,
			sizeof(Hostname), NULL, 0, NI_NUMERICHOST);
		std::cout << "7" << std::endl;
		if (RetVal != 0) {
			fprintf(stderr, "getnameinfo() failed with error %d: %s\n",
				RetVal, PrintError(RetVal));
			strcpy_s(Hostname, NI_MAXHOST, "<unknown>");
		}
		std::cout << "8" << std::endl;
		//printf("Received a %d byte datagram from %s : %s: [%.*s]\n",
		//	AmountRead, Hostname, ntohs(SS_PORT(&From)), AmountRead, Buffer);
		std::cout << data << std::endl;
		//RetVal = sendto(ServSock[i], Buffer, AmountRead, 0,
		//	(LPSOCKADDR)& From, FromLen);
		
		//if (RetVal == SOCKET_ERROR) {
		//	fprintf(stderr, "send() failed with error %d: %s\n",
		//		WSAGetLastError(), PrintError(WSAGetLastError()));
		//}
		
	}
	*/

	return 0;
}
