//Arbike v2.0
//It completely has all basic feature, and also fixed the problem about blink eyes.
 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Timer.h"            //https://github.com/JChristensen/Timer
#include "Adafruit_GFX.h"
#include "Adafruit_HT1632.h"
#include <avr/pgmspace.h>
 

///////////////////////////////////////////////////////////////
////////// Set speeder///////////////// 
///////////////////////////////////////////
//set metric = false to use miles true to use kilometer
boolean metric = true;
//wheel circumference in centimeters
float wheelC = 26 * 2.54 * 3.14;


////////// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
///////////////////////////////////////////
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


///////////////////////////////////////////////////////////////
////////////////////////////Arduino Pin//////////////////////
///////////////////////////////////////////
//digital
//SCL是最上面的
#define leftbot 2 //接Pin2，接無段式開關，然後接地
#define rightbot 3 //接Pin3，接無段式開關，然後接地
#define FRONTPower 6 //接給前燈 Vcc
#define TRIGPIN 5
#define ECHOPIN 7
#define rightLEDPower 8 //接右方向燈的Vcc(前＋後）
#define leftLEDPower 9 //接右方向燈的Vcc(前＋後）
#define Autobot 10 //接Pin10，接有段式開關，然後接地
#define LEDbot 13 //開關LED表情電源 接Pin13，接有段式開關，然後接地
#define BACKbot 12 //開關FRONT電源  接Pin12，接有段式開關，然後接地
#define LCDbot 11 //開關LCD電源  接Pin11，接有段式開關，然後接地 
#define reedPin 15 //磁簧開關 一端插5V 一端插一欄  一欄分叉到D15&10K電阻接地
#define HT_DATA 16 //LED Matrix系列 /I2C腳位SDA
#define HT_WR   17 //LED Matrix系列 /I2C腳位SLA
#define HT_CS   4 //LED Matrix系列 //RD
#define HT_CS2  1 //LED Matrix系列 //CS

//analog
#define Lightread 0 //高數值為亮 5V接光敏，光敏接A0/接10K電阻再接地 

///////////////////////////////////////////////////////////////
////////////////////////////picture＆LED matrix setting//////////////////////
///////////////////////////////////////////
// use this line for single matrix
Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS);

const  uint16_t slow[] PROGMEM = {
  B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000,
  B00010000, B00000000, B00001000,
  B00100000, B00000000, B00000100,
  B01000000, B00000000, B00000010,
  B10000000, B00000000, B00000001,
  B00111100, B00000000, B00111100,
  B00111100, B00000000, B00111100,
  B00000000, B00000000, B00000000,
  B00000000, B01000100, B00000000,
  B00000000, B10010010, B00000000,
  B01111000, B10010010, B00011110,
  B00000000, B01101100, B00000000,
  B01111000, B00000000, B00011110,
  B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000,
};

const  uint16_t normal[] PROGMEM = {
  B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000,
  B00010000, B00000000, B00001000,
  B00100000, B00000000, B00000100,
  B01000000, B00000000, B00000010,
  B10001100, B00000000, B00110001,
  B00010010, B00000000, B01001000,
  B00010010, B00000000, B01001000,
  B00001100, B00000000, B00110000,
  B00000000, B01000100, B00000000,
  B00000000, B10010010, B00000000,
  B01111000, B10010010, B00011110,
  B00000000, B01101100, B00000000,
  B01111000, B00000000, B00011110,
  B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000,
};

const  uint16_t fast[] PROGMEM = {
  B00000000, B00000000, B00000000,
  B00001000, B00000000, B00010000,
  B00000100, B00000000, B00100000,
  B00000010, B00000000, B01000000,
  B00000001, B00000000, B10000000,
  B00001100, B00000000, B00110000,
  B00010010, B00000000, B01001000,
  B00010010, B00000000, B01001000,
  B00001100, B00000000, B00110000,
  B00000000, B00000000, B00000000,
  B00000000, B10010010, B00000000,
  B01111000, B10010010, B00011110,
  B00000000, B01101100, B00000000,
  B01111000, B00000000, B00011110,
  B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000,
};

