///////////////////////////////////////////////////////////////
////////////////////////////Initial//////////////////////
///////////////////////////////////////////
void InitialBlink() {  //OK
  for (int i = 0; i < 3; i++) {
    lcd.backlight();  // 開啟背光
    delay(250);
    lcd.noBacklight();// 關閉背光
    delay(250);
  }
  lcd.backlight();
}


void InitialOpen() {  //OK
  lcd.setCursor(0, 0); // 設定游標位置在第一行行首
  lcd.print("Hello, ArBike!");
  delay(1000);
  lcd.setCursor(0, 1); // 設定游標位置在第二行行首
  lcd.print("NTUME Shannon.");
  delay(5000);
}


void InitialReady() {  //OK
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Speed: 00.00km/s");
  lcd.setCursor(0, 1);
  lcd.print("Ready to Go! ");
  delay(1000);
  lcd.clear();
}

