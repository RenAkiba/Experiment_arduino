/*
void setup(){
  Serial.begin(9600);
}

void loop(){
  int timer1;
  int timer2;

  timer1 = millis();

  for(int i = 0; i < 10; i++){
    serial.println("御社が第一志望です");
  }

  timer2 = millis();

  serial.print("timer1:");
  serial.println(timer1);
  serial.print("timer2:");
  serial.println(timer2);
}
*/






/*
  乾燥時1, 湿潤時0
  論理積をとる
  11111 = 1, 異常なし
  11101 = 0, 異常あり 
*/


#include <avr/sleep.h>

void WakeUp(){
  delay(1);
}

//IRセンサの入力
const int IR1 = 1;
const int IR2 = 2;//真ん中
const int IR3 = 3;

//Moisture senser
const int MS1 = A1;
const int MS2 = A2;
const int MS3 = A3;

const int SPEAKER = 12;

int set_time;

#define BEATTIME 200 //音を出している時間(msec)
#define n 450

void SetTime(){
  set_time = millis();
}

void setup(){
  Serial.begin(9600);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(SPEAKER, OUTPUT);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  attachInterrupt(IR2, WakeUp, FALLING);
  attachInterrupt(IR2, SetTime, RISING);
}

void loop(){

  int IRin_1 = digitalRead(IR1);
  int IRin_2 = digitalRead(IR2);
  int IRin_3 = digitalRead(IR3);
  int MSin_1, MSin_2, MSin_3; //水分センサの入力値
  int Logic1, Logic2, Logic3; //水分センサの入力値で変化する論理

  //1番のセンサ////////////////////////////
  if(IRin_1 == 1){
    MSin_1 = analogRead(MS1);//値をとる

    if(MSin_1 > n){//閾値以上なら
      Logic1 = 1;//問題なし
    }else{
      Logic1 = 0;//以下なら問題あり
    }

  }else{
    MSin_1 = -1;//値をとらない
    Logic1 = 1;//問題なし
  }
  

  //2番のセンサ////////////////////////////
  if(IRin_2 == 1){
    MSin_2 = analogRead(MS2);

    if(MSin_2 > n){
      Logic2 = 1;
    }
    else{
      Logic2 = 0;
    }

  }else{
    MSin_2 = -1;
    Logic2 = 1;
  }
  


  //3番のセンサ////////////////////////////
  if(IRin_3 == 1){
    MSin_3 = analogRead(MS3);

    if(MSin_3 > n){
      Logic3 = 1;
    }
    else{
      Logic3 = 0;
    }

  }else{
    MSin_3 = -1;
    Logic3 = 1;
  }


  if((Logic1 && Logic2) && Logic3 == 0){ //Logicの論理積が0なら(湿潤時なら)
    tone(SPEAKER,440,BEATTIME) ; // ラ
    delay(BEATTIME) ;
  }

  /*
  int now_time = millis();
  if(set_time - now_time == 3*60*1000){ //millis()は50日でオーバーフロー。絶対値をとることでset_time - now_timeが負の値になることを防ぐ
    //sleepモードと割り込みを有効化
    sleep_enable();

    //割り込みの条件を設定、真ん中のIR2の立下り(タオルを持ち上げたとき)
    sleep_cpu();

    //sleepモードと割り込みを無効化
    sleep_disable();
  }
  */


  Serial.print("IR1:");
  Serial.println(IRin_1);

  Serial.print("IR2:");
  Serial.println(IRin_2);

  Serial.print("IR3:");
  Serial.println(IRin_3);

  Serial.print("MSin_1:");
  Serial.println(MSin_1);

  Serial.print("MSin_2:");
  Serial.println(MSin_2);

  Serial.print("MSin_3:");
  Serial.println(MSin_3);

  Serial.print("Logic1:");
  Serial.println(Logic1);

  Serial.print("Logic2:");
  Serial.println(Logic2);

  Serial.print("Logic3:");
  Serial.println(Logic3);
  Serial.println("--------------------------------");
}








