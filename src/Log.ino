void Log_init()
{
    Serial.begin(115200);
    delay(500);
    Serial.println("Connected");
    pinMode(2, OUTPUT);
    pinMode(16, OUTPUT);
    logFlash(5, 500);
}

void log(String text)
{
  if (DEBUG)
    Serial.println(text);
}

void logFlash(int times, int ms)
{
    for (int i = 0; i <times; i++)
    {
        digitalWrite(2, HIGH);
        digitalWrite(16, LOW);
        delay(ms);
        digitalWrite(2, LOW);
        digitalWrite(16, HIGH);
        delay(ms);
    }
    digitalWrite(16, HIGH);
    digitalWrite(2, HIGH);
}

void Serial_loop(){
    if (Serial.available() > 0){
        Serial.println(Serial.readString());
    }

}