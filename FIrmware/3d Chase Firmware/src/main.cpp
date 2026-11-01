#include <Arduino.h>

const unsigned long debounce = 40;

enum Directions {
  xplus, xminus, yplus, yminus, zplus, zminus, none
};

struct Switches
{
  const Directions direction;
  const int pin;
  bool state;
  bool prevState;
  unsigned long prevTime;
 
};

struct Coords 
{
  int x;
  int y;
  int z;
};

Switches power = {none, 0, false, 0};

Switches controls[6] = {
  {xminus, 1, false, 0},
  {xplus, 2, false, 0},
  {yplus, 3, false, 0},
  {yminus, 4, false, 0},
  {zplus, 5, false, 0},
  {zminus, 6, false, 0},
};

const int MOS[3] = {7,8,9};

const int columns[9] = {10,11,12,13,14,15,16,17,18};

int gameState;

bool image[27] = {
  false, false, false,
  false, false, false,
  false, false, false,

  false, false, false,
  false, false, false,
  false, false, false,

  false, false, false,
  false, false, false,
  false, false, false
};

Coords player;

bool pressed(Switches&sw);

void coordConvert(Coords&input, bool onOff);

void setup() {
  pinMode(power.pin, INPUT_PULLUP);
  for (int i = 0; i < 6; i++) {
    pinMode(controls[i].pin, INPUT_PULLUP);
  }
  for (int i = 0; i < 3; i++) {
    pinMode(MOS[i], OUTPUT);
  }
  for (int i = 0; i < 9; i++) {
    pinMode(columns[i], OUTPUT);
  }
  gameState = 0;

  player = {0,0,0};
}

void loop() {
  for (int i = 0; i<27; i++) {
    image[i] = false;
  }
  if (pressed(power) == true){
    gameState = 1;
  }
}

bool pressed(Switches&sw) {
  bool current = !digitalRead(sw.pin);
  if (current != sw.prevState) {
    sw.prevState = current;
    sw.prevTime = millis();
    return false;
  }
  else {
    if (millis() - sw.prevTime >= debounce) {
      if (sw.prevState == true && sw.state == false) {
        sw.state = sw.prevState;
        return true;
      }
      else {
        sw.state = sw.prevState;
        return false;
      }
    }
    else {
      return false;
    }
  }
}

void coordConvert(Coords&input, bool onOff) {
  if (input.x >= 0 && input.x <= 2 && input.y >= 0 && input.y <= 2 && input.z >= 0 && input.z <= 2) {
    image[input.x + input.y *3 + input.z * 9] = !image[input.x + input.y *3 + input.z * 9];
  } 
}