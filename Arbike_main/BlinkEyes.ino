void BlinkEyes() {
  switch (FaceType) {
    case 1:
      blinkornot++;
      if ((blinkornot % 100) == 0) {
        for (int i = 0 ; i < 2; i++) {
          slowface();
          delay(100);
          normalface();
          delay(100);
        }
      }
      break;
    case 2:
      blinkornot++;
      if ((blinkornot % 100) == 0) {
        for (int i = 0 ; i < 2; i++) {
          fastBlinkface();
          delay(100);
          fastface();
          delay(100);
        }
      }
      break;
    case 20:
      blinkornot++;
      if ((blinkornot % 100) == 0) {
        for (int i = 0 ; i < 2; i++) {
          SPslowBlinkface();
          delay(100);
          SPslowface();
          delay(100);
        }
      }
      break;
    case 21:
      blinkornot++;
      if ((blinkornot % 100) == 0) {
        for (int i = 0 ; i < 2; i++) {
          SPnormalBlinkface();
          delay(100);
          SPnormalface();
          delay(100);
        }
      }
      break;
    case 22:
      blinkornot++;
      if ((blinkornot % 100) == 0) {
        for (int i = 0 ; i < 2; i++) {
          SPfastBlinkface();
          delay(100);
          SPfastface();
          delay(100);
        }
      }
      break;
  }
}
