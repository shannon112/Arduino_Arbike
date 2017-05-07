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
              motorAction(0);
              break;
            case 1:
              switch (MainStatecounter) {
                case 0 ... 3 :
                  ADface();
                  motorAction(1);
                  break;
                case 4 ... 5 :
                  slowface();
                  motorAction(1);
                  break;
                case 7 ... 8 :
                  normalface();
                  motorAction(2);
                  break;
                case 9 ... 11 :
                  fastface();
                  motorAction(3);
                  break;
                case 12 :
                  g8face();
                  motorAction(4);
                  break;
                default :
                  boringface();
                  motorAction(randomNumber);
                  break;
              }
              break;
          }
      }
      break;
  }
}
