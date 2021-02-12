#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

int x_size = 10, y_size = 10;
bool friend;

typedef struct Node {
    bool ship;      //true: when this node contains ship - false: this node doesn't contain ship
    char state;    // ' ':Unknown - 'w' = water - e = exploded - 'c' = completely  exploded
    int key;
    struct Node *next;
} node;

typedef struct Player {
    char name[20];
    int score;
    struct Player *next;
} player;

void insert_player(char name[20], int score, player **head) {
    player *link = (player *) malloc(sizeof(player));
    strcpy(link->name, name);
    link->score = score;
    link->next = *head;

    *head = link;
}

player *find_player(player **head, char name[20]) {
    player *current = *head;
    if (head == NULL) {
        return NULL;
    }
    while (strcmp(name, (*head)->name) != 0) {
        if (current->next == NULL) {
            return NULL;
        } else {
            current = current->next;
        }
    }
    return current;
}
void sortList(player *head) {
    player *current = head, *index = NULL;
    player *temp;

    if (head == NULL) {
        return;
    } else {
        while (current != NULL) {
            index = current->next;

            while (index != NULL) {
                if (current->score > index->score) {
                    temp = current;
                    current = index;
                    index = temp;
                }
                index = index->next;
            }
            current = current->next;
        }
    }
}

node *head1 = NULL;
node *head2 = NULL;
player *player_list = NULL;
player *player1 = NULL, *player2 = NULL;
int score1 = 0, score2 = 0, turn = 1;

void insert(int key, bool ship, char state, node **head) {
    node *link = (node *) malloc(sizeof(node));
    link->key = key;
    link->ship = ship;
    link->state = state;
    link->next = *head;

    *head = link;
}

void creat_map(node **head) {
    for (int i = 0; i < (x_size * y_size); i++) {
        insert((x_size * y_size) - i, false, ' ', head);
    }
}

node *find(node **head, int y, int x) {
    int key = ((y - 1) * x_size) + x;

    node *current = *head;

    if (*head == NULL) {
        return NULL;
    }
    while (current->key != key) {

        if (current->next == NULL) {
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
            ((y1 == y2) && ((abs(x1 - x2) != size - 1) || (x > (x_size + 1) - size)))) {
            printf("Wrong input! try again.\n");
            continue;
        }
        bool state = true;
        int Y = y - 1;
        int X = x - 1;

        if (y1 == y2) {
            for (int i = 0; i < 3; +i++) {
                if (Y == 0) {
                    Y++;
                    i++;
                }
                for (int j = 0; j < size + 2; j++) {
                    if (X == 0) {
                        X++;
                        j++;
                    }
                    node *test = find(&head, Y, X);
                    if (test->ship == true) {
                        state = false;
                        break;
                    }
                    X++;
                    if (X == 11) {
                        break;
                    }
                }
                Y++;
                if (Y == 11) {
                    break;
                }
                X = x - 1;
            }

            if (!state) {
                printf("ships have Overlap! try again.\n");
                continue;
            }
            for (int j = 0; j < size; j++) {
                node *tmp = find(&head, y, x);
                tmp->ship = true;
                x++;
            }
        }

        if (x1 == x2) {
            for (int i = 0; i < 3; i++) {
                if (X == 0) {
                    X++;
                    i++;
                }
                for (int j = 0; j < size; j++) {
                    if (Y == 0) {
                        Y++;
                        j++;
                    }
                    node *test = find(&head, Y, X);
                    if (test->ship == true) {
                        state = false;
                        break;
                    }
                    Y++;
                    if (Y == 11) {
                        break;
                    }
                }
                Y = y - 1;
                X++;
                if (X == 11) {
                    break;
                }
            }

            if (!state) {
                printf("ships have Overlap! try again.\n");
                continue;
            }
            x = (x1 >= x2) ? x2 : x1;
            y = (y1 >= y2) ? y2 : y1;
            for (int j = 0; j < size; j++) {
                node *tmp = find(&head, y, x);
                tmp->ship = true;
                y++;
            }
        }
        break;
    }
}