const  uint16_t sfast[] PROGMEM = {
  B00000000, B00000000, B00000000,
  B00001000, B00000000, B00010000,
  B00000100, B00000000, B00100000,
  B00000010, B00000000, B01000000,
  B00000001, B00000000, B10000000,
  B00000000, B00000000, B00000000,
  B00111100, B00000000, B00111100,
  B00111100, B00000000, B00111100,
  B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000,
  B00000000, B10010010, B00000000,
  B01111000, B10010010, B00011110,
  B00000000, B01101100, B00000000,
  B01111000, B00000000, B00011110,
  B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000,
};
long blinkornot = 0;

///////////////////////////////////////////////////////////////
////////////////////////////Variable//////////////////////
///////////////////////////////////////////
volatile boolean leftPowerState = true;//一開始就會變化一次
volatile boolean rightPowerState = true;//一開始就會變化一次

byte AutoState = 0; //是否開啟自動
byte LEDPowerState = 0; //表情
byte LCDPowerState = 1; //LCD
byte FRONTPowerState = 0; //前燈
byte LightState = 0; //天亮or暗
byte PeopleState = 0; //人在or不在
byte FaceType = 1; //現在是哪種臉 0 1 2  slow normal fast

/////Reed Switch//////////////
int circleNum = 0;
float odometer = 0;
float miles = 0;
float kilometers = 0;
float speedometer = 0;
float MPH = 0;
float KPH = 0;
int reedTime = 0;
int reedTimeDelta = 0;
boolean reedOn = false;

//////////Timer Variables///////
int blinkTime = 1000;
Timer scrollTimer;
int scrollTime = 130;
Timer strobeTimer;
int strobeTime = 100;
boolean blinkOn;

///////////////////////////////////////////////////////////////
////////////////////////////Setup//////////////////////
///////////////////////////////////////////

void setup() {
  Serial.begin(9600);  // 用於手動輸入文字
  matrix.begin(ADA_HT1632_COMMON_16NMOS);
  matrix.fillScreen();
  delay(500);
  readbitmap(normal);
  matrix.writeScreen();
  delay(2000);

  readbitmap(slow);
  lcd.begin(16, 2);  // 初始化 LCD，一行 16 的字元，共 2 行，預設開啟背光


  ////////// 方向燈開關功能初始化
  pinMode(leftbot, INPUT_PULLUP);
  pinMode(rightbot, INPUT_PULLUP);
  attachInterrupt(0, ChangeleftbotState, FALLING);
  attachInterrupt(1, ChangerightbotState, FALLING);

  ////////// 供給電源系列
  pinMode(FRONTPower, OUTPUT);
  digitalWrite(FRONTPower, LOW);
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

  ////////// 磁簧開關
  pinMode(reedPin, INPUT);

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
  checkReed();
  printSpeed();
  printDistance();
  //  Serial.println(leftPowerState);
  //  Serial.println(rightPowerState);
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
    FRONTPowerState = 1;
  } else {
    FRONTPowerState = 0;
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
  //  Serial.println(cm);
  if (cm <= 10) {
    PeopleState = 1;
  } else {
    PeopleState = 0;
  }
}

void readbitmap(const uint16_t input[]) {        //讀取bitmap的圖形
  int iii;
  for (int j = 0; j < 48; j++) { //j為矩陣中第幾個元素
    int jj = j / 3;              //jj為該元素在LEDmatrix所屬的排數上到下0~15排
    int jjj = j % 3;             //jjj為該元素的位置的小塊LED在哪個colume
    iii = pgm_read_word_near(input + j);            //iii為現在讀到元素的值8bit binary 0~255
    //    Serial.print("row:");
    //    Serial.println(jj);
    //    Serial.print("colume:");
    //    Serial.println(jjj);
    for (int i = 0; i < 8; i++) {
      int ii = iii % 2;          //ii為把元素分解成8bit binary的最後一位
      iii = iii / 2;
      //      Serial.println(ii);
      if (ii == 1) matrix.drawPixel(15 - jj, (7 + (jjj * 8)) - i, 1);
      else if (ii == 0) matrix.drawPixel(15 - jj, (7 + (jjj * 8)) - i, 0);
    }
  }
}



