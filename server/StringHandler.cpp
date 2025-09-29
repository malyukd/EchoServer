#include "StringHandler.h"
#include <cstring> 
#include <iostream>
#include <cstdio> 

StringHandler::StringHandler(char* str) {
    strcpy(this->str, str);
    try {
        divide(this->str, sizeof(this->str), 0);
    }
    catch (std::exception ex) {
        sprintf(this->error, "Request is invalid. Invalid format");
        valid = false;
    }
}

void StringHandler::divide(char* str, size_t size, int format) {
    char* ind = str;
    if (format == 3) {
        strcpy(this->volume_name, ind);
        return;
    }
    char* p = strstr(ind, this->symbol[format]);
    if (p == nullptr || strlen(ind) <= strlen(this->symbol[format])) {
        throw std::invalid_argument("error");
    }
    int i = 0;
    while (ind != p && this->format[format]+1)
        *(this->format[format]++) = *(ind++);
    *(this->format[format]) = '\0';
    ind += strlen(this->symbol[format]);
    divide(ind, size, format+1);
}

bool StringHandler::isProtocolValid() {
    if (!strcmp(this->protocol, "tcp") || !strcmp(this->protocol, "iscsi") || !strcmp(this->protocol, "nbd"))
        return true;
    return false;
}

bool StringHandler::isIPValid() {
    char ip[4][4];
    int ind=0;
    char buffer[4];
    for (int i = 0; i < 4; i++) {
        int count = 0;
        while (this->ip_a[ind] && this->ip_a[ind] != '.') {
            if(count>2)
                return false;
            buffer[count++] = this->ip_a[ind++];
        }
        buffer[count] = '\0';
        strcpy(ip[i], buffer);
        ind++;
    }
    for (int i = 0; i < 4; i++) {
            if (atoi(ip[i]) < 0 || atoi(ip[i]) > 255)
                return false;
            if (ip[i][0] != '0' && atoi(ip[i]) == 0)
                return false;
    }
    return true;
}

bool StringHandler::isPortValid() {
    int i = atoi(this->port);
    if (this->port[0] == '0' || i > 0 && i < 65536)
        return true;
    return false;
}

bool StringHandler::isVolumeNameValid() {
    if (strlen(this->volume_name)==0)
        return false;
    for (int i = 0; i < strlen(this->volume_name); i++) {
        char tmp = this->volume_name[i];
        if (!(tmp >= '0' && tmp <= '9' || tmp>='a' && tmp <= 'z' || tmp>='A' && tmp <= 'Z' || tmp == '_'))
            return false;
    }
    return true;
}

bool StringHandler::isValid() {
    if (!this->valid)
        return false;
    if (!isProtocolValid()) {
        sprintf(this->error, "Request is invalid. Wrong parameter %s: %s", name[0], this->protocol);
        return false;
    }
    if (!isIPValid()) {
        sprintf(this->error, "Request is invalid. Wrong parameter %s: %s", name[1], this->ip_a);
        return false;
    }
    if (!isPortValid()) {
        sprintf(this->error, "Request is invalid. Wrong parameter %s: %s", name[2], this->port);
        return false;
    }
    if (!isVolumeNameValid()) {
        sprintf(this->error, "Request is invalid. Wrong parameter %s: %s", name[3], this->volume_name);
        return false;
    }
    return true;
}

