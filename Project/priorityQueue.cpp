/*
 * *********************************** READ ME *******************
 * This code is for a special linked list. They are called Queues here but they do not function like Queues.
 * In this code, I have left many functions for you to use. The ones that will be most beneficial to you are:
 *      - inElevatorPress()
 *      - outElevatorPress()
 *      - getNextFloor()
 *  These functions have been created for your conveneince. They simply add and remove things as needed.
 *  NOTE: Get next floor does remove the element that it returns.
 *  Furthermore, the script main.c should not be used. Its only there so you can see how I implimented code from the console
 *  For what I've tested so far, the code does work for our intents and purposes. If there is a need for a change, I have documented everything down below.
 */

#include "priorityQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <Arduino.h>

// Function that creates a Queue
/**
 * @param up - Uses this integer value to assign the type of Queue. 1 is for up requests, and 0 is for down requests.
 * @return - Returns an empty Queue with the default properties. No front node with a proper up value
 */
Queue *createQ(int up)
{
    // Creates a queue pointer and then allocates the memory
    Queue *q = (Queue *)(malloc(sizeof(Queue)));
    // Checks if the Queue is NULL
    if (q != NULL)
    {
        // Setup all the default values for the Queue.
        q->front = NULL;
        q->count = 0;
        q->up = up;
    }
    else
    {
        // If the Queue doesn't exist, then say it
        printf("Queue is null\n");
    }
    // Return the Queue
    return q;
}

// Function that creates the nodes for the Queue.
/**
 *  @param floor - Uses this integer to determine the floor request
 *  @returns - The node created with this floor value. By default, the next node does not point to anything.
 */
Node *createNode(int floor)
{
    // Create the node
    Node *n = (Node *)(malloc(sizeof(Node)));
    // Check if the node exists or not.
    if (n != NULL)
    {
        // Assign values to the floor
        n->toFloor = floor;
        n->nextNode = NULL;
    }
    else
    {
        printf("Node is null\n");
    }
    // Returns the node
    return n;
}
// Checks if the Queue is empty
/**
 * @param q - Input Queue to check if the Queue is empty
 * @return - 1 if its empty, 0 if its not empty
 */

int isEmpty(Queue *q)
{
    if (q->count == 0)
        return 1;
    return 0;
}
// Adds an object to a Queue
/**
 * @param q - The queue to add a node to
 * @param n - The node to add to the Queue
 * @return - 1 if successful, -1 if failed.
 * Realistically, you shouldn't use this because I already have another function setup for the inside and outside button presses
 */
int onQ(Queue *q, Node *n)
{
    // Checks if the queue or node is null
    if (q == NULL || n == NULL)
        return EXIT_ERR;
    // Checks if the Queue is empty
    if (isEmpty(q))
    {
        q->front = n;
        q->count++;
        return EXIT_OK;
    }
    // If the queue isn't empty, create a new node and set it to the front of the queue.
    Node *temp = q->front;
    // The variable up dictates what type of Queue it is
    // 0 is for the Down Queue
    // 1 is for the Up Queue
    switch (q->up)
    {
        // down requests, Sorted in descending order
    case 0:
        // First check if the first node is smaller than the node you want to add
        if (temp->toFloor < n->toFloor)
        {
            // If it is, then add the node in front
            n->nextNode = temp;
            // Set the front pointer to the new node
            q->front = n;
        }
        else
        {
            // Otherwise, go through the loop until the next node is greater than the front node
            while (temp->nextNode != NULL && temp->nextNode->toFloor > n->toFloor)
            {
                temp = temp->nextNode;
                if(temp->toFloor == n->toFloor) return EXIT_ERR;
            }
            // Insert the node in the right spot
            n->nextNode = temp->nextNode;
            temp->nextNode = n;
            printf("Case 0");
        }
        break;
    case 1:
        // For the up requests, we want to sort them in ascending order
        // Check if the floor we want to add is less than the current floor
        if (temp->toFloor > n->toFloor)
        {
            // Add to the front of the request queue
            n->nextNode = temp;
            // Point the front to the front
            q->front = n;
        }
        else
        {
            // Go through the request queue until you find a node with a floor value higher than the current node
            while (temp->nextNode != NULL && temp->nextNode->toFloor < n->toFloor)
            {
                temp = temp->nextNode;
                if(temp->toFloor == n->toFloor) return EXIT_ERR;
            }
            // Put the node before the node with the higher value
            n->nextNode = temp->nextNode;
            temp->nextNode = n;
            printf("Case 1");
            break;
        }
    default:
        break;
    }
    printf("\n");
    q->count++;
    return EXIT_OK;
}

// Removes nodes from the queue
/**
 * @param q - The queue to remove the node from
 * @param e - The elevator struct to keep track of the floors.
 * @return - The next floor to go to. -1 if there is an error.
 * Realistically, you shouldn't use this because I already have another function setup for the removal and output
 */
