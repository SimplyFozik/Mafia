#include <iostream>
#include <string>
#include <winsock2.h>
#include <thread>
#include <chrono>
#include <conio.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

using namespace std;

const int Max_players = 10;
SOCKET Connections[10];
int Counter = 0;

void recvMsg(int index)
{
    int msg_size;
    while (true)
    {
        int RecievedBytes = recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);

        if (RecievedBytes <= 0) // Check If player disconnected
        {
            cout << "Error #1\n" << "No Bytes Recieved, Client disconnected\n";
            ExitThread(1);
        }

        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        int RecievedMsg = recv(Connections[index], msg, msg_size, NULL);

        if (RecievedMsg <= 0) // Check if player disconnected
        {
            cout << "Error #2\n" << "No Bytes Recieved, Client disconnected\n";
            ExitThread(2);
        }
        for (int i = 0; i < Counter; i++) {
            if (i == index) {
                continue;
            }
            send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
            send(Connections[i], msg, msg_size, NULL);
        }
        delete[] msg;
    }
}

int main()
{
    setlocale(LC_ALL, "RU");

    //WSAStartup
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        cout << "[Server] Error" << endl;
        exit(1);
    }

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("192.168.0.71"); // 192.168.1.67
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);

    SOCKET newConnection;
    for (int i = 0; i < 100; i++) {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

        if (newConnection == 0) {
            cout << "[Server] Error #2\n";
        }
        else {
            cout << "[Server] Client Connected!\n";
            string msg = "[Server] Connection Completed.";
            int msg_size = msg.size();
            send(newConnection, (char*)&msg_size, sizeof(int), NULL);
            send(newConnection, msg.c_str(), msg_size, NULL);

            Connections[i] = newConnection;
            Counter++;
            //thread connectionThread(recvMsg, i);
            //connectionThread.join();
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)recvMsg, (LPVOID)(i), NULL, NULL);
        }
    }
}