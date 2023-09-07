#include <Servo.h>
Servo myservo;

// Define the analog input, LEDs, and switch pins
const int piezo = A0;
const int redLed = 5;
const int yellowLed = 4;
const int greenLed = 3;
const int switchPin = 2;

// Variables to store sensor readings and system state
int knockVal;
int switchVal;

// Thresholds for distinguishing quiet and loud knocks
const int quietKnock = 10;
const int loudKnock = 100;

boolean locked = false; // Boolean to represent whether the system is locked or not
int numberOfKnocks = 0; // Counter for the number of valid knocks

void setup() {
  // Setup code to run once when the Arduino starts
  myservo.attach(9); // Attach the servo to pin 9
  pinMode(yellowLed, OUTPUT); // Set yellow LED as an output
  pinMode(redLed, OUTPUT);    // Set red LED as an output
  pinMode(greenLed, OUTPUT);  // Set green LED as an output
  pinMode(switchPin, INPUT);  // Set switch pin as an input

  Serial.begin(9600); // Initialize serial communication with a baud rate of 9600

  digitalWrite(greenLed, HIGH); // Turn on the green LED
  myservo.write(0); // Set the servo to the unlocked position
  Serial.println("The door is unlocked"); // Print a message to the serial monitor
}

void loop() {
  // Main code to run repeatedly

  // Check if the system is unlocked
  if (locked == false) {
    switchVal = digitalRead(switchPin); // Read the state of the switch

    // If the switch is pressed, lock the system
    if (switchVal == HIGH) {
      locked = true; // Set the locked flag to true
      digitalWrite(greenLed, LOW); // Turn off the green LED
      digitalWrite(redLed, HIGH);  // Turn on the red LED
      myservo.write(90); // Move the servo to the locked position
      Serial.println("The door is locked"); // Print a message to the serial monitor
      delay(1000); // Wait for 1 second
    }
  }

  // If the system is locked, listen for knocks
  if (locked == true) {
    knockVal = analogRead(piezo); // Read the analog value from the piezo sensor

    // Check if the number of valid knocks is less than 3 and a knock is detected
    if (numberOfKnocks < 3 && knockVal > 0) {
      if (checkForKnock(knockVal)) {
        numberOfKnocks++; // Increment the count of valid knocks
      }
      Serial.print(3 - numberOfKnocks);
      Serial.println("Knocks away");
    }

    // If three valid knocks are detected, unlock the system
    if (numberOfKnocks >= 3) {
      locked = false; // Set the locked flag to false
      myservo.write(0); // Move the servo to the unlocked position
      delay(20); // Wait for a short time
      digitalWrite(greenLed, HIGH); // Turn on the green LED
      digitalWrite(redLed, LOW);   // Turn off the red LED
      Serial.println("The box is unlocked"); // Print a message to the serial monitor
      numberOfKnocks = 0; // Reset the count of valid knocks
    }
  }
}

// Function to check if a knock is valid
boolean checkForKnock(int value) {
  if (value > quietKnock && value < loudKnock) {
    digitalWrite(yellowLed, HIGH); // Turn on the yellow LED
    delay(50); // Wait for a short time
    digitalWrite(redLed, LOW); // Turn off the red LED
    Serial.println("Valid knock Value"); // Print a message to the serial monitor
    Serial.print(value); // Print the knock value to the serial monitor
    return true; // Return true to indicate a valid knock
  } else {
    Serial.println("Bad knock!"); // Print a message to the serial monitor
    Serial.print(value); // Print the knock value to the serial monitor
    return false; // Return false to indicate an invalid knock
  }
}
