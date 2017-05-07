//Arbike v4.0

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Timer.h"            //https://github.com/JChristensen/Timer
#include "Adafruit_GFX.h"
#include "Adafruit_HT1632.h"
#include <avr/pgmspace.h>


///////////////////////////////////////////////////////////////
////////// Set speeder/////////////////
///////////////////////////////////////////
boolean metric = true;
float wheelC = 26 * 2.54 * 3.14;//wheel circumference in centimeers


////////// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
///////////////////////////////////////////
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


///////////////////////////////////////////////////////////////
////////////////////////////Arduino Pin//////////////////////
///////////////////////////////////////////現在只剩：前大燈，儀表板燈，後表情燈
//digital
#define leftbot 2 //接Pin2，接無段式開關，然後接地
#define rightbot 3 //接Pin3，接無段式開關，然後接地
#define FRONTPower 6 //接給前燈 Vcc
#define TRIGPIN 5  //超音波測距器
#define ECHOPIN 7  //超音波測距器
#define Autobot 10 //接Pin10，接有段式開關，然後接地 //OFF=全關，ONcounter＝0自動/1只亮儀表板/2全亮
#define Mainbot 13 //開關LED表情電源 接Pin13，接有段式開關，然後接地  //加入counter，開關切換車宣and互動表情
#define servoPinb 12 //接servo 開蓋子
#define servoPinh 9 //接servo 關開關
#define DHT11data 8 //接DHT11 data pinn
#define reedPin 15 //磁簧開關 一端插5V 一端插一欄  一欄分叉到D15&10K電阻接地
#define HT_DATA 16 //LED Matrix系列 /I2C腳位SDA
#define HT_WR   17 //LED Matrix系列 /I2C腳位SLA
#define HT_CS   4 //LED Matrix系列 //CS
#define HT_CS2  11 //LED Matrix系列 //RD
//analog
#define Lightread 0 //高數值為亮 5V接光敏，光敏接A0/接10K電阻再接地 


///////////////////////////////////////////////////////////////
////////////////////////////picture＆LED matrix setting//////////////////////
///////////////////////////////////////////
// use this line for single matrix
Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS);
long blinkornot = 0;


///////////////////////////////////////////////////////////////
////////////////////////////Variable//////////////////////
///////////////////////////////////////////
//********left/rightbot***********************
volatile boolean leftPowerState = true;//一開始就會變化一次
volatile boolean rightPowerState = true;//一開始就會變化一次
int TurningState = 00;
//********AutoBot***********************************
byte AutoState = 0; //是否開啟自動
int AutoStatecounter = 0; //數開關被開了幾次
//********MainBot***********************************
int MainState = 0; //是否開啟開關
int MainStatecounter = 0; //數開關被開了幾次
//********SensorState***********************************
byte LightState = 0; //天亮or暗
byte PeopleState = 0; //人在or不在
byte FaceType = 1; //現在是哪種臉 0 1 2  slow normal fast
//********SpeedDetect***********************************
boolean ifSpeeding = 0;
float oldKPH = 0;
float newKPH = 0;
float KPH = 0;
int circleNum = 0;
float odometer = 0;
float kilometers = 0;
float speedometer = 0;
int reedTime = 0;
int reedTimeDelta = 0;
boolean reedOn = false;


