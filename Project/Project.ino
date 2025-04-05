// Combined Arduino Elevator System with LCD, Stepper Motor, and Priority Queue
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Stepper.h>
#include "board_definition.h"
#include "priorityQueue.h"


// Stepper config
const int stepsPerRevolution = 200;
const int stepsPerFloor = 2400; // Adjust as needed
Stepper stepper(stepsPerRevolution, 2, 3); // Adjust pins as needed

// LCD config
LiquidCrystal* lcd;

// Elevator system
Queue* upQ;
Queue* downQ;
Elevator* e;
int priority = 1;

// Move elevator from current floor to destination
void moveToFloor(int dest) {
  int diff = dest - e->curFloor;
  int steps = diff * stepsPerFloor;
  stepper.step(steps);
  e->curFloor = dest;
}

void setup() {
  lcd = InitLCD(8, 9, 4, 5, 6, 7); // RS, E, D4-D7 pins
  upQ = createQ(1);
  downQ = createQ(0);
  e = createElevator(1); // Starting floor
  Serial.begin(9600);
  stepper.setSpeed(400); // RPM
  lcd->print("Elevator Ready");
  delay(1000);
  lcd->clear();
}

void loop() {
  int choice = Menu(lcd);
  delay(300);
  if (choice == 1) { // Inside elevator
    int floor = AskFloor(lcd);
    delay(100);
    inElevatorPress(floor, e, upQ, downQ);
    lcd->clear();
    lcd->print("Request added");
    delay(1000);
  }
  else if (choice == 2) { // Waiting outside
    int floor = AskFloor(lcd);
    delay(300);
    char dir = AskDirection(lcd);
    outElevatorPress(floor, dir, upQ, downQ);
    lcd->clear();
    lcd->print("Call added");
    delay(1000);
  }
  else if (choice == 3) { // Move elevator
    lcd->clear();
    lcd->print("Moving...");
    delay(1000);
    int nextFloor = getNextFloor(&priority, upQ, downQ, e);
    if (nextFloor != -1 && nextFloor != e->curFloor) {
      moveToFloor(nextFloor);
    }
    lcd->clear();
    lcd->print("Arrived: ");
    lcd->print(e->curFloor);
    delay(2000);
  }
  printQ(upQ);
  printQ(downQ);
}