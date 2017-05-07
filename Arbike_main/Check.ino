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


