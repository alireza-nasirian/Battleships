#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

int x_size = 10, y_size = 10;

typedef struct Node{
    bool ship;      //true: when this node contains ship - false: this node doesn't contain ship
    char state;    // ' ':Unknown - 'w' = water - e = exploded - 'c' = completely  exploded
    int key;
    struct Node *next;
}node;

node *head1 = NULL;
node *head2 = NULL;

void insert(int key,bool ship,char state,node** head){
    node *link = (node*)malloc(sizeof(node));
    link->key = key;
    link->ship = ship;
    link->state = state;
    link->next = *head;

    *head = link;
}

void creat_map(node **head) {
    for (int i = 0; i < (x_size * y_size); i++) {
        insert(i+1, false, ' ',head);
    }
}

node* find(node **head,int y,int x) {
    int key = ((y-1)*10) + x;

    node* current = *head;

    if(*head == NULL) {
        return NULL;
    }
    while(current->key != key) {

        if(current->next == NULL) {
            return NULL;
        } else {
            current = current->next;
        }
    }
    return current;
}
void make_ship(int size, node *head) {
    int x1, x2, y1, y2;
    while (1) {
        scanf("%d %d-%d %d", &y1, &x1, &y2, &x2);
        int x = (x1 >= x2) ? x2 : x1;
        int y = (y1 >= y2) ? y2 : y1;

        if ((x1 > x_size) || (y1 > y_size) || (x2 > x_size) || (y2 > y_size) || ((x1 != x2) && (y1 != y2)) ||
            ((x1 == x2) && ((abs(y1 - y2) != size - 1) || (y > (y_size + 1) - size))) ||
            ((y1 == y2) && (abs(x1 - x2) != size - 1) || (x > (x_size + 1) - size))) {
            printf("Wrong input! try again.\n");
            continue;
        }
        bool state = true;

        if (y1 == y2) {
            for (int i = 0; i < size; i++) {
                node* test = find(&head,y,x);
                if (test->ship == true){
                    state = false;
                    break;
                }
                x++;
            }
            if (!state){
                printf("ships have Overlap! try again.");
                continue;
            }
             x = (x1 >= x2) ? x2 : x1;
             y = (y1 >= y2) ? y2 : y1;
            for (int j = 0; j < size; j++) {
                node* tmp = find(&head,y,x);
                tmp->ship = true;
                x++;
            }
        }

        if (x1 == x2) {
            for (int i = 0; i < size; i++) {
                node* test = find(&head,y,x);
                if (test->ship == true){
                    state = false;
                    break;
                }
                y++;
            }
            if (!state){
                printf("ships have Overlap! try again.");
                continue;
            }
             x = (x1 >= x2) ? x2 : x1;
             y = (y1 >= y2) ? y2 : y1;
            for (int j = 0; j < size; j++) {
                node* tmp = find(&head,y,x);
                tmp->ship = true;
                y++;
            }
        }
        break;
    }
}
void arrange_ships(node *head) {

    printf("Enter position of the first and last block of your 1*5 ship\trow column for example: 3 2-3 6\n");
    make_ship(5,head);

    for (int i = 0; i < 2; i++) {
        printf("Enter position of the first and last block of your %dth 1*3 ship\n", i + 1);
        make_ship(3,head);
    }
    for (int i = 0; i < 3; i++) {
        printf("Enter position of the first and last block of your %dth 1*2 ship\n", i + 1);
        make_ship(2,head);
    }
    for (int i = 0; i < 4; i++) {
        printf("Enter position of your %dth 1*1 ship\tfor example: 4 7\n", i + 1);
        int y,x;
        scanf("%d %d",&y,&x);
        node *tmp = find(&head, y, x);
        tmp->ship =true;
    }
}
void print_map(node* head) {
    node *ptr = head;
    printf("    ");
    for (int j = 0; j < x_size ; j++) {
        printf(" %2d  |", j + 1);
    }
    printf("\n");
    for (int i = 0; i < x_size; i++) {
        printf("------");
    }
    printf("---");
    for (int i = 0; i < y_size; i++) {
        printf("\n");
        printf("%2d |", i + 1);
        for (int j = 0; j < x_size; j++) {
            printf("  %c  |", ptr->state);
            ptr = ptr->next;
        }
        printf("\n");
        for (int j = 0; j < x_size; j++) {
            printf("------");
        }
        printf("---");
    }
}

