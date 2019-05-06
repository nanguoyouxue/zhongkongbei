/*超声波传感器串口控制器 1.1
 * by czl
 * 2019/05/05
 * 1.本质上不是串口通信所以删去名字中的“串口”
 * 2.为减少测量时间，缩短了测量之间的时间差
       @是否会影响测量准确性-否
       @是否会导致误差值连续出现-距离较远时可能会出现不稳定

 */
byte pingPin = 11; //Trig接11号引脚
byte echoPin = 10; // Echo接10号引脚

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
   Serial.println(cm);//以厘米为单位
   delay(100);
}
