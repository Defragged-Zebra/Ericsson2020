#include <iostream>
#include "antiVirus.h"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 11223

void debug() {
    u_long factors[4] = {1569741360, 1785505948, 516548029, 1302116447};
    Random rd = Random(factors);
    size_t i = 0;
    int index = 2;
    do {
        std::cout << "i= " << i << " \tfactor2: " << rd.getFactor(2) << "    \tfactor3: " << rd.getFactor(3)
                  << "   \tfactor4: " << rd.getFactor(4) << std::endl;
        rd.next(2);
        rd.next(3);
        rd.next(4);
        ++i;
    } while (rd.getFactor(2) != 6014329);
    std::cout << "i= " << i << " \tfactor2: " << rd.getFactor(2) << "    \tfactor3: " << rd.getFactor(3)
              << " \tfactor4: " << rd.getFactor(4) << std::endl;
    //std::cout << "i= " << i << " factor: " << rd.getFactor(index) << std::endl;
    /*do  {
        ++i;
    } while (rd.next(index) != 413011888); //6014329);
    std::cout << "i= " << i << std::endl;*/

}


void socketConnect() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "ericsson.dyndns.org", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
    }

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
    }
    send(sock, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);
};


int main() {
    //socketConnect();
    //debug();
    //return 0;
    std::stringstream ss;
    ss << "START 1 44 1" << std::endl;
    ss << "FACTORS 1569741360 1785505948 516548029 1302116447" << std::endl;
    ss << "FIELDS 6 4" << std::endl;
    ss << "FD 0 0 1 0 5" << std::endl;
    ss << "FD 0 1 1 1 1" << std::endl;
    ss << "FD 0 2 1 0 5" << std::endl;
    ss << "FD 0 3 3 0 5" << std::endl;
    ss << "FD 1 0 0 0 5" << std::endl;
    ss << "FD 1 1 1 0 4" << std::endl;
    ss << "FD 1 2 3 2 1" << std::endl;
    ss << "FD 1 3 3 0 2" << std::endl;
    ss << "FD 2 0 0 1 1" << std::endl;
    ss << "FD 2 1 2 0 5" << std::endl;
    ss << "FD 2 2 3 0 5" << std::endl;
    ss << "FD 2 3 5 0 5" << std::endl;
    ss << "FD 3 0 0 0 2" << std::endl;
    ss << "FD 3 1 2 1 1" << std::endl;
    ss << "FD 3 2 2 0 4" << std::endl;
    ss << "FD 3 3 5 2 1" << std::endl;
    ss << "FD 4 0 0 0 5" << std::endl;
    ss << "FD 4 1 2 0 5" << std::endl;
    ss << "FD 4 2 5 0 4" << std::endl;
    ss << "FD 4 3 5 0 3" << std::endl;
    ss << "FD 5 0 4 0 5" << std::endl;
    ss << "FD 5 1 4 1 1" << std::endl;
    ss << "FD 5 2 4 0 2" << std::endl;
    ss << "FD 5 3 4 0 5" << std::endl;
    ss << "." << std::endl;
    for (int i = 0; i < 44; ++i) {
        ss << "REQ 1 " << i << " 0" << std::endl;
        ss << "." << std::endl;
    }


    u_long factors[4] = {2, 3, 4, 6};
    AntiVirus av = AntiVirus(new Protocol(ss, std::cout, std::cerr));
    //int maxTicks=300;
    //AntiVirus av = AntiVirus(25, 25, 0, maxTicks, factors, new Graphics());
    av.startInterface();
    //std::cout << av;
//    for (int i = 0; i < maxTicks; ++i) {
//        //district clear/not clear checker function is not created yet
//        av.play1Tick();
//        av.updateInterface();
//    }

    return 0;
}