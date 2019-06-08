/*
   title: Neopixel Control Slave code
   by. legatalee
   tel. 010-8512-1863
   mail. legatalee@Gmail.com
*/

#include<Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial HC12(9, 6);//HC-12 TX Pin, HC-12 RX Pin

#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(30, 10, NEO_GRB + NEO_KHZ800);//10번pwm핀에 연결된 네오픽셀 스트립 선언
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(30, 11, NEO_GRB + NEO_KHZ800);//11번, 30은 led의 개수

float delayvalue_ow;
float delayvalue_fk;
float maintainvalue;
int brightness;
int fred, fgreen, fblue;//front color
int bred, bgreen, bblue;//background color

int check = 1;//while문 loop탈출용 변수

int timecheck = 1;//while문 loop탈출용 변수

void setup() {
  Serial.begin(9600);//시리얼 통신속도 초기화
  HC12.begin(9600);//HC12 통신속도 초기화
  strip1.begin();//네오픽셀 스트립 시작
  strip2.begin();
  fred = 0; fgreen = 0; fblue = 0; //초기화
  bred = 0; bgreen = 0; bblue = 0; //초기화
}

void loop() {
  String input = "";//input 초기화
  while (HC12.available() > 0)//HC12 신호감지시
  {
    input += (char) HC12.read();//string형 input에 char형식으로 한글자씩 저장
    delay(1);
    Serial.println(input);
  }
  check = 1;
  if (input == "da") {//양쪽 동시제어
    Serial.println("readDATA");
    readDATA();//컬러 읽기 함수로 이동
  }
  if (input == "ld") {//왼쪽만 제어
    Serial.println("readDATA");
    readDATA();
  }

  if (input == "ov") {//색 덮어씌우기
    readdelay1vlaue();
    /*  for (int i = 0; i <= 30; i++) {//함수 넘어가기 전 배경색 채우기(딜레이 없이 한번에)
          strip1.setPixelColor(i, strip1.Color(bred, bgreen, bblue));
          strip2.setPixelColor(i, strip2.Color(bred, bgreen, bblue));
          strip1.show();
          strip2.show();//이 소스를 사용하려면 overwrite 함수에 전경색 채우기와 배경색 채우기 순서를 바꿔주셔야 합니다.
        }*/
    overwrite(delayvalue_ow);
  }
  if (input == "lo") {
    readdelay1vlaue();
    overwrite(delayvalue_ow);
  }

  if (input == "fl") {//배경색 전경색 깜빡임
    readdelay2vlaue();
    flicker(delayvalue_fk);
  }
  if (input == "lf") {
    readdelay2vlaue();
    flicker(delayvalue_fk);
  }

  if (input == "sh") {//전경색 n밀리초동안 밝히기
    readmaintainvalue();
    shine(maintainvalue);
  }
  if (input == "ls") {
    readmaintainvalue();
    shine(maintainvalue);
  }

  if (input == "sb") {//set brightness
    Serial.println("set brightness!");
    setbrightness();
  }

  if (input == "value") {//테스트용, 현재 전경색 배경색 값 출력
    Serial.println("OC");
    Serial.println(fred);
    Serial.println(fgreen);
    Serial.println(fblue);
    Serial.println("BC");
    Serial.println(bred);
    Serial.println(bgreen);
    Serial.println(bblue);
  }
  if (input == "show") {//테스트용, 전경색 네오픽셀에 출력
    showcolor();
  }
  if (input == "showb") {//테스트용, 배경색 네오픽셀에 출력
    showbcolor();
  }
  if (input == "clear") {//테스트용, 네오픽셀 led 전부 끄기
    for (int i = 0; i <= 30; i++) {
      strip1.setPixelColor(i, strip1.Color(0, 0, 0));
      strip2.setPixelColor(i, strip2.Color(0, 0, 0));
      strip1.show();
      strip2.show();
      delay(30);
    }
  }
}

void readDATA() {//색읽기 함수
  while (check) {
    timecheck = 1;
    String input = "";
    while (HC12.available() > 0)
    {
      input += (char) HC12.read();
      delay(1);
      Serial.println(input);
    }
    if (input == "f") {//전경색 읽기
      readFCOLOR();
    }
    if (input == "b") {//배경색 읽기
      readBCOLOR();
    }
    if (input == "set")//색 설정을 마치고 loop함수로 회귀
      check = 0;
  }
}


