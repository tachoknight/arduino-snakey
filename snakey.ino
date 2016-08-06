#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FOR_ARDUINO

//
// Lights stuff
//

// 0x000 = none
// 0x001 = LED 1
// 0x201 = LED 1
// 0x002 = LED 2
// 0x004 = LED 3
// 0x008 = LED 4
// 0x010 = LED 5
// 0x020 = LED 6
// 0x040 = LED 7
// 0x080 = LED 8
// 0x100 = LED 9
// 0x281 = LEDs 1-8
// 0x101 = LEDs 1-9
// 0x301 = LEDs 1-9
// 0x221 = LEDs 1-6
// 0x211 = LEDs 1-5
// 0x411 = LEDs 1-5
// 0x009 = LEDs 1-4
// 0x006 = LEDs 2-3
// 0x030 = LEDs 5-6
// 0x050 = LEDs 5-7
// 0x814 = LEDs 3-5

// 0x034 = LEDs 3-5-6
// 0x514 = LEDs 3-5-9
// 0x381 = LEDs 1-8-9
// 0x121 = LEDs 1-6-9
// 0x385 = LEDs 1-3-8-9
// 0x712 = LEDs 2-5-9

const int BOTTOM = 0;
const int MIDDLE = 1;
const int TOP = 2;

// The array we are populating to show
unsigned int table[4];

// This is a table of x,y coords to numbers
unsigned int pos2NumTable[][3] = {
  {0, 2, 0x001},
  {1, 2, 0x002},
  {2, 2, 0x004},
  {0, 1, 0x008},
  {1, 1, 0x010},
  {2, 1, 0x020},
  {0, 0, 0x040},
  {1, 0, 0x080},
  {2, 0, 0x100},
};

//
// Snakey stuff
//
const int GRID_SIZE = 3;
// How big the snake is...
const int SNAKE_SIZE = 3;
const int SNAKE_HEAD = 0;

enum POS { X, Y, Z };

struct COORDS {
  int x, y, z;
};

// Our snake position; [0] = X, [1] = Y, [2] = Z
int snakePos[SNAKE_SIZE][3];

int startX = 0;
int startY = 0;
int startZ = 0;


POS chooseDirection() {
  return static_cast<POS>(rand() % 3);
}

bool makeMove(const int &fromPos, const int &prevPos, int &toPos) {
  bool moved = false;

  int doNothing = rand() % 2;
  if (doNothing == 0) {
    return false;
  }

  if (fromPos >= prevPos && (fromPos + 1 < GRID_SIZE)) {
    toPos += 1;
    return true;
  }

  if (fromPos <= prevPos && (fromPos - 1 >= 0)) {
    toPos -= 1;
    return true;
  }

  if (fromPos == prevPos && (fromPos + 1 < GRID_SIZE) && (fromPos - 1 >= 0)) {
    int randomDirection = rand() % 2;
    if (randomDirection == 0) {
      toPos += 1;
      return true;
    } else {
      toPos -= 1;
      return true;
    }
  }

  return moved;
}

COORDS setNewHead(const int snakePos[SNAKE_SIZE][3]) {
  COORDS newCoords;
  newCoords.x = snakePos[0][X];
  newCoords.y = snakePos[0][Y];
  newCoords.z = snakePos[0][Z];

  bool newHeadFound = false;

  do {

    POS posToMove = chooseDirection();

    switch (posToMove) {
      case X:
        if (makeMove(snakePos[0][X], snakePos[1][X], newCoords.x)) {
          newHeadFound = true;
          break;
        }
      case Y:
        if (makeMove(snakePos[0][Y], snakePos[1][Y], newCoords.y)) {
          newHeadFound = true;
          break;
        }
      case Z:
        if (makeMove(snakePos[0][Z], snakePos[1][Z], newCoords.z)) {
          newHeadFound = true;
        }

        break;
    }

  } while (newHeadFound == false);

  return newCoords;
}

void setup() {
  srand((unsigned int)time(0));

  for (int pos = 0; pos < SNAKE_SIZE + 1; ++pos) {
    snakePos[pos][X] = startX;
    snakePos[pos][Y] = startY;
    snakePos[pos][Z] = startZ;
  }

  for (int a = 2; a <= 13; a++)
    pinMode(a, OUTPUT);

}

unsigned int getValForKeys(int x, int y) {
  for (int row = 0; row < 9; ++row) {
    if (pos2NumTable[row][0] == x && pos2NumTable[row][1] == y)
      return pos2NumTable[row][2];
  }

  return 0x000;
}

void setTable(int x, int y, int z) {

  switch (z) {
    case BOTTOM:
      table[BOTTOM] |= getValForKeys(x, y);
      break;
    case MIDDLE:
      table[MIDDLE] |= getValForKeys(x, y);
      break;
    case TOP:
      table[TOP] |= getValForKeys(x, y);
      break;
  };

  // And set the brightness
  table[3] = 0x02ff;
}

void loop() {
  COORDS newHeadCoords = setNewHead(snakePos);
  assert(newHeadCoords.x != -1);
  assert(newHeadCoords.y != -1);
  assert(newHeadCoords.z != -1);

  // Okay, since we're past the loop, currentX or currentY should now
  // be changed. So now we want to update the array with the previous
  // values (for all the parts below HEAD) and then set HEAD to the
  // new X and Y
  for (int turn = SNAKE_SIZE; turn > 0; --turn) {
    snakePos[turn][X] = snakePos[turn - 1][X];
    snakePos[turn][Y] = snakePos[turn - 1][Y];
    snakePos[turn][Z] = snakePos[turn - 1][Z];
  }
  // And now update HEAD
  snakePos[SNAKE_HEAD][X] = newHeadCoords.x;
  snakePos[SNAKE_HEAD][Y] = newHeadCoords.y;
  snakePos[SNAKE_HEAD][Z] = newHeadCoords.z;

  // Now print our current snake position
#ifndef FOR_ARDUINO
  printf("\t---------\n");
#endif
  for (int x = 0; x < SNAKE_SIZE; ++x) {
    setTable(snakePos[x][X], snakePos[x][Y], snakePos[x][Z]);
#ifndef FOR_ARDUINO
    printf("\t(%d, %d, %d)\n", snakePos[x][X], snakePos[x][Y], snakePos[x][Z]);
#endif
  }

  //
  // Now show the table
  //

  for (int b = 0; b < (table[3] >> 8); b++)
  {
    for (int c = 0; c < 33; c++) {
      digitalWrite(2, (table[c % 3] & 0x1));
      digitalWrite(4, (table[c % 3] & 0x2));
      digitalWrite(7, (table[c % 3] & 0x4));
      digitalWrite(8, (table[c % 3] & 0x8));
      digitalWrite(9, (table[c % 3] & 0x10));
      digitalWrite(10, (table[c % 3] & 0x20));
      digitalWrite(11, (table[c % 3] & 0x40));
      digitalWrite(12, (table[c % 3] & 0x80));
      digitalWrite(13, ((table[c % 3] & 0x100) >> 1));

      if (c % 3 == 0)
        analogWrite(3, (0xff - (table[3] & 0xff)));
      else if (c % 3 == 1)
        analogWrite(5, (0xff - (table[3] & 0xff)));
      else if (c % 3 == 2)
        analogWrite(6, (0xff - (table[3] & 0xff)));
      delay(2);
      digitalWrite(3, 1);
      digitalWrite(5, 1);
      digitalWrite(6, 1);
    }
    delay(1);
  }

  // And reset the table
  table[0] = 0x000;
  table[1] = 0x000;
  table[2] = 0x000;
}
