// ============================================================================
// Arduino Programm für D1 mini (ESP8266) mit DHT11 und OLED-Display
// Messung von Temperatur und Luftfeuchtigkeit mit Anzeige auf 0.66" SSD1306
// ============================================================================

// Einfügen der erforderlichen Bibliotheken
#include <Wire.h>                    // I2C Kommunikationsbibliothek
#include <Adafruit_SSD1306.h>        // SSD1306 OLED Display Treiber
#include <Adafruit_GFX.h>            // Graphics Library für Display
#include "DHT.h"                     // DHT Sensor Bibliothek

// ============================================================================
// KONFIGURATION DER HARDWAREKOMPONENTEN
// ============================================================================

// OLED Display Konfiguration
#define SCREEN_WIDTH 64              // OLED Display Breite in Pixeln
#define SCREEN_HEIGHT 48             // OLED Display Höhe in Pixeln
#define OLED_RESET -1                // OLED Reset Pin (nicht verwendet, -1)
#define OLED_ADDRESS 0x3C            // I2C Adresse des OLED Displays (0x3C oder 0x3D)

// DHT11 Sensor Konfiguration
#define DHTPIN D4                    // DHT11 Datenleitung an GPIO2 (D4) angeschlossen
#define DHTTYPE DHT11                // Sensortyp: DHT11

// I2C Pins für ESP8266 (D1 mini)
#define SDA_PIN D2                   // GPIO4 - Serial Data Line (I2C)
#define SCL_PIN D1                   // GPIO5 - Serial Clock Line (I2C)

// ============================================================================
// OBJEKTDEKLARATIONEN
// ============================================================================

// Erstelle OLED Display Objekt mit der Adresse OLED_ADDRESS
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Erstelle DHT11 Sensor Objekt am Pin DHTPIN
DHT dht(DHTPIN, DHTTYPE);

// ============================================================================
// GLOBALE VARIABLEN
// ============================================================================

// Speichervariablen für Sensor-Messwerte
float temperatur = 0.0;              // Aktuelle Temperaturmessung in °C
float luftfeuchtigkeit = 0.0;        // Aktuelle Luftfeuchtemessung in %RH

// Timing-Variable für Messzyklus
unsigned long letzteMessung = 0;     // Zeitstempel der letzten Messung
const unsigned long MESS_INTERVALL = 2000; // Messzyklus: 2000ms (2 Sekunden)

// ============================================================================
// SETUP - INITIALISIERUNG BEI PROGRAMMSTART
// ============================================================================

void setup() {
  // Serielle Kommunikation starten für Debug-Ausgaben (115200 Baud)
  Serial.begin(115200);
  
  // Kurze Verzögerung für Stabilität
  delay(1000);
  
  // Debug-Ausgabe: Programm startet
  Serial.println("\n\n=== Programm Start ===");
  Serial.println("Initialisiere D1 mini mit DHT11 und OLED Display");
  
  // I2C Bus mit benutzerdefinierten Pins initialisieren
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println("I2C Bus initialisiert");
  
  // OLED Display initialisieren
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    // Fehlerbehandlung: Display konnte nicht initialisiert werden
    Serial.println("FEHLER: SSD1306 Display konnte nicht initialisiert werden!");
    
    // Zeige Fehler auf dem Display an (falls Display funktioniert)
    while (1) {
      delay(1000);
      Serial.println("OLED Fehler - Bitte Hardware überprüfen");
    }
  }
  
  // Display Grundeinstellungen
  display.clearDisplay();             // Lösche alle Pixel vom Display
  display.setTextSize(1);             // Schriftgröße: 1 (normal)
  display.setTextColor(SSD1306_WHITE); // Schriftfarbe: Weiß
  display.setCursor(0, 0);            // Setze Cursor auf Position (0,0)
  
  // Zeige Initialisierungsmeldung
  display.println("Initalisierung...");
  display.display();                  // Schreibe Änderungen auf das Display
  
  Serial.println("OLED Display initialisiert");
  
  // DHT Sensor initialisieren
  dht.begin();
  
  // Debug-Ausgabe: Initialisierung abgeschlossen
  Serial.println("DHT11 Sensor initialisiert");
  Serial.println("Starte fortlaufende Messung...");
  Serial.println("=== Initialisierung abgeschlossen ===\n");
  
  // Kleine Verzögerung vor Programm-Start
  delay(1000);
}

// ============================================================================
// HAUPTPROGRAMM - WIRD KONTINUIERLICH AUSGEFÜHRT
// ============================================================================

