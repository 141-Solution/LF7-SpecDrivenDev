# LF7 – Spec Driven Development

Arduino-Prototyp für den D1 mini (ESP8266) zur Messung und Anzeige von Temperatur und Luftfeuchtigkeit über einen DHT11-Sensor und ein 0,66" OLED-Display (SSD1306).

## Komponenten

| Komponente | Details |
|---|---|
| Mikrocontroller | D1 mini (ESP8266) |
| Sensor | DHT11 Shield |
| Display | OLED Shield 0,66" SSD1306 (64×48 px) |

## Schnittstellen

- **OLED** – I2C, Adresse `0x3C` / `0x3D`
- **DHT11** – Single-Wire-Digitalinterface, Pin `D4`

## Funktionen

- Fortlaufende Messung alle 2 Sekunden
- Anzeige auf dem OLED-Display (Temperatur & Luftfeuchtigkeit, je 2 Nachkommastellen)
- Serielle Debug-Ausgabe (115200 Baud)
- Fehlerausgabe bei ungültiger Messung

## Display-Layout

```
Temp:
XX.XX °C
Humidity:
XX.XX %RH
```

## Abhängigkeiten (Arduino Libraries)

- [Adafruit SSD1306 Wemos Mini OLED](https://github.com/stblassitude/Adafruit_SSD1306_Wemos_OLED) by Adafruit
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library) by Adafruit
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library) by Adafruit

## Installation

1. Arduino IDE öffnen
2. Libraries über den Library Manager installieren (siehe Abhängigkeiten)
3. Board: **LOLIN(WEMOS) D1 mini** auswählen
4. `main.ino` öffnen und auf den D1 mini hochladen

## Team

Made by **team 141-solution**

## Lizenz

[MIT](LICENSE)
