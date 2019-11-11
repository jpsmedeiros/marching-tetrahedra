#include "LinkedList.h"
#include <iostream>
#include <mpi.h>

LinkedList::LinkedList(){
    this->length = 0;
    this->head = NULL;
    this->tail = NULL;
}

void LinkedList::add(float x, float y, float z, float type){
    Node* node = new Node();
    node->x = x;
    node->y = y;
    node->z = z;
    node->type = type;
    if (this->head == NULL) {
        this->head = node;
    } else {
        this->tail->next = node;
    }
    node->next = NULL;
    this->tail = node;
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

void LinkedList::send(int chunk_size, int comm_array_size){
    Node* head = this->head;
    float vector[chunk_size][comm_array_size];
    int i = 0;
    while(head){
        vector[i][0] = head->x;
        vector[i][1] = head->y;
        vector[i][2] = head->z;
        vector[i][3] = head->type;
        if (i + 1 == chunk_size || !head->next) {
            MPI_Send(&vector, chunk_size * comm_array_size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
            i = -1;
        }
        head = head->next;
        i++;
   }
}