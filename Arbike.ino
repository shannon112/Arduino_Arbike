//Arbike v1.0
//A basic demo used leds to simulation
#include <Wire.h>  // Arduino IDE 內建
#include <LiquidCrystal_I2C.h>

////////// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


///////////////////////////////////////////////////////////////
////////////////////////////Arduino Pin//////////////////////
///////////////////////////////////////////
//digital
#define leftbot 2 //接Pin2，接無段式開關，然後接地
#define rightbot 3 //接Pin3，接無段式開關，然後接地
#define LEDPower 4 //接給LED Matrix Vcc
//#define LCDPower 5 //接給LCD Vcc  >>直接接給5V
#define BACKPower 6 //接給後燈 Vcc
//#define PIRread 7 //接PIR OUTPUT
#define TRIGPIN 5 
#define ECHOPIN 7
#define rightLEDPower 8 //接右方向燈的Vcc(前＋後）
#define leftLEDPower 9 //接右方向燈的Vcc(前＋後）
#define Autobot 10 //接Pin10，接有段式開關，然後接地
#define LEDbot 13 //開關LED表情電源 接Pin13，接有段式開關，然後接地
#define BACKbot 12 //開關BACK電源  接Pin12，接有段式開關，然後接地
#define LCDbot 11 //開關LCD電源  接Pin11，接有段式開關，然後接地 
//analog
#define Lightread 0 //高數值為亮 5V接光敏，光敏接A0/接10K電阻再接地 


///////////////////////////////////////////////////////////////
////////////////////////////Variable//////////////////////
///////////////////////////////////////////
volatile boolean leftPowerState = true;//一開始就會變化一次
volatile boolean rightPowerState = true;//一開始就會變化一次

byte AutoState = 0; //是否開啟自動
byte LEDPowerState = 0; //表情
byte LCDPowerState = 1; //LCD
byte BACKPowerState = 0; //後燈
byte LightState = 0; //天亮or暗
byte PeopleState = 0; //人在or不在



///////////////////////////////////////////////////////////////
////////////////////////////Setup//////////////////////
///////////////////////////////////////////

void setup() {
  Serial.begin(9600);  // 用於手動輸入文字
  lcd.begin(16, 2);  // 初始化 LCD，一行 16 的字元，共 2 行，預設開啟背光

  ////////// 方向燈開關功能初始化
  pinMode(leftbot, INPUT_PULLUP);
  pinMode(rightbot, INPUT_PULLUP);
  attachInterrupt(0, ChangeleftbotState, FALLING);
  attachInterrupt(1, ChangerightbotState, FALLING);

  ////////// 供給電源系列
  pinMode(LEDPower, OUTPUT);
  digitalWrite(LEDPower, LOW);
  //  pinMode(LCDPower, OUTPUT);
  //  digitalWrite(LCDPower, LOW);
  pinMode(BACKPower, OUTPUT);
  digitalWrite(BACKPower, LOW);
  pinMode(rightLEDPower, OUTPUT);
  digitalWrite(rightLEDPower, LOW);
  pinMode(leftLEDPower, OUTPUT);
  digitalWrite(leftLEDPower, LOW);

  ////////// 其他開關系列 if on -> 電壓為LOW
  pinMode(LEDbot, INPUT_PULLUP);
  pinMode(LCDbot, INPUT_PULLUP);
  pinMode(BACKbot, INPUT_PULLUP);
  pinMode(Autobot, INPUT_PULLUP);


  ////////// 超音波測距
  // pinMode(PIRread, INPUT);
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);

  InitialBlink();  //LCD開機時閃三下
  InitialOpen();  // 輸出開機畫面
  InitialReady(); // 輸出初始運行畫面
}

///////////////////////////////////////////////////////////////
////////////////////////////Main function//////////////////////
///////////////////////////////////////////

void loop() {
  CheckAuto();
  CheckTurnOn();
  CheckLight();
  CheckPeople();
  SetLCDLED(); //打開儀表板 後燈 前燈(表情)
  SetTurning(); //打開方向燈  用中斷Check是否turning用中斷Check是否turning
  CheckReed();
  printSpeed();
  printDistance();
  SetFace();
  Serial.println(leftPowerState);
  Serial.println(rightPowerState);
  Serial.println("");
}

///////////////////////////////////////////////////////////////
////////////////////////////check//////////////////////
///////////////////////////////////////////
void CheckAuto() { //OK
  if (digitalRead(Autobot) == LOW) { //when Auto 開關是On 啟動自動模式
    AutoState = 1;
  } else {
    AutoState = 0;
  }
}

