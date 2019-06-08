/*
   title: Neopixel Control Master code
   by. legatalee
   tel. 010-8512-1863
   mail. legatalee@Gmail.com
*/

#include <SoftwareSerial.h>
SoftwareSerial HC12(10, 11);//HC-12 TX Pin, HC-12 RX Pin

void setup() {
  Serial.begin(9600);//시리얼 통신속도 초기화
  HC12.begin(9600);//hc12 통신속도 초기화
}

void loop() {
  while (Serial.available()) {//시리얼에 들어온 신호 감지
    HC12.write(Serial.read());//HC12모듈로 시리얼에 들어온 값을 넘겨줌
  }
  while (HC12.available()) {//테스트용으로 사용한 코드, hc12로부터 값을 넘겨받을경우
    Serial.write(HC12.read());//시리얼로 넘겨받은 값 출력
  }
}
