// TODO list

// анимация полоски
// кнопки
// шакал протект
// вкл/выкл свет


#include <Arduino.h>
#include <FastLED.h>
#include <GyverTimer.h>


//Изменяемые настройки
#define MATRIX_PIN 6    //Пин ленты

#define FADE_TIME 60    //Бета

#define MATRIX_WIDTH 3      //Высота
#define MATRIX_HEIGHT 14    //Ширина

#define RADIUS 3

//ищзменяемые настройки

//Не изменять 

#define MAX_VALUE 255


#define SOFTNESS MAX_VALUE/RADIUS                    //Мягкость сглаживания линий

#define MATRIX_NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT)    //количество светодиодов

#define MATRIX_COLORS 3      //Кол-во цветов
//Не изменять


//Переменные
boolean horizontal_line = true;   //true - горизонтальное расположение ленты, false -вертикальное

//Классы
GTimer_ms fade_timer(1);      //таймер анимации полоски

struct CRGB matrix [MATRIX_NUM_LEDS];   //лента
//Классы

void setup() {
  FastLED.addLeds<WS2811, MATRIX_PIN, RGB>(matrix,MATRIX_NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(25);

}

void loop() {

  FastLED.show();

}

//получение номера пикселя по кординатам
uint32_t getByXY(uint32_t x, uint32_t y){
  return horizontal_line ? _getByXY(x, y) : _getByXY(y,x);
}

//вспомогательная функция для получения номера пикселя
uint32_t _getByXY(uint32_t x, uint32_t y){
      if (x > MATRIX_HEIGHT){
          x = MATRIX_HEIGHT;
      }
      if(y > MATRIX_WIDTH){
        y = MATRIX_WIDTH;
      }
      return (y%2 == 0) ? MATRIX_HEIGHT * y - x : ( y -  1 ) * MATRIX_HEIGHT + x - 1;
}

//Установить цвет отдельного пикселя классом CRGB
void setColorToPixel(uint32_t x, uint32_t y, CRGB color){
  matrix[getByXY(x,y)] = color;
}

//Установить цвет отдельного пикселя поцветно
void setColorToPixel(uint32_t x, uint32_t y, byte red, byte green, byte blue){
  setColorToPixel(x,y, CRGB(red,green,blue));
}

//установить цвет на линию классом CRGB
void setColorToLine(uint32_t line, CRGB color){
  uint32_t columns = horizontal_line ? MATRIX_WIDTH : MATRIX_HEIGHT;
  for(uint32_t column = 1; column <= columns; column++){
    setColorToPixel(line, column, color);
  }
}

//установить цвет на линию поцветно
void setColorToLine(uint32_t line, byte red, byte green, byte blue){

  setColorToLine(line, CRGB(red,green,blue));

}

//Установить всё одиним цветом по классу CRGB
void setAllOneColor(CRGB color){
  for (uint32_t pixel; pixel<MATRIX_NUM_LEDS; pixel++){
    matrix[pixel] = color;
  }
}

//установить всё одним цветом поцветно
void setAllOneColor(byte red, byte green, byte blue){
  setAllOneColor(CRGB(red,green,blue));
}

uint8_t filteredColor(uint32_t line, uint32_t evolved_line){
  uint32_t ans = abs(line - evolved_line)* SOFTNESS;

  return (ans < MAX_VALUE) ? ans : MAX_VALUE;
}

void setEvolvedLine(uint32_t evolved_line){
  uint32_t lines = horizontal_line ? MATRIX_HEIGHT : MATRIX_WIDTH;

  for (uint32_t line = 1; line  <= lines; line++){

    setColorToLine(line, MAX_VALUE, filteredColor(line, evolved_line), filteredColor(line, evolved_line));

  }

}