void readFCOLOR() {//전경색 읽기 함수
  fred = 0;
  fgreen = 0;
  fblue = 0;
  Serial.println("FrontRed:");
  while (fred == 0) {//전경색 RED
    String input = "";
    while (HC12.available() > 0)
    {
      input += (char) HC12.read();
      delay(1);
      fred = (input.toInt());//string형 input을 int형으로 형변환하여 값 저장
    }
  }
  if (fred == 256)//256의 값을 받으면 0으로 저장
    fred = 0;
  Serial.println(fred);
  Serial.println("FrontGreen:");
  while (fgreen == 0) {
    String input = "";
    while (HC12.available() > 0)
    {
      input += (char) HC12.read();
      delay(1);
      fgreen = (input.toInt());
    }
  }
  if (fgreen == 256)
    fgreen = 0;
  Serial.println(fgreen);
  Serial.println("FrontBlue:");
  while (fblue == 0) {
    String input = "";
    while (HC12.available() > 0)
    {
      input += (char) HC12.read();
      delay(1);
      fblue = (input.toInt());
    }
  }
  if (fblue == 256)
    fblue = 0;
  Serial.println(fblue);
  Serial.println("FCOLOR read success!");
}
void readBCOLOR() {//배경색 읽기 함수
  bred = 0;
  bgreen = 0;
  bblue = 0;
  Serial.println("BackgroundRed:");
  while (bred == 0) {//배경색 RED
    String input = "";
    while (HC12.available() > 0)
    {
      input += (char) HC12.read();
      delay(1);
      bred = (input.toInt());
    }
  }
  if (bred == 256)
    bred = 0;
  Serial.println(bred);
  Serial.println("BackgroundGreen:");
  while (bgreen == 0) {
    String input = "";
    while (HC12.available() > 0)
    {
      input += (char) HC12.read();
      delay(1);
      bgreen = (input.toInt());
    }
  }
  if (bgreen == 256)
    bgreen = 0;
  Serial.println(bgreen);
  Serial.println("BackgroundBlue:");
  while (bblue == 0) {
    String input = "";
    while (HC12.available() > 0)
    {
      input += (char) HC12.read();
      delay(1);
      bblue = (input.toInt());
    }
  }
  if (bblue == 256)
    bblue = 0;
  Serial.println(bblue);
  Serial.println("BCOLOR read success!");
}


void readdelay1vlaue() {//딜레이값 읽기 함수, overwrite용 10단계
  delayvalue_ow = 0;
  while (delayvalue_ow == 0) {
    Serial.println("while in!");
    String input = "";
    while (HC12.available() > 0)
    {
      input += (char) HC12.read();
      delay(1);
    }
    if (input == "1")
      delayvalue_ow = 95;
    if (input == "2")
      delayvalue_ow = 85;
    if (input == "3")
      delayvalue_ow = 75;
    if (input == "4")
      delayvalue_ow = 65;
    if (input == "5")
      delayvalue_ow = 55;
    if (input == "6")
      delayvalue_ow = 45;
    if (input == "7")
      delayvalue_ow = 35;
    if (input == "8")
      delayvalue_ow = 25;
    if (input == "9")
      delayvalue_ow = 15;
    if (input == "10")
      delayvalue_ow = 5;
  }
  Serial.println(delayvalue_ow);
  Serial.println("delayvalue_owRead escape!");
}
void readdelay2vlaue() {//딜레이값 읽기 함수, flicker용 10단계
  delayvalue_fk = 0;
  while (delayvalue_fk == 0) {
    Serial.println("while in!");
    String input = "";
    while (HC12.available() > 0)
    {
      input += (char) HC12.read();
      delay(1);
    }
    if (input == "1")
      delayvalue_fk = 1850;
    if (input == "2")
      delayvalue_fk = 1650;
    if (input == "3")
      delayvalue_fk = 1450;
    if (input == "4")
      delayvalue_fk = 1250;
    if (input == "5")
      delayvalue_fk = 1050;
    if (input == "6")
      delayvalue_fk = 850;
    if (input == "7")
      delayvalue_fk = 650;
    if (input == "8")
      delayvalue_fk = 450;
    if (input == "9")
      delayvalue_fk = 250;
    if (input == "10")
      delayvalue_fk = 50;
  }
  Serial.println(delayvalue_fk);
  Serial.println("delayvalue_fkRead escape!");
}
void readmaintainvalue() {//실수형으로 밀리초값 읽기 함수
  maintainvalue = 0;
  while (maintainvalue == 0) {
    Serial.println("while in!");
    String input = "";
    while (HC12.available() > 0)
    {
      input += (char) HC12.read();
      delay(1);
    }
    maintainvalue = (input.toFloat());//string형 float형변환하여 저장
  }
  Serial.println(maintainvalue);
  Serial.println("MaintainValueRead escape!");
}


