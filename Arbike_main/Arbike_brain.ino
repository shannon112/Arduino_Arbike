void SetFace() {
  switch (TurningState) {
    case 10 :
      leftturnface();
      break;
    case 1 :
      rightturnface();
      break;
    case 11 :
      brokenface();
      break;
    case 0 :
      if (ifSpeeding == 1) {
        if ((KPH < 10) && (KPH > 0)) {
          SPslowface();
        } else if ((KPH >= 10) && (KPH <= 20)) {
          SPnormalface();
        } else if ((KPH < 100) && (KPH > 20)) {
          SPfastface();
        } else {
          normalface();
        }
      } else {
        if (digitalRead(Mainbot) == HIGH) {
          switch (MainStatecounter) {
            case 0 ... 3 :
              ADface();
              motorAction(0);
              break;
            case 4 ... 5 :
              normalface();
              motorAction(0);
              break;
            case 6 ... 7 :
              fastface();
              motorAction(0);
              break;
            case 8 ... 9 :
              angryface();
              motorAction(0);
              break;
            case 10 ... 15 :
              veryangryface();
              motorAction(0);
              break;
            case 16 ... 17 :
              QQface();
              motorAction(0);
              break;
            case 18 ... 19 :
              Nononoface();
              motorAction(0);
              break;
            default :
              boringface();
              motorAction(0);
              break;
          }
        } else {
          switch (MainStatecounter) {
            case 0 ... 3 :
              ADface();
              motorAction(1);
              MainStatecounter++;
              break;
            case 4 ... 5 :
              normalface();
              motorAction(2);
              MainStatecounter++;
              break;
            case 6 ... 7 :
              fastface();
              motorAction(2);
              MainStatecounter++;
              break;
            case 8 ... 9 :
              angryface();
              motorAction(3);
              MainStatecounter++;
              break;
            case 10 ... 15 :
              veryangryface();
              motorAction(3);
              MainStatecounter++;
              break;
            case 16 ... 17 :
              QQface();
              motorAction(4);
              MainStatecounter++;
              break;
            case 18 ... 19 :
              Nononoface();
              motorAction(4);
              MainStatecounter++;
              break;
            default :
              boringface();
              getrandomNumber();
              motorAction(randomNumber);
              MainStatecounter++;
              break;
          }
        }
      } break;
  }
}
