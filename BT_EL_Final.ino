#include <LiquidCrystal.h>
#include "DHT.h"

// Define the pins for the LCD
#define LCD_RS PB3
#define LCD_EN PB4
#define LCD_D4 PB5
#define LCD_D5 PB6
#define LCD_D6 PB7
#define LCD_D7 PB8

// Define the pin where the DHT11 is connected
#define DHTPIN PA0
#define DHTTYPE DHT11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Pins for switches
const int vegSwitchPin = PA3;
const int fruitSwitchPin = PA2;
const int resetSwitchPin = PA0;
const int showTempHumiditySwitchPin = PA1;

// Pins for relays
const int relayFanPin = PB0; // Pin for controlling the fan relay
const int relayHeaterPin = PB1; // Pin for controlling the heater relay

// Define LCD object
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Mode selection variables
enum Mode { SELECT_TYPE, SELECT_ITEM, DISPLAY_TEMP_HUMIDITY, DISPLAY_IDEAL_TEMP, COMPARE_TEMP };
Mode currentMode = SELECT_TYPE;
int itemType = 0; // 0 for Vegetable, 1 for Fruit
int currentSelection = 0;
unsigned long lastPressTime = 0;
unsigned long displayStartTime = 0;

// Ideal temperatures and humidity levels for vegetables and fruits
const int vegTemperatures[] = {7, 80, 30, 51, 90};
const int fruitTemperatures[] = {4, 29, 74, 10, 60};
const float vegHumidity[] = {10, 55, 90, 50, 45};
const float fruitHumidity[] = {53, 15, 90, 28, 50};

// Names of vegetables and fruits
const char* vegetables[] = {"Carrot", "Potato", "Tomato", "Onion", "Lettuce"};
const char* fruits[] = {"Apple", "Banana", "Orange", "Grapes", "Mango"};

float getIdealHumidity() {
  return (itemType == 0) ? vegHumidity[currentSelection] : fruitHumidity[currentSelection];
}

int getItemCount() {
  return (itemType == 0) ? sizeof(vegTemperatures) / sizeof(vegTemperatures[0]) : sizeof(fruitTemperatures) / sizeof(fruitTemperatures[0]);
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(vegSwitchPin, INPUT_PULLUP);
  pinMode(fruitSwitchPin, INPUT_PULLUP);
  pinMode(resetSwitchPin, INPUT_PULLUP);
  pinMode(showTempHumiditySwitchPin, INPUT_PULLUP);

  pinMode(relayFanPin, OUTPUT);
  pinMode(relayHeaterPin, OUTPUT);
  digitalWrite(relayFanPin, LOW); // Ensure relays are off initially
  digitalWrite(relayHeaterPin, LOW);

  dht.begin();
  displayType();
}

void loop() {
  int vegSwitchState = digitalRead(vegSwitchPin);
  int fruitSwitchState = digitalRead(fruitSwitchPin);
  int resetSwitchState = digitalRead(resetSwitchPin);
  int showTempHumiditySwitchState = digitalRead(showTempHumiditySwitchPin);
  unsigned long currentTime = millis();

  if (vegSwitchState == LOW) {
    if (currentMode == SELECT_TYPE) {
      itemType = 0; 
      currentMode = SELECT_ITEM;
      currentSelection = 0;
      displayItem();
    } else if (currentMode == SELECT_ITEM && itemType == 0) {
      currentSelection = (currentSelection + 1) % getItemCount();
      displayItem();
    }
    lastPressTime = currentTime;
  }

  if (fruitSwitchState == LOW) {
    if (currentMode == SELECT_TYPE) {
      itemType = 1; 
      currentMode = SELECT_ITEM;
      currentSelection = 0;
      displayItem();
    } else if (currentMode == SELECT_ITEM && itemType == 1) {
      currentSelection = (currentSelection + 1) % getItemCount();
      displayItem();
    }
    lastPressTime = currentTime;
  }

  if (resetSwitchState == LOW) {
    resetDevice();
  }

  if (showTempHumiditySwitchState == LOW) {
    currentMode = DISPLAY_TEMP_HUMIDITY;
    unsigned long startTime = millis();
    while (millis() - startTime < 4000) { // Show temp and humidity for 4 seconds
      displayTempHumidity();
      delay(200); // Update display every 200 milliseconds
    }
    currentMode = SELECT_TYPE;
    displayType();
  }

  if (currentMode == SELECT_ITEM && (millis() - lastPressTime) >= 3000) {
    currentMode = DISPLAY_IDEAL_TEMP;
    displayStartTime = millis();
    displayIdealTemp();
  }

  if (currentMode == DISPLAY_IDEAL_TEMP && millis() - displayStartTime >= 4000) {
    currentMode = COMPARE_TEMP;
  }

  if (currentMode == COMPARE_TEMP) {
    monitorHumidity();
    delay(5000); // Allow time for the user to see the comparison result
    currentMode = SELECT_TYPE;
    displayType();
  }

  delay(100);
}

void displayType() {
  lcd.clear();
  lcd.print("Select:");
  lcd.setCursor(0, 1);
  lcd.print("Vegetable/Fruit");
}

void displayItem() {
  lcd.clear();
  lcd.print("Select item:");
  lcd.setCursor(0, 1);
  if (itemType == 0) {
    lcd.print(vegetables[currentSelection]);
  } else {
    lcd.print(fruits[currentSelection]);
  }
}

void displayIdealTemp() {
  lcd.clear();
  lcd.print("Ideal Temp: ");
  if (itemType == 0) {
    lcd.print(vegTemperatures[currentSelection]);
  } else {
    lcd.print(fruitTemperatures[currentSelection]);
  }
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(getIdealHumidity(), 1);
}

void displayTempHumidity() {
  lcd.clear();
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    lcd.print("Read Error!");
    return;
  }

  lcd.print("Temp: ");
  lcd.print(t, 1);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(h, 1);
  lcd.print(" %");
}

void monitorHumidity() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    lcd.clear();
    lcd.print("Read Error!");
    return;
  }

  int idealTemperature = (itemType == 0) ? vegTemperatures[currentSelection] : fruitTemperatures[currentSelection];
  float idealHumidity = getIdealHumidity();

  lcd.clear();
  lcd.setCursor(0, 0);

  // Display temperature verdict
  if (t < idealTemperature - 2) {
    lcd.print("Move to warmer place");
  } else if (t > idealTemperature + 2) {
    lcd.print("Move to cooler place");
  } else {
    lcd.print("Keep here");
  }

  // Move to the second row for humidity verdict
  lcd.setCursor(0, 1);

  // Control relays based on humidity
  if (h < idealHumidity - 5) {
    lcd.print("Turn on fan");
    digitalWrite(relayFanPin, HIGH); // Turn on fan
    digitalWrite(relayHeaterPin, LOW); // Ensure heater is off
  } else if (h > idealHumidity + 5) {
    lcd.print("Turn on heater");
    digitalWrite(relayFanPin, LOW); // Ensure fan is off
    digitalWrite(relayHeaterPin, HIGH); // Turn on heater
  } else {
    lcd.print("No action required");
    digitalWrite(relayFanPin, LOW); // Ensure fan is off
    digitalWrite(relayHeaterPin, LOW); // Ensure heater is off
  }
}

void resetDevice() {
  currentMode = SELECT_TYPE;
  lcd.clear();
  displayType();
}
