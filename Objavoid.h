void speed(char status, int v) {
  int vlg = 0, vlb = 0, vrg = 0, vrb = 0; float k=2;
  if (status == 'l') vrg = v; //go left side
  else if (status == 'r') vlg = v; //right
  else if (status == 'L') {
    vlb = v / k;  //focus to left side
    vrg = v - vlb;
  }
  else if (status == 'R') {
    vrb = v / k;  //right
    vlg = v - vrb;
  }
  else if (status == 'a') {
    vlg = v / k;  //go left as circle
    vrg = v;
  }
  else if (status == 'b') {
    vrg = v / k;  //right
    vlg = v;
  }
  else {
    vlg = v;  //go front as simple
    vrg = v;
  }
  analogWrite(lg, vlg);
  analogWrite(lb, vlb);
  analogWrite(rg, vrg);
  analogWrite(rb, vrb);
}
void goBack(){
  analogWrite(lg, 200);
  analogWrite(lb, 0);
  analogWrite(rg, 0);
  analogWrite(rb, 200);
  delay(50);
}

