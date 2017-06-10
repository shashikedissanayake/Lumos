
void setup()
{
  Serial.begin(115200);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop()
{
  digitalWrite(5,0);
  digitalWrite(6,1);
int read = analogRead(A5);
//delay(10);
if(read > 500){
  analogWrite(3,1023);
  }else{
    analogWrite(3,0);
    }
}