/*5個分

#include <var/sleep.h>

//IRセンサの入力
const int IR0 = 0;//一番左端
const int IR1 = 1;
const int IR2 = 2;//真ん中
const int IR3 = 3;
const int IR4 = 4;//一番右端

//Moisture senser
const int MS0 = A0;
const int MS1 = A1;
const int MS2 = A2;
const int MS3 = A3;
const int MS4 = A4;

const int SPEAKER = 12

#define BEATTIME 200 //音を出している時間(msec)
#define n 450

void setup(){
  Serial.begin(9600);
  pinMode(IR0, INPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(SPEAKER, OUTPUT);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  attachInterrupt(IR2, WakeUp(), FALLING);
  attachInterrupt(IR2, SetTime(), RISING);
}

void loop(){

  int IRin_0 = digitalRead(IR0); //IR赤外線回避センサの入力値
  int IRin_1 = digitalRead(IR1);
  int IRin_2 = digitalRead(IR2);
  int IRin_3 = digitalRead(IR3);
  int IRin_4 = digitalRead(IR4);
  int MSin_0, MSin_1, MSin_2, MSin_3, MSin_4; //水分センサの入力値
  int Logic0, Logic1, Logic2, Logic3, Logic4; //水分センサの入力値で変化する論理

  //0番のセンサ////////////////////////////
  if(IRin_0 == 1){ //IR回避センサが反応しているなら
    MSin_0 = analogRead(A0); //土壌水分センサで値を読み取る

    if(MSin_0 > n){//読み取った値が閾値以上(乾燥)なら
      Logic0 = 1; //異常なし
    }
    else{
      Logic0 = 0; //異常あり
    }
    
  }else{ //IRが反応していないなら
    Logic0 = 1; 
    //値はとらない.
    //スピーカーを鳴らす関数に干渉しないようにLogic1
  }
  

  //1番のセンサ////////////////////////////
  if(IRin_1 == 1){
    MSin_1 = analogRead(MS1);

    if(MSin_1 > n){
    Logic1 = 1;
    }
    else{
      Logic1 = 0;
    }

  }else{
    Logic1 = 1;
  }
  

  //2番のセンサ////////////////////////////
  if(IRin_2 == 1){
    MSin_2 = analogRead(MS2);

    if(MSin_2 > n){
      Logic2 = 1;
    }
    else{
      Logic2 = 0;
    }

  }else{
    Logic2 = 1;
  }
  


  //3番のセンサ////////////////////////////
  if(IRin_3 == 1){
    MSin_3 = analogRead(MS3);

    if(MSin_3 > n){
      Logic3 = 1;
    }
    else{
      Logic3 = 0;
    }

  }else{
    Logic3 = 1;
  }


  //4番のセンサ////////////////////////////
  if(IRin_4 == 1){
    MSin_4 = analogRead(MS4);

    if(MSin_4 > n){
    Logic4 = 1;
    }
    else{
      Logic4 = 0;
    }

  }else{
    Logic4 = 1;
  }
  

  if(Logic0 && Logic1 && Logic2 && Logic3 && Logic4 != 1){ //Logicの論理積が0なら(湿潤時なら)
    tone(SPEAKER,440,BEATTIME) ; // ラ
    delay(BEATTIME) ;
  }

  if(abs(set_time - now_time) == 3*60*1000){ //millis()は50日でオーバーフロー。絶対値をとることでset_time - now_timeが負の値になることを防ぐ
    //sleepモードと割り込みを有効化
    sleep_enable();

    //割り込みの条件を設定、真ん中のIR2の立下り(タオルを持ち上げたとき)
    sleep_cpu();

    //sleepモードと割り込みを無効化
    sleep_disable();
  }
}

void SetTime(){
  set_time = millis();
}

void WakeUp(){
  ;
}



*/



/*
void setup() {
  Serial.begin(9600); //Arduinoとパソコンで通信を始めますよ
}

void loop() {
  int q = analogRead(A0); //「in」こと「A0」の値を読む
  int w = analogRead(A1);
  int e = analogRead(A2);
  int r = analogRead(A3);
  int t = analogRead(A4);

  Serial.print("a0:");
  Serial.println(q); //読んだ値をパソコンのシリアルモニタに表示する
  Serial.print("a1:");
  Serial.println(w);
  Serial.print("a2:");
  Serial.println(e);
  Serial.print("a3:");
  Serial.println(r);
  Serial.print("a4:");
  Serial.println(t);

  delay(1000);
}
*/