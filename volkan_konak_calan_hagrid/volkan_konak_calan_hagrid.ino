#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySerial(10, 11); // TX, RX
DFRobotDFPlayerMini myDFPlayer;

const int buttonPin = 2;
unsigned long buttonPressTime = 0;
bool isButtonHeld = false;
bool isPlaying = false;
int currentSong = 1;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  mySerial.begin(9600);
  Serial.begin(9600);

  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("Unable to begin DFPlayer Mini");
    while (true);
  }

  myDFPlayer.volume(25); // Range 0-30
}

void loop() {
  static bool lastButtonState = HIGH;
  bool buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH) {
    // dugmeye bastik
    buttonPressTime = millis();
    isButtonHeld = false;
  }

  if (buttonState == LOW && !isButtonHeld && (millis() - buttonPressTime > 700)) {
    // uzun bas
    isButtonHeld = true;
    if (isPlaying) {
      myDFPlayer.stop();
      isPlaying = false;
    } else {
      myDFPlayer.play(currentSong);
      isPlaying = true;
    }
  }

  if (buttonState == HIGH && lastButtonState == LOW && !isButtonHeld) {
    // kisa bas
    if (!isPlaying) {
      currentSong++;
      if (currentSong > 3) currentSong = 1;
    }
  }

  lastButtonState = buttonState;
}
