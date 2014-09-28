#include <OneWire.h>
#include <DallasTemperature.h>

// Arduino數位腳位2接到1-Wire裝置
#define ONE_WIRE_BUS 2

// 運用程式庫建立物件
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Temperature Sensor");
  // 初始化 
  sensors.begin();
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

  delay(1000);
}
