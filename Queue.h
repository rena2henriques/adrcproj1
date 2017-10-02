#ifndef QUEUE
#define QUEUE

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

struct Queue {
    int info;
    struct Queue *ptr;
};

//  *front,*rear,*temp,*front1

void enq(int data, struct Queue *front, struct Queue *rear);
void display(struct Queue *front, struct Queue *rear);
void deq(struct Queue *front, struct Queue *rear);
int frontelement(struct Queue *front, struct Queue *rear);
void empty(struct Queue *front, struct Queue *rear);

#endif