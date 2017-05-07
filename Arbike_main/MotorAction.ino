void motorAction(byte mode) {
  //b表示打開盒子的模式，h表示伸手關開關的模式
  //b 0慢速打開 1正常打開 2快速打開 3抖動後打開
  //h 0慢速伸手 1正常伸手 2快速伸手 3假動作後關閉
  switch (mode) {
    case 0://no
      servoh.write(65); //125推  65縮
      servob.write(90); //90為關蓋 180開蓋
      break;
    case 1://slow mode
      servob.write(180);
      delay(1500);
      servoh.write(125);
      delay(500);
      servoh.write(65);
      delay(1000);
      servob.write(90);
      break;
    case 2://normal mode
      servob.write(180);
      delay(500);
      servoh.write(125);
      delay(500);
      servoh.write(65);
      delay(500);
      servob.write(90);
      break;
    case 3:
      servoh.write(125);
      delay(300);
      servoh.write(65);
      break;
    case 4:
      servob.write(120);
      delay(300);
      servob.write(90);
      delay(300);
      servob.write(120);
      delay(300);
      servob.write(90);
      delay(300);
      servob.write(120);
      delay(1000);
      servob.write(180);
      delay(300);
      servoh.write(125);
      delay(300);
      servoh.write(65);
      delay(300);
      servob.write(90);
      break;
  }
}
void getrandomNumber() {
  randomNumber = random(1, 5);
  // get a random number from 1 to 4
}
