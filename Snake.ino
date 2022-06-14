#include "LedControl.h"

// pins
const int SW_pin = 2;
const int X_pin = 0;
const int Y_pin = 1;
const int DIN = 12;
const int CS = 11;
const int CLK = 10;
const int BUZZER = 8;

//variables
const int screenWidth = 8;
const int screenHeight = 8;
int snakeX, snakeY, foodX, foodY, score = 0, snakeSize = 1;
char direction;
int tailX[100], tailY[100];
bool isGameOver = false;

LedControl lc = LedControl(DIN, CS, CLK, 1);

void setup() {
  setupPins();
  setupLedBoard();
  setupSnakePosition();
  setupFoodPosition();
}

void setupSnakePosition() {
  snakeX = 4;
  snakeY = 4;
}

void setupFoodPosition() {
  foodX = rand() % screenWidth;
  foodY = rand() % screenHeight;
}

void setupLedBoard() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 1);
  lc.clearDisplay(0);
}

void setupPins() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
}

void loop() {
  if (isGameOver) {
    playGameOverSong();
    showGameOverScreen();
  } else {
    startGame();
  }
}

void playGameOverSong() {
  tone(BUZZER, 1000, 1000);
  delay(100);
  tone(BUZZER, 2000, 1000);
  delay(100);
  tone(BUZZER, 3000, 1000);
  delay(100);
  tone(BUZZER, 4000, 1000);
  delay(100);
  tone(BUZZER, 5000, 2000);
}

void playFoodEatenSong() {
  tone(BUZZER, 500, 100);
}

void startGame() {
  manageGameOver();
  setJoystickDirection();
  changeSnakeDirection();
  manageSnakeOutOfBounds();
  manageEatenFood();
  manageSnakeTailCoordinates();
  drawSnake();
  delay(300);
}

void manageGameOver() {
  for (int i = 1; i < snakeSize; i++) {
    if (tailX[i] == snakeX && tailY[i] == snakeY) {
      isGameOver = true;
    }
  }
}

void manageSnakeOutOfBounds() {
  if (snakeX >= screenWidth) {
    snakeX = 0;
  } else if (snakeX < 0) {
    snakeX = screenWidth - 1;
  }
  if (snakeY >= screenHeight) {
    snakeY = 0;
  } else if (snakeY < 0) {
    snakeY = screenHeight - 1;
  }
}

void manageSnakeTailCoordinates() {
  int previousX, previousY, prevX, prevY;
  previousX = tailX[0];
  previousY = tailY[0];
  tailX[0] = snakeX;
  tailY[0] = snakeY;

  for (int i = 1; i < snakeSize; i++) {
    prevX = tailX[i];
    prevY = tailY[i];
    tailX[i] = previousX;
    tailY[i] = previousY;
    previousX = prevX;
    previousY = prevY;
  }
}

void manageEatenFood() {
  if (snakeX == foodX && snakeY == foodY) {
    playFoodEatenSong();
    score++;
    snakeSize++;
    setupFoodPosition();
  }
}

void setJoystickDirection() {
  if (analogRead(X_pin) > 1000) {
    direction = 'u';
  } else if (analogRead(X_pin) < 100) {
    direction = 'd';
  } else if (analogRead(Y_pin) > 1000) {
    direction = 'l';
  } else if (analogRead(Y_pin) < 100) {
    direction = 'r';
  }
}

void changeSnakeDirection() {
  switch (direction) {
    case 'l':
      snakeX--;
      break;
    case 'r':
      snakeX++;
      break;
    case 'u':
      snakeY--;
      break;
    case 'd':
      snakeY++;
      break;
  }
}

void showGameOverScreen() {
  for (int i = 0; i < screenHeight; i++) {
    for (int j = 0; j < screenWidth; j++) {
      showLed(j, i);
      delay(50);
    }
  }
  resetVariables();
}

void resetVariables() {
  setupSnakePosition();
  setupFoodPosition();
  direction = ' ';
  isGameOver = false;
  score = 0;
  snakeSize = 1;
}

void showLed(int row, int column) {
  lc.setLed(0, row, column, true);
}

void hideLed(int row, int column) {
  lc.setLed(0, row, column, false);
}

void drawSnake() {
  for (int i = 0; i < screenHeight; i++) {
    for (int j = 0; j < screenWidth; j++) {
      if (i == snakeY && j == snakeX) {
        showLed(snakeX, snakeY);
      } else if (i == foodY && j == foodX) {
        showLed(foodX, foodY);
      } else {
        bool isShown = false;
        for (int k = 0; k < snakeSize; k++) {
          if (tailX[k] == j && tailY[k] == i) {
            showLed(j, i);
            isShown = true;
          }
        }
        if (!isShown) {
          hideLed(j, i);
        }
      }
    }
  }

}
