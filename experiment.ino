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

  
  int now_time = millis();
  if(set_time - now_time == 3*60*1000){ //millis()は50日でオーバーフロー。絶対値をとることでset_time - now_timeが負の値になることを防ぐ
    //sleepモードと割り込みを有効化
    sleep_enable();

    //割り込みの条件を設定、真ん中のIR2の立下り(タオルを持ち上げたとき)
    sleep_cpu();

    //sleepモードと割り込みを無効化
    sleep_disable();
  }
  


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


void SetTime(){
  set_time = millis();
}

void WakeUp(){
  ;
}


