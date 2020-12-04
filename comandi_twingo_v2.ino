//Emanuele Manzoni
//in this sript we read renault steering command as a matrix and we send a signal to a preset pin
//pins control npn transistor which close the circuit with a correct resistance value
//it's a good idea to use a digital potentiomeeter instead



byte L[3] = {2, 3, 4};  //rows L0,L1,L2
byte IN[3] = {5, 6, 7}; //cols IN0, IN1, IN2
byte lastRotary = 0;   //to be able to understand which way rotary is rotating
byte output[3][3] = {
  {8, 9, 10},
  {11, 12, 13},
  {14, 15, 16}
};

void next() {                               //function for next track
  Serial.print("up ");
  //digitalWrite(output[2][2], HIGH);
  delay(10000);
  //digitalWrite(output[2][2], LOW);
}

void prev() {                               //function for previous track
  Serial.print("down");
  //digitalWrite(output[2][0], HIGH);
  //delay(1000);
  //digitalWrite(output[2][0], LOW);
}

void ordinary(byte i) {                     //function for buttons (L0, L1)
  for (int j = 0; j < 3; j++) {
    delayMicroseconds(5);
    if (digitalRead(IN[j]) == 0) {
      digitalWrite(output[i][j], HIGH);
      Serial.print("\n output:");
      Serial.print(output[i][j]);
      while (digitalRead(IN[j]) == LOW)
        delay(40);
    }
  }
}

void rotary() {                             //function for rotary, not the cleanest but it works
  byte temp = lastRotary;
  for (int j = 0; j < 3; j++) {
    delayMicroseconds(5);
    if (digitalRead(IN[j]) == 0)
      temp = j;
  }
  if (temp != lastRotary) {
    switch (lastRotary) {
      case 0:
        if (temp == 1)
          next();
        else if (temp == 2)
          prev();
        break;
      case 1:
        if (temp == 2)
          next();
        else if (temp == 0)
          prev();
        break;
      case 2:
        if (temp == 0)
          next();
        else if (temp == 1)
          prev();
        break;
      default: break;

    }
    for (int j = 0; j < 3; j++) {         // we scan again before saving lastRotary in case it changed during next() or prev()
      delayMicroseconds(5);
      if (digitalRead(IN[j]) == 0)
        lastRotary = j;
    }
  }
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 3; i++)           //set all INx lines high, we'll use low for reading
    pinMode(IN[i], INPUT_PULLUP);

  for (int i = 0; i < 3; i++) {         //set all Lx lines high
    pinMode(L[i], OUTPUT);
    digitalWrite(L[i], HIGH);
  }

  for (int i = 0; i < 3; i++) {         //set all output low
    for (int j = 0; j < 3; j++) {
      pinMode(output[i][j], OUTPUT);
      digitalWrite(output[i][j], LOW);
    }
  }
}

void loop() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(L[i], LOW);
    if (i == 2) {
      rotary();
    }
    else {
      ordinary(i);
    }
    digitalWrite(L[i], HIGH);
  }


}
