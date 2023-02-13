#include <iostream>
//#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <fstream>
#include <openssl/ssl.h>
#include <openssl/err.h>

using namespace std;

class HostAddr{
public:
    HostAddr(string input){
        string _hoststring, temp;
        bool isUri = false;
        for (int i = 0;  i < input.size(); i++){
            if (input[i] == ':'){
                if (temp == "http") typeConn = 0;
                if (temp == "https") typeConn = 1;
                temp = "";
                _hoststring = "";
                i = i + 2;
                continue;
            }
            if ((input[i] == '/') && (isUri == false)) {
                isUri = true;
                url = temp;
                temp = "";
            }
            if (!isUri) _hoststring += input[i];
            temp += input[i];
        }
        if (isUri) uri = temp; else url = temp;
        if (uri == "") uri = "/";
        hoststring = _hoststring;
    }
    const char* cstr(){
        return hoststring.c_str();
    }
    string GetRequest() {
        string temp = "GET ";
        temp += uri;
        temp += " HTTP/1.1\r\n";
        temp += "Host: ";
        temp += url;
        temp += "\r\nConnection: close\r\n\r\n";
        return temp;
    }
    string hoststring;
    string url, uri;
    char* hostctr;
    int typeConn;
};

void GetHTTPResponce(HostAddr addr){
    int sock,iRes;
    struct addrinfo hints;
    struct addrinfo *result;

    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    iRes = getaddrinfo(addr.cstr(),"80",&hints,&result);
    if (iRes != 0){
        cout << "getaddrinfo() error: " << iRes << '\n';
        return;
    }

    sock = socket(AF_INET,SOCK_STREAM,0);
    if (sock < 0){
        perror("socket");
        exit(1);
    }

    iRes = connect(sock,result->ai_addr,result->ai_addrlen);
    if (iRes < 0){
        cout << "Connection error code: " << iRes <<'\n';
    }

    string req = addr.GetRequest();
    int len = req.size();
    char buf[1024];

    iRes=send(sock,req.c_str(),len,0);
    cout << "Bytes sended: " << iRes << '\n';
    if (iRes == -1){
        cout << "Send error: " << iRes << '\n';
    }

    ofstream file;
    file.open("output.txt",ios::binary);
    int TotalRecv = 0;
    do {
        iRes = recv(sock, buf, sizeof(buf),0);
        TotalRecv += iRes;
        /*if (iRes == 1024){
            file << buf;
        } else for (int i = 0; i <= iRes; i++) file << buf[i];*/
        for (int i = 0; i < iRes; i++) file << buf[i];
        //cout << buf;
        cout << "Bytes recieved: " << iRes << '\n';
    } while (iRes > 0);

    cout << "Total bytes recieved: " << TotalRecv << '\n';
    file.close();
    close(sock);
}

void GetHTTPSResponce(HostAddr addr){
    int sock,iRes;
    struct addrinfo hints;
    struct addrinfo *result;

    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    iRes = getaddrinfo(addr.cstr(),"443",&hints,&result);
    if (iRes != 0){
        cout << "getaddrinfo() error: " << iRes << '\n';
        return;
    }

    sock = socket(AF_INET,SOCK_STREAM,0);
    if (sock < 0){
        perror("socket");
        exit(1);
    }

    iRes = connect(sock,result->ai_addr,result->ai_addrlen);
    if (iRes < 0){
        cout << "Connection error code: " << iRes <<'\n';
    }
    const SSL_METHOD *method = TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (ctx == nullptr)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    SSL *ssl = SSL_new(ctx);
    if (ssl == nullptr)
    {
        fprintf(stderr, "SSL_new() failed\n");
        exit(EXIT_FAILURE);
    }
    SSL_set_fd(ssl,sock);
    iRes = SSL_connect(ssl);
    if (iRes != 1)
    {
        SSL_get_error(ssl, iRes);
        ERR_print_errors_fp(stderr); //High probability this doesn't do anything
        fprintf(stderr, "SSL_connect failed with SSL_get_error code %d\n", iRes);
        exit(EXIT_FAILURE);
    }
    string req = addr.GetRequest();
    SSL_write(ssl,req.c_str(),req.size());

    char buf[1024];
    ofstream file;
    file.open("output.txt",ios::binary);
    int TotalRecv = 0;
    do {
        iRes = SSL_read(ssl,buf,sizeof(buf));
        TotalRecv += iRes;
        if (iRes == 1024){
            file << buf;
        } else for (int i = 0; i <= iRes; i++) file << buf[i];

        //cout << buf;
        cout << "Bytes recieved: " << iRes << '\n';
    } while (iRes > 0);
    cout << "Total bytes recieved: " << TotalRecv << '\n';
    file.close();
    SSL_free(ssl);
    close(sock);
    SSL_CTX_free(ctx);
}

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
