#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <Bounce2.h>

// Arduino數位腳位2接到1-Wire裝置
#define ONE_WIRE_BUS 2
#define relay1 31      //控制製熱系統開關
#define relay2 33      //控制冷卻系統開關  
#define relay3 35      //控制製冷系統開關
#define relay4 37      //  熱水出水閥開關
#define relay5 39      //  溫水出水閥開關
#define relay6 41      //  冰水出水閥開關
#define relay7 43
#define relay8 45      //控制生水與熱水槽雙向水閥開關

#define key1  8      //  熱水出水開關
#define key2  9     //  溫水出水開關
#define key3  10      //  冰水出水開關
#define key4  11      //  再沸騰水開關
#define WT588DPin 4


// 運用程式庫建立物件
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

LiquidCrystal lcd(5,6,7,38,40,42,44);   //ok
uint8_t degree[8]  = { 
  140,146,146,140,128,128,128,128};
int hottemp = 100;
int warmtemp = 27;
int icetemp = 7 ;
int tempratio = 3 ;
int hotwaterstatus = 1 ;      // = 0,加熱中 / 1 = 可飲用
int warmwaterstatus = 1 ;     // = 0,加熱中 / 1 = 可飲用
int icewaterstatus = 1 ;     // = 0,加熱中 / 1 = 可飲用
int drinkstatus = 0 ;
Bounce btn1 = Bounce() ;
Bounce btn2 = Bounce() ;
Bounce btn3 = Bounce() ;
Bounce btn4 = Bounce() ;


void setup(void)
{
  Serial.begin(9600);
  Serial.println("Temperature Sensor");
  // 初始化 
  sensors.begin();
  lcd.begin(20, 4);
  lcd.createChar(0, degree);
  lcd.noAutoscroll() ;
  lcd.clear();

  pinMode(relay1,OUTPUT) ;
  pinMode(relay2,OUTPUT) ;
  pinMode(relay3,OUTPUT) ;
  pinMode(relay4,OUTPUT) ;
  pinMode(relay5,OUTPUT) ;
  pinMode(relay6,OUTPUT) ;
  pinMode(relay7,OUTPUT) ;
  pinMode(relay8,OUTPUT) ;
  //=======================
  pinMode(WT588DPin, OUTPUT);
  //=========================
  pinMode(key1,INPUT) ;
  pinMode(key2,INPUT) ;  
  pinMode(key3,INPUT) ;
  pinMode(key4,INPUT) ;
  btn1.attach(key1) ;
  btn1.interval(5);
   btn2.attach(key2) ;
  btn2.interval(5);
   btn3.attach(key3)  ;
  btn3.interval(5);
   btn4.attach(key4)  ;
  btn4.interval(5); 
}

void loop(void)
{
  double sensor1 ,sensor2 ,sensor3;
  // 要求匯流排上的所有感測器進行溫度轉換
  sensors.requestTemperatures();

  // 取得溫度讀數（攝氏）並輸出，
  // 參數0代表匯流排上第0個1-Wire裝置 
  sensor1 = sensors.getTempCByIndex(0) ;
  sensor2 = sensors.getTempCByIndex(1) ;
  sensor3 = sensors.getTempCByIndex(2) ;
  Serial.print("Hot Water Temperatures is ") ;
  Serial.print(sensor1);
  Serial.print("\n") ;
  Serial.print("Warm Water Temperatures is ") ;
  Serial.print(sensor2);
  Serial.print("\n") ;
  Serial.print("Ice Water Temperatures is ") ;
  Serial.print(sensor3);
  Serial.print("\n") ;
  showdegree(sensor1,1) ;
  showdegree(sensor2,2) ;
  showdegree(sensor3,3) ;
  //================
  checkhot(sensor1,hottemp,tempratio) ;
  checkwarm(sensor2,warmtemp,tempratio) ;
  checkice(sensor3,icetemp,tempratio) ;
  //  Serial.println(hottemp) ;
  //   Serial.println(tempratio) ; 
  drinkhotwater() ;
  drinkwarmwater() ;
  drinkicewater() ;
  // digitalWrite(relay4, HIGH) ;
  //digitalWrite(relay5, HIGH) ;
 //  digitalWrite(relay6, HIGH) ;
  delay(1000);
}

void drinkhotwater()
{
      btn1.update() ;
      int bt = btn1.read() ;
      Serial.print("the btn1 is ");
      Serial.print(bt) ;
      Serial.print("\n");     
   if (hotwaterstatus == 0 )
        return ;         
  if (drinkstatus == 0 )
  {
     if (bt) 
     {
          drinkstatus = 1 ;         
          digitalWrite(relay4, HIGH) ;
            PlayVoice(4,WT588DPin);      //4飲用熱水    
          userstatus(1,0) ;
     }
  }
  else
  {
    if (drinkstatus == 1 )
          drinkstatus = 0 ;
        digitalWrite(relay4, LOW) ;
        userstatus(1,1) ;
  }
}


void drinkwarmwater()
{
     btn2.update() ;
      int bt = btn2.read() ;
      Serial.print("the btn2 is ");
      Serial.print(bt) ;
      Serial.print("\n");   
   if (warmwaterstatus  == 0 )
        return ;   
 if (bt) 
  {
    if (drinkstatus == 0 )
     {          
         drinkstatus = 2 ;     
          digitalWrite(relay5, HIGH) ;
         PlayVoice(5,WT588DPin);      // 5飲用溫水       
            userstatus(2,0) ;
     }
  }
  else
  {
    if (drinkstatus == 2 )
              drinkstatus = 0 ;       
    digitalWrite(relay5, LOW) ;
        userstatus(2,1) ;
  }
}


void drinkicewater()
{
     btn3.update() ;
      int bt = btn3.read() ;
      Serial.print("the btn3 is ");
      Serial.print(bt) ;
      Serial.print("\n");      
   if (icewaterstatus   == 0 )
        return ;         
 if (bt) 
  {
    if (drinkstatus == 0 )
     {          

         drinkstatus = 3 ; 
        digitalWrite(relay6, HIGH) ;
         PlayVoice(6,WT588DPin);      // 6飲用冰水           
        userstatus(3,0) ;
     }
  }
  else
  {
    if (drinkstatus == 3 )
              drinkstatus = 0 ;           
      digitalWrite(relay6, LOW) ;
      userstatus(3,1) ;
  }
}


void checkhot(int s1 , int maxtemp , int ratiotemp)
{

  Serial.print("S1 = ");
  Serial.print(s1);
  Serial.print("\n");

  if (s1 <= (maxtemp - ratiotemp))
  {
    if (hotwaterstatus != 0)
    {      

      Serial.println("Hoting") ; 
      digitalWrite(relay1, HIGH) ;
      digitalWrite(relay8, HIGH) ;
      hotwaterstatus = 0 ;      
      showstatus(1,hotwaterstatus) ;

     PlayVoice(1,WT588DPin);      //1熱水加熱中
           delay(3000);      
    }
  }

  if (s1 >= maxtemp)
  {
    if (hotwaterstatus == 0)
    {
      Serial.println("HOT Water Ready") ; 
      digitalWrite(relay1, LOW) ;
      digitalWrite(relay8, LOW) ;
     hotwaterstatus = 1 ;      
      showstatus(1,hotwaterstatus) ;           
 

    }
    else
    {
      Serial.println("HOT Water Ready") ; 
      showstatus(1,hotwaterstatus) ;             
    }       
  }


}

void checkwarm(int s1 , int maxtemp , int ratiotemp)
{
  Serial.print("S2 = ");
  Serial.print(s1);
  Serial.print("\n");
  //  Serial.print("S1 = ");
  //  Serial.print(s1);
  //  Serial.print("\n");

  if (s1 >= (maxtemp + ratiotemp))
  {
    if (warmwaterstatus != 0)
    {
      Serial.println("Cooling") ; 
      digitalWrite(relay2, HIGH) ;
      warmwaterstatus = 0 ;      
      showstatus(2,warmwaterstatus) ;

       PlayVoice(2,WT588DPin);      //2溫水冷卻中
      delay(3000);
    }
  }

  if (s1 <= maxtemp)
  {
    if (warmwaterstatus == 0)
    {
      Serial.println("Warm Water Ready") ; 
      digitalWrite(relay2, LOW) ;
        warmwaterstatus = 1 ;  
      showstatus(2,warmwaterstatus) ;             

  }
    else
    {
      Serial.println("Warm Water Ready") ; 
      showstatus(2,warmwaterstatus) ;             
    }

  }


}

void checkice(int s1 , int maxtemp , int ratiotemp)
{
  Serial.print("S3 = ");
  Serial.print(s1);
  Serial.print("\n");
  //  Serial.print("S1 = ");
  //  Serial.print(s1);
  //  Serial.print("\n");

  if (s1 >= (maxtemp + ratiotemp))
  {
    if (icewaterstatus != 0)
    {
      Serial.println("Ice Water Colding") ; 
      digitalWrite(relay3, HIGH) ;
      icewaterstatus = 0 ;
      showstatus(3,icewaterstatus) ;
      PlayVoice(3,WT588DPin);      //3冰水製造中      
      delay(3000);      
    }
  }

  if (s1 <= maxtemp)
  {
    if (icewaterstatus == 0)
    {
      Serial.println("ICE Water Ready") ; 
      digitalWrite(relay3, LOW) ;
      icewaterstatus = 1 ;
      showstatus(3,icewaterstatus) ;             
    }
    else
    {
      Serial.println("ICE Water Ready") ; 
      showstatus(3,icewaterstatus) ;             
    }       
  }


}


void showtitle()
{
  lcd.setCursor(0,0);
  lcd.print("Arduino Water Fountain");
}

void showdegree(int degno, int type)
{
  switch (type)
  {
  case 1 :
    lcd.setCursor(0,1);
    lcd.print("HOT :");
    lcd.print(degno);
#if ARDUINO >= 100
    lcd.write((byte)0);
#else
    lcd.print(0, BYTE);
#endif
    lcd.print("C");      
    break ;

  case 2 :
    lcd.setCursor(0,2);
    lcd.print("WARM :");
    lcd.print(degno);
#if ARDUINO >= 100
    lcd.write((byte)0);
#else
    lcd.print(0, BYTE);
#endif
    lcd.print("C");      
    break ;

  case 3 :
    lcd.setCursor(0,3);
    lcd.print("ICE :");
    lcd.print(degno);
#if ARDUINO >= 100
    lcd.write((byte)0);
#else
    lcd.print(0, BYTE);
#endif
    lcd.print("C");      
    break ;

  }
}


void showstatus(int watertype, int type)
{
  // watertype =1.熱水/2.溫水/3.冰水 
  // type  = 0,加熱中 / 1 = 可飲用
  switch (watertype)
  {
  case 1 :
    if (type == 0 )
    {
      lcd.setCursor(12,1);
      lcd.print("Hotting ");
    }
    else
    {
      lcd.setCursor(12,1);
      lcd.print("Ready  ");
    }
    break ;

  case 2 :
    if (type == 0 )
    {
      lcd.setCursor(12,2);
      lcd.print("Cooling  ");

    }
    else
    {
      lcd.setCursor(12,2);
      lcd.print("Ready  ");
    }
    break ;


  case 3 :
    if (type == 0 )
    {
      lcd.setCursor(12,3);
      lcd.print("ICEing  ");

    }
    else
    {
      lcd.setCursor(12,3);
      lcd.print("Ready  ");
    }
    break ;


  }
}

void userstatus(int watertype, int type)
{
  // watertype =1.熱水/2.溫水/3.冰水 
  // type  = 0,供應中 / 1 = 可飲用
  switch (watertype)
  {
  case 1 :
    if (type == 0 )
    {
      lcd.setCursor(12,1);
      lcd.print("Supply  ");
    }
    else
    {
      lcd.setCursor(12,1);
      lcd.print("Ready   ");
    }
    break ;

  case 2 :
    if (type == 0 )
    {
      lcd.setCursor(12,2);
      lcd.print("Supply   ");
    }
    else
    {
      lcd.setCursor(12,2);
      lcd.print("Ready  ");
    }
    break ;


  case 3 :
    if (type == 0 )
    {
      lcd.setCursor(12,3);
      lcd.print("Supply     ");
    }
    else
    {
      lcd.setCursor(12,3);
      lcd.print("Ready      ");
    }
    break ;


  }
}

void PlayVoice(unsigned char addr,unsigned int addrpin)
{
  digitalWrite(addrpin,0);
  delay(5);
  for(int i=0;i<8;i++)
  {
    digitalWrite(addrpin,1);
    if(addr & 1)
    {
      delayMicroseconds(600);
      digitalWrite(addrpin,0);
      delayMicroseconds(300);
    }
    else
    {
      delayMicroseconds(300);
      digitalWrite(addrpin,0);
      delayMicroseconds(600);
    }
    addr>>=1;  //此行用>>=还是=>>还不确定
  }
  digitalWrite(addrpin,1);
}


/*用于播放由一串地址组成的语句*/
void PlayVoiceSerial(unsigned int addrserial[],unsigned int len,unsigned int addrpin,unsigned int busypin)
{
  for(int i=0;i<len;i++)
  {
    int addr ;
    addr=addrserial[i];
    PlayVoice(addr,addrpin);
    pulseIn(busypin,0);
    pulseIn(busypin,1);
  }
}

