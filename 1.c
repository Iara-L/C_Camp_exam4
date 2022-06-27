#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define epsilon 0.0001
#define ITEMS_COUNT 5

typedef struct Item{
    uint16_t number;
    char description[251];
    double weight;
    double price;
} Item;

typedef struct node{
    Item item;
    struct node* next;
} node_t;

double rand_double(double min, double max);
uint16_t rand_uint(uint16_t min, uint16_t max);
char* rand_desc(char* name);

node_t* create_item();
void push_end(node_t* first, node_t* item);
void print(node_t* first);

double total_items_weight(node_t* first);
node_t *min_price_item(node_t* first);

int main(){
    srand(time(NULL));

    node_t* first = create_item();

    for (int i = 0; i < ITEMS_COUNT - 1; i++){
        push_end(first, create_item());
    }

    node_t* min = min_price_item(first);

    print(first);
    printf("Total weight: %lf\n", total_items_weight(first));
    printf("Min price: %.3lf\n", min->item.price);

    return 0;
}

double rand_double(double min, double max){
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

uint16_t rand_uint(uint16_t min, uint16_t max){
    return min + rand() % (max - min + 1);
}

char* rand_desc(char* desc){
    int index = 0;

    desc[index++] = rand_uint('A', 'Z');

    for(int i = 1; i < rand_uint(5, 10); i++){
        for(int i = 1; i < rand_uint(1, 10); i++){
            desc[index++] = rand_uint('a', 'z');
        }
        desc[index++] = ' ';
    }

    desc[index++] = '\0';
}

node_t* create_item(){
    node_t* current = malloc(sizeof(node_t));

    current->item.number = rand_uint(5, 2000);
    rand_desc(current->item.description);
    current->item.weight = rand_double(0.100, 100.00);
    current->item.price = rand_double(1.00, 1500.00);

    return current;
}

void push_end(node_t* first, node_t* item){
    node_t* current = first;

    while (current->next){
        current = current->next;
    }
    current->next = item;
}

void print(node_t* first){
    node_t* current = first;

    while (current){
        printf("%d\t", current->item.number);
        printf("%220s\t", current->item.description);
        printf("%10.3lf\t", current->item.weight);
        printf("%10.3lf\n", current->item.price);

        current = current->next;
    }
}

double total_items_weight(node_t* first){
    node_t* current = first;

    double weight_sum = 0;

    if(!current){
        perror("-1");
    }

    while(current){
        weight_sum += current->item.weight;
        current = current->next;
    }
    return weight_sum;
}

node_t *min_price_item(node_t* first){
    node_t* current = first;
    node_t* min = current;

    if(!current){
        return NULL;
    }

    current = current->next;

    while(current->next){
        if(fabs(current->item.price - min->item.price) < epsilon){
            return 0;
        } else if(current->item.price < min->item.price){
            min->item.price = current->item.price;
        }
        current = current->next;
    }
    return min;
}
