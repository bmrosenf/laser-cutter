//  MS1   MS2   MS3   Microstep Resolution
//  --------------------------------------
//  LOW   LOW   LOW   Full Step
//  HIGH  LOW   LOW   Half Step
//  LOW   HIGH  LOW   Quarter Step 
//  HIGH  HIGH  LOW   Eighth Step
//  HIGH  HIGH  HIGH  Sixteenth Step

const int X_CONTROLLER_PIN = A0; // Analog x reading
int xPos = 0;
const int X_DIR_PIN = 4; // Set HIGH for forward, LOW for reverse
const int X_STEP_PIN = 3; // PULSE to set speed
const int X_MS1 = 0;
const int X_MS2 = 1;
const int X_MS3 = 2;

const int Y_CONTROLLER_PIN = A1; // Analog y reading
int yPos = 0;
const int Y_DIR_PIN = 12; // Set HIGH for forward, LOW for reverse
const int Y_STEP_PIN = 11; // PULSE to set speed
const int Y_MS1 = 8; 
const int Y_MS2 = 9;
const int Y_MS3 = 10;

const int LONGEST_PULSE_WIDTH_US = 1000;
const int SHORTEST_PULSE_WIDTH_US = 400;
const int RAMP_DELTA = 3;
int xPulseWidth = 0;
int controllerXPulseWidth = 0;
int yPulseWidth = 0;
int controllerYPulseWidth = 0;

void setup() {
  // Serial.begin(9600);
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

int calculateNewPulseWidth(int currentWidth, int newWidth) {
  int offset = newWidth - currentWidth;

  if (offset > 0) {
    return currentWidth + RAMP_DELTA;
  } else if (offset < 0) {
    return currentWidth - RAMP_DELTA;
  } else {
    return currentWidth;
  }
}

void sendMotorPulse(int pin, int pulseWidth) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(pin, LOW);
  delayMicroseconds(pulseWidth);
}

void loop() {
  xPos = analogRead(X_CONTROLLER_PIN) + 5;
  yPos = analogRead(Y_CONTROLLER_PIN) + 5;

  if (xPos >= 525) {
    digitalWrite(X_DIR_PIN, HIGH);
    controllerXPulseWidth = map(xPos, 550, 1024, LONGEST_PULSE_WIDTH_US, SHORTEST_PULSE_WIDTH_US);
    xPulseWidth = calculateNewPulseWidth(xPulseWidth, controllerXPulseWidth);
  } else if (xPos <= 475) {
    digitalWrite(X_DIR_PIN, LOW);
    controllerXPulseWidth = map(xPos, 500, 0, LONGEST_PULSE_WIDTH_US, SHORTEST_PULSE_WIDTH_US);
    xPulseWidth = calculateNewPulseWidth(xPulseWidth, controllerXPulseWidth);
  } else {
    xPulseWidth = 0;
  }

  if (yPos >= 550) {
    digitalWrite(Y_DIR_PIN, HIGH);
    controllerYPulseWidth = map(yPos, 550, 1024, LONGEST_PULSE_WIDTH_US, SHORTEST_PULSE_WIDTH_US);
    yPulseWidth = calculateNewPulseWidth(yPulseWidth, controllerYPulseWidth);
  } else if (yPos <= 475) {
    digitalWrite(Y_DIR_PIN, LOW);
    controllerYPulseWidth = map(yPos, 500, 0, LONGEST_PULSE_WIDTH_US, SHORTEST_PULSE_WIDTH_US);
    yPulseWidth = calculateNewPulseWidth(yPulseWidth, controllerYPulseWidth);
  } else {
    yPulseWidth = 0;
  }

  if (xPulseWidth > 0) {
    sendMotorPulse(X_STEP_PIN, xPulseWidth);
  }
  if (yPulseWidth > 0) {
    sendMotorPulse(Y_STEP_PIN, yPulseWidth);
  }
}

