/*
    (c) 2020 codeA, C.A.

    THIS SOFTWARE IS SUPPLIED BY CODEA "AS IS". NO WARRANTIES,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
    INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
    MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS
    INTERACTION WITH CODEA PRODUCTS, COMBINATION WITH ANY OTHER
    PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL CODEA,C.A. BE LIABLE FOR ANY INDIRECT,
    SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS,
    DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
    SOFTWARE.

    CODEA,C.A. PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR
    ACCEPTANCE OF THESE TERMS.

    Author: Alberto Medrano (alberto@codeagroup.com)
*/

#include <Arduino.h>
#include <U8g2lib.h>

// U8g2 Contructor List (Frame Buffer)
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ A5, /* data=*/ A4, /* reset=*/ U8X8_PIN_NONE);

int butSTOP =  2;
int butSTART = A1;
int Buzzer = 4;

int encPinSW = 5;
int encPinA = 7;
int encPinB = 6;
//int encPos = 0;
int encPinALast = HIGH;
int n = LOW;

int ledVentON = A3;
int ledVentOFF = A2;

bool VentEnable = false;

// Constantes
int valBPM = 20;      // 10 to 40

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(butSTOP, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(butSTOP), forceSTOP, CHANGE);
  pinMode(butSTART, INPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);

  pinMode(encPinA, INPUT);
  pinMode(encPinB, INPUT);
  pinMode(encPinSW, INPUT);
  
  pinMode(ledVentON, OUTPUT);
  digitalWrite(ledVentON, HIGH);
  pinMode(ledVentOFF, OUTPUT);
  digitalWrite(ledVentOFF, HIGH);

  Serial.begin (9600);

  delay(10);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.sendBuffer();
  delay(2000);
  u8g2.setFont(u8g2_font_logisoso16_tf);
  u8g2.drawStr(0,16,"codeA, C.A.");
  u8g2.setFont(u8g2_font_lastapprenticebold_tr);
  u8g2.drawStr(0,32,"codeagroup.com");
  u8g2.sendBuffer();
  delay(2000);
  u8g2.clearBuffer();
  u8g2.sendBuffer();
  delay(10);

  digitalWrite(Buzzer, HIGH);
  delay(50);
  digitalWrite(Buzzer, LOW);
  delay(10);
  PrintValueControl();
}

void loop() {
// put your main code here, to run repeatedly:

  // LED Service
  if (VentEnable == true) {
    digitalWrite(ledVentON, LOW);
    digitalWrite(ledVentOFF, HIGH);
  } else {
    digitalWrite(ledVentON, HIGH);
    digitalWrite(ledVentOFF, LOW);
  }

  // START PushButtom Service
  if (digitalRead(butSTART) == LOW) {
    VentEnable = true;
  }

  // ENCODER Service
  n = digitalRead(encPinA);
  if ((encPinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encPinB) == LOW) {
// - - - - - - - - - - - - - - - - - - - - - - - - - -
//    Pos--;
      if (valBPM != 10) {
        valBPM--;
      }
// - - - - - - - - - - - - - - - - - - - - - - - - - -
    } else {
// - - - - - - - - - - - - - - - - - - - - - - - - - -
//    Pos++;
      if (valBPM != 40) {
        valBPM++;
      }
// - - - - - - - - - - - - - - - - - - - - - - - - - -
    }
    digitalWrite(Buzzer, HIGH);
    delay(2);
    digitalWrite(Buzzer, LOW);
    PrintValueControl();
  }
  encPinALast = n;
}

void PrintValueControl() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_logisoso32_tf);
  u8g2.drawStr(0,32,"BPM:");
  u8g2.setFont(u8g2_font_logisoso32_tf);
  u8g2.setCursor(80, 32);
  u8g2.print(valBPM);
  u8g2.sendBuffer();
}

void forceSTOP() {
  VentEnable = false;
}
