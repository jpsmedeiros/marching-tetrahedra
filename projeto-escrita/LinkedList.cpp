#include "LinkedList.h"
#include <iostream>
#include <mpi.h>

LinkedList::LinkedList(){
    this->length = 0;
    this->head = NULL;
}

void LinkedList::add(float x, float y, float z, float type){
    Node* node = new Node();
    node->x = x;
    node->y = y;
    node->z = z;
    node->type = type;
    node->next = this->head;
    this->head = node;
    this->length++;
}

void LinkedList::print(){
    Node* head = this->head;
    int i = 1;
    while(head){
        char type = 's';
        if (head->type > 0) type = 'n';
        std::cout << type << "," << head->x << ',' << head->y << ',' << head->z << std::endl;
        head = head->next;
        i++;
    }
}

void LinkedList::send(){
    Node* head = this->head;
    float vector[4];
    while(head){
        vector[0] = head->x;
        vector[1] = head->y;
        vector[2] = head->z;
        vector[3] = head->type;
        MPI_Send(&vector, 4, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
        head = head->next;
   }
}