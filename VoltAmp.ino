#include <Tiny4kOLED.h>
#include "ModernDos.h"
#include "ModernDos8.h"
#include "5x5_font.h"


int ADCV;
int ADCV1;
int ADCA;
int ADCA1;

float VOLT;
float AMP;

bool CH;

long MS;

//------------------------------------------------------------------------------
void setup() {
  analogReference(INTERNAL1V1);

  Wire.begin();
  Wire.setClock(400000L);
  oled.begin();
  oled.clear();
  oled.on();


}
//------------------------------------------------------------------------------
void loop() {

  ADCV = analogRead(PB3) * 0.985;        //Читаем 3 канал АЦП и калибруем
  ADCA = analogRead(PB4);                //Читаем 2 канал


  /*

    Дальше идет конструкция с флагами чтобы не тратить ресурсы на обновление дисплея если нет изменений.

  */

  if (ADCV1 != ADCV) {                  //Если резервное значение не равно значению с АЦП
    ADCV1 = ADCV;                       //Записать в резервное значение новое значение АЦП
    CH = 1;                             //Поднимаем флаг означающий новые данные
    VOLT = float(map(ADCV, 0, 1023, 0, 1760)) / 100;    //Стандартной функцией приводим значение из диапазона АЦП в диапазон нужный нам. Все это безобразие конверуем во Float и делим до реального значения.
  }
  if (ADCA1 != ADCA) {
    ADCA1 = ADCA;
    CH = 1;
    AMP = float(map(ADCA, 0, 1023, 0, 47333)) / 1000;
  }



  if (CH && millis() > MS + 200) {              //Если произошло изменение в данных, но не чаще 200мС, запускаем отрисовку данных на дисплей
    MS = millis();

    oled.setFontX2(FONT8X16MDOS);
    oled.setCursor(0, 0);
    oled.print(AMP, 1);

    oled.setFontX2(FONT8X8MDOS);
    oled.setCursor(64, 0);
    oled.print("<A");

    oled.setFontX2(FONT5X5);
    oled.setCursor(64, 2);
    oled.print(VOLT, 2);
    oled.print("V");
    CH = 0;                                   //Сбрасываем флаг как только все сделано
  }

}
