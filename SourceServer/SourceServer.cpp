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
bool debug = false;

void fireClient(char* msg, int msg_size, int index) // Server->Client
{
    SOCKET Connection = Connections[index];
    send(Connection, (char*)&msg_size, sizeof(int), NULL);
    send(Connection, msg, msg_size, NULL);
    delete[] msg;
}

void fireAllClients(char* msg,int msg_size, int index) // Server->Clients
{
    for (int i = 0; i < Counter; i++) // send msg to everyone
    {
        if (i == index)
        {
            continue;
        }
        send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
        send(Connections[i], msg, msg_size, NULL);
    }
    delete[] msg;
}

void getCommand(char* msg, int msg_size, int index)
{
    if (strcmp(msg, "?debug") == 0)
    {
        debug = true;
    }

    if (msg[0] == '?' && msg_size > 1 && debug == true) // ex. of command ?kill index
    {
        if (strcmp(msg, "?kill") == 0)
        {
            // killing person 
        }
        else if (strcmp(msg, "?heal") == 0)
        {
            // healing someone
        }
        else if (strcmp(msg, "?help") == 0)
        {
            // list of commands and help... idk
        }
        else if (strcmp(msg, "?debug") == 0)
        {
            cout << "[Client]" << '[' << index << ']' << " Activated debug mode" << endl;
        }
        else
        {
            cout << "[Client]" << '[' << index << ']' << " Wrote undefined command." << endl;
        }
    }
    else
    {
        fireAllClients(msg, msg_size, index);
    }
}

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

        getCommand(msg, msg_size, index);
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