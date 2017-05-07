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
