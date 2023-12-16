#pragma comment(lib, "ws2_32.lib")

#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>
#include<string>
#include<thread>
#include<vector>
#include<mutex>
#include<fstream>

using namespace std;
mutex mtx;
ofstream file;

void scan_port(const string& targetIp, int port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup Fail: " << WSAGetLastError() << endl;
        return;
    }

    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
        cerr << "Socket Creating Fail: " << WSAGetLastError() << endl;
        return;
    }

    sockaddr_in serv_addr;
    ZeroMemory((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, targetIp.c_str(), &(serv_addr.sin_addr)) <= 0) {
        cerr << "IP Addr Setting Fail: " << WSAGetLastError() << endl;
        return;
    }

    if (connect(sockfd, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        mtx.lock();
        cout << "Port " << port << ": Closed" << endl;
        mtx.unlock();
    }
    else {
        mtx.lock();
        cout << "Port " << port << ": Open" << endl;
        if (file.is_open()) {
            file << "Port " << port << ": Open" << endl;
        }
        mtx.unlock();
    }

    closesocket(sockfd);
    WSACleanup();
}

int main(int argc, char* argv[]) {
    string targetIp;
    int startPort = 1;
    int endPort = 65535;

    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "-targetIp") {
            if (i + 1 < argc) {
                targetIp = argv[++i];
            }
        }
        else if (string(argv[i]) == "-port") {
            if (i + 2 < argc) {
                startPort = stoi(argv[++i]);
                endPort = stoi(argv[++i]);
            }
        }
        else if (string(argv[i]) == "-save") {
            if (i + 1 < argc) {
                file.open(argv[++i], ios::out);
            }
        }
    }

    vector<thread> threads;
    for (int port = startPort; port <= endPort; ++port) {
        threads.emplace_back(scan_port, targetIp, port);

        if (threads.size() >= 30 || port == endPort) {
            for (auto& thread : threads) {
                thread.join();
            }
            threads.clear();
        }
    }

    if (file.is_open()) {
        file.close();
    }

    return 0;
}