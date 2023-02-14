#include "json.h"

#include <iostream>

using namespace std;

node_list::node_list(){
    currentnode = new node;
}

void node_list::push_back(string data){
    currentnode->data = data;
}

void node_list::next_node(){
    node* next = new node;
    next->uppernode = currentnode->uppernode;
    currentnode->nextnode = next;
    next->prevnode = currentnode;
    currentnode = next;
}

void node_list::deeper(){
    node* deep = new node;
    currentnode->deepernode = deep;
    deep->uppernode = currentnode;
    currentnode = currentnode->deepernode;
}

void node_list::upper(){
    currentnode = currentnode->uppernode;
}

void ParseJson(node_list list, string data){
    int isArray = 0;
    string temp = "";
    int isString = 0;
    int isValue = 0;
    int level = 0;
    for (int i = 0; i < data.size(); i++){
        if ((data[i] == ' ')&&(!isString)){
            continue;
        }
        if ((data[i] == '\n')&&(!isString)){
            continue;
        }
        if ((data[i] == '\t')&&(!isString)){
            continue;
        }
        if ((data[i] == '{')&&(!isString)){
            continue;
        }
        if ((data[i] == '"')&&(!isString)){
            isString = true;
            continue;
        }
        if ((data[i] == '"')&&(isString)){
            isString = false;
            continue;
        }
        if ((data[i] == ':')&&(!isString)){
            list.push_back(temp);
            cout << "level: " << level << " string: " << temp << '\n';
            temp = "";
            list.deeper();
            level++;
            isValue = 1;
            continue;
        }
        if (data[i] == '}'){
            if (temp != ""){
                list.push_back(temp);
                cout << "level: " << level << " string: " << temp << '\n';
                temp = "";
            }
            list.upper();
            level--;
            if (isValue) isValue = 0;
            continue;
        }
        if (data[i] == ','){
            if (isArray > 0) {
                list.push_back(temp);
                cout << "level: " << level << " string: " << temp << '\n';
                temp = "";
                list.next_node();
                continue;
            }
            if (isValue) {
                list.push_back(temp);
                cout << "level: " << level << " string: " << temp << '\n';
                isValue = 0;
            }
            list.upper();
            level--;
            temp = "";
            isValue = 0;
            list.next_node();
            continue;
        }
        if (data[i] == '['){
            isArray++;
            if (isArray > 1) {
                list.deeper();
                level++;
            }
            continue;
        }
        if (data[i] == ']'){
            isArray--;
            if (isArray > 0){
                list.push_back(temp);
                cout << "level: " << level << " string: " << temp << '\n';
                temp = "";
                list.upper();
                level--;
            }
            continue;
        }
        temp += data[i];
    }
    cout << "end";
}

string GetPayload(string input){
    string temp;
    bool payload = false;
    node entrypoint;
    for (int i = 0; i < input.size(); i++){
        if (input[i] == '{') payload = true;
        if (payload) temp += input[i];
    }
    //cout << temp;
    return temp;
}
