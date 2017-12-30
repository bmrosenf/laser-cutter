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

const int XY_Low_Bound = 475;
const int XY_High_Bound = 550;

const int LONGEST_PULSE_WIDTH_US = 2000;
const int SHORTEST_PULSE_WIDTH_US = 200;
const int RAMP_DELTA = 10;
int xPulseWidth = LONGEST_PULSE_WIDTH_US;
int controllerXPulseWidth = 0;
boolean sendXPulse = false;
int yPulseWidth = LONGEST_PULSE_WIDTH_US;
int controllerYPulseWidth = 0;
boolean sendYPulse = false;

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
  xPos = analogRead(X_CONTROLLER_PIN);
  yPos = analogRead(Y_CONTROLLER_PIN);

  if (xPos >= XY_High_Bound) {
    sendXPulse = true;
    digitalWrite(X_DIR_PIN, HIGH);
    controllerXPulseWidth = map(xPos, XY_High_Bound, 1024, LONGEST_PULSE_WIDTH_US, SHORTEST_PULSE_WIDTH_US);
    xPulseWidth = calculateNewPulseWidth(xPulseWidth, controllerXPulseWidth);
  } else if (xPos <= XY_Low_Bound) {
    sendXPulse = true;
    digitalWrite(X_DIR_PIN, LOW);
    controllerXPulseWidth = map(xPos, XY_Low_Bound, 0, LONGEST_PULSE_WIDTH_US, SHORTEST_PULSE_WIDTH_US);
    xPulseWidth = calculateNewPulseWidth(xPulseWidth, controllerXPulseWidth);
  } else {
    sendXPulse = false;
    xPulseWidth = LONGEST_PULSE_WIDTH_US;
  }

  if (yPos >= XY_High_Bound) {
    sendYPulse = true;
    digitalWrite(Y_DIR_PIN, HIGH);
    controllerYPulseWidth = map(yPos, XY_High_Bound, 1024, LONGEST_PULSE_WIDTH_US, SHORTEST_PULSE_WIDTH_US);
    yPulseWidth = calculateNewPulseWidth(yPulseWidth, controllerYPulseWidth);
  } else if (yPos <= XY_Low_Bound) {
    sendYPulse = true;
    digitalWrite(Y_DIR_PIN, LOW);
    controllerYPulseWidth = map(yPos, XY_Low_Bound, 0, LONGEST_PULSE_WIDTH_US, SHORTEST_PULSE_WIDTH_US);
    yPulseWidth = calculateNewPulseWidth(yPulseWidth, controllerYPulseWidth);
  } else {
    sendYPulse = false;
    yPulseWidth = LONGEST_PULSE_WIDTH_US;
  }

  if (sendXPulse) {
    sendMotorPulse(X_STEP_PIN, xPulseWidth);
  }
  if (sendYPulse) {
    sendMotorPulse(Y_STEP_PIN, yPulseWidth);
  }
}

