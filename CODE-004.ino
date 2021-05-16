//https://uclalemur.com/blog/use-plx-daq-to-transform-data-between-arduino-and-excel/PLX-DAQ.zip
#include "DFRobot_EC.h"
#include <EEPROM.h>
#define EC_PIN A1
float voltage, ecValue, temperature = 25;
int Count = 0;    // Biến Count dùng để xác định số bộ dữ liệu gửi trong 1 vòng
DFRobot_EC ec;
//.........................................................
void setup()
{
  Serial.begin(9600);
  Serial.println("CLEARDATA");  // xóa dữ liệu trên sheet đầu tiên của file excel
  Serial.println("LABEL,Time,EC (us/cm)"); // Nhãn (label) cho 2 cột đầu tiên trên sheet đầu tiên của file excel
  ec.begin();
}
//.........................................................
void loop()
{
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) //time interval: 1s
  {
    Count++;
    Serial.print("DATA,TIME,");   // Gán giá trị cho cột đầu tiên = thời gian hiện hành
    timepoint = millis();
    voltage = analogRead(EC_PIN) / 1024.0 * 5000000; // read the voltage
    //temperature = readTemperature();          // read your temperature sensor to execute temperature compensation
    ecValue =  ec.readEC(voltage, temperature); // convert voltage to EC with temperature compensation
    //    Serial.print("temperature:");
    //    Serial.print(temperature, 1);
    //Serial.print("EC:");
    Serial.println(ecValue, 2);// giá trị cho cột thứ 2
    //Serial.println("ms/cm");
    delay(2000);
  }
  ec.calibration(voltage, temperature);         // calibration process by Serail CMD

  if (Count >= 10000) {     // giới hạn số bộ dữ liệu trong 43200 lần gửi, sau đó lặp lại & lưu chồng lên dữ liệu cũ
    Count = 0;
    Serial.println("ROW,SET,2");  // Bắt đầu từ Row 2 của sheet Excel
  }
}
//.........................................................
float readTemperature()
{
  //add your code here to get the temperature from your temperature sensor
}
