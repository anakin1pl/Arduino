volatile word steps;

void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), onStep, FALLING);
}

void loop()
{
  Serial.println(steps);
  delay(300);
}

void onStep()
{
  static unsigned long lastTime;
  unsigned long timeNow = millis();
  if (timeNow - lastTime < 500)
 
  return;
  steps++;
  lastTime = timeNow;
}
