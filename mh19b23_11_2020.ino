bool debug_mode = true;                //"true" fuer aktiviert, "false" für deaktiviert
String software_version = "1.3";       //Aktuelle Software-Version

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>
#include <SSD1306.h>
#include <SSD1306Brzo.h>        //Bibliothek für SSD1306-gesteuerte Displays, über I2C steuerbar
#include <SSD1306Wire.h>

#include <brzo_i2c.h>           //Bibliothek für eine I2C-Master Implementation
#include <MHZ.h>                //Bibliothek für den CO2-Sensor
#include <SoftwareSerial.h>     //Bibliothek für die serielle Schnittstelle zwischen ATMEGAs und UARTs

#define scl D1
#define sda D2
#define led_red D3                   //Pin fuer die rote LED
#define led_yellow D6                //Pin fuer die gelbe LED
#define led_green D5                 //Pin fuer die gruene LED
#define sound D0                     //Pin fuer die Piezo-Hupe
#define MH_Z19_RX D9                 //RX-Datenpin für den CO2-Sensor
#define MH_Z19_TX D10                //TX-Datenpin für den CO2-Sensor
#define CO2_IN D7                    //PWM-Datenpin für den CO2-Sensor

MHZ co2(MH_Z19_RX, MH_Z19_TX, CO2_IN, MHZ19B);         //Erstellen einer neuen Instanz des Co2-Sensors
SSD1306Brzo display(0x3c, sda, scl);                   //Erstellen einer neuen Instanz des OLED-Displays, Parameter: I2C-Adresse, SDA-Pin, SDL-Pin

int green  = 600;   //Grenzwert fuer die gruene LED
int red    = 1000;  //Grenzwert fuer die rote LED

int _co2;            //Aktueller CO2-Wert aus der loop()-Funktion
int _temperature;    //Aktuelle Temperatur aus der loop()-Funktion

bool wasSoundPlayed = false;

//#########################################################################################################################################
/*
  Funktion, welche waehrend des Aufwaermens des CO2-Sensors einen String an das
  OLED-Display sendet.
*/
void drawToDisplay() {
  display.drawString(0, 26, "Aufwärmen..." + String(180 - millis() / 1000));
}

/*
  Funktion, welche waehrend der Sensor vorheizt Informationen an das Display sendet
*/
void manageDisplay() {
  display.clear();    //Leert den Pixel-Buffer des Controllers
  display.display();  //Legt den aktuellen Pixel-Buffer im Speicher des Displays ab
  drawToDisplay();    //Fuehrt die Funktion "drawToDisplay()" aus
  display.display();  //Legt den aktuellen Pixel-Buffer im Speicher des Displays ab
  display.clear();    //Leert den Pixel-Buffer des Controllers
}
/*
  Gibt, je nach aktuellem CO2-Wert, einen Integer aus, welcher
  vorgibt, welche der 3 LEDs wann leuchten soll
  Undefiniert: 0
  Rot: 1
  Gelb: 2
  Gruen: 3
*/
int checkCO2(int current_co2) {
  if (current_co2 <= green) {                                                                        //Die gruene LED soll angesprochen werden
    return 3;
  } else if (current_co2 > green && current_co2 < red) {                                             //Die gelbe LED soll angesprochen werden
    return 2;
  } else if (current_co2 >= red) {                                                                   //Die rote LED soll angesprochen werden
    return 1;
  } else {                                                                                           //Die Eingabe war ungültig
    if (debug_mode == true) Serial.print("\nWert fuer LED-Anzeige konnte nicht zugeordnet werden");
    return 0;
  }


}

//#########################################################################################################################################

