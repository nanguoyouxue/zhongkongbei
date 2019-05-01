/*超声波传感器串口控制器 1.1 beta
 * by czl
 * 2019/05/01
 */
byte pingPin = 11; //Trig接11号引脚
byte echoPin = 12; // Echo接12号引脚

void setup() {
   Serial.begin(9600);
   pinMode(pingPin, OUTPUT);
   pinMode(echoPin, INPUT);
}

void loop() {
   int duration, cm;   //事实上我们并用不到如此高的精度，所以直接用整型就够了
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   cm =duration / 29 / 2 ;
   Serial.print(cm);//以厘米为单位
   delay(100);
}
