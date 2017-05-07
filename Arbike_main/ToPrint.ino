void toPrint() {
  if (printCounter == 20) {
    printWeather();
    printCounter = 0;
  } else {
    printSpeed();
    printDistance();
    printCounter++;
  }
}

void printSpeed() {
  speedometer = wheelC / reedTimeDelta;
  if (metric) {
    //Speed: 13.25 m/s
    KPH = speedometer * 36;
    lcd.setCursor(0, 0);
    lcd.print("Speed: ");
    lcd.setCursor(8, 0);
    lcd.print(KPH, 2);
    lcd.setCursor(14, 0);
    lcd.print("m/s");
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
    //Distance: 2213.23km
    kilometers = odometer / 100000;
    lcd.setCursor(0, 1);
    lcd.print("Distance: ");
    lcd.setCursor(11, 1);
    lcd.print(kilometers, 2);
    lcd.setCursor(15, 1);
    lcd.print("km");
    //Serial.println("KM:");
    //Serial.println(kilometers);
  }
}

void printWeather() {
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    lcd.setCursor(0, 0);
    lcd.print("Failed to read");
    lcd.setCursor(0, 1);
    lcd.print("from DHT sensor!");
    return;
  }
  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.setCursor(10, 0);
  lcd.print(h);
  lcd.setCursor(16, 0);
  lcd.print("%");

  lcd.print("Temp: ");
  lcd.setCursor(6, 1);
  lcd.print(t);
  lcd.setCursor(12, 1);
  lcd.print(" *C ");
//  Serial.print(f);
//  Serial.print(" *F\t");
//  Serial.print("Heat index: ");
//  Serial.print(hic);
//  Serial.print(" *C ");
//  Serial.print(hif);
//  Serial.println(" *F");
}
