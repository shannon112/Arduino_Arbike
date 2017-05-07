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

