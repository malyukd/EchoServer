#include <cstddef>



class StringHandler {
    char str[110];
    char protocol[10];
    char ip_a[18];
    char port[6];
    char volume_name[64];
    char* format[4]{protocol,ip_a,port,volume_name };
    const char* name[4]{ "protocol","ip_a", "port", "volume_name"};
    char symbol[4][4]{ "://", ":","/","" };
    
    bool valid = true;

    void divide(char* str, size_t size, int format);
    bool isProtocolValid();
    bool isIPValid();
    bool isPortValid();
    bool isVolumeNameValid();

public:
    StringHandler(char* str);
    bool isValid();
    char error[256];
    
}; 
