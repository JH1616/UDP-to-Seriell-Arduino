# UDP-to-Seriell-Arduino

# Anschlussbelegung:
  GND(Ground)     - gemeinsamer GND mit Arduino
  CI(ClockInput)  - clockPin vom Ardunio siehe Leonie_Kopf.ino (11)
  DI(DataInput)   - dataPin vom Ardunio siehe Leonie_Kopf.ino (12)
  5V              - 5V von extrener Stromversorgung oder bei so kurzem 
                    Streifen direkt vom Arduino


# Troubleshoting:
- COM-Port überprüfen:
  Unter Linux in /dev/ die Liste vergleichen mit angestecktem arduino und nicht 
  angestecktem. Achtung Python skript sucht zwischen COM 1-10 (weil 0 immer mit
  einem anderen Gerät belegt war.
  Unter Windows Geräte manager...
   
- RGB-Strip überprüfen:
  Arduino IDE Laden(Google); APA-Bibliothek einbinden(diese Git), Arduino IDE 
  Neustarten; Menüpunkt Datei -> Beispiele -> APA102 beispielprogramm aufspielen
  und testen; Danach Leonie_Kopf.ino wieder aufspielen.
  
- Spannungsversorgung testen + zu GND sollten 5V haben.
   
