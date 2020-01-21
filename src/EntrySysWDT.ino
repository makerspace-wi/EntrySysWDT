/* started on 14Jan2020 - by Michael

Commands from RASPI
* serial commands no available now

Pulse from Entry Mini (Pin 13)
* decoding with ATTiny and Reset Entry Mini with RESET-PIN or power off with relais
* pulse length low:  1,5 sec
* pulse length high: 0,5 sec WDT is runnig
*                   RESET:
*                    > 2 sec LOW or > 1 sec HIGH --> Pin
*                    > 10 sec                    --> Relais
*
* last changes on 19.01.2020 by Michael Muehl
* changed: add externel watch dog with ATTiny
*/
#define Version "1.0"

#include <Arduino.h>

// Pin Assinements ATTiny x5 -------
#define IP_PULS 2   // [ATTiny 2 / Nano 2] input - watch dog pulse ()
#define IP_SERI 0   // [ATTiny 0 / Nano 3] input - Serial (RS232)

#define RES_REL 4   // [ATTiny 4 / Nano 8] output - reset over relais
#define RES_PIN 3   // [ATTiny 3 / Nano 9] output - reset over pin
#define LEDPuls 1   // [ATTiny 1 / Nano 10] output - puls shown with LED

#define PulsScan  1 // scan every 16,5ms
#define PulsHigh 30 // =495ms / PulsScan
#define PulsLow  91 // =1500ms / PulsScan
#define PulsTol  32 // tolerance + pulses

#define PulsReset 600 // = 9900 / PulsScan
#define maxNOPuls 625 // =~ 10313 / PulsScan

// VARIABLES
unsigned int wdTimeL =  0;  // count puls for high
unsigned int wdTimeH =  0;  // count puls for low
unsigned int wdtCount = 0;  // count periode
// time Control
unsigned long prev;         // ms previus time
boolean valN;               // new input value
boolean valO;               // old input value

int ledStatus =LOW;         // Status für die Blink-LED

void setup() {
  pinMode(RES_REL, OUTPUT);
  pinMode(RES_PIN, OUTPUT);
  pinMode(LEDPuls, OUTPUT);

  digitalWrite(RES_REL, LOW);
  digitalWrite(RES_PIN, HIGH);
  digitalWrite(LEDPuls, HIGH);

  prev = millis();
}

// PROGRAM --------------------------------------
void loop() {
  if ((millis() - prev) >= PulsScan) {
    if (!digitalRead(RES_PIN)) wdtCount =0;
    if (!digitalRead(LEDPuls)) digitalWrite(LEDPuls, HIGH);
    prev = millis();
    valN = digitalRead(IP_PULS);
    if (valN == LOW) {
      ++wdTimeL;
    } else {
      ++wdTimeH;
    }
    if (valN == LOW && valO == HIGH) {
      digitalWrite(LEDPuls, LOW);
      if (wdtCount > 5 && (wdTimeL > PulsLow + PulsTol || wdTimeH > PulsHigh + PulsTol)) {
        digitalWrite(RES_PIN, LOW);
        delay(PulsReset);
        digitalWrite(RES_PIN, HIGH);
        wdtCount = 0;
      }
      wdTimeL = 0;
      wdTimeH = 0;
      ++wdtCount;
    }
    valO = valN;
  }
  if (wdTimeL > maxNOPuls) {
    digitalWrite(LEDPuls, LOW);
    digitalWrite(RES_PIN, LOW);
    delay(PulsReset);
    digitalWrite(RES_PIN, HIGH);
    wdTimeL = 0;
    wdTimeH = 0;
    wdtCount = 0;
  }
  if (wdTimeH > maxNOPuls) {
    digitalWrite(LEDPuls, LOW);
    digitalWrite(RES_REL, HIGH);
    delay(PulsReset * 5);
    digitalWrite(RES_REL, LOW);
    wdTimeL = 0;
    wdTimeH = 0;
    wdtCount = 0;
  }
}
