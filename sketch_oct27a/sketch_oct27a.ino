/*
TextTextTextTextTextTextTextTextTextTextTextTextTextTextTextTextTextTextTextTextTextTextText
TextTextTextTextTextTextTextTextTextTextTextTextTextTextTextTextTextTextTextTextTextTextText
*/
//LCD       引入Wire與LiquidCrystal_I2C函式庫
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
// 設定 LCD I2C 位址
// Set the pins on the I2C chip used for LCD connections:
// addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0X3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//LiquidCrystal_I2C lcd(0x27, 16,2);

//DHT
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN            7 
#define DHTTYPE           DHT11

//NewPing
#include <NewPing.h>
#define TRIGGER_PIN    12          // trigger Pin
#define ECHO_PIN    11          // Echo Pin
#define MAX_DISTANCE 200        // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
DHT dht(DHTPIN, DHTTYPE);       //#define DHTTYPE DHT11
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);    // 設定 NewPing 物件，並給與最遠測試距離



int CountsTimer1=0;
int CountsTimer2=0;
int draw_Limit=0;
int draw_Data=0;
char Waveform_H[]="9999";

String FormLcd[]={"","","",""}; //4 Line Lcd
String Waveform_O[] = {
  "14      11", 
  "13      21", 
  "12      31", 
  "11      41", 
};

//const char * Waveform_O[]={"12      11", "12      21", "12      31", "12      41" };



void setup() {
// Serial.begin(9600);
 Serial.begin(115200);   
 Serial.println("Arduino Test!");

  
// 初始化 LCD，一行 16 的字元，共 2 行，預設開啟背光
  lcd.begin(20, 4);
// 閃爍三次
  for(int i = 0; i < 3; i++) {
  lcd.backlight(); // 開啟背光
  delay(250);
  lcd.noBacklight(); // 關閉背光
  delay(250);
  }
  lcd.backlight();
// 輸出初始化文字
  lcd.setCursor(0, 0); // 設定游標位置在第一行行首
  lcd.print("Hello, Maker!");
  delay(1000);
  lcd.setCursor(0, 1); // 設定游標位置在第二行行首
  lcd.print("       LH7HL");  
  lcd.setCursor(0, 2); // 設定游標位置在第二行行首    
  lcd.print("    LH7     7HL");   
  lcd.setCursor(0, 3); // 設定游標位置在第二行行首
  lcd.print("LH7            7HL");
  delay(2000);
  lcd.clear(); //顯示清除
}


void Byebyeyo(){
/***********結束迴圈***********/
CountsTimer1=32767;
while(CountsTimer1==32767){
  /*
  byebye:CountsTimer1=32767
  empty
  */
  }
/***********結束迴圈***********/
}


void Project_DHT11(){
  dht.begin();
  //kid0924
  float h = dht.readHumidity();  
  float t = dht.readTemperature();  
  float f = dht.readTemperature(true);  
  if (isnan(h) || isnan(t) || isnan(f)) {  
    /*[||,&&,!]=[or,and,not]
      isnan()=test float is number (1/0).*/
    Serial.println("Failed to read from DHT sensor!");  
    return; //end and again
  }  

  float hif = dht.computeHeatIndex(f, h);  
  float hic = dht.computeHeatIndex(t, h, false);  
  char display_h[7];  
  dtostrf(h,2,0,display_h);  
  char display_t[7];  
  dtostrf(t,2,0,display_t); 
  Serial.print("Humidity: ");  
  Serial.print(h);  
  Serial.print(" %\t");  
  Serial.print("Temperature: ");  
  Serial.print(t);  
  Serial.print(" *C ");  
  Serial.print(f);  
  Serial.print(" *F\t");  
  Serial.print("Heat index: ");  
  Serial.print(hic);  
  Serial.print(" *C ");  
  Serial.print(hif);  
  Serial.println(" *F");   
  Serial.print(CountsTimer1); 
  Serial.print( float(int(h*10)/10)); 
  Serial.print("\t"); 

//lcd.setCursor(0, 0); // 設定游標位置在第一行行首
//lcd.print(String("Kai-Lin Hsu    ")+CountsTimer1);  //11+6
////lcd.setCursor(0, 1); // 設定游標位置在第二行行首
////lcd.print(String("Counts : ") +CountsTimer1);
//lcd.setCursor(0, 2); // 設定游標位置在第三行行首
////lcd.print(String("") + "H:" + display_h + "%");  
//lcd.print(String("") + "H:" + display_h + "%    "+"T:"+ display_t + (char)223 + "C");
//lcd.setCursor(0, 3); // 設定游標位置在第四行行首
////lcd.print(millis()/100);

}


void loop() {
  delay(200);  
  unsigned int uS = sonar.ping();   // 送出 ping，並取得微秒 microseconds(uS) 時間
  Serial.print("Ping: ");
  Serial.print(sonar.convert_cm(uS));    // 換算時間為公分，如顯示 0，表示超出距離範圍
  Serial.println("cm");

  for (int i=0; i<4;i++){
  Waveform_H[i]=' ';
  }
  
  draw_Limit=20;
  draw_Data=sonar.convert_cm(uS);
  for (int i=0; i<12;i=i+3){
    if (draw_Data>=draw_Limit-i&&draw_Data<draw_Limit+1-i){
      Waveform_H[i/3]=char(255); //top
      }
      else if  (draw_Data>=draw_Limit-1-i&&draw_Data<draw_Limit-1+1-i){
       Waveform_H[i/3]=char(45); //mid
      }
      else if  (draw_Data>=draw_Limit-2-i&&draw_Data<draw_Limit-1+1-i){
       Waveform_H[i/3]=char(95); //bottom
      }
    }
  if (draw_Data<=draw_Limit-12){
    Waveform_H[3]='O'; //overload
    }
  if (draw_Data>draw_Limit){
  Waveform_H[0]='O'; ////overload
  }


String CountsSensorValue="0";

  lcd.setCursor(0, 0); // 設定游標位置在第一行行首
  if (sonar.convert_cm(uS)<10){
  //lcd.print(String("                 0")+sonar.convert_cm(uS));
  CountsSensorValue=String("0")+sonar.convert_cm(uS);
  }
  else {
  //lcd.print(String("                 ")+sonar.convert_cm(uS));
  CountsSensorValue=sonar.convert_cm(uS);
  }
  
//  if (sonar.convert_cm(uS)==5){
//  Serial.println("goodgood");
//  lcd.setCursor(0, 3); // 設定游標位置在第一行行首
//  lcd.print(String("                a")+CountsTimer2);
//  CountsTimer2++;
//  delay(1000);
//  }
//  else{
//  Serial.println("badbad");
//  }
  
  for (int i=0; i<4;i++){         //顯示
  Waveform_O[i].remove(0,1);
  Waveform_O[i]=Waveform_O[i]+Waveform_H[i];
  FormLcd[i]=Waveform_O[i]+String(" ")+(draw_Limit+(-3*i)); //build_draw_array 10+1+2
  }




  FormLcd[0]=FormLcd[0]+String("     ")+CountsSensorValue; 
  FormLcd[1]=FormLcd[1]+String("    ");
  FormLcd[2]=FormLcd[2]+String("    ");
  FormLcd[3]=FormLcd[3]+String("    KID");

  for (int i=0; i<4;i++){         //顯示
//    Waveform_O[i].remove(0,1);
//    Waveform_O[i]=Waveform_O[i]+Waveform_H[i];
    lcd.setCursor(0, i); // 設定游標位置在第一行行首
    lcd.print(FormLcd[i]);
//    lcd.print(Waveform_O[i]+String(" ")+(-3*i+draw_Limit));
  }

  if (CountsTimer1==100){

  }

  if (CountsTimer1==999){
  Serial.println ("EndTheProject");
   Byebyeyo();
  }
  CountsTimer1++;
}
//if (t>=25){
//  Waveform_H[0]='O';
//  }
//else if(t>=24&&t<25){
//   Waveform_H[1]='O';
//  }
//  else if(t>=23&&t<24){
//   Waveform_H[1]='O';
//  }
//  else if(t>=22&&t<23){
//   Waveform_H[1]='O';
//  }
//  else if(t>=21&t<22){
//   Waveform_H[1]='O';
//  }
//  