void loop() {
  // Prüfe ob Messzyklus-Intervall abgelaufen ist
  if (millis() - letzteMessung >= MESS_INTERVALL) {
    // Aktualisiere Zeitstempel der letzten Messung
    letzteMessung = millis();
    
    // Führe Sensor-Messung durch
    messung_durchfuehren();
    
    // Aktualisiere OLED Display mit neuen Messwerten
    display_aktualisieren();
    
    // Gebe Messwerte auf serieller Konsole aus
    serial_debug_ausgabe();
  }
}

// ============================================================================
// FUNKTIONEN
// ============================================================================

/**
 * Funktion: messung_durchfuehren()
 * Beschreibung: Liest Temperatur und Luftfeuchtigkeit vom DHT11 Sensor
 * Fehlerbehandlung: Prüft auf ungültige Messwerte
 */
void messung_durchfuehren() {
  // Lese Luftfeuchtigkeit vom DHT11 Sensor (in % RH)
  luftfeuchtigkeit = dht.readHumidity();
  
  // Lese Temperatur vom DHT11 Sensor (in °C)
  temperatur = dht.readTemperature();
  
  // Prüfe ob Messwerte gültig sind (DHT gibt NaN zurück bei Fehler)
  if (isnan(temperatur) || isnan(luftfeuchtigkeit)) {
    // Fehlerausgabe: Ungültige Messwerte
    Serial.println("FEHLER: DHT11 Messwert ungültig - bitte Sensor überprüfen");
    
    // Setze Fehler-Kennzeichnung (optional: könnte auch auf Display angezeigt werden)
    temperatur = 0.0;
    luftfeuchtigkeit = 0.0;
  }
}

/**
 * Funktion: display_aktualisieren()
 * Beschreibung: Zeigt Temperatur und Luftfeuchtigkeit auf dem 64x48 OLED Display
 * Layout: textSize 1 (6x8px pro Zeichen, 6 Zeilen verfuegbar)
 *   y= 4: "Temp:" Label
 *   y=12: Temperaturwert z.B. " 23.5 C"
 *   y=23: Trennlinie
 *   y=28: "Humi:" Label
 *   y=36: Feuchtigkeitswert z.B. " 45.2 %"
 */
void display_aktualisieren() {
  // Lösche den kompletten Display-Puffer
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);             // Schriftgröße 1: 6px breit, 8px hoch

  // --- Temperatur Label ---
  display.setCursor(17, 4);           // "Temp:" zentriert (5*6=30px → x=(64-30)/2≈17)
  display.print("Temp:");

  // --- Temperaturwert ---
  char tempBuf[11];
  dtostrf(temperatur, 1, 1, tempBuf); // z.B. "23.5"
  strcat(tempBuf, " C");              // z.B. "23.5 C"
  int tX = (SCREEN_WIDTH - (int)strlen(tempBuf) * 6) / 2;
  display.setCursor(tX, 14);
  display.print(tempBuf);

  // --- Trennlinie ---
  display.drawFastHLine(4, 23, SCREEN_WIDTH - 8, SSD1306_WHITE);

  // --- Luftfeuchte Label ---
  display.setCursor(17, 26);          // "Humi:" zentriert
  display.print("Humi:");

  // --- Feuchtigkeitswert ---
  char humiBuf[11];
  dtostrf(luftfeuchtigkeit, 1, 1, humiBuf); // z.B. "45.2"
  strcat(humiBuf, " %");                     // z.B. "45.2 %"
  int hX = (SCREEN_WIDTH - (int)strlen(humiBuf) * 6) / 2;
  display.setCursor(hX, 36);
  display.print(humiBuf);

  // Änderungen auf das physische Display schreiben
  display.display();
}

/**
 * Funktion: serial_debug_ausgabe()
 * Beschreibung: Gibt aktuelle Messwerte auf der seriellen Konsole aus
 * Format: Zeitstempel, Temperatur, Luftfeuchtigkeit
 */
void serial_debug_ausgabe() {
  // Gebe Trennlinie aus
  Serial.print("---");
  
  // Gebe aktuellen Zeitstempel aus (in Millisekunden seit Programmstart)
  Serial.print(" Zeit: ");
  Serial.print(millis());
  Serial.print("ms");
  
  // Gebe gemessene Temperatur mit 2 Nachkommastellen aus
  Serial.print(" | Temperatur: ");
  Serial.print(temperatur, 2);
  Serial.print("°C");
  
  // Gebe gemessene Luftfeuchtigkeit mit 2 Nachkommastellen aus
  Serial.print(" | Luftfeuchtigkeit: ");
  Serial.print(luftfeuchtigkeit, 2);
  Serial.println("%RH");
}

// ============================================================================
// ENDE DES PROGRAMMS
// ============================================================================
