#include <Arduino.h>

const unsigned long debounce = 40;
// debounce in ms

const int blinkLength = 200;
// Blink length in ms

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

enum gameStates {
  standby, game, death
};

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

Coords goal;

int score;

gameStates currState;

bool pressed(Switches&sw);

void coordConvert(Coords&input, bool onOff);

void printer();

void setup() {
  randomSeed(analogRead(A5));
  pinMode(power.pin, INPUT_PULLUP);
  for (int i = 0; i < 6; i++) {
    pinMode(controls[i].pin, INPUT_PULLUP);
  }
  for (int i = 0; i < 3; i++) {
    pinMode(MOS[i], OUTPUT);
    digitalWrite(MOS[i], LOW);
  }
  for (int i = 0; i < 9; i++) {
    pinMode(columns[i], OUTPUT);
  }
  currState = standby;

  player = {0,0,0};
  goal = {1,1,1};
  score = 0;
}

void loop() {
  for (int i = 0; i<27; i++) {
    image[i] = false;
  }
  bool powerState = pressed(power);

  bool conState[6];
  for (int i = 0; i<6; i++) {
    conState[i] = pressed(controls[i]);
  }

  if (currState == standby) {
    if (powerState == true){
      currState = game;
      score = 0;
      player = {0,0,0};
    }
  }
  else if (currState == game){
    if (player.x == goal.x && player.y == goal.y && player.z == goal.z) {
      score++;
      while (player.x == goal.x && player.y == goal.y && player.z == goal.z) {
      goal.x = random(0,3);
      goal.y = random(0,3);
      goal.z = random(0,3);
      }
    }
    else {
      bool blink = millis() / blinkLength % 2;
      coordConvert(goal, blink);
    }
    Coords ghost = player;
    for (int i = 0; i < 6; i++) {
      if (conState[i] == true){
        switch (controls[i].direction) {
        case xminus:
          ghost.x--;
          break;
        case xplus:
          ghost.x++;
          break;
        case yminus:
          ghost.y--;
          break;
        case yplus:
          ghost.y++;
          break;
        case zminus:
          ghost.z--;
          break;
        case zplus:
          ghost.z++;
          break;
        default:
          break;
        }
      }
    }
    if (
      ghost.x <= 2 && ghost.y <= 2 && ghost.z <= 2 &&
      ghost.x >= 0 && ghost.y >= 0 && ghost.z >= 0) {
        player = ghost;
        coordConvert(player, true);
      }
    else {
      currState = death;
    }
  }

  printer();
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
    image[input.x + input.y *3 + input.z * 9] = onOff;
  } 
}

void printer() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(MOS[i], LOW);
    for (int c = 0; c < 9; c++) {
      digitalWrite(columns[c],image[c + i*9]);
    }
    digitalWrite(MOS[i], HIGH);
    delay(2);
    digitalWrite(MOS[i], LOW);
  }
}