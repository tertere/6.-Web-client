#ifndef WEBCLIENT_H
#define WEBCLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
#include <fstream>


using namespace std;

//The class handles host addresses like "http://host.domain/uri" for further use
class HostAddr{
public:
    //Constructor where "input" its web adress
    HostAddr(string input);

    //Web adress without protocol prefix
    string hoststring;

    //Separate URL and URI
    string url, uri;

    //Pointer to char for sockets functions
    char* hostctr;

    //Typeconn = 1 its secure type, 0 its unsecured type
    int typeConn;

    //Function give char pointer for sockets functions
    const char* cstr();

    //Funtion give string-typed GET HTTP request
    string GetRequest();
};

//Unsequred Connect, send request and get responce to "output.txt" file
void GetHTTPResponce(HostAddr addr);

//Secured Connect, send request and get responce to "output.txt" file
void GetHTTPSResponce(HostAddr addr);

#endif // WEBCLIENT_H