void setbrightness() {
  brightness = 0;
  Serial.println("input:");
  while (brightness == 0) {
    String input = "";
    while (HC12.available() > 0)
    {
      input += (char) HC12.read();
      delay(1);
      brightness = (input.toInt());//string형 input을 int형으로 형변환하여 값 저장
    }
  }
  Serial.println(brightness);
  strip1.setBrightness(brightness);
  strip2.setBrightness(brightness);
}


void overwrite(int delaytime) {//overwrite 함수
  while (timecheck) {
    String input = "";
    for (int i = 0; i <= 30; i++) {
      while (HC12.available() > 0)
      {
        input += (char) HC12.read();
        delay(1);
        Serial.println(input);
      }
      if (input == "stop" || input == "⸮") {//stop신호 입력시 즉시 led 초기화 후 리턴
        for (int i = 0; i <= 30; i++) {
          strip1.setPixelColor(i, strip1.Color(0, 0, 0));
          strip2.setPixelColor(i, strip2.Color(0, 0, 0));
          strip1.show();
          strip2.show();
        }
        return;
      }
      strip1.setPixelColor(i, strip1.Color(bred, bgreen, bblue));
      strip2.setPixelColor(i, strip2.Color(bred, bgreen, bblue));
      strip1.show();
      strip2.show();
      delay(delaytime);
    }
    for (int i = 0; i <= 30; i++) {
      while (HC12.available() > 0)
      {
        input += (char) HC12.read();
        delay(1);
        Serial.println(input);
      }
      if (input == "stop" || input == "⸮") {
        for (int i = 0; i <= 30; i++) {
          strip1.setPixelColor(i, strip1.Color(0, 0, 0));
          strip2.setPixelColor(i, strip2.Color(0, 0, 0));
          strip1.show();
          strip2.show();
        }
        return;
      }
      strip1.setPixelColor(i, strip1.Color(fred, fgreen, fblue));
      strip2.setPixelColor(i, strip2.Color(fred, fgreen, fblue));
      strip1.show();
      strip2.show();
      delay(delaytime);
    }
  }
}
void flicker(int delaytime) {//깜빡임 함수
  while (timecheck) {
    String input = "";
    for (int i = 0; i <= 30; i++) {//배경색 깔기
      strip1.setPixelColor(i, strip1.Color(bred, bgreen, bblue));
      strip2.setPixelColor(i, strip2.Color(bred, bgreen, bblue));
      strip1.show();
      strip2.show();
    }
    delay(delaytime);
    while (HC12.available() > 0)
    {
      input += (char) HC12.read();
      Serial.println(input);
    }
    if (input == "stop" || input == "⸮") {
      for (int i = 0; i <= 30; i++) {
        strip1.setPixelColor(i, strip1.Color(0, 0, 0));
        strip2.setPixelColor(i, strip2.Color(0, 0, 0));
        strip1.show();
        strip2.show();
      }
      break;
    }
    for (int i = 0; i <= 30; i++) {//전경색
      strip1.setPixelColor(i, strip1.Color(fred, fgreen, fblue));
      strip2.setPixelColor(i, strip2.Color(fred, fgreen, fblue));
      strip1.show();
      strip2.show();
    }
    delay(delaytime);
    while (HC12.available() > 0)
    {
      input += (char) HC12.read();
      Serial.println(input);
    }
    if (input == "st" || input == "⸮") {
      for (int i = 0; i <= 30; i++) {
        strip1.setPixelColor(i, strip1.Color(0, 0, 0));
        strip2.setPixelColor(i, strip2.Color(0, 0, 0));
        strip1.show();
        strip2.show();
      }
      break;
    }
  }
}
void shine(int delaytime) {//n초동안 전경색 켰다 끄기 함수
  for (int i = 0; i <= 30; i++) {
    strip1.setPixelColor(i, strip1.Color(fred, fgreen, fblue));
    strip2.setPixelColor(i, strip2.Color(fred, fgreen, fblue));
    strip1.show();
    strip2.show();
  }
  delay(delaytime);
  for (int i = 0; i <= 30; i++) {
    strip1.setPixelColor(i, strip1.Color(0, 0, 0));
    strip2.setPixelColor(i, strip2.Color(0, 0, 0));
    strip1.show();
    strip2.show();
  }
}

void showcolor() {//테스트용
  for (int i = 0; i <= 30; i++) {
    strip1.setPixelColor(i, strip1.Color(fred, fgreen, fblue));
    strip2.setPixelColor(i, strip2.Color(fred, fgreen, fblue));
    strip1.show();
    strip2.show();
  }
}

void showbcolor() {//테스트용
  for (int i = 0; i <= 30; i++) {
    strip1.setPixelColor(i, strip1.Color(bred, bgreen, bblue));
    strip2.setPixelColor(i, strip2.Color(bred, bgreen, bblue));
    strip1.show();
    strip2.show();
  }
}
