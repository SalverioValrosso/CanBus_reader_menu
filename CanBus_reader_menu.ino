


/*


    ************       GND ----- GND
    *  I2C LCD *       VCC ----- 5V
    ************       SDA ----- A4
                       SCL ----- A5



                        CS -------- 10CA
**********************  DIN ------- 11
  MONITOR 7 SEGMENTI *  CLK ------- 13
**********************  VCC ------- 5V
                        GND ------- GND




    ************        GND ------- GND
      PULSANTE
    ************        VCC ------- 2



*/

#include <Wire.h>

// Cooking API libraries
#include <arduinoUtils.h>
 
// Include always these libraries before using the CAN BUS functions
#include <arduinoCAN.h>
#include <SPI.h>

// Create an instance
CAN myCAN = CAN();

#include <LiquidCrystal_I2C.h>
#include "LedControl.h"
#include <HCMAX7219.h>
#include "SPI.h"
#define loadtime 50
#define NUMMENUS 4
#define buttonPin 2
#define ledPin 9
#define tempoanim 20
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LedControl lc = LedControl(11, 13, 10, 1);

int flag = 0;

  void settings(){
  // Inits the UART
  Serial.begin(115200);
  delay(100);
  
  // Print init message 
  Serial.println("Initializing CANBUS...");  
  
  // Configuring the BUS at 500 Kbit/s
  // Only allowed in SOCKET 0
  myCAN.begin(500);   
    
  Serial.println("CANBUS initialized at 500 KBits/s");    
  }

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  lcd.begin(20, 4);
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 10);
  /* and clear the display */
  lc.clearDisplay(0);


settings();
  
}



int select = 0;
int buttonState = 0;

bool buttonlisten() {

  buttonState = digitalRead(buttonPin);

  if (buttonState == 0) {
    delay(80);
    select++;
    select = select % NUMMENUS;
    if (select == 0) {
      animation();
    }
    else         {
      animation();
    }

    return 1;
  }
  return 0;
}


void animation() {
  HCMAX7219 HCMAX7219(10);
  for (int a = 8;; a--) {

    HCMAX7219.Clear();
    HCMAX7219.print7Seg("-", a);
    delay(tempoanim);
    HCMAX7219.Refresh();
    if (a == 0) {
      break;
    }
  }

}

void loading() {
  flag = 1;
  for (int l = 0; l < 5; l++) {
    int c = 0;
    for (int a = 7;; a--) {
      //7 led, in ordine accesi o spenti
      lc.setRow(0, a, B1000000);
      lc.setRow(0, c, B1000000);

      c++;
      delay(loadtime);
      lc.clearDisplay(0);
      if (a == 0) {
        break;
      }
    }


    lc.setRow(0, 0, B0100000);
    lc.setRow(0, 7, B0000010);
    delay(loadtime);
    lc.clearDisplay(0);

    lc.setRow(0, 0, B0010000);
    lc.setRow(0, 7, B0000100);
    delay(loadtime);
    lc.clearDisplay(0);
    c = 6;
    for (int a = 0; a < 7; a++) {
      lc.setRow(0, a, B0001000);
      lc.setRow(0, c, B0001000);
      c--;
      delay(loadtime);
      lc.clearDisplay(0);
    }
    lc.setRow(0, 0, B0010000);
    lc.setRow(0, 7, B0000100);
    delay(loadtime);
    lc.clearDisplay(0);

    lc.setRow(0, 0, B0100000);
    lc.setRow(0, 7, B0000010);
    delay(loadtime);
    lc.clearDisplay(0);

  }

  for (int l = 0; l < 4; l++) {
    lc.setRow(0, 0, B1111000);
    lc.setRow(0, 7, B1001110);
    delay(90);
    lc.clearDisplay(0);
    lc.setRow(0, 0, B0000000);
    lc.setRow(0, 7, B0000000);
    delay(90);
    lc.clearDisplay(0);
  }
  lc.setRow(0, 0, B1111000);
  lc.setRow(0, 7, B1001110);
  delay(410);
  lc.clearDisplay(0);
}



void speedview() {
  HCMAX7219 HCMAX7219(10);
  lcd.setCursor(0,0);
  lcd.print("ciao");
  
  
  for (int a = 10; a <= 310; a++) {
    HCMAX7219.Clear();



    if (a < 100) {
      HCMAX7219.print7Seg(a, 2);
    }
    else {
      HCMAX7219.print7Seg(a, 3);
    }
    delay(28);
    
    HCMAX7219.Refresh();
    if (buttonlisten()) {
      return;
    }
  }
  lcd.clear();
}



void fuelview() {
  char text[10];
  HCMAX7219 HCMAX7219(10);
  for (float a = 3.50; ; a = a - 0.0001) {
    String texto = "fuel";
    texto = texto + a;
    texto.toCharArray(text, 10);
    Serial.println(text);
    HCMAX7219.Clear();

    HCMAX7219.print7Seg(text, 8);

    HCMAX7219.Refresh();
    texto = "";
    if (buttonlisten()) {
      return;
    }
    delay(28);
  }
}


void oilview() {
  char text[10];
  HCMAX7219 HCMAX7219(10);
  for (float a = 2.70; ; a = a - 0.0001) {
    String texto = "OIL ";
    texto = texto + a;
    texto.toCharArray(text, 10);
    Serial.println(text);
    HCMAX7219.Clear();

    HCMAX7219.print7Seg(text, 8);

    HCMAX7219.Refresh();
    texto = "";
    if (buttonlisten()) {
      return;
    }
    delay(28);
  }
}



void rpmview() {
  char text[10];
  HCMAX7219 HCMAX7219(10);
  for (int a = 1000; a <= 5800; a = a + 50) {
    String texto = "Rpm ";
    texto = texto + a;
    texto.toCharArray(text, 10);
    Serial.println(text);
    HCMAX7219.Clear();

    HCMAX7219.print7Seg(text, 8);
    delay(14);
    HCMAX7219.Refresh();
    texto = "";
    if (buttonlisten()) {
      return;
    }

  }
}


void loop() {
  if (flag == 0) {
    loading();
  
  
  HCMAX7219 HCMAX7219(10);

  //lettura pulsante

  Serial.println(select);


  switch (select) {
    case 0: speedview();
      break;
    case 1: rpmview();
      break;
    case 2: fuelview();
      break;
    case 3: oilview();
      break;
    default: break;
  }


}
}