void greenLED() {                                     //Aktiviert die Befehle fuer die gruene LED
  if (debug_mode == true) Serial.println("Grün");   //Wenn der Debug-Modus aktiviert ist, wird die Farbe Gruen in der Konsole angezeigt
  digitalWrite(led_red, LOW);                       //Rote LED aus
  digitalWrite(led_yellow, LOW);                    //Gelbe LED aus
  digitalWrite(led_green, HIGH);                    //Gruene LED an
  display.clear();                                  //Leert den Pixel-Buffer des Controllers
  display.display();                                //Legt den aktuellen Pixel-Buffer im Speicher des Displays ab
  display.drawString(64, 22, "Gut. " );
  display.display();
  delay(1000);                                      //Warte 1s
}
void yellowLED() {                                    //Aktiviert die Befehle fuer die gelbe LED
  if (debug_mode == true) Serial.println("Gelb");   //Wenn der Debug-Modus aktiviert ist, wird die Farbe Gelb in der Konsole angezeigt
  digitalWrite(led_red, LOW);                       //Rote LED aus
  digitalWrite(led_yellow, HIGH);                   //Gelbe LED an
  digitalWrite(led_green, LOW);                     //Gruene LED aus
  display.clear();                                  //Leert den Pixel-Buffer des Controllers
  display.display();                                //Legt den aktuellen Pixel-Buffer im Speicher des Displays ab
  display.drawString(64, 22, " Achtung! ");
  display.display();
  wasSoundPlayed = false;
  delay(1000);                                      //Warte 1s
}
void redLED() {                                       //Aktiviert die Befehle fuer die rote LED
  if (debug_mode == true) Serial.println("Rot");    //Wenn der Debug-Modus aktiviert ist, wird die Farbe Gelb in der Konsole angezeigt
  digitalWrite(led_red, HIGH);                      //Rote LED an
  digitalWrite(led_yellow, LOW);                    //Gelbe LED aus
  digitalWrite(led_green, LOW);                     //Gruene LED aus
  display.clear();                                  //Leert den Pixel-Buffer des Controllers
  display.display();                                //Legt den aktuellen Pixel-Buffer im Speicher des Displays ab
  display.drawString(64, 22, " Lüften! " );
  display.display();
  delay(1000);                                      //Warte 1s
}
void yellowBlinkLED() {                               //Aktiviert die Befehle fuer die gelb-blinkende LED
  digitalWrite(led_red, LOW);                       //Rote LED aus
  digitalWrite(led_green, LOW);                     //Gruene LED aus
  digitalWrite(led_yellow, HIGH);                   //Gelbe LED an
  delay(500);                                       //Warte 0,5s
  digitalWrite(led_yellow, LOW);                    //Gelbe LED aus
  delay(500);                                       //Warte 0,5s
}
void playSound() {
  digitalWrite(sound, HIGH);                        //Hupe an
  delay(250);                                       //Warte 0,25s
  digitalWrite(sound, LOW);                         //Hupe aus
  delay(250);                                       //Warte 0,25s
}

//#########################################################################################################################################

void setup() {

  pinMode(led_red, OUTPUT);    //Pin D3 wird als Ausgang definiert
  pinMode(led_yellow, OUTPUT); //Pin D6 wird als Ausgang definiert
  pinMode(led_green, OUTPUT);  //Pin D5 wird als Ausgang definiert
  pinMode(CO2_IN, INPUT);      //Pin D7 wird als Eingang definiert
  pinMode(sound, OUTPUT);      //Pin D0 wird als Ausgang definiert

  display.init();                        //Initialisierung der Display-Instanz
  display.flipScreenVertically();        //Das Display wird softwarebasiert gedreht
  display.setFont(ArialMT_Plain_16);     //Eine Standart-Schriftart wird festgelegt


  if (debug_mode == true) {              //Ueberpruefe, ob das Programm im Debug-Modus läuft
    Serial.begin(9600);                //Geschwindigkeit für die Datenübertragung zum seriellen Monitor festlegen
    delay(100);
    Serial.print("Start des Programmes unter der Software-Version " + String(software_version));
  }

  if (co2.isPreHeating()) {                                      //Ueberpruefe, ob sich der CO2-Sensor im Moment aufwärmt
    if (debug_mode == true) Serial.println("\nAufwaermen..");  //In die serielle Konsole loggen, wenn der Debug-Modus aktiviert ist

    while (co2.isPreHeating()) {                               //Schleife ausfuehren, so lange sich der Sensor aufwärmt
      manageDisplay();                                       //manageDisplay()-Funktion wird aufgerufen
      yellowBlinkLED();                                      //yellowBlinkLED()-Funktion wird aufgerufen
    }
  }
}

//#########################################################################################################################################

void loop() {
  _co2         = co2.readCO2PWM();                  //Aktueller Wert des PWM-Ausgangs am CO2-Sensor
  _temperature = co2.getLastTemperature();          //Aktueller Wert der Temperatur am CO2-Sensor

  if (!co2.isPreHeating()) {
    /*
      Wenn der Debug-Modus aktiviert ist, werden in der seriellen Konsole
      Informationen über die aktuell am CO2-Sensor anliegenden Werte geliefert
    */
    if (debug_mode == true) {
      Serial.print("CO2: ");

      if (_co2 > 0) {
        Serial.print(_co2);
        Serial.println("ppm");
      } else {
        Serial.print("CO2-Wert nicht verfuegbar");
      }
    };

    switch (checkCO2(_co2)) {
      case 0:
        if (debug_mode == true) Serial.println("Unbekannter Wert im LED-Check gefunden");  //Wert = 0 bedeutet unbekannter Funktionsparameter in checkCO2
        break;

      case 1:
        redLED();                                                                          //Rote LED soll leuchten

        if (wasSoundPlayed == false) {
            for (int i = 0; i < 3; i++) {
                playSound();                                                               //Hupe soll angehen
            }
            wasSoundPlayed = true;
        }
        break;

      case 2:
        yellowLED();                                                                       //Gelbe LED soll leuchten
        break;

      case 3:
        greenLED();                                                                        //Gruene LED soll leuchten
        break;

      default:
        if (debug_mode == true) Serial.println("Falscher Wert in der Eingabe fuer die LED-Ansteuerung");
        break;
    }
  }
}
