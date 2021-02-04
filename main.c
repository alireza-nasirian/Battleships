#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int x_size = 10, y_size = 10;

typedef struct Node{
    bool ship;      //true: when this node contains ship - false: this node doesn't contain ship
    char state;    // ' ':Unknown - 'w' = water - e = exploded - 'c' = completely  exploded
    int x,y;
    struct Node *next;
}node;

node *head1 = NULL;
node *head2 = NULL;

void insert(int y,int x,bool ship,char state,node** head){
    node *link = (node*)malloc(sizeof(node));
    link->x = x;
    link->y = y;
    link->ship = ship;
    link->state = state;
    link->next = *head;

    *head = link;
}

void creat_map(node **head) {
    for (int i = 0; i < (x_size * y_size); i++) {
        insert((i / 10) + 1, (i % 10) + 1, false, ' ',head);
    }
}

node* find(node *head,int y,int x) {

    node* current = head;

    if(head == NULL) {
        return NULL;
    }
    while((current->y != y)&&(current->x != x)) {

        if(current->next == NULL) {
            return NULL;
        } else {
            current = current->next;
        }
    }
    return current;
}





