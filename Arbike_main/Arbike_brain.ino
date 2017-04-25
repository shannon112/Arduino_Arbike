void SetFace() {
  switch (TurningState) {
    case 10 :
      leftturnface();
      break;
    case 01 :
      rightturnface();
      break;
    case 11 :
      brokenface();
      break;
    case 00 :
      switch (ifSpeeding) {
        case 1:
          if (KPH < 10) {
            SPslowface();
          } else if ((KPH >= 10) && (KPH <= 20)) {
            SPnormalface();
          } else {
            SPfastface();
          }
          break;
        case 0:
          switch (MainState) {
            case 0:
              noface();
              break;
            case 1:
              switch (MainStatecounter) {
                case 0 ... 3 :
                  ADface();
                  motorAction(0 , 0);
                  break;
                case 4 ... 5 :
                  slowface();
                  motorAction(0 , 0);
                  break;
                case 7 ... 8 :
                  normalface();
                  motorAction(1 , 1);
                  break;
                case 9 ... 11 :
                  fastface();
                  motorAction(2 , 2);
                  break;
                case 12 :
                  g8face();
                  motorAction(3 , 2);
                  break;
                default :
                  int a;
                  randomface(a);
                  //motorAction(3 , 2);
                  break;
              }
              break;
          }
      }
      break;
  }
}

void BlinkEyes() {
  if (FaceType == 0) {
  } else if (FaceType == 1) {
    blinkornot++;
    if ((blinkornot % 100) == 0) {
      for (int i = 0 ; i < 2; i++) {
        slowface();
        delay(100);
        normalface();
        delay(100);
      }
    }
  } else if (FaceType == 2) {
    blinkornot++;
    if ((blinkornot % 100) == 0) {
      for (int i = 0 ; i < 2; i++) {
        sfastface();
        delay(100);
        fastface();
        delay(100);
      }
    }
  } else {}
}

void motorAction(byte box , byte hand) {
  //b表示打開盒子的模式，h表示伸手關開關的模式
  //b 0慢速打開 1正常打開 2快速打開 3抖動後打開
  //h 0慢速伸手 1正常伸手 2快速伸手 3假動作後關閉
  switch (box) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      ;
  }
  switch (hand) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      ;
  }
  switch (box) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      ;
  }
}
