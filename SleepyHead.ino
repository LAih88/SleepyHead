#include <rgb_lcd.h>


class Clock {
  private:
    int year, month, day, hour, minute, second;
    int down_minute, down_second;
    bool is_count_down, is_end;
  public:
    Clock(int year, int month, int day, int hour, int minute, int second);
    String getDate();
    String getTime();
    String getCountDownTime();
    bool getCountDownStatus();
    bool getIsEnd();
    void addSec();
    void passSec();
    void startCountDown();
    void endCountDown();
    void resetCountDown();
};

Clock::Clock(int year, int month, int day, int hour, int minute, int second) {
  this->year = year;
  this->month = month;
  this->day = day;
  this->hour = hour;
  this->minute = minute;
  this->second = second;
  this->is_count_down = false;
  this->is_end = false;
}

String Clock::getDate() {
  String d = String(this->year) + "/";
  if (this->month < 10) d += "0";
  d += String(this->month) + "/";
  if (this->day < 10) d += "0";
  d += String(this->day);
  return d;
}

String Clock::getTime() {
  String d = "";
  if (this->hour < 10) d += "0";
  d += String(this->hour) + ":";
  if (this->minute < 10) d += "0";
  d += String(this->minute) + ":";
  if (this->second < 10) d += "0";
  d += String(this->second);
  return d;
}

String Clock::getCountDownTime() {
  String d = "";
  if (this->down_minute < 10) d += "0";
  d += String(this->down_minute) + ":";
  if (this->down_second < 10) d += "0";
  d += String(this->down_second);
  return d;
}

bool Clock::getCountDownStatus() {
  return this->is_count_down;
}

bool Clock::getIsEnd() {
  return this->is_end;
}

void Clock::addSec() {
  this->second++;
  if (this->second >= 60) {
    this->second = 0;
    this->minute++;
    if (this->minute >= 60) {
      this->minute = 0;
      this->hour++;
      if (this->hour >= 24) {
        this->hour = 0;
        this->day++;
        if (this->day > 31) {
          this->day = 1;
          this->month++;
          if (this->month > 12) {
            this->month = 1;
            this->year++;
          }
        }
      }
    }
  }
}

void Clock::passSec() {
  this->down_second--;
  if (this->down_second < 0) {
    this->down_minute--;
    this->down_second = 59;
    if (this->down_minute < 0) this->endCountDown();
  }
}

void Clock::startCountDown() {
  this->down_minute = 0;
  this->down_second = 20;
  this->is_count_down = true;
}

void Clock::endCountDown() {
  this->is_count_down = false;
  this->is_end = true;
}

void Clock::resetCountDown() {
  this->down_minute = 0;
  this->down_second = 20;
  this->is_count_down = true;
}


rgb_lcd lcd;
Clock clock(2023, 2, 19, 12, 10, 0);
int timer = 0;
int btn_pin = 2, sound_pin = 7, accel_pin = 9;
void setup() {
  lcd.begin(16, 2);
  pinMode(btn_pin, INPUT);
  pinMode(sound_pin, INPUT);
  pinMode(accel_pin, INPUT);
  randomSeed(100);
}

void initial() {
  if (timer == 0) {
    // todo
    lcd.clear();
    clockDisplay();
  }
}

void next() {
  delay(1);
  timer++;
  if (timer >= 1000) {
    //todo
    timer = 0;
    if (clock.getCountDownStatus()) clock.passSec();
    clock.addSec();
  }
}

void clockDisplay() {
  if (clock.getCountDownStatus()) {
    lcd.setRGB(124, 120, 32);
    lcd.print("Sleep Count Down");
    lcd.setCursor(0, 1);
    lcd.print(clock.getCountDownTime());
  } else {
    if (clock.getIsEnd()) lcd.setRGB(0, 0, 0);
    else {
      int r = random(0, 255);
      int g = random(0, 255);
      int b = random(0, 255);
      lcd.setRGB(r, g, b);
    }
    lcd.print("Time: ");
    lcd.print(clock.getDate());
    lcd.setCursor(0, 1);
    lcd.print(clock.getTime());
  }
}

void countDown() {
  int press = digitalRead(btn_pin);
  if (press == HIGH) {
    if (clock.getCountDownStatus()) clock.resetCountDown();
    else clock.startCountDown();
  }
}

bool soundDetect() {
  if (timer % 100 == 0) {
    int value = analogRead(sound_pin);
    return !(value < 297 || value > 303);
  }
  return true;
}

bool accelDetect() {
  if (timer % 100 == 0) {
    int value = analogRead(accel_pin);
    return !(value < 285 || value > 291);
  }
  return true;
}

void loop() {
  initial();
  if (timer % 200 == 0) countDown();
  next();
}