///////////////////////////////////////////////////////////////
////////////////////////////Setup//////////////////////
///////////////////////////////////////////
void setup() {
  /////////// serial port初始化
  Serial.begin(9600);

  ////////// 表情LED matrix初始化
  matrix.begin(ADA_HT1632_COMMON_16NMOS);
  matrix.fillScreen();
  delay(500);
  fastface();
  delay(2000);

  ////////// LCD儀表板初始化
  lcd.begin(16, 2);  // 初始化 LCD，一行 16 的字元，共 2 行，預設開啟背光
  InitialBlink();  //LCD開機時閃三下
  InitialOpen();  // 輸出開機畫面
  InitialReady(); // 輸出初始運行畫面

  ////////// 方向燈開關功能初始化
  pinMode(leftbot, INPUT_PULLUP);
  pinMode(rightbot, INPUT_PULLUP);
  attachInterrupt(0, ChangeleftbotState, FALLING);
  attachInterrupt(1, ChangerightbotState, FALLING);

  ////////// 供給電源系列
  pinMode(FRONTPower, OUTPUT);
  digitalWrite(FRONTPower, LOW);

  ////////// 其他開關系列 if on -> 電壓為LOW
  pinMode(Mainbot, INPUT_PULLUP);
  pinMode(Autobot, INPUT_PULLUP);

  ////////// 超音波測距
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);

  ////////// 磁簧開關
  pinMode(reedPin, INPUT);

  ////////// 伺服馬達
  Servo servob;
  Servo servoh;
  servob.attach(servoPinb);
  servoh.attach(servoPinh);
}


///////////////////////////////////////////////////////////////
////////////////////////////Main function//////////////////////
///////////////////////////////////////////
void loop() {
  Serial.println("===================");
  CheckAuto();
  CheckLight();
  CheckPeople();
  CheckPlay();
  CheckTurningState();
  CheckSpeed();
  printSpeed();
  printDistance();
  SetPower(); //打開儀表板 後燈 前燈(表情)
  BlinkEyes();
  Serial.println("===================");
}


///////////////////////////////////////////////////////////////
////////////////////////////check//////////////////////
///////////////////////////////////////////
void CheckAuto() { //OFF=全關，ONcounter＝0自動/1只亮儀表板/2全亮
  if (digitalRead(Autobot) == LOW) {
    AutoState = -1;
  } else {
    AutoStatecounter ++;
    AutoState = AutoStatecounter % 3;
  }
}

void CheckLight() {
  int Lightval = analogRead(Lightread);
  if (Lightval < 512) {
    LightState = 0;
  } else {
    LightState = 1;
  }
}

void CheckPeople() {
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

void CheckSpeed() {
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

void CheckPlay() {
  if (digitalRead(Mainbot) == LOW) {
    MainState = 1;
    MainStatecounter++;
  } else
    MainState = 0;
}

void CheckTurningState() {
  String tempTurningState = "";
  tempTurningState = String(leftPowerState) + String(rightPowerState);
  TurningState = tempTurningState.toInt();
}


///////////////////////////////////////////////////////////////
////////////////////////////action//////////////////////
///////////////////////////////////////////
//OFF=全關，ONcounter＝0自動/1只亮儀表板/2全亮
void SetPower() {                  //OK
  switch (AutoState) {
    case -1 : //全關
      digitalWrite(FRONTPower, LOW); //關閉前燈
      lcd.noBacklight();  //關閉儀表板
      matrix.clearScreen(); //關閉後燈
      break;
    case 0 :  //全自動
      if (PeopleState == 1) {
        if (LightState == 1) {
          digitalWrite(FRONTPower, LOW);
          matrix.clearScreen();
          lcd.backlight();  //開啟背光
        } else {
          digitalWrite(FRONTPower, HIGH);
          SetFace();
          lcd.backlight();  //開啟背光
        }
      } else {
        digitalWrite(FRONTPower, LOW);
        matrix.clearScreen();
        lcd.noBacklight();  //關閉背光
      }
      break;
    case 1 :  //手動，僅開啟儀表板
      digitalWrite(FRONTPower, LOW) ;//關閉前燈
      lcd.backlight();  //開啟儀表板
      matrix.clearScreen(); //關閉後燈
      break;
    case 2 : //手動，全亮
      digitalWrite(FRONTPower, HIGH) ;//開啟前燈
      lcd.backlight();  //開啟儀表板
      SetFace(); //開啟後燈
      break;
    default:
      Serial.println("WTF");
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
  oldKPH = newKPH;
  newKPH = KPH;
  if (oldKPH == newKPH)
    ifSpeeding = 0;
  else
    ifSpeeding = 1;
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
}

void ChangeleftbotState() {
  leftPowerState = ! leftPowerState;
}

void ChangerightbotState() {
  rightPowerState = ! rightPowerState;
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