int deQ(Queue *q, Elevator *e)
{
    // Checks if either the queue or the elevator exist. If they're NULL, report the error
    if (q == NULL || e == NULL)
        return -1;
    // Have a previous node and a temp node to keep track of the current traversal node and next node
    Node *temp = q->front;
    Node *prev = NULL;
    // Declare the value variable to return
    int value;
    // if the request queue only contains one element, remove that element and set the
    if (q->count == 1)
    {
        // Just remove the front node
        q->front = temp->nextNode;
        value = temp->toFloor;
        q->count--;
        free(temp);
        return value;
    }
    switch (q->up)
    {
        // Going down
    case 0:
        // Find the first node which is less than the current floor
        while (temp)
        {
            if (temp->toFloor < e->curFloor)
            {
                // This is the node we want to remove
                int value = temp->toFloor;

                // Check for front removal
                if (prev == NULL)
                {
                    // Remove the front
                    q->front = temp->nextNode;
                }
                else
                {
                    // Skip the next node
                    prev->nextNode = temp->nextNode;
                }
                // Free temp
                free(temp);
                // Remove 1 from the count
                q->count--;
                return value;
            }
            // Traverse the list
            prev = temp;
            temp = temp->nextNode;
        }
        break;
        // Front request line
        // Literally just the oppoiste of the back
    case 1:
        while (temp)
        {
            if (temp->toFloor > e->curFloor)
            {
                int value = temp->toFloor;

                if (prev == NULL)
                {
                    // removing the front
                    q->front = temp->nextNode;
                }
                else
                {
                    // removing from middle
                    prev->nextNode = temp->nextNode;
                }
                free(temp);
                q->count--;
                return value;
            }
            // Traverse
            prev = temp;
            temp = temp->nextNode;
        }
        // If no node meets the condition, return -1
        return -1;
        break;
    default:
        break;
    }
    return -1;
}

/**
 * Function to print the queue
 * @param q - The queue to print
 */
void printQ(Queue *q)
{
    if (isEmpty(q))
    {
        Serial.print("Queue is Empty\n");
    }
    else
    {
        Node *temp = q->front;
        while (temp != NULL)
        {
            switch (q->up)
            {
            case -1:
                Serial.println("Request Queue");
                break;
            case 0:
                Serial.println("Down Queue");
                break;
            case 1:
                Serial.println("Up Queue");
                break;
            default:
                Serial.println("I have no clue what this Queue is");
            }
            Serial.println("Floor Request: " + temp->toFloor);
            temp = temp->nextNode;
            
        }
    }
}
/**
 * Function to see the top value of a queue
 * @param q - the queue to peek
 */
int peek(Queue *q)
{
    if (q == NULL)
    {
        return -1;
    }
    return q->front->toFloor;
}
/**
 * Initializes an elevator
 * @param floor - The starting floor of the elevator
 */
Elevator *createElevator(int floor)
{
    Elevator *e = (Elevator *)(malloc(sizeof(Elevator)));
    if (e == NULL)
        printf("No elevator was created\n");
    else
    {
        e->curFloor = floor;
    }
    return e;
}

/**
 * Function that handles button presses inside the elevator
 * @param floor - The floor to queue next
 * @param e - The elevator
 * @param upQ - The queue that holds the up requests
 * @param downQ - The queue that holds the down requests
 */
int inElevatorPress(int floor, Elevator *e, Queue *upQ, Queue *downQ)
{
    if(floor > 5 || floor < 1) return -1;
    char up;
    if (floor < e->curFloor)
        up = 'd';
    else
        up = 'u';
    int goingUP = -1;
    if (up == 'u')
        goingUP = 1;
    else
        goingUP = 0;
    Node *new1 = createNode(floor);
    if (goingUP)
    {
        onQ(upQ, new1);
    }
    else
    {
        onQ(downQ, new1);
    }
}

/**
 * Function that handles button presses inside the elevator
 * @param floor - The floor to queue next
 * @param dir - The direction that the elevator should go. (u) for up, (d) for down
 * @param upQ - The queue that holds the up requests
 * @param downQ - The queue that holds the down requests
 */

int outElevatorPress(int floor, char dir, Queue *upQ, Queue *downQ)
{
    if(floor > 5 || floor < 1) return -1;
    int goingUP = -1;
    if (dir == 'u')
        goingUP = 1;
    else
        goingUP = 0;
    if(floor == 5) goingUP = 1;
    if(floor == 1) goingUP = 0;
    Node *new1 = createNode(floor);
    if (goingUP)
    {
        onQ(upQ, new1);
    }
    else
    {
        onQ(downQ, new1);
    }
}

/**
 * Function that obtains the next floor to traverse to
 * @param priority - The integer that tells the script which place to check first. (0) for down and (1) for up
 * @param upQ - The queue for the up requests
 * @param downQ - The queue for the down requests
 * @param e - The elevator struct
 * @return - The next floor to traverse
 */
int getNextFloor(int *priority, Queue *upQ, Queue *downQ, Elevator *e)
{
    int nextFloor = 0;
    if (isEmpty(upQ))
        *priority = 0;
    if (isEmpty(downQ))
        *priority = 1;
    if (*priority)
    {
        nextFloor = deQ(upQ, e);
        if (nextFloor == -1)
        {
            nextFloor = deQ(downQ, e);
            *priority = 0;
        }
    }
    else
    {
        nextFloor = deQ(downQ, e);
        if (nextFloor == -1)
        {
            nextFloor = deQ(upQ, e);
            *priority = 1;
        }
    }
    if (nextFloor == 1)
    {
        *priority = 1;
    }
    else if (nextFloor == 5)
    {
        *priority = 0;
    }
    if (nextFloor == -1)
    {
        printf("Problem!\n");
    }
    else
    {
        return nextFloor;
    }
    return -1;
}