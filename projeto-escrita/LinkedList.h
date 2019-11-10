//
// Created by Gustavo Pergola on 08/11/19.
//

#ifndef PROJETO_ESCRITA_LINKEDLIST_H
#define PROJETO_ESCRITA_LINKEDLIST_H

class Node {
public:
    Node* next;
    float x;
    float y;
    float z;
    float type; // -1 pra S e 1 pra N
};

class LinkedList {
public:
    int length;
    Node* head;

    LinkedList();
    void add(float x, float y, float z, float type);
    void print();
    void send();
};
#endif //PROJETO_ESCRITA_LINKEDLIST_H
