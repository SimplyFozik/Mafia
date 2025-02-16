#include <iostream> 
#include <winsock2.h> 
#include <ws2tcpip.h> 

#pragma warning(disable: 4996)
#pragma comment(lib, "ws2_32.lib") 

int main() {
    WSADATA wsaData;
    char hostname[1024];
    struct hostent* host_entry;
    char* ip;

    // Initialize Winsock 
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // Get the local hostname 
    gethostname(hostname, sizeof(hostname));
    host_entry = gethostbyname(hostname);

    if (host_entry == nullptr) {
        std::cerr << "gethostbyname failed." << std::endl;
        WSACleanup();
        return 1;
    }

    // Convert the first IP address to a readable format 
    ip = inet_ntoa(*(struct in_addr*)*host_entry->h_addr_list);
    std::cout << "Local IP Address: " << ip << std::endl;

    WSACleanup();
    return 0;
}
