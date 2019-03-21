//实现arduino返回电脑发送的指令
//by czl
//2019/03/21/9:17

String comdata = "";

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    while (Serial.available() > 0)  
    {
        comdata += char(Serial.read());
        delay(2);
    }
    if (comdata.length() > 0)
    {
        Serial.println(comdata);
        comdata = "";
    }
}
