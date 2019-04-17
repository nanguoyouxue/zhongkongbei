/*arduino接收电脑指令
 * by czl
 * 2019/04/17
 */

String inString = "";
void  setup(){
  Serial.begin(9600);
}
void loop(){
  uartReceive();
  if(inString[0]=='1'&&inString[1]=='2'&&inString[2]=='3')Serial.println("hello world");
  inString="";
  delay(500);
}
//接收串口发来的字符串
void uartReceive(){        
    while (Serial.available()>0) {   //如果串口有数据
        char inChar = Serial.read(); //读取串口字符
        //inString += inChar;
        inString.concat(inChar);     //连接接收到的字符组
        delayMicroseconds(100);      //为了防止数据丢失,在此设置短暂延时100us
        Serial.flush();             //清空串口接收缓存
        if(inString.length() > 200){
            inString ="";
        }
    }
}
