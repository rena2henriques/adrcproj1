#include "Queue.h"


/* Enqueing the queue */
void enq(int data, struct Queue *front, struct Queue *rear ) {

	struct Queue *temp = NULL;

    if (rear == NULL)
    {
        rear = (struct Queue *)malloc(1*sizeof(struct Queue));
        rear->ptr = NULL;
        rear->info = data;
        front = rear;
    }
    else
    {
        temp=(struct Queue *)malloc(1*sizeof(struct Queue));
        rear->ptr = temp;
        temp->info = data;
        temp->ptr = NULL;
 
        rear = temp;
    }
}
 
/* Displaying the queue elements */
void display(struct Queue *front, struct Queue *rear) {

    struct Queue *front1 = front;
 
    if ((front1 == NULL) && (rear == NULL))
    {
        printf("Queue is empty");
        return;
    }
    while (front1 != rear)
    {
        printf("%d", front1->info);
        front1 = front1->ptr;
    }
    if (front1 == rear)
        printf("%d", front1->info);
}
 
/* Dequeing the queue */
void deq(struct Queue *front, struct Queue *rear){

    struct Queue * front1 = front;
 
    if (front1 == NULL)
    {
        printf("\n Error: Trying to display elements from empty queue");
        return;
    }
    else
        if (front1->ptr != NULL)
        {
            front1 = front1->ptr;
            printf("\n Dequed value : %d", front->info);
            free(front);
            front = front1;
        }
        else
        {
            printf("\n Dequed value : %d", front->info);
            free(front);
            front = NULL;
            rear = NULL;
        }
        
}
 
/* Returns the front element of queue */
int frontelement(struct Queue *front, struct Queue *rear){

    if ((front != NULL) && (rear != NULL))
        return(front->info);
    else
        return 0;
}
 
/* Display if queue is empty or not */
void empty(struct Queue *front, struct Queue *rear) {
     if ((front == NULL) && (rear == NULL))
        printf("\n Queue empty");
    else
       printf("Queue not empty");
}