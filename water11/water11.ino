#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>


// Arduino數位腳位2接到1-Wire裝置
#define ONE_WIRE_BUS 2

// 運用程式庫建立物件
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

LiquidCrystal lcd(5,6,7,38,40,42,44);   //ok
uint8_t degree[8]  = {
  140,146,146,140,128,128,128,128};


void setup(void)
{
  Serial.begin(9600);
  Serial.println("Temperature Sensor");
  // 初始化 
  sensors.begin();
  lcd.begin(20, 4);
  lcd.createChar(0, degree);
  lcd.clear();
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
  
  delay(1000);
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
    break ;

  }
}


