int Speed = 0;

void Forward(){
    analogWrite(in1, Speed);
    analogWrite(in2, 0);
    analogWrite(in3, Speed);
    analogWrite(in4, 0);
}
void Back(){
    analogWrite(in1, 0);
    analogWrite(in2, Speed);
    analogWrite(in3, 0);
    analogWrite(in4, Speed);
}
void Right(){
    analogWrite(in1, Speed);
    analogWrite(in2, 0);
    analogWrite(in3, 0);
    analogWrite(in4, Speed);
}
void Left(){
    analogWrite(in1, 0);
    analogWrite(in2, Speed);
    analogWrite(in3, Speed);
    analogWrite(in4, 0);
}
void Stop(){
    analogWrite(in1, 0);
    analogWrite(in2, 0);
    analogWrite(in3, 0);
    analogWrite(in4, 0);
}
void Horn(boolean state){
    if(state == true)
      tone(horn, 1900);
    else
      noTone(horn);
}
void Blue(boolean state){
    digitalWrite(blue, state);    
}
void Yellow(boolean state){
    digitalWrite(yellow, state);
}
