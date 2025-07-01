#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Servo.h>

// Serial communication for DFPlayer
SoftwareSerial mySerial(10, 11); // Arduino TX, RX

DFRobotDFPlayerMini myDFPlayer;
Servo myServo;

// Pins
const int buttonPin = 2;
const int servoPin = 5;

// State variables
unsigned long buttonPressTime = 0;
bool isButtonHeld = false;
bool isPlaying = false;
int currentSong = 1;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Uses internal pull-up resistor

  mySerial.begin(9600);
  Serial.begin(9600);

  // Initialize DFPlayer
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("Unable to begin DFPlayer Mini");
    while (true); // Halt
  }

  myDFPlayer.volume(25); // volume 0-30

  // Initialize servo
  myServo.attach(servoPin);
  myServo.write(0); // Start at 0 deg
}

void loop() {
  static bool lastButtonState = HIGH;
  bool buttonState = digitalRead(buttonPin);

  // Button just pressed
  if (buttonState == LOW && lastButtonState == HIGH) {
    buttonPressTime = millis();
    isButtonHeld = false;
  }

  // Button held long enough
  if (buttonState == LOW && !isButtonHeld && (millis() - buttonPressTime > 700)) {
    isButtonHeld = true;

    if (isPlaying) {
      myDFPlayer.stop();
      isPlaying = false;
    } else {
      myDFPlayer.play(currentSong);
      isPlaying = true;

      // Special action for Song 2
      if (currentSong == 2) {
        myServo.write(180);
        delay(500);           // Allow servo to reach position
        myServo.write(0);     // Return to start
      }
    }
  }

  // Button released before 700ms = short press
  if (buttonState == HIGH && lastButtonState == LOW && !isButtonHeld) {
    if (!isPlaying) {
      currentSong++;
      if (currentSong > 3) currentSong = 1;
    }
  }

  lastButtonState = buttonState;
}
