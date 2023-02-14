#ifndef JSON_H
#define JSON_H
#include <string>

using namespace std;

struct node{
    string data = "";
    node* deepernode = NULL;
    node* nextnode = NULL;
    node* prevnode = NULL;
    node* uppernode = NULL;
};

class node_list{
public:
    node_list();
    node* currentnode = NULL;
    node* deepernode = NULL;
    node* nextnode = NULL;
    node* prevnode = NULL;
    node* uppernode = NULL;

    void push_back(string data);

    void next_node();

    void deeper();

    void upper();
};

void ParseJson(node_list list, string data);

string GetPayload(string input);

#endif // JSON_H
