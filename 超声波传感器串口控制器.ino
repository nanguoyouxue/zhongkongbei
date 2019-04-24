/*超声波传感器串口控制器 1.0
 * by czl
 * 2019/04/24
 */
const int pingPin = 11; //Trig接11号引脚
const int echoPin = 12; // Echo接12号引脚

void setup() {
   Serial.begin(9600);
}

void loop() {
   long duration, inches, cm;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   cm =duration / 29 / 2 ;
   Serial.print(cm);//以厘米为单位
   delay(100);
}
