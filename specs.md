1. Komponentenübersicht
• D1 mini (ESP8266)
• DHT11 Shield
• OLED Shield (0,66", SSD1306)
2. Schnittstellen / Kommunikation
• OLED:
- Kommunikationsschnittstelle:	I2C
- I2C Adresse:				0x3C/0x3D
- 
• DHT11:
- Kommunikationsschnittstelle:	I2C
- I2C Adresse:				0x3C/0x3D

3. Relevante technische Daten
OLED:
• OLED-Auflösung: 0.66" 64x48
• OLED-Treiber-IC: SSD1306
DH11:
- Arbeitsbereich Temperatur:		40-80Grad
- Arbeitsbereich Luftfeuchtigkeit:	0-100%RH
- Genauigkeit Temperatur:		<+-0,5Grad
- Genauigkeit Luftfeuchtigkeit:		+-2-5%
- Auflösung Temperatur:		0,1% Grad
- Auflösung Luftfeuchtigkeit:		0,1% RH

4. Funktionsanforderungen an den Prototyp
• fortlaufende Messung
• Anzeige auf OLED
• serielle Debug-Ausgabe (optional/empfohlen)
• Fehlerausgabe bei ungültiger Messung
5. Darstellungsanforderungen (Display)
• Welche Werte sollen angezeigt werden?
- darstellung von Temperatur und Luftfeuchtigkeit
• Mit welchen Einheiten?
- in °C und RH%
• Mit wie vielen Nachkommastellen?
- 2 nachkommastellen
• Wie wird mit dem begrenzten Displayplatz umgegangen
- 1. zeile Temperatur
- 2. zeile Wert
- 3. Zeile Humidity
- 4. Zeile Wert