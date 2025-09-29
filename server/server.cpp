#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "TCPacceptor.h"
#include <thread>
#include <iostream>
#include "MetricsCollector.h"

MetricsCollector g_metrics;
bool program = true;

void console_input() {
    std::string line;
    while (program) {  
        if (std::getline(std::cin, line)) {
            if (line == "metrics") {
                  auto m = g_metrics.get_metrics();
            std::cout << "RPS: " << m.rps
                << ", MB/s: " << m.mbps
                << ", Total: " << m.total_requests << "\n";
            } else {
                std::cout << "Ты ввёл: " << line << "\n";
            }
        }
    }
}

int main(int argc, char** argv)
{
    if (argc < 2 || argc > 4) {
        printf("usage: server <port> [<ip>]\n");
        exit(1);
    }
    std::thread t(console_input); 
    TCPStream* stream = NULL;
    TCPAcceptor* acceptor = NULL;
    if (argc == 3) {
        acceptor = new TCPAcceptor(atoi(argv[1]), argv[2]);
    }
    else {
        acceptor = new TCPAcceptor(atoi(argv[1]));
    }
    
    if (acceptor->start() == 0) {
        while (program) {
            stream = acceptor->accept();
            if (stream != NULL) {
                ssize_t len;
                char line[256];
                while ((len = stream->receive(line, sizeof(line))) > 0) {
                    line[len] = 0;
                    
                    printf("received - %s\n", line);
                    if(!strcmp(line, "exit")){
                        program = false;
                        break;
                    }else{
                        stream->send(line, len);
                        g_metrics.record_request(len);
                        printf("send - %s\n", line);

                    }
                    
                }
                delete stream;
            }
        }
    }
    t.join();
    exit(0);
}