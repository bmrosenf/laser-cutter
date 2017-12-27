int x = A0;
int y = A1;
int xpos = 0;
int ypos = 0;
int dirPin = 4;
int stepPin = 3;
int MS1 = 0;
int MS2 = 1;
int MS3 = 2;

int delaySpeed = 1000;
int topSpeed = 400;
int rampSpeed = 3;
int stopSpeed = 8;
int range;
int speedSet;
int Speed;
// int off;
int trigger = 0;


void setup() {
//  Serial.begin(9600);
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  //pinMode(0, OUTPUT);
  //digitalWrite(MS1, HIGH);
  //digitalWrite(MS2, HIGH);
  //digitalWrite(MS3, HIGH);
}

void updateSpeed(int newSpeedSet) {
  int off = abs(Speed - speedSet);

  if (off !=  0 && Speed < speedSet) {
    Speed = Speed + rampSpeed;
  }
  else if (off !=  0 && Speed > speedSet) {
    Speed = Speed - rampSpeed;
  }
}

void motorStep(int pin) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(Speed);
    digitalWrite(pin, LOW);
    delayMicroseconds(Speed);
}

void loop() {
  xpos = analogRead(x)+5;
//  Serial.println(xpos);

  if (xpos >= 525) {
    digitalWrite(dirPin, HIGH);
    trigger = 1;
    speedSet = map(xpos, 550, 1024, delaySpeed, topSpeed);
//    Serial.print("HIGH speedSet=");
//    Serial.println(speedSet);
    updateSpeed(speedSet);
  }
  else if (xpos <= 475) {
    digitalWrite(dirPin, LOW);
    trigger = 1;
    speedSet = map(xpos, 500, 0, delaySpeed, topSpeed);
//    Serial.print("LOW speedSet=");
//    Serial.println(speedSet);
    updateSpeed(speedSet);
  }
  else {
    trigger = 0;
  }

  if (trigger == 1) {
    motorStep(stepPin);
  }
  else {
    if (Speed < delaySpeed) {
      Speed = Speed + stopSpeed;
      motorStep(stepPin);
    }
    else if (Speed >= delaySpeed) {
      Speed = delaySpeed;
    }
  }
}
