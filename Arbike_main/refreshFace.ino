void  refreshFace(){
  oldMainStatecounter = newMainStatecounter;
  newMainStatecounter = MainStatecounter;
  if (newMainStatecounter == oldMainStatecounter) {
    zeroMainStatecounter++;
  }
  else {
    zeroMainStatecounter = 0;
  }
  if (zeroMainStatecounter > refreshFaceLoopNumber) {
    MainStatecounter = 0;
  }
}

