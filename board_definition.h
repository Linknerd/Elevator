#ifndef BOARD_DEFINITION_H_
#define BOARD_DEFINITION_H_
#include <Arduino.h>
#include <LiquidCrystal.h>

// Input buttons
typedef enum {none, right, left, up, down, select} Button;

// Button check from analog pin
Button ButtonCheck();

// Menu interface
int Menu(LiquidCrystal *lcd); // 1 = in elevator, 2 = waiting, 3 = move

// Ask for floor number
int AskFloor(LiquidCrystal *lcd);

// Ask for direction
char AskDirection(LiquidCrystal *lcd);

// Initialize LCD
LiquidCrystal* InitLCD(int pin_rs, int pin_en, int pin_d4, int pin_d5, int pin_d6, int pin_d7);

#endif