void arrange_ships(node *head) {

    printf("Enter position of the first and last block of your 1*5 ship\trow column for example: 3 2-3 6\n");
    make_ship(5, head);

    for (int i = 0; i < 2; i++) {
        printf("Enter position of the first and last block of your %dth 1*3 ship\n", i + 1);
        make_ship(3, head);
    }
    for (int i = 0; i < 3; i++) {
        printf("Enter position of the first and last block of your %dth 1*2 ship\n", i + 1);
        make_ship(2, head);
    }
    for (int i = 0; i < 4; i++) {
        printf("Enter position of your %dth 1*1 ship\tfor example: 4 7\n", i + 1);
        int y, x;
        while (1) {
            bool state = true;
            scanf("%d %d", &y, &x);
            int X = x - 1, Y = y - 1;
            for (int j = 0; j < 3; j++) {
                if (Y == 0) {
                    Y++;
                    j++;
                }
                for (int k = 0; k < 3; k++) {
                    if (X == 0) {
                        X++;
                        k++;
                    }
                    node *tmp = find(&head, Y, X);
                    if (tmp->ship == true) {
                        state = false;
                    }
                    X++;
                    if (X == 11) {
                        break;
                    }
                }
                Y++;
                X -= 3;
                if (Y == 11) {
                    break;
                }

            }
            if (!state) {
                printf("ships have Overlap! try again.\n");
                continue;
            }
            node *tmp = find(&head, y, x);
            tmp->ship = true;
            break;
        }
    }
}

void print_map(node *head) {

    node *tmp = head;

    printf("    ");
    for (int j = 0; j < x_size; j++) {
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
            printf("  %c  |", tmp->state);
            tmp = tmp->next;
        }
        printf("\n");
        for (int j = 0; j < x_size; j++) {
            printf("------");
        }
        printf("---");
    }
    printf("\n\n");

}

void auto_make_ship(node *head, FILE *map, int size) {
    int x1, x2, y1, y2;
    fscanf(map, "%d %d-%d %d", &y1, &x1, &y2, &x2);
    int x = (x1 >= x2) ? x2 : x1;
    int y = (y1 >= y2) ? y2 : y1;
    if (y1 == y2) {

        for (int j = 0; j < size; j++) {
            node *tmp = find(&head, y, x);
            tmp->ship = true;
            x++;
        }
    }
    if (x1 == x2) {
        for (int j = 0; j < size; j++) {
            node *tmp = find(&head, y, x);
            tmp->ship = true;
            y++;
        }
    }

}

void auto_arrange_ships(node *head) {
    FILE *map;

    int d = (rand() % 5) + 1;
    switch (d) {
        case 1:
            map = fopen("1.txt", "rb");
            break;
        case 2:
            map = fopen("2.txt", "rb");
            break;
        case 3:
            map = fopen("3.txt", "rb");
            break;
        case 4:
            map = fopen("4.txt", "rb");
            break;
        case 5:
            map = fopen("5.txt", "rb");
            break;
    }

    auto_make_ship(head, map, 5);
    for (int i = 0; i < 2; i++) {
        auto_make_ship(head, map, 3);
    }
    for (int i = 0; i < 3; i++) {
        auto_make_ship(head, map, 2);
    }
    for (int i = 0; i < 4; i++) {
        int x, y;
        fscanf(map, "%d %d", &y, &x);
        node *tmp = find(&head, y, x);
        tmp->ship = true;
    }
}

int find_ship(node *head, int y, int x) {
    bool up = false, down = false, right = false, left = false;
    if (y != y_size) {
        node *test = find(&head, y + 1, x);
        if (test->ship == 1) {
            down = true;
        }
    }
    if (y != 1) {
        node *test = find(&head, y - 1, x);
        if (test->ship == 1) {
            up = true;
        }
    }
    if (x != x_size) {
        node *test = find(&head, y, x + 1);
        if (test->ship == 1) {
            right = true;
        }
    }
    if (x != 1) {
        node *test = find(&head, y, x - 1);
        if (test->ship == 1) {
            left = true;
        }
    }
    if (left && right) {
        return 34;
    } else if (up && down) {
        return 12;
    } else if (up) {
        return 1;
    } else if (down) {
        return 2;
    } else if (left) {
        return 3;
    } else if (right) {
        return 4;
    } else {
        return 0;
    }

}

void W_maker(node *head, int y, int x) {

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int Y = (y - 1) + i, X = (x - 1) + j;
            if ((Y == y_size + 1) || (Y == 0) || (X == x_size + 1) || (X == 0)) {
                continue;
            }
            node *tmp = find(&head, Y, X);
            if (tmp->ship == false) {
                tmp->state = 'W';
            }
        }
    }
}

int count_lines() {
    char c;
    int count = 0;
    FILE *file = fopen("save_list.txt", "r");
    if (file == NULL) {
        printf("FILE open error");
        return 0;
    }
    for (c = getc(file); c != EOF; c = getc(file)) {
        if (c == '\n') {
            count = count + 1;
        }
    }
    fclose(file);
    return count;
}

void scan_line(int n, char file_name[20], char name1[20], char name2[20]) {
    int x;
    FILE *file = fopen("save_list.txt", "r");
    char line[256];
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        i++;
        if (i == n - 1) {
            fscanf(file, "%d) %s  %s: %d  %s: %d", &x, file_name, name1, &score1, name2, &score2);

        }
    }

    fclose(file);

}


void save(node *head, char FILE_name[20]) {
    FILE *save;
    save = fopen(FILE_name, "wb");
    if (save != NULL) {
        node *tmp = head;
        while (tmp != NULL) {
            fseek(save, 0, SEEK_END);
            fwrite(tmp, sizeof(node), 1, save);
            tmp = tmp->next;
        }
        fclose(save);
        save = NULL;
    } else {
        printf("FILE open error\n");
    }
}

int save_game() {
    int s;
    char s_name[20], *one, *two;
    scanf("%d", &s);
    if (s == -1) {
        printf("enter yor save name:\n");
        scanf(" %[^\n]", s_name);
        FILE *save_list = fopen("save_list.txt", "a");
        int lines = count_lines();
        if (friend) {
            fprintf(save_list, "%d) %s  %s: %d  %s: %d\n", lines + 1, s_name, player1->name, score1, player2->name,
                    score2);
        } else {
            fprintf(save_list, "%d) %s  %s: %d  Bot: %d\n", lines + 1, s_name, player1->name, score1, score2);
        }
        char s_name2[20];
        strcpy(s_name2,s_name);
        one = strcat(s_name, "1.bin");
        two = strcat(s_name2, "2.bin");
        save(head1, one);
        save(head2, two);
        fclose(save_list);

    }
    return s;
}


void save_player(player *head) {
    FILE *save;
    save = fopen("players.bin", "ab");
    if (save != NULL) {
        player *tmp = head;
        while (tmp != NULL) {
            fseek(save, 0, SEEK_END);
            fwrite(tmp, sizeof(player), 1, save);
            tmp = tmp->next;
        }
        fclose(save);
        save = NULL;
    } else {
        printf("FILE open error\n");
    }
}


void shoot(node *head, bool manual, int *score) {

    int y, x;

    if (!manual) {
        y = (rand() % y_size) + 1;
        x = (rand() % x_size) + 1;
    }
    if (manual) {
        printf("Enter the point you want to shoot    <row><column> ex:4 6\n");
        y = save_game();
       if (y == -1) {
            shoot(head, manual, score);
        }
        scanf(" %d", &x);
    }

    if ((x > x_size) || (y > y_size)) {
        printf("Wrong input. try again.\n");
        shoot(head, manual, score);
    }
    node *tmp = find(&head, y, x);
    if (tmp->state != ' ') {
        printf("Wrong input. choose empty point!\n");
        shoot(head, manual, score);
    }
    if (tmp->ship == false) {
        tmp->state = 'W';
        if (turn == 1) {
            turn = 2;
        } else {
            turn = 1;
        }
    }
    if (tmp->ship == true) {
        bool state = true;
        if (find_ship(head, y, x) == 0) {
            tmp->state = 'C';
            *score += 25;
            W_maker(head, y, x);
        }
        if (find_ship(head, y, x) == 1) {
            int Y = y, X = x;
            while (1) {
                Y--;
                if (Y == 0) {
                    break;
                }
                node *test = find(&head, Y, X);
                if (test->ship == false) {
                    break;
                }
                if (test->state == ' ') {
                    state = false;
                    break;
                }
                if (test->state == 'W') {
                    break;
                }
            }
            if (state) {
                int cnt = 0;
                while ((y != 0) && (tmp->state != 'W') && (tmp->ship)) {
                    tmp->state = 'C';
                    cnt++;
                    W_maker(head, y, x);
                    y--;
                    tmp = find(&head, y, x);
                }
                switch (cnt) {
                    case 2:
                        *score += 13;
                        break;
                    case 3:
                        *score += 9;
                        break;
                    case 5:
                        *score += 6;
                        break;
                }
            } else {
                tmp->state = 'E';
                *score += 1;
            }
        }
        if (find_ship(head, y, x) == 2) {
            int Y = y, X = x;
            while (1) {
                Y++;
                if (Y == y_size + 1) {
                    break;
                }
                node *test = find(&head, Y, X);
                if (test->ship == false) {
                    break;
                }
                if (test->state == ' ') {
                    state = false;
                    break;
                }
                if (test->state == 'W') {
                    break;
                }

            }
            if (state) {
                int cnt = 0;
                while ((y != y_size) && (tmp->state != 'W') && (tmp->ship)) {
                    tmp->state = 'C';
                    cnt++;
                    W_maker(head, y, x);
                    y++;
                    tmp = find(&head, y, x);
                }
                switch (cnt) {
                    case 2:
                        *score += 13;
                        break;
                    case 3:
                        *score += 9;
                        break;
                    case 5:
                        *score += 6;
                        break;
                }
            } else {
                tmp->state = 'E';
                *score += 1;
            }
        }
        if (find_ship(head, y, x) == 3) {
            int Y = y, X = x;
            while (1) {
                X--;
                if (X == 0) {
                    break;
                }
                node *test = find(&head, Y, X);
                if (test->ship == false) {
                    break;
                }
                if (test->state == ' ') {
                    state = false;
                    break;
                }
                if (test->state == 'W') {
                    break;
                }
            }
            if (state) {
                int cnt = 0;
                while ((x != 0) && (tmp->state != 'W') && (tmp->ship)) {
                    tmp->state = 'C';
                    cnt++;
                    W_maker(head, y, x);
                    x--;
                    tmp = find(&head, y, x);
                }
                switch (cnt) {
                    case 2:
                        *score += 13;
                        break;
                    case 3:
                        *score += 9;
                        break;
                    case 5:
                        *score += 6;
                        break;
                }
            } else {
                tmp->state = 'E';
                *score += 1;
            }
        }
        if (find_ship(head, y, x) == 4) {
            int Y = y, X = x;
            while (1) {
                X++;
                if (X == x_size + 1) {
                    break;
                }
                node *test = find(&head, Y, X);
                if (test->ship == false) {
                    break;
                }
                if (test->state == ' ') {
                    state = false;
                    break;
                }
                if (test->state == 'W') {
                    break;
                }
            }
            if (state) {
                int cnt = 0;
                while ((x != x_size) && (tmp->state != 'W') && (tmp->ship)) {
                    tmp->state = 'C';
                    cnt++;
                    W_maker(head, y, x);
                    x++;
                    tmp = find(&head, y, x);
                }
                switch (cnt) {
                    case 2:
                        *score += 13;
                        break;
                    case 3:
                        *score += 9;
                        break;
                    case 5:
                        *score += 6;
                        break;
                }
            } else {
                tmp->state = 'E';
                *score += 1;
            }

        }
        if (find_ship(head, y, x) == 34) {
            int Y = y, X = x;
            while (1) {
                X++;
                if (X == x_size + 1) {
                    break;
                }
                node *test = find(&head, Y, X);
                if (test->ship == false) {
                    break;
                }
                if (test->state == ' ') {
                    state = false;
                    break;
                }
                if (test->state == 'W') {
                    break;
                }
            }

            Y = y, X = x;
            while (1) {
                X--;
                if (X == 0) {
                    break;
                }
                node *test = find(&head, Y, X);
                if (test->ship == false) {
                    break;
                }
                if (test->state == ' ') {
                    state = false;
                    break;
                }
                if (test->state == 'W') {
                    break;
                }
            }
            if (state) {
                int cnt = 0;
                while ((x != 0) && (tmp->state != 'W') && (tmp->ship)) {
                    tmp->state = 'C';
                    cnt++;
                    W_maker(head, y, x);
                    x--;
                    tmp = find(&head, y, x);
                }
                x += cnt + 1;
                while ((x != x_size) && (tmp->state != 'W') && (tmp->ship)) {
                    tmp->state = 'C';
                    cnt++;
                    W_maker(head, y, x);
                    x++;
                    tmp = find(&head, y, x);
                }
                switch (cnt) {
                    case 2:
                        *score += 13;
                        break;
                    case 3:
                        *score += 9;
                        break;
                    case 5:
                        *score += 6;
                        break;
                }
            } else {
                tmp->state = 'E';
                *score += 1;
            }
        }
        if (find_ship(head, y, x) == 12) {
            int Y = y, X = x;
            while (1) {
                Y++;
                if (Y == y_size + 1) {
                    break;
                }
                node *test = find(&head, Y, X);
                if (test->ship == false) {
                    break;
                }
                if (test->state == ' ') {
                    state = false;
                    break;
                }
                if (test->state == 'W') {
                    break;
                }
            }

            Y = y, X = x;
            while (1) {
                Y--;
                if (Y == 0) {
                    break;
                }
                node *test = find(&head, Y, X);
                if (test->ship == false) {
                    break;
                }
                if (test->state == ' ') {
                    state = false;
                    break;
                }
                if (test->state == 'W') {
                    break;
                }
            }
            if (state) {
                int cnt = 0;
                while ((y != 0) && (tmp->state != 'W') && (tmp->ship)) {
                    tmp->state = 'C';
                    cnt++;
                    W_maker(head, y, x);
                    y--;
                    tmp = find(&head, y, x);
                }
                y += cnt + 1;
                while ((y != x_size) && (tmp->state != 'W') && (tmp->ship)) {
                    tmp->state = 'C';
                    cnt++;
                    W_maker(head, y, x);
                    y++;
                    tmp = find(&head, y, x);
                }
                switch (cnt) {
                    case 2:
                        *score += 13;
                        break;
                    case 3:
                        *score += 9;
                        break;
                    case 5:
                        *score += 6;
                        break;
                }
            } else {
                tmp->state = 'E';
                *score += 1;
            }
        }
    }
}

node *read_FILE(node *head, FILE *load) {

    if (head == NULL) {
        head = (node *) malloc(sizeof(node));
        fread(head, sizeof(node), 1, load);
        head->next = NULL;
    } else {
        node *tmp = head;
        node *new = (node *) malloc(sizeof(node));
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        fread(new, sizeof(node), 1, load);
        tmp->next = new;
        new->next = NULL;
    }
    return head;
}

node *load(node *head, char *FILE_name) {
    FILE *load;
    load = fopen(FILE_name, "rb");
    if (load != NULL) {
        head = NULL;
        fseek(load, 0, SEEK_END);
        long FILE_size = ftell(load);
        rewind(load);
        int num = (int) (FILE_size / (sizeof(node)));

        for (int i = 0; i < num; i++) {
            fseek(load, (int) sizeof(node) * i, SEEK_SET);
            head = read_FILE(head, load);
        }
    } else {
        printf("FILE open error\n");
    }
    return head;
}

player *read_player(player *head) {
    FILE *load = fopen("players.bin", "rb");
    if (head == NULL) {
        head = (player *) malloc(sizeof(player));
        fread(head, sizeof(player), 1, load);
        head->next = NULL;
    } else {
        player *tmp = head;
        player *new = (player *) malloc(sizeof(player));
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        fread(new, sizeof(player), 1, load);
        tmp->next = new;
        new->next = NULL;
    }
    return head;

}

player *load_player(player *head) {
    FILE *load = fopen("players.bin", "rb");
    if (load != NULL) {
        head = NULL;
        fseek(load, 0, SEEK_END);
        long FILE_size = ftell(load);
        rewind(load);
        int num = (int) (FILE_size / (sizeof(player)));
        for (int i = 0; i < num; i++) {
            fseek(load, (int) sizeof(player) * i, SEEK_SET);
            head = read_player(head);
        }
    } else {
        printf("FILE open error\n");
    }
    return head;
}

