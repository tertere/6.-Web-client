#include <iostream>
//#include <errno.h>
#include <string>
#include <cstring>
#include <fstream>
#include "webclient.h"
#include "json.h"


using namespace std;

int main()
{
    /*string input;
    while (true){
    cin >> input;
    HostAddr addr(input);
    if (addr.typeConn == 1){
        GetHTTPSResponce(addr);
    } else GetHTTPResponce(addr);
}*/
    ifstream file;
    file.open("output.txt",ios::binary);
    char ch;
    string data;
    while (!file.eof()){
        file.get(ch);
        data += ch;
    }
    node_list list;
    ParseJson(list,GetPayload(data));
    file.close();
    return 0;
}
