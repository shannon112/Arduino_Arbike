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
              normalface();
              motorAction(0);
              break;
            case 1:
              switch (MainStatecounter) {
                case 0 ... 3 :
                  ADface();
                  motorAction(1);
                  break;
                case 4 ... 5 :
                  normalface();
                  motorAction(2);
                  break;
                case 7 ... 8 :
                  fastface();
                  motorAction(2);
                  break;
                case 9 ... 10 :
                  angryface();
                  motorAction(3);
                  break;
                case 11 ... 12 :
                  veryangryface();
                  motorAction(3);
                  break;
                case 13 :
                  QQface();
                  motorAction(4);
                  break;
                case 14 :
                  Nononoface();
                  motorAction(4);
                  break;
                default :
                  boringface();
                  getrandomNumber();
                  motorAction(randomNumber);
                  break;
              }
              break;
          }
      }
      break;
  }
}