void checkReed() {
  int r = digitalRead(reedPin);
  if (r == 1 && reedOn == false) {
    reedOn = true;
    reedTimeDelta = millis() - reedTime;
    reedTime = millis();
    circleNum++;
  }
  else if (r == 0 && reedOn) {
    reedOn = false;
  }
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
  delay(3000);
  lcd.clear();
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
        digitalWrite(FRONTPower, LOW);
        matrix.clearScreen();
        lcd.backlight();  // 開啟背光
      } else {
        digitalWrite(FRONTPower, HIGH);
        SetFace();
        BlinkEyes();
        lcd.backlight();  // 開啟背光
      }
    } else {
      digitalWrite(FRONTPower, LOW);
      matrix.clearScreen();
      lcd.noBacklight();  // 關閉背光
    }
  } else {
    if (LEDPowerState == 1) {
      SetFace();
      BlinkEyes();
    } else {
      matrix.clearScreen();
    }
    if (LCDPowerState == 1) {
      lcd.backlight();  // 開啟背光
    } else {
      lcd.noBacklight();  // 關閉背光
    }
    if (FRONTPowerState == 1) {
      digitalWrite(FRONTPower, HIGH);
    } else {
      digitalWrite(FRONTPower, LOW);
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

void printSpeed() {
  speedometer = wheelC / reedTimeDelta;
  if (metric) {
    KPH = speedometer * 36;
    lcd.setCursor(1, 0);
    lcd.print("KPH: ");
    lcd.setCursor(6, 0);
    lcd.print(KPH, 2);
    //Serial.println("KPH:");
    //Serial.println(KPH);
  }
  else {
    MPH = speedometer * 22.369;
    lcd.setCursor(1, 0);
    lcd.print("MPH: ");
    lcd.setCursor(6, 0);
    lcd.print(MPH, 2);
    //Serial.println("MPH:");
    //Serial.println(MPH);
  }
}

void printDistance() {
  odometer = wheelC * circleNum;
  if (metric) {
    kilometers = odometer / 100000;
    lcd.setCursor(1, 1);
    lcd.print("KM: ");
    lcd.setCursor(5, 1);
    lcd.print(kilometers, 2);
    //Serial.println("KM:");
    //Serial.println(kilometers);
  }
  else {
    miles = odometer / 160934.4;
    lcd.setCursor(1, 1);
    lcd.print("Miles: ");
    lcd.setCursor(5, 1);
    lcd.print(miles, 2);
    //Serial.println("Mile:");
    //Serial.println(miles);
  }
}

void SetFace() {
  if (KPH < 10) {
    FaceType = 0;
    readbitmap(slow);
    matrix.writeScreen();
    Serial.println("Face0");
  } else if ((KPH >= 10) && (KPH <= 20)) {
    FaceType = 1;
    readbitmap(normal);
    matrix.writeScreen();
    Serial.println("Face1");
  } else {
    FaceType = 2;
    readbitmap(fast);
    matrix.writeScreen();
    Serial.println("Face2");
  }
}

void BlinkEyes() {
  if (FaceType == 0) {
    Serial.println("Blink0");
  } else if (FaceType == 1) {
    blinkornot++;
    if ((blinkornot % 100) == 0) {
      for (int i = 0 ; i < 2; i++) {
        readbitmap(slow);
        matrix.writeScreen();
        delay(100);
        readbitmap(normal);
        matrix.writeScreen();
        delay(100);
      }
    }
    Serial.println("Blink1");
  } else {
    blinkornot++;
    if ((blinkornot % 100) == 0) {
      for (int i = 0 ; i < 2; i++) {
        readbitmap(sfast);
        matrix.writeScreen();
        delay(100);
        readbitmap(fast);
        matrix.writeScreen();
        delay(100);
      }
    }
    Serial.println("Blink2");
  }
}

void ChangeleftbotState() {
  leftPowerState = ! leftPowerState;
}

void ChangerightbotState() {
  rightPowerState = ! rightPowerState;
}
