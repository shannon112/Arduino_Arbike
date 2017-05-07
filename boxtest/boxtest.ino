#include <Servo.h>


#define leftbot 2 //接Pin2，接無段式開關，然後接地
#define rightbot 3 //接Pin3，接無段式開關，然後接地
#define FRONTPower 6 //接給前燈 Vcc
#define TRIGPIN 5  //超音波測距器
#define ECHOPIN 7  //超音波測距器
#define Autobot 10 //接Pin10，接有段式開關，然後接地 //OFF=全關，ONcounter＝0自動/1只亮儀表板/2全亮
#define Mainbot 12 //開關LED表情電源 接Pin13，接有段式開關，然後接地  //加入counter，開關切換車宣and互動表情
#define servoPinb 13 //接servo 開蓋子
#define servoPinh 9 //接servo 關開關
#define DHT11data 8 //接DHT11 data pinn
#define reedPin 15 //磁簧開關 一端插5V 一端插一欄  一欄分叉到D15&10K電阻接地
#define HT_DATA 16 //LED Matrix系列 /I2C腳位SDA
#define HT_WR   17 //LED Matrix系列 /I2C腳位SLA
#define HT_CS   4 //LED Matrix系列 //CS
#define HT_CS2  11 //LED Matrix系列 //RD
//analog
#define Lightread 0 //高數值為亮 5V接光敏，光敏接A0/接10K電阻再接地 
Servo servob;
Servo servoh;
void setup() {
  // put your setup code here, to run once:
  ////////// 其他開關系列 if on -> 電壓為LOW
  pinMode(Mainbot, INPUT_PULLUP);
  pinMode(Autobot, INPUT_PULLUP);
  ////////// 伺服馬達
  Serial.begin(9600);
  servob.attach(servoPinb);
  servoh.attach(servoPinh);

}

void loop() {
  int m = digitalRead(Mainbot);
  if (m == HIGH) {
    Serial.println("off");
    servoh.write(65); //約155打到  90為躺平
    servob.write(90); //90為躺平 180台盒子

  } else {
    Serial.println("on");
    servob.write(180); 
    delay(1500);
    servoh.write(125); 
    delay(500);
    servoh.write(65); 
    delay(1000);
    servob.write(90); 
  }
}

