#define EXIT_OK 1
#define EXIT_ERR 0
#define SWAP -1
//*********************STRUCTS***************** 
//Node Structure
typedef struct node{
    struct node* nextNode;
    int toFloor;
}Node;
//Queue structure
typedef struct queue{
    Node* front;
    int count;
    int up;
}Queue;

typedef struct elevator{
    int curFloor;
}Elevator;
//***************** FUNCTION PROTOTYPES ******************
//Create queue
Queue* createQ(int up);

//Create a node
Node* createNode(int floor);

//Check if queue is empty
int isEmpty(Queue* q);

//Enqueue object
int onQ(Queue* q, Node* n);

//Dequeue Object
int deQ(Queue* q, Elevator* e);

//Prints the Queue
void printQ(Queue* q);

//Can view the top element of the Queue
int peek(Queue *q);
//Creates a new elevator struct.
Elevator* createElevator(int floor);
//Uses onQ to queue the elevator press when you are in the elevator
int inElevatorPress(int floor, Elevator *e, Queue *upQ, Queue *downQ);
//Uses onQ to queue the elevator press outside the elevator
int outElevatorPress(int floor, char dir, Queue* upQ, Queue* downQ);

int getNextFloor(int *priority, Queue *upQ, Queue *downQ, Elevator *e);