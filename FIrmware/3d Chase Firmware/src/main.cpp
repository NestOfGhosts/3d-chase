#include <Arduino.h>

const unsigned long debounce = 40;
// debounce in ms

const int blinkLength = 200;
// Blink length in ms

const int animationFSpeed = 500;

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

const bool litLeds[27] = {
  true, true, true, 
  true, true, true, 
  true, true, true, 

  true, true, true, 
  true, true, true, 
  true, true, true,

  true, true, true,
  true, true, true,
  true, true, true
};

bool layerLit[3] = {false,false,false};

bool columnLit[9] = {
false, false, false, 
false, false, false, 
false, false, false};

const unsigned int timeTable[31] = {
  3500, 2940, 2505, 2168, 1907, 1703, 1546, 1424, 1329, 1255, 1198, 1154, 1119, 1093, 1072, 1056, 1043, 1034, 1026, 1020, 1016, 1012, 1009, 1007, 1006, 1005, 1004, 1003, 1002, 1001, 1000
};

Coords player;

Coords goal;

int score;

gameStates currState;

bool pressed(Switches&sw);

unsigned long lastTime;

int randomStandby;

bool arrayComp(const bool arr1[27], const bool arr2[27]);

void coordConvert(Coords&input, bool onOff);

void printer();

void clear(bool tf);

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

  randomStandby = random(0,3);
  lastTime = millis();
}

void loop() {
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
      goal = {1,1,1};
      lastTime = millis();
    }
    else {
      switch (randomStandby) {
        case 0: {
        Coords randomCoord = {random(0,3), random(0,3), random(0,3)};

        if (image[randomCoord.x + randomCoord.y * 3 + randomCoord.z * 9] == false && millis() - lastTime > animationFSpeed) {
          coordConvert(randomCoord, true);
          lastTime = millis();
        }
        if (arrayComp(image, litLeds) == true) {
          randomStandby = random(0,3);
          for (int i = 0; i < 82; i++) {
            printer();
          }
          clear(false);
        }
        break;
        }

        case 1: {
        int randomLayer = random(0,3);
        if (layerLit[randomLayer] == false && millis() - lastTime > animationFSpeed) {
          for (int i = 0; i<9; i++) {
            Coords layer = {i/3, i%3, randomLayer};
            coordConvert(layer, true);
            lastTime = millis();
            layerLit[randomLayer] = true;
          }
        }
        if (arrayComp(image, litLeds) == true) {
          randomStandby = random(0,3);
          for (int i = 0; i < 82; i++) {
            printer();
          }
          clear(false);
          for (int i = 0; i < 3; i++) {
            layerLit[i] = false;
          }
        }
        break;
        }

        case 2: {
        int randomColumn = random(0,9);
        if (columnLit[randomColumn] == false && millis() - lastTime > animationFSpeed) {
          for (int i = 0; i<3; i++) {
            Coords column = {randomColumn/3, randomColumn%3, i};
            coordConvert(column, true);
            lastTime = millis();
            columnLit[randomColumn] = true;
          }
        }
        if (arrayComp(image, litLeds) == true) {
          randomStandby = random(0,3);
          for (int i = 0; i < 82; i++) {
            printer();
          }
          clear(false);
          for (int i = 0; i < 9; i++) {
            columnLit[i] = false;
          }
        }
        break;
        }
      }
    }
  }
  else if (currState == game){
    clear(false);
    if (timeTable[score/3 > 30 ? 30 : score/3] > millis() - lastTime) {
      if (player.x == goal.x && player.y == goal.y && player.z == goal.z) {
        score++;
        lastTime = millis();
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
      if (powerState == true) {
        currState = death;
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
    else {
      currState = death;
    }
  }
  else {
    clear(false);
    int flashes = score/27;
    int remain = score%27;
    for (int i = 0; i < flashes; i++) {
      clear(true);
      for (int x = 0; x < 82; x++) {
        printer();
      }
      clear(false);
      for (int x = 0; x < 82;x++) {
        printer();
      }
    }
    clear(false);
    for (int i = 0; i < remain; i++) {
      image [i] = true;
    }
    for (int x = 0; x < 82; x++) {
      printer();
    }
    clear(false);
    currState = standby;
    for (int i = 0; i < 3; i++) {
      layerLit[i] = false;
    }
    for (int i = 0; i < 9; i++) {
      columnLit[i] = false;
    }

    randomStandby = random(0,3);
    lastTime = millis();
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

void clear(bool tf) {
  for (int i = 0; i<27; i++) {
    image[i] = tf;
  }
}

bool arrayComp(const bool arr1[27], const bool arr2[27]) {
  for (int i=0; i < 27; i++) {
    if (arr1[i] != arr2[i]) {
      return false;
    }
  }
  return true;
}