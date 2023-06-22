/*
  乾燥時1, 湿潤時0
  論理積をとる
  11111 = 1, 異常なし
  11101 = 0, 異常あり 

  IRセンサの真理値
  接触時 0
  非接触時 1
  なので以下では反転させる
*/

/*--------------------------include, 変数宣言-----------------------------------*/
#include <avr/sleep.h>

//IRセンサの入力
const int IR1 = 4;
const int IR2 = 2; //真ん中のセンサ, 割り込み処理を行うために2番ピン
const int IR3 = 3;

//Moisture senser
const int MS1 = A1;
const int MS2 = A2;
const int MS3 = A3;

const int SPEAKER = 12;

int sleep_count = 0;

#define BEATTIME 200 //音を出している時間(msec)
#define S 50

/*---------------------------------------------------------------------*/

void setup(){
  Serial.begin(9600);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(SPEAKER, OUTPUT);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  attachInterrupt(digitalPinToInterrupt(IR2), wakeup, CHANGE);
}

void loop(){

  int IRin_1 = !(digitalRead(IR1));
  int IRin_2 = !(digitalRead(IR2));
  int IRin_3 = !(digitalRead(IR3));
  int MSin_1, MSin_2, MSin_3; //水分センサの入力値
  int Logic1, Logic2, Logic3; //水分センサの入力値で変化する論理

  /*---------------------------1番センサ-------------------------------*/
  if(IRin_1 == 1){
    MSin_1 = analogRead(MS1);//値をとる

    if(MSin_1 > 470){//閾値以上なら
      Logic1 = 1;//問題なし
    }else{
      Logic1 = 0;//以下なら問題あり
    }

  }else{
    MSin_1 = -1;//エラー値とする。センサーの値はとらない
    Logic1 = 1;//スピーカーを鳴らすif文に干渉しないように,問題なし1
  }
  /*---------------------------------------------------------------------*/
  

  /*----------------------------2番センサ------------------------------*/
  if(IRin_2 == 1){
    MSin_2 = analogRead(MS2);

    if(MSin_2 > 470){
      Logic2 = 1;
    }
    else{
      Logic2 = 0;
    }

  }else{
    MSin_2 = -1;
    Logic2 = 1;
  }
  /*---------------------------------------------------------------------*/
  


  /*----------------------------3番センサ-------------------------------*/
  if(IRin_3 == 1){
    MSin_3 = analogRead(MS3);

    if(MSin_3 > 480){
      Logic3 = 1;
    }
    else{
      Logic3 = 0;
    }

  }else{
    MSin_3 = -1;
    Logic3 = 1;
  }
  /*---------------------------------------------------------------------*/


  if((Logic1 && Logic2 && Logic3) != 1){ //Logicの論理積が0なら(湿潤時なら)
    tone(SPEAKER,440,BEATTIME);
    delay(BEATTIME);
    sleep_count++;

    if(sleep_count > S){
      sleep_enable();
      sleep_cpu();
      sleep_disable();
      sleep_count = 0;
    }
  }

  sleep_count++;

  if(sleep_count > S){
      sleep_enable();
      sleep_cpu();
      sleep_disable();
      sleep_count = 0;
  }
   

/*-----------------------------デバッグ用----------------------------------
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
  */
}


void wakeup(){
  ;
}


