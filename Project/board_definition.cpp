#include <Arduino.h>
#include "board_definition.h"

Button ButtonCheck() {
  int x = analogRead(A0);
  if (x < 60) return right;
  if (x < 200) return up;
  if (x < 400) return down;
  if (x < 600) return left;
  if (x < 800) return select;
  return none;
}

LiquidCrystal* InitLCD(int pin_rs, int pin_en, int pin_d4, int pin_d5, int pin_d6, int pin_d7) {
  LiquidCrystal* lcd = new LiquidCrystal(pin_rs, pin_en, pin_d4, pin_d5, pin_d6, pin_d7);
  lcd->begin(16, 2);
  return lcd;
}

int Menu(LiquidCrystal *lcd) {
  int option = 1;
  lcd->clear();
  lcd->print("1: In 2: Out");
  lcd->setCursor(0, 1);
  lcd->print("3: Move");

  while (true) {
    Button b = ButtonCheck();
    if (b == select) {
      return option;
    }
    if (b == up || b == right) {
      option = (option % 3) + 1;
    }
    if (b == down || b == left) {
      option = (option == 1) ? 3 : option - 1;
    }

    lcd->setCursor(15, 1);
    lcd->print(option);
    delay(250);
  }
}

int AskFloor(LiquidCrystal *lcd) {
  int floor = 1;
  lcd->clear();
  lcd->print("Select Floor (1-5)");

  while (true) {
    Button b = ButtonCheck();
    if (b == up || b == right) floor = (floor % 5) + 1;
    if (b == down || b == left) floor = (floor == 1) ? 5 : floor - 1;
    if (b == select) return floor;

    lcd->setCursor(0, 1);
    lcd->print("Floor: ");
    lcd->print(floor);
    delay(250);
  }
}

char AskDirection(LiquidCrystal *lcd) {
  char dir = 'u';
  lcd->clear();
  lcd->print("Dir: up/down?");

  while (true) {
    Button b = ButtonCheck();
    if (b == up || b == right) dir = 'u';
    if (b == down || b == left) dir = 'd';
    if (b == select) return dir;

    lcd->setCursor(0, 1);
    lcd->print(dir == 'u' ? "Going UP " : "Going DOWN ");
    delay(250);
  }
}
