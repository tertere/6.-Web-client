#include <iostream>
//#include <errno.h>
#include <string>
#include <cstring>
#include <fstream>
#include "webclient.h"


using namespace std;


int main()
{
    string input;

    while (true){
    cin >> input;
    HostAddr addr(input);



    if (addr.typeConn == 1){
        GetHTTPSResponce(addr);
    } else GetHTTPResponce(addr);
}
    return 0;
}
