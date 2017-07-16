#include <FastGPIO.h>
#include <APA102.h>
#define APA102_USE_FAST_GPIO



// Define which pins to use.
const uint8_t dataPin = 11;
const uint8_t clockPin = 12;

// Create an object for writing to the LED strip.
APA102<dataPin, clockPin> ledStrip;

// Create a buffer for holding 509 colors.
// This takes 1527 bytes and uses up most of the 2KB RAM on an Uno,
// so we should be very sparing with additional RAM use and keep
// an eye out for possible stack overflow problems.
#define LED_COUNT 31
rgb_color colors[LED_COUNT];

// Set the brightness to use (the maximum is 31).
const uint8_t standartBrightness = 20;
uint8_t globalBrightness = standartBrightness;

//serial Comunication
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

int befehl[6] = {0, 0, 0 ,0 , 0, 0}; //1. rot 2. grun 3. blau, 4. Helligkeit, 5. Sonderfunktionen 6.Dauer
int blinker = 1;
double  zeitDelta = 0;
double  zeitStop = 0;  
int loopCounter, rr, ss = 0;
int ledCounter = 0;
bool thomas = false;


/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

/* Converts a color from HSV to RGB.
 * h is hue, as a number between 0 and 360.
 * s is the saturation, as a number between 0 and 255.
 * v is the value, as a number between 0 and 255. */
rgb_color hsvToRgb(uint16_t h, uint8_t s, uint8_t v)
{
    uint8_t f = (h % 60) * 255 / 60;
    uint8_t p = (255 - s) * (uint16_t)v / 255;
    uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch((h / 60) % 6){
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    return (rgb_color){r, g, b};
}




void setup() {
  // put your setup code here, to run once:
  // all off intalize colors array
  for (int i = 0; i < LED_COUNT; i++)
    colors[i] = (rgb_color){0, 0, 0};
  ledStrip.write(colors, LED_COUNT, globalBrightness);
  
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  

}

void loop() {
  loopCounter++;
    // print the string when a newline arrives:
  if (stringComplete) 
  {
    Serial.println("Du hast das eingegeben: "+inputString);
    bool x =inputString.substring(0,10)=="24.07.1990";
    Serial.println(inputString.substring(0,10));
 Serial.println(x);
 if(x){
       thomas = true;   
    Serial.println(inputString[11]);
      Serial.println(inputString[13]);
  /*int(inputString[11])!=0?ss=int(inputString[11]):4;
    int(inputString[13])!=0?rr=int(inputString[13]):8;*/
      }
else {
        // String zerlegung
        thomas = false;
        for (int i = 0, befehlCounter = 0, dezimalCounter = 0; '\n'!=inputString[i]; i++){
          if ('.'!=inputString[i]){
            if (dezimalCounter == 0){
              befehl[befehlCounter] = 0;
              dezimalCounter = 1;
            }
            else
             befehl[befehlCounter] *= 10;
             
            //Serial.println(int(inputString[i])-48);
            befehl[befehlCounter] += (int(inputString[i])-48);
          }
          else {
            befehlCounter ++;
            dezimalCounter = 0;
          }
         }
         
       }

    if (befehl[3] == 0)
      globalBrightness = standartBrightness;
    else
      globalBrightness = int(float(befehl[3])*0.31);
      
    for (int i = 0; i < LED_COUNT; i++)
      colors[i] = (rgb_color){befehl[0], befehl[1], befehl[2]};
    
    // clear the string:
    inputString = "";
    stringComplete = false;
    blinker = 1;
  }

  
  if(thomas == true){
     uint8_t time = (millis() >> 4);//*ss;
     for(uint16_t i = 0; i < LED_COUNT; i++){
       uint8_t p = time - i * 8;// rr;
       colors[i] = hsvToRgb((uint32_t)p * 359 / 256, 255, 255);
     } 
   }
  else{
    
      if (0 < befehl[5]){
      zeitStop = millis() + befehl[5]*1000;
      befehl[5] = -1;
      }
      else {
        if ((zeitStop - millis() < 0) && (-1 == befehl[5])){
          befehl[4] = 0;
          befehl[5] = 0;
          blinker = 0;
        }
      }   
  
      if (40 < befehl[4]){
        if (zeitDelta - millis() < 0){
          if (blinker == 1)
            blinker = 0;
          else
            blinker = 1;
        zeitDelta = millis() + befehl[4];
        }
        
      }
      else if (0 < befehl[4] && loopCounter % (5*befehl[4]) == 0){  
        ledCounter ++; 
         for (int i = 0; i < LED_COUNT; i++){
          colors[i] = (rgb_color){0, 0, 0};
         }
        
        int i = 0;
        i = ledCounter % LED_COUNT;
        
        colors[i] = (rgb_color){befehl[0], befehl[1], befehl[2]};
      }

    }
    
  ledStrip.write(colors, LED_COUNT, globalBrightness*blinker);
}








