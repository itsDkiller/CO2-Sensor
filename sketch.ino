bool debug_mode = true;
String software_version = "1.4";

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>
#include <SSD1306.h>
#include <SSD1306Brzo.h>
#include <SSD1306Wire.h>

#include <brzo_i2c.h>
#include <MHZ.h>
#include <SoftwareSerial.h>

#define scl D1
#define sda D2
#define led_red D3
#define led_yellow D6
#define led_green D5
#define sound D0
#define MH_Z19_RX D9
#define MH_Z19_TX D10
#define CO2_IN D7

MHZ co2(MH_Z19_RX, MH_Z19_TX, CO2_IN, MHZ19B);
SSD1306Brzo display(0x3c, sda, scl);

int green  = 600;
int red    = 1000;

int _co2;

bool wasSoundPlayed = false;

//#########################################################################################################################################

void drawToDisplay() {
  display.drawString(0, 26, "Preheating.." + String(180 - millis() / 1000));
}

void manageDisplay() {
  display.clear();
  display.display();
  drawToDisplay();
  display.display();
  display.clear();
}

int checkCO2(int current_co2) {
  if (current_co2 <= green) {
    return 3;
  } else if (current_co2 > green && current_co2 < red) {
    return 2;
  } else if (current_co2 >= red) {
    return 1;
  } else {
    if (debug_mode == true) Serial.print("\nInvalid CO2 value in the checkCO2() function");
    return 0;
  }


}

//#########################################################################################################################################

void greenLED() {
  if (debug_mode == true) Serial.println("Green");
  digitalWrite(led_red, LOW);
  digitalWrite(led_yellow, LOW);
  digitalWrite(led_green, HIGH);
  display.clear();
  display.display();
  display.drawString(64, 22, " Good. " );
  display.display();
  delay(1000);
}
void yellowLED() {
  if (debug_mode == true) Serial.println("Yellow");
  digitalWrite(led_red, LOW);
  digitalWrite(led_yellow, HIGH);
  digitalWrite(led_green, LOW);
  display.clear();
  display.display();
  display.drawString(64, 22, " Caution! ");
  display.display();
  wasSoundPlayed = false;
  delay(1000);
}
void redLED() {
  if (debug_mode == true) Serial.println("Red");
  digitalWrite(led_red, HIGH);
  digitalWrite(led_yellow, LOW);
  digitalWrite(led_green, LOW);
  display.clear();
  display.display();
  display.drawString(64, 22, " Ventilate! " );
  display.display();
  delay(1000);
}
void yellowBlinkLED() {
  digitalWrite(led_red, LOW);
  digitalWrite(led_green, LOW);
  digitalWrite(led_yellow, HIGH);
  delay(500);
  digitalWrite(led_yellow, LOW);
  delay(500);
}
void playSound() {
  digitalWrite(sound, HIGH);
  delay(250);
  digitalWrite(sound, LOW);
  delay(250);
}

//#########################################################################################################################################

void setup() {

  pinMode(led_red, OUTPUT);
  pinMode(led_yellow, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(CO2_IN, INPUT);
  pinMode(sound, OUTPUT);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);


  if (debug_mode == true) {
    Serial.begin(9600);
    delay(100);
    Serial.print("Starting the program with software version " + String(software_version));
  }

  if (co2.isPreHeating()) {
    if (debug_mode == true) Serial.println("\nPreheating..");

    while (co2.isPreHeating()) {
      manageDisplay();
      yellowBlinkLED();
    }
  }
}

//#########################################################################################################################################

void loop() {
  _co2         = co2.readCO2PWM();

  if (!co2.isPreHeating()) {
    if (debug_mode == true) {
      Serial.print("CO2: ");

      if (_co2 > 0) {
        Serial.print(_co2);
        Serial.println("ppm");
      } else {
        Serial.print("CO2 value not availble");
      }
    };

    switch (checkCO2(_co2)) {
      case 0:
        if (debug_mode == true) Serial.println("\nInvalid CO2 value in the switch statement, case 0");
        break;

      case 1:
        redLED();
        if (wasSoundPlayed == false) {
            for (int i = 0; i < 3; i++) {
                playSound();
            }
            wasSoundPlayed = true;
        }
        break;

      case 2:
        yellowLED();
        break;

      case 3:
        greenLED();
        break;

      default:
        if (debug_mode == true) Serial.println("\nInvalid CO2 value in the switch statement, case default");
        break;
    }
  }
}
