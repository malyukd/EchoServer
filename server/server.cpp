#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "TCPacceptor.h"
#include "StringHandler.h"

int main(int argc, char** argv)
{
    if (argc < 2 || argc > 4) {
        printf("usage: server <port> [<ip>]\n");
        exit(1);
    }
    TCPStream* stream = NULL;
    TCPAcceptor* acceptor = NULL;
    if (argc == 3) {
        acceptor = new TCPAcceptor(atoi(argv[1]), argv[2]);
    }
    else {
        acceptor = new TCPAcceptor(atoi(argv[1]));
    }
    bool program = true;
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
                        StringHandler str(line);
                        if(str.isValid()){
                            stream->send("Success", len);
                            printf("Success");
                        }else{
                            stream->send(str.error,sizeof(str.error));
                            printf("sent - %s\n",  str.error);
                        }
                    }
                    
                }
                delete stream;
            }
        }
    }
    exit(0);
}