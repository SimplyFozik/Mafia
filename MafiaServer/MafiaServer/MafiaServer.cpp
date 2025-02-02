﻿#include <iostream>
#include <winsock2.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <string>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable: 4996)

void funcSendMessage(SOCKET newConnection, std::string text)
{
	std::getline(std::cin, text);
	int msg_size = text.size();
	send(newConnection, (char*)&msg_size, sizeof(int), NULL);
	send(newConnection, text.c_str(), msg_size, NULL);
	funcSendMessage(newConnection, text);
}

void funcWaitForAnwser(SOCKET Connection, std::string text)
{
	int msg_size;
	std::string msg;
	for (;;)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		recv(Connection, (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connection, msg, msg_size, NULL);
		std::cout << msg << std::endl;
		delete[] msg;
	}

}

//void funcCheckConnection(SOCKET Connection)
//{
//	if (Connection == 0)
//	{
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//		exit(1);
//	}
//	else
//	{
//		funcCheckConnection(Connection);
//	}
//}

int main(int argc, char* argv[])
{
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		std::cout << "Error\n";
		exit(1);
	}
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

	if (newConnection == 0)
	{
		std::cout << "Error #2\n";
	}
	else
	{
		std::cout << "Client Connection - OK.\n";
		std::string text_out;
		std::string text_in;
		std::thread th1(funcSendMessage, newConnection, text_out);
		std::thread th2(funcWaitForAnwser, newConnection, text_in);
		/*std::thread th3(funcCheckConnection, newConnection);*/
		th1.join();
		th2.join();
		/*th3.join();*/
	}

	system("pause");
	return 0;
}