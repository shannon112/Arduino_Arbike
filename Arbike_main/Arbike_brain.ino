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
        if (KPH < 10) {
          SPslowface();
        } else if ((KPH >= 10) && (KPH <= 20)) {
          SPnormalface();
        } else {
          SPfastface();
        }
      } else {
        if (digitalRead(Mainbot) == HIGH) {
          if((MainStatecounter>0)&&(MainStatecounter<4))
          normalface();
          motorAction(0);
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
            case 7 ... 8 :
              fastface();
              motorAction(2);
              MainStatecounter++;
              break;
            case 9 ... 10 :
              angryface();
              motorAction(3);
              MainStatecounter++;
              break;
            case 11 ... 12 :
              veryangryface();
              motorAction(3);
              MainStatecounter++;
              break;
            case 13 :
              QQface();
              motorAction(4);
              MainStatecounter++;
              break;
            case 14 :
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