void CheckTurnOn() {    //OK
  //for LED
  if (digitalRead(LEDbot) == LOW) {
    LEDPowerState = 1;
  } else {
    LEDPowerState = 0;
  }
  //for LCD
  if (digitalRead(LCDbot) == LOW) {
    LCDPowerState = 1;
  } else {
    LCDPowerState = 0;
  }
  if (digitalRead(BACKbot) == LOW) {
    BACKPowerState = 1;
  } else {
    BACKPowerState = 0;
  }
}

void CheckLight() {     //OK
  int Lightval = analogRead(Lightread);
  if (Lightval < 512) {
    LightState = 0;
  } else {
    LightState = 1;
  }
}

/*用被動式紅外線感測
  void CheckPeople() {     //OK
  int Peopleval = digitalRead(PIRread);
  if (Peopleval == HIGH) {
    PeopleState = 1;
  } else {
    PeopleState = 0;
  }
  }
*/
void CheckPeople() {     //OK
  long cm = ping();
  Serial.println(cm);
  if (cm <= 10) {
    PeopleState = 1;
  } else {
    PeopleState = 0;
  }
}


void CheckReed() {
}
///////////////////////////////////////////////////////////////
////////////////////////////Initial//////////////////////
///////////////////////////////////////////

void InitialReady() {  //OK
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Speed: 00 km/s");
  lcd.setCursor(0, 1);
  lcd.print("Ready to Go! ");
}

void InitialOpen() {  //OK
  lcd.setCursor(0, 0); // 設定游標位置在第一行行首
  lcd.print("Hello, ArBike!");
  delay(1000);
  lcd.setCursor(0, 1); // 設定游標位置在第二行行首
  lcd.print("NTUME Shannon.");
  delay(5000);
}

void InitialBlink() {  //OK
  for (int i = 0; i < 3; i++) {
    lcd.backlight();  // 開啟背光
    delay(250);
    lcd.noBacklight();// 關閉背光
    delay(250);
  }
  lcd.backlight();
}

long ping() {
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
  return pulseIn(ECHOPIN, HIGH) / 58;
}
///////////////////////////////////////////////////////////////
////////////////////////////action//////////////////////
///////////////////////////////////////////

void SetLCDLED() {                  //OK
  if (AutoState == 1) {
    if (PeopleState == 1) {
      if (LightState == 1) {
        digitalWrite(BACKPower, LOW);
        digitalWrite(LEDPower, LOW);
        lcd.backlight();  // 開啟背光
      } else {
        digitalWrite(BACKPower, HIGH);
        digitalWrite(LEDPower, HIGH);
        lcd.backlight();  // 開啟背光
      }
    } else {
      digitalWrite(BACKPower, LOW);
      digitalWrite(LEDPower, LOW);
      lcd.noBacklight();  // 關閉背光
    }
  } else {
    if (LEDPowerState == 1) {
      digitalWrite(LEDPower, HIGH);
    } else {
      digitalWrite(LEDPower, LOW);
    }
    if (LCDPowerState == 1) {
      lcd.backlight();  // 開啟背光
    } else {
      lcd.noBacklight();  // 關閉背光
    }
    if (BACKPowerState == 1) {
      digitalWrite(BACKPower, HIGH);
    } else {
      digitalWrite(BACKPower, LOW);
    }
  }
}

void SetTurning() {           //OK
  if ((leftPowerState == 1) && (rightPowerState == 0)) {
    digitalWrite(rightLEDPower, LOW);
    digitalWrite(leftLEDPower, HIGH);
    delay(250);
    digitalWrite(leftLEDPower, LOW);
    delay(250);
    digitalWrite(leftLEDPower, HIGH);
  }
  if ((leftPowerState == 0) && (rightPowerState == 1)) {
    digitalWrite(leftLEDPower, LOW);
    digitalWrite(rightLEDPower, HIGH);
    delay(250);
    digitalWrite(rightLEDPower, LOW);
    delay(250);
    digitalWrite(rightLEDPower, HIGH);

  }
  if ((leftPowerState == 0) && (rightPowerState == 0)) {
    digitalWrite(rightLEDPower, LOW);
    digitalWrite(leftLEDPower, LOW);

  } if ((leftPowerState == 1) && (rightPowerState == 1)) {
    digitalWrite(leftLEDPower, HIGH);
    digitalWrite(rightLEDPower, HIGH);
    delay(250);
    digitalWrite(rightLEDPower, LOW);
    digitalWrite(leftLEDPower, LOW);
    delay(250);
    digitalWrite(rightLEDPower, HIGH);
    digitalWrite(leftLEDPower, HIGH);
  }
}

void SetFace() {
}

void printSpeed() {
}


void printDistance() {
}


void ChangeleftbotState() {
  leftPowerState = ! leftPowerState;
}

void ChangerightbotState() {
  rightPowerState = ! rightPowerState;
}
