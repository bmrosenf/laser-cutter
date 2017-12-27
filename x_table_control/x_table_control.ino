const int X_CONTROLLER_PIN = A0;
int xPos = 0;
const int X_DIR_PIN = 4;
const int X_STEP_PIN = 3;
// NOTE: MS pins are currently unused
const int X_MS1 = 0;
const int X_MS2 = 1;
const int X_MS3 = 2;

// TODO: change
const int Y_CONTROLLER_PIN = A1;
int yPos = 0;
const int Y_DIR_PIN = 12;
const int Y_STEP_PIN = 11;
const int Y_MS1 = 8;
const int Y_MS2 = 9;
const int Y_MS3 = 10;

const int DELAY_SPEED = 1000;
const int TOP_SPEED = 400;
const int RAMP_SPEED = 3;
const int STOP_SPEED = 8;
/*int speedSet;*/
int xSpeed = 0;
int controllerXSpeed = 0;
int ySpeed = 0;
int controllerYSpeed = 0;
// int off;
/*boolean xShouldMove = false;*/
/*boolean ySouldMove = false;*/


void setup() {
  /*Serial.begin(9600);*/
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_MS1, OUTPUT);
  pinMode(X_MS2, OUTPUT);
  pinMode(X_MS3, OUTPUT);
  digitalWrite(X_MS1, LOW);
  digitalWrite(X_MS2, LOW);
  digitalWrite(X_MS3, LOW);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_MS1, OUTPUT);
  pinMode(Y_MS2, OUTPUT);
  pinMode(Y_MS3, OUTPUT);

  digitalWrite(Y_MS1, LOW);
  digitalWrite(Y_MS2, LOW);
  digitalWrite(Y_MS3, LOW);
}

int calculateNewSpeed(int currentSpeed, int newSpeed) {
  int off = abs(currentSpeed - newSpeed);

  if (off !=  0 && currentSpeed < newSpeed) {
    return currentSpeed + RAMP_SPEED;
    /*Speed = Speed + RAMP_SPEED;*/
  } else if (off !=  0 && currentSpeed > newSpeed) {
    return currentSpeed - RAMP_SPEED;
    /*Speed = Speed - RAMP_SPEED;*/
  }
}

void motorStep(int pin, int pinSpeed) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(pinSpeed);
  digitalWrite(pin, LOW);
  delayMicroseconds(pinSpeed);
}

void loop() {
  xPos = analogRead(X_CONTROLLER_PIN) + 5;
  yPos = analogRead(Y_CONTROLLER_PIN) + 5;
  /*Serial.println(xPos);*/
  /*Serial.println(yPos);*/

  if (xPos >= 525) {
    digitalWrite(X_DIR_PIN, HIGH);
    /*shouldMove = true;*/
    controllerXSpeed = map(xPos, 550, 1024, DELAY_SPEED, TOP_SPEED);
//    Serial.print("HIGH speedSet=");
//    Serial.println(speedSet);
    xSpeed = calculateNewSpeed(xSpeed, controllerXSpeed);
  } else if (xPos <= 475) {
    digitalWrite(X_DIR_PIN, LOW);
    /*shouldMove = true;*/
    controllerXSpeed = map(xPos, 500, 0, DELAY_SPEED, TOP_SPEED);
//    Serial.print("LOW speedSet=");
//    Serial.println(speedSet);
    xSpeed = calculateNewSpeed(xSpeed, controllerXSpeed);
  } else {
    /* shouldMove = false; */
    xSpeed = 0;
  }

  if (yPos >= 550) {
    digitalWrite(Y_DIR_PIN, HIGH);
    /*shouldMove = true;*/
    controllerYSpeed = map(yPos, 550, 1024, DELAY_SPEED, TOP_SPEED);
    /*Serial.print("HIGH controllerYSpeed=");*/
    /*Serial.println(controllerYSpeed);*/
    ySpeed = calculateNewSpeed(ySpeed, controllerYSpeed);
  } else if (yPos <= 475) {
    digitalWrite(Y_DIR_PIN, LOW);
    /*shouldMove = true;*/
    controllerYSpeed = map(yPos, 500, 0, DELAY_SPEED, TOP_SPEED);
    /*Serial.print("LOW controllerYSpeed=");*/
    /*Serial.println(controllerYSpeed);*/
    ySpeed = calculateNewSpeed(ySpeed, controllerYSpeed);
  } else {
    /*shouldMove = false;*/
    ySpeed = 0;
  }

  /*Serial.print("ySpeed=");*/
  /*Serial.println(ySpeed);*/

  // bool shouldMove = ((ySpeed > 0) || (xSpeed > 0));

  if (xSpeed > 0) {
    motorStep(X_STEP_PIN, xSpeed);
  }
  if (ySpeed > 0) {
    motorStep(Y_STEP_PIN, ySpeed);
  }

  // ySpeed = 0;
  // xSpeed = 0;
    /*if (speed < delay_speed) {*/
      /*speed = speed + stop_speed;*/
      /*motorstep(steppin);*/
    /*} else if (speed >= delay_speed) {*/
      /*speed = delay_speed;*/
    /*}*/
  //}
}

