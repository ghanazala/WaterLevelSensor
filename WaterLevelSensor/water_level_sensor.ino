int getLevelData1(){
  int val;
  int level;
  val = analogRead(A0);
  if(val>=0 && val<200){
    level = 1;
  }
  else if(val>=200 && val<450){
    level = 2;
  }
  else if(val>=450 && val<600){
    level = 3;
  }
  else if(val>=600 && val<620){
    level = 4;
  }
  else if(val>=620 && val<700){
    level = 5;
  }
  return level;
}

int getLevelData2(){
  int val;
  int level;
  val = analogRead(A1);
  if(val>=0 && val<200){
    level = 1;
  }
  else if(val>=200 && val<450){
    level = 2;
  }
  else if(val>=450 && val<600){
    level = 3;
  }
  else if(val>=600 && val<620){
    level = 4;
  }
  else if(val>=620 && val<700){
    level = 5;
  }
  return level;
}

