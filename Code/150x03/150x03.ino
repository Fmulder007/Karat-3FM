char ver[ ] = "150x03";

//#define SI_OVERCLOCK 750000000L
#define ENCODER_OPTIMIZE_INTERRUPTS

#define crcmod 3// поправка расчета CRC для НЕ СОВМЕСТИМОСТИ со старыми прошивками

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_I2C_ADRESS 0x3C // Display I2c adress

#define max_number_of_bands	99 // Максимальное оличество диапазонов.
#define Si_Xtall_Freq 27000000UL // Частота кварца si5351, Гц.
#define si_cload SI5351_CRYSTAL_LOAD_10PF// 
#define lo_max_freq 550000UL // Максимальная частота 500 КГц опоры, Гц.
#define lo_min_freq 450000UL // Минимальная частота 500 КГц опоры, Гц.
#define bfo_max_freq 30000000UL // Максимальная частота 21.7 МГц опоры, Гц.
#define bfo_min_freq 1000000UL // Минимальная частота 21,7 МГц опоры, Гц.
#define lo_freq 500000UL // Частота опоры 500 КГц.
#define min_hardware_freq 10 // *100KHz Минимальный железный предел частоты диапазона VFO
#define max_hardware_freq 199 // *100KHz Максимальный железный предел частоты диапазона VFO
#define ONE_WIRE_BUS 14 // Порт датчика температуры
#define myEncBtn 4 // Порт нажатия кноба.
#define fwdpin 15 // Порт fwd показометра мощности. А0
#define revpin 16 // Порт fwd показометра мощности. А0
#define mybattpin 21 // Порт датчика АКБ
#define txsenspin 17 //Порт датчика ТХ
#define pttpin 6 // PTT
#define dotpin 5 // CW TX dot
#define dashpin 7 // CW TX dash
#define txenpin 9 // TX en out pin
#define rxenpin 8 // RX en out pin
#define tonepin 12 // Порт выхода тонального сигнала для настройки TX.
#define tonefreq 500 // Частота тонального сигнала для настройки TX.



#include "Adafruit_SSD1306.h" // Use version 1.2.7!!!
#include "si5351a.h"
#include "Wire.h"
#include "Encoder.h"
#include "Eeprom24C32_64.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "DS1307RTC.h"

//Общие настройки
struct general_set {
  uint8_t stp_set = 3; //Начальный шаг настройки.
  uint8_t band_set = 0; // Стартовый диапазон.
  uint8_t number_of_bands_set = 0; // Количество диапазонов.
  uint32_t usb_bfo_freq_set = 21200000UL; // Начальная частота опоры USB при первом включении.
  uint32_t lsb_bfo_freq_set = 22200000UL; // Начальная частота опоры LSB при первом включении.
  int16_t lo_cal_freq_set  = 0; // калибровка опоры 500кГц.
  int16_t Si_Xtall_calFreq_set = 5850; // Начальная частота калибровки кварца, Гц.
  uint8_t batt_cal_set = 208; // Начальная калибровка вольтметра.
  bool reverse_encoder_set = false; //Реверс энкодера.
  uint8_t mem_enc_div = 4; // Делитель импульсов энкодера
  int8_t temp_cal = 0; //Калибровка термометра
  uint8_t cwdelay = 50; // Задержка переключения на прием после CW передачи * 10мсек
  uint8_t cwtone = 70; // Сдвиг частоты CW *10 Гц
  bool cwreverse = false; // Реверс ключа
  uint8_t pttdelay = 25; // Задержка PTT
} general_setting;

#define stp general_setting.stp_set
#define band general_setting.band_set
#define number_of_bands general_setting.number_of_bands_set
#define usb_bfo_freq general_setting.usb_bfo_freq_set
#define lsb_bfo_freq general_setting.lsb_bfo_freq_set
#define lo_cal_freq general_setting.lo_cal_freq_set
#define Si_Xtall_calFreq general_setting.Si_Xtall_calFreq_set
#define batt_cal general_setting.batt_cal_set
#define reverse_encoder general_setting.reverse_encoder_set
#define mem_enc_div general_setting.mem_enc_div
#define temp_cal general_setting.temp_cal
#define cwdelay general_setting.cwdelay
#define cwtone general_setting.cwtone
#define cwreverse general_setting.cwreverse
#define pttdelay general_setting.pttdelay

// Диапазонные настройки
struct band_set {
  bool mode_set = 0; // LSB=0, USB=1.
  uint32_t vfo_freq_set = 7100000UL; // Начальная частота VFO при первом включении.
  uint8_t min_freq_set = 15; // *100KHz Минимальный предел частоты диапазона VFO.
  uint8_t max_freq_set = 80; // *100KHz Максимальный предел частоты диапазона VFO.
} band_setting;
#define mode band_setting.mode_set
#define vfo_freq band_setting.vfo_freq_set
#define min_freq band_setting.min_freq_set
#define max_freq band_setting.max_freq_set

//
//
//

uint8_t menu = 0; //Начальное положение меню.
uint16_t arraystp[] = {1, 10, 50, 100, 500, 1000, 10000}; //шаги настройки * 10 герц.

uint8_t enc_div = 4;
uint8_t mypower = 0;
uint8_t mybatt = 0;
int8_t temperature = 0;
int16_t screenstep = 1000;

bool toneen = false;

long oldPosition  = 0;
bool actencf = false;
bool txen = false;
bool knobup = true;
bool exitmenu = false;
bool reqtemp = false;
bool timesetup = false;
bool actfmenuf = false;

// CW flags
bool cwtxen = false;
bool cwkeydown = false;
uint32_t keyupmillis = 0;
uint8_t cwkeycount = 0;
uint8_t cwcount = 0;

// PTT flags
bool ptten = 0;
uint8_t pttcount = 0;
uint32_t pttreleasetimer = 0;

uint32_t previousdsp = 0;
uint32_t previoustemp = 0;
uint32_t previoustime = 0;
uint32_t knobMillis = 0;
uint32_t actenc = 0;



static Eeprom24C32_64 AT24C32(0x50);
Si5351 si;
Encoder myEnc(2, 3); //порты подключения енкодера.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
tmElements_t tm;


void setup() {
  //PTT control setup
  pinMode (pttpin, INPUT); // PTT pin input
  pinMode (txenpin, OUTPUT); // TX control pin output
  pinMode (rxenpin, OUTPUT); // RX control pin output

  digitalWrite (pttpin, LOW); // PTT pin pullup
  digitalWrite (txenpin, LOW); // TX control pin pullup
  digitalWrite (rxenpin, LOW); // RX control pin pullup

  //CW pin`s setup
  pinMode (dotpin, INPUT);			//CW dotpin input
  pinMode (dashpin, INPUT);         //CW dashpin input
  digitalWrite (dotpin, LOW);      // CW dotpin pin pullup Disable
  digitalWrite (dashpin, LOW);     // CW dashpin pin pullup Disable

  // other pin`s setup
  pinMode(myEncBtn, INPUT);
  pinMode(fwdpin, INPUT);
  pinMode(revpin, INPUT);
  pinMode(tonepin, OUTPUT);
  digitalWrite(myEncBtn, HIGH);
  analogReference(INTERNAL);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADRESS);
  display.clearDisplay();
  display.display();
  sensors.begin();
  memread();
  enc_div = mem_enc_div;
  si5351init();
  si5351correction();
  vfosetup();
  battmeter();
  powermeter();
  tempsensor ();
  timenow ();
  versionprint ();
  mainscreen();
}

void loop() { // Главный цикл
  pttsensor();
  //cw();
  pushknob();
  readencoder();
  txsensor();
  tonegen();
  if (!menu) {
    if (txen) screenstep = 100;
    else screenstep = 1000;
    if (millis() - previousdsp > screenstep) {
      storetomem();
      battmeter();
      powermeter();
      tempsensor();
      timenow();
      mainscreen();
      previousdsp = millis();
    }
  }
}

void timenow () {
  if (timesetup) {
    tm.Second = 0;
    RTC.write(tm);
    timesetup = false;
  }
  else {
    if (millis() - previoustime > 1000 || !previoustime) {
      previoustime = millis();
      if (!RTC.read(tm)) {
        if (RTC.chipPresent()) {
          tm.Hour = 0;  tm.Minute = 0; tm.Second = 0;
          RTC.write(tm);
        }
      }
    }
  }
}

void tempsensor () {
  if (millis() - previoustemp > 5000 && !reqtemp) {
    sensors.setWaitForConversion(false);
    sensors.requestTemperatures();
    sensors.setWaitForConversion(true);
    reqtemp = true;
  }
  if (millis() - previoustemp > 8000 && reqtemp) {
    temperature = (int8_t)(0.5 + sensors.getTempCByIndex(0));
    temperature = temperature + temp_cal;
    previoustemp = millis();
    reqtemp = false;
  }
}

void txsensor () {
  bool txsens = digitalRead(txsenspin);
  //Если радио на приеме и нажали ТХ то txen = true
  if (txsens && !txen) {
    txen = true;
    mainscreen();
  }
  //Если радио на передаче и отпустили ТХ то txen = false
  if (!txsens && txen) {
    txen = false;
    mainscreen();
  }
}

void pushknob () {  // Обработка нажатия на кноб

  bool knobdown = digitalRead(myEncBtn);   //Читаем состояние кноба
  if (!knobdown && knobup) {   //Если кноб был отпущен, но нажат сейчас
    knobup = false;   // отмечаем флаг что нажат
    knobMillis = millis();  // запускаем таймер для антидребезга
  }

  if (knobdown && !knobup) { //Если кноб нажат
    knobup = true; // отмечаем флаг что кноб отпущен
    long knobupmillis = millis();
    if (knobupmillis - knobMillis >= 1000) { //Если длительное нажатие
      if (menu == 0) menu = 4;
      else if (menu != 0) menu = 0;
    }

    if (knobupmillis - knobMillis < 1000 && knobupmillis - knobMillis > 100) { //Если кноб отпущен и был нажат и времени от таймера прошло 100Мс
      if (menu < 4 && menu > 0 && actfmenuf) { //Если 0<меню<4 и крутили енкодер в быстром меню, то выйти на главный экран
        actfmenuf = false;
        menu = 0;
      }
      else {
        menu ++; //Переходим на меню дальше
        if (menu == 4) menu = 0; //Если меню 5 выйти на главный экран
        if (menu > 19) menu = 4; //Если меню больше 18 перейти на меню 5
      }
      if (!number_of_bands && menu == 1) menu++;
    }

    mainscreen();
  }
}

void storetomem() { // Если крутили енкодер, то через 10 секунд все сохранить

  if ((millis() - actenc > 10000UL) && actencf) {
    actenc = millis();
    actencf = false;
    memwrite ();
  }
}

void readencoder() { // работа с енкодером
  long newPosition;
  newPosition = myEnc.read() / enc_div;
  if (reverse_encoder) newPosition *= (-1);
  if (newPosition != oldPosition && digitalRead(myEncBtn)) { // ЕСЛИ КРУТИЛИ энкодер

    if (menu > 0 && menu < 5) actfmenuf = true; // Если крутили энкодер в быстром меню - флаг вверх!
    switch (menu) {

      case 0: //Основная настройка частоты
        if (newPosition > oldPosition && vfo_freq <= max_freq * 100000UL) {
          if (vfo_freq % (arraystp[stp] * 10UL)) {
            vfo_freq = vfo_freq + (arraystp[stp] * 10UL) - (vfo_freq % (arraystp[stp] * 10UL));
          }
          else {
            vfo_freq = vfo_freq + (arraystp[stp] * 10UL);
          }
        }
        if (newPosition < oldPosition && vfo_freq >= min_freq * 100000UL) {
          if (vfo_freq % (arraystp[stp] * 10UL)) {
            vfo_freq = vfo_freq - (vfo_freq % (arraystp[stp] * 10UL));
          }
          else {
            vfo_freq = vfo_freq - (arraystp[stp] * 10UL);
          }
        }
        if (vfo_freq < min_freq * 100000UL) vfo_freq = min_freq * 100000UL;
        if (vfo_freq > max_freq * 100000UL) vfo_freq = max_freq * 100000UL;
        vfosetup();
        break;

      case 1: //Переключение диапазонов
        if (newPosition > oldPosition && band < number_of_bands) band++;
        if (newPosition < oldPosition && band > 0) band--;
        if (band > number_of_bands) band = number_of_bands;
        band_memread();
        vfosetup();
        break;

      case 2: //Настройка ШАГА настройки
        if (newPosition > oldPosition && stp < (sizeof(arraystp) / sizeof(arraystp[0]) - 1)) stp++;
        if (newPosition < oldPosition && stp > 0) stp--;
        //if (stp < 10) stp = 10;
        if (stp > (sizeof(arraystp) / sizeof(arraystp[0]) - 1)) stp = (sizeof(arraystp) / sizeof(arraystp[0]) - 1);
        break;

      case 3: //Переключение LSB|USB.
        mode = !mode;
        vfosetup();
        break;

      case 4: //Настройка min_freq
        if (newPosition > oldPosition && min_freq <= max_hardware_freq) min_freq++;
        if (newPosition < oldPosition && min_freq >= min_hardware_freq) min_freq--;
        min_freq = constrain(min_freq, min_hardware_freq, max_freq - 1);
        break;

      case 5: //Настройка maxfreq
        if (newPosition > oldPosition && max_freq <= max_hardware_freq) max_freq++;
        if (newPosition < oldPosition && max_freq >= min_hardware_freq) max_freq--;
        max_freq = constrain(max_freq, min_freq + 1, max_hardware_freq);
        break;

      case 6: //Настройка BFO гетеродина LSB
        if (newPosition > oldPosition && lsb_bfo_freq <= bfo_max_freq) lsb_bfo_freq += arraystp[stp];
        if (newPosition < oldPosition && lsb_bfo_freq >= bfo_min_freq) lsb_bfo_freq -= arraystp[stp];
        lsb_bfo_freq = constrain(lsb_bfo_freq, bfo_min_freq, bfo_max_freq);
        vfosetup();
        break;

      case 7: //Настройка BFO гетеродина USB
        if (newPosition > oldPosition && usb_bfo_freq <= bfo_max_freq) usb_bfo_freq += arraystp[stp];
        if (newPosition < oldPosition && usb_bfo_freq >= bfo_min_freq) usb_bfo_freq -= arraystp[stp];
        usb_bfo_freq = constrain(usb_bfo_freq, bfo_min_freq, bfo_max_freq);
        vfosetup();
        break;

      case 8: //Настройка опорного гетеродина 500кГц
        if (newPosition > oldPosition) lo_cal_freq += arraystp[stp];
        if (newPosition < oldPosition) lo_cal_freq -= arraystp[stp];
        //lo_cal_freq = constrain(lo_cal_freq, lo_min_freq, lo_max_freq);
        vfosetup();
        break;

      case 9: //Настройка калибровки кварца PLL
        if (newPosition > oldPosition && Si_Xtall_calFreq <= 30000) Si_Xtall_calFreq += arraystp[stp];
        if (newPosition < oldPosition && Si_Xtall_calFreq >= - 30000) Si_Xtall_calFreq -= arraystp[stp];
        Si_Xtall_calFreq = constrain(Si_Xtall_calFreq, -30000, 30000);
        si5351correction();
        vfosetup();
        break;

      case 10: //Настройка калибровки по питанию
        if (newPosition > oldPosition && batt_cal <= 254) batt_cal++;
        if (newPosition < oldPosition && batt_cal >= 100) batt_cal--;
        if (batt_cal > 254) batt_cal = 254;
        if (batt_cal < 100) batt_cal = 100;
        break;

      case 11: // Настройка количества диапазонов
        if (newPosition > oldPosition && number_of_bands < max_number_of_bands) number_of_bands++;
        if (newPosition < oldPosition && number_of_bands > 0) number_of_bands--;
        if (number_of_bands > max_number_of_bands) number_of_bands = max_number_of_bands;
        if (band > number_of_bands) band = number_of_bands;
        break;

      case 12: //Настройка Часов
        if (newPosition > oldPosition && tm.Hour < 24) tm.Hour++;
        if (newPosition < oldPosition && tm.Hour > 0) tm.Hour--;
        if (tm.Hour > 23) tm.Hour = 23;
        timesetup = true;
        break;

      case 13: //Настройка Минут
        if (newPosition > oldPosition && tm.Minute < 60) tm.Minute++;
        if (newPosition < oldPosition && tm.Minute > 0) tm.Minute--;
        if (tm.Minute > 59) tm.Minute = 0;
        timesetup = true;
        break;

      case 14: //Инверсия энкодера.
        if (reverse_encoder) {
          reverse_encoder = false;
        }
        else {
          reverse_encoder = true;
        }
        newPosition *= (-1);
        break;

      case 15: // Настройка делителя импульсов энкодера
        if (newPosition > oldPosition && mem_enc_div < 255) mem_enc_div++;
        if (newPosition < oldPosition && mem_enc_div > 1) mem_enc_div--;
        mem_enc_div = constrain(mem_enc_div, 1, 255);
        break;

      case 16: //Калибровка термодатчика
        if (newPosition > oldPosition && temp_cal <= 30) temp_cal++;
        if (newPosition < oldPosition && temp_cal >= - 30) temp_cal--;
        temp_cal = constrain(temp_cal, -30, 30);
        break;

      case 17: // Настройка CW-Delay
        if (newPosition > oldPosition && cwdelay < 255) cwdelay++;
        if (newPosition < oldPosition && cwdelay > 1) cwdelay--;
        cwdelay = constrain(cwdelay, 10, 255);
        break;

      case 18: // Настройка CW-Tone
        if (newPosition > oldPosition && cwtone < 255) cwtone++;
        if (newPosition < oldPosition && cwtone > 1) cwtone--;
        cwtone = constrain(cwtone, 10, 255);
        break;

      case 19: // Настройка PTT-Delay
        if (newPosition > oldPosition && pttdelay < 255) pttdelay += 5;
        if (newPosition < oldPosition && pttdelay > 5) pttdelay -= 5;
        pttdelay = constrain(pttdelay, 5, 255);
        break;


    }
    actenc = millis();
    actencf = true;
    mainscreen();
    oldPosition = newPosition;
  }
}

void powermeter () { // Измеритель уровня выхода
  //fwdpin
  //revpin
  int16_t fwdpower = analogRead(fwdpin);
  mypower = map(fwdpower, 0, 1023, 0, 100);
}

void battmeter () { // Измеритель напряжения питания
  int16_t rawbatt = analogRead(mybattpin);
  mybatt = map(rawbatt, 0, 1023, 0, batt_cal);
}

void mainscreen() { //Процедура рисования главного экрана
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.setTextSize(3);
  switch (menu) {

    case 0: //Если не в меню, то рисовать главный экран
      //Вывод частоты на дисплей
      if ((vfo_freq / 1000000) < 10) display.print(" ");
      display.print(vfo_freq / 1000000);//Вывод МГц
      display.setCursor(display.getCursorX() + 5, display.getCursorY()); //Переводим курсор чуть правее текущего положения
      if ((vfo_freq % 1000000) / 1000 < 100) display.print("0");
      if ((vfo_freq % 1000000) / 1000 < 10) display.print("0");
      display.print((vfo_freq % 1000000) / 1000); //Выводим КГц
      display.setTextSize(2); // Для сотен и десятков герц делаем шрифт поменьше
      display.setCursor(display.getCursorX() + 5, 7); //Переводим курсор чуть ниже текущего положения
      if ((vfo_freq % 1000) / 10 < 10) display.print("0"); //Если герц <10 то выводим "0" перед ними.
      display.println((vfo_freq % 1000) / 10);

      //Выводим вторую строку на дисплей
      display.setTextSize(1);// Ставим маленький шрифт
      if (mybatt - 100 < 0) display.print("0");
      display.print(mybatt / 10);
      display.print(".");
      display.print(mybatt % 10);
      display.print("v");
      display.setTextSize(1);
      /*if (ptten) display.print("PTT");
        if (cwtxen) display.print("CWtxen");*/

      if (txen) {//Если передача, то вывод показометра мощности
        if (ptten) {
          display.print("SSB ");
        }
        else if (cwtxen) {
          display.print(" CW ");
        }
        else {
          display.print("PWR ");
        }
        display.fillRect(64, 23, mypower, 9, WHITE);
      }
      else {// Если прием, то рисовать температуру часы, полосу и диапазон
        //char ddot
        if (temperature >= -50 && temperature <= 50) {
          if (temperature >= 0) display.print(" ");
          display.print(temperature);
          display.print((char)247);
          display.print("C");
        }
        else {
          display.print(" err ");
        }
        if (actencf) {
          display.print(" ");
        }
        else {
          display.print(".");
        }
        if (tm.Hour < 10) display.print(" ");
        display.print(tm.Hour);
        if (tm.Second % 2) {
          display.print(":");
        }
        else {
          display.print(" ");
        }
        if (tm.Minute < 10) display.print("0");
        display.print(tm.Minute);
        if (mode) {
          display.print(" U");
        }
        else {
          display.print(" L");
        }
        if (band < 10) display.print(" ");
        display.print(band);
      }
      //if (cwtxen) display.fillRect(0, 0, 3, 3, WHITE);
      //if (cwkeydown) display.fillRect(0, 10, 3, 3, WHITE);
      break;

    case 1: //Меню 1 - диапазон
      display.println(band);
      display.setTextSize(1);
      display.print(menu);
      display.print("  BAND from 0 to ");
      display.print(number_of_bands);
      break;

    case 2: //Меню 2 - шаг настройки
      display.println(arraystp[stp] * 10UL);
      display.setTextSize(1);
      display.print(menu);
      display.print("  Step Hz");
      break;

    case 3: //Меню 3 - LSB|USB
      if (mode) {
        display.println("USB");
      }
      else
      {
        display.println("LSB");
      }
      display.setTextSize(1);
      display.print(menu);
      display.println("  LSB|USB Switch");
      break;

    case 4: //Меню 4 - Настройка min_freq
      display.println(min_freq * 100);
      display.setTextSize(1);
      display.print(menu);
      display.print("  Min Freq ");
      display.print((char)240);
      display.print("kHz");
      break;

    case 5: //Меню 5 - Настройка maxfreq
      display.println(max_freq * 100);
      display.setTextSize(1);
      display.print(menu);
      display.print("  Max Freq ");
      display.print((char)240);
      display.print("kHz");
      break;

    case 6: //Меню 6 - Настройка BFO гетеродина LSB
      display.setTextSize(2);
      display.println(lsb_bfo_freq);
      display.setTextSize(1);
      display.print(menu);
      display.print("  BFO LSB ");
      display.print((char)240);
      display.print("Hz");
      break;

    case 7: //Меню 7 - Настройка BFO гетеродина USB
      display.setTextSize(2);
      display.println(usb_bfo_freq);
      display.setTextSize(1);
      display.print(menu);
      display.print("  BFO USB ");
      display.print((char)240);
      display.print("Hz");
      break;

    case 8: //Меню 8 - Настройка опорного гетеродина 500кГц
      display.println(lo_cal_freq);
      display.setTextSize(1);
      display.print(menu);
      display.print("  lo 500kHz cal ");
      display.print((char)240);
      display.print("Hz");
      break;

    case 9: //Меню 9 - Настройка калибровки кварца
      display.println(Si_Xtall_calFreq);
      display.setTextSize(1);
      display.print(menu);
      display.print("  Xtal Cal ");
      display.print((char)240);
      display.print("Hz");
      break;

    case 10: //Меню 10 - Настройка калибровки по питанию
      display.println(batt_cal);
      display.setTextSize(1);
      display.print(menu);
      display.print("  Batt Cal");
      break;

    case 11: //Меню 11 - Количество диапазонов
      display.println(number_of_bands);
      display.setTextSize(1);
      display.print(menu);
      display.print("  MAX Num Bands");
      break;

    case 12: //Меню 12 - Настройка Часов
      if (tm.Hour < 10) display.print("0");
      display.println(tm.Hour);
      display.setTextSize(1);
      display.print(menu);
      display.print("  Hour");
      break;

    case 13: //Меню 13 - Настройка Минут
      if (tm.Minute < 10) display.print("0");
      display.println(tm.Minute);
      display.setTextSize(1);
      display.print(menu);
      display.print("  Minute");
      break;

    case 14: //Меню 14 - Reverse Encoder
      if (reverse_encoder) {
        display.println("Yes");
      }
      else
      {
        display.println("NO");
      }
      display.setTextSize(1);
      display.print(menu);
      display.println("  Reverse Encoder");
      break;

    case 15: //Меню 15 - Encoder Divider
      display.println(mem_enc_div);
      display.setTextSize(1);
      display.print(menu);
      display.print("  Encoder Divider");
      break;

    case 16: //Калибровка термодатчика
      display.println(temp_cal);
      display.setTextSize(1);
      display.print(menu);
      display.print(" Temp CAL ");
      display.print((char)240);
      display.print((char)247);
      display.print("C");
      break;

    case 17: //Меню 17 - CW-Delay
      display.println(cwdelay * 10);
      display.setTextSize(1);
      display.print(menu);
      display.print("  CW Delay msec");
      break;

    case 18: //Меню 18 - CW-Tone
      display.println(cwtone * 10);
      display.setTextSize(1);
      display.print(menu);
      display.print("  CW Tone Hz");
      break;


    case 19: //Меню 19 - PTT-Delay
      display.println(pttdelay * 10);
      display.setTextSize(1);
      display.print(menu);
      display.print("  PTT Delay msec");
      break;


  }
  display.display();
  //debug();

}


void vfosetup() {
  if (cwtxen) {
    if (cwkeydown) {
      if (mode) {
        si.set_freq((vfo_freq + usb_bfo_freq + lo_freq + lo_cal_freq), (usb_bfo_freq + lo_freq + lo_cal_freq - (cwtone * 10)), 0);
      }
      else {
        si.set_freq((vfo_freq + lsb_bfo_freq - lo_freq + lo_cal_freq), (lsb_bfo_freq - lo_freq + lo_cal_freq + (cwtone * 10)), 0);
      }
    }
    else {
      if (mode) {
        si.set_freq((vfo_freq + usb_bfo_freq + lo_freq + lo_cal_freq), 0, 0);
      }
      else {
        si.set_freq((vfo_freq + lsb_bfo_freq - lo_freq + lo_cal_freq), 0, 0);
      }
    }

  }
  else {
    if (mode) {
      si.set_freq((vfo_freq + usb_bfo_freq + lo_freq + lo_cal_freq), 0, (usb_bfo_freq));
    }
    else {
      si.set_freq((vfo_freq + lsb_bfo_freq - lo_freq + lo_cal_freq), 0, (lsb_bfo_freq));
    }
  }

}

void si5351init() {
  si.setup(0, 0, 0);
  si.cload(si_cload);
}

void si5351correction() {
  si.set_xtal_freq(Si_Xtall_Freq + Si_Xtall_calFreq);
  si.update_freq(0);
  si.update_freq(1);
}

void memwrite () { //Запись general_setting
  int16_t crc = 0;
  uint8_t i = 0;
  uint8_t * adr;
  adr =  (uint8_t*)(& general_setting);
  while (i < (sizeof(general_setting)))
  {
    crc += *(adr + i);
    i++;
  }
  AT24C32.writeEE(2, general_setting);
  AT24C32.writeEE(0, (crc + crcmod));

  // Запись band_setting
  crc = 0;
  i = 0;
  adr =  (uint8_t*)(& band_setting);
  while (i < (sizeof(band_setting)))
  {
    crc += *(adr + i);
    i++;
  }
  AT24C32.writeEE(sizeof(general_setting) + 2 + ((sizeof(band_setting) + 2)*band) + 2, band_setting);
  AT24C32.writeEE(sizeof(general_setting) + 2 + ((sizeof(band_setting) + 2)*band), (crc + crcmod));
}

void memread() {
  int16_t crc = 0;
  int16_t crcrom = 0;
  uint8_t i = 0;

  // Чтение general_setting
  AT24C32.readEE (0, crc);
  while (i < (sizeof(general_setting)))
  {
    crcrom += AT24C32.readByte ((i + 2));
    i++;
  }
  if (crc == (crcrom + crcmod)) {
    AT24C32.readEE (2, general_setting);
  }
  else {
    memwrite ();
  }
  band_memread();
}

void band_memread() {

  int16_t crc = 0;
  int16_t crcrom = 0;
  uint8_t i = 0;
  AT24C32.readEE (sizeof(general_setting) + 2 + ((sizeof(band_setting) + 2) * band), crc);
  while (i < (sizeof(band_setting)))
  {
    crcrom += AT24C32.readByte ((i + sizeof(general_setting) + 2 + ((sizeof(band_setting) + 2) * band)) + 2);
    i++;
  }
  if (crc == (crcrom + crcmod)) {
    AT24C32.readEE (sizeof(general_setting) + 2 + ((sizeof(band_setting) + 2)*band) + 2, band_setting);
  }
  else {
    memwrite ();
  }
}

void versionprint() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.println(ver);
  display.setTextSize(1);
  display.println("    From UD0CAJ");
  display.display();
  delay(1000);
}

void tonegen() {
  if (txen && !toneen) {
    if (menu != 0 && menu <= 3) {
      tone(tonepin, tonefreq);
      menu = 0;
      toneen = true;
    }
  }
  if (!txen && toneen) {
    noTone(tonepin);
    toneen = false;
  }
}

void cw() { // Процедура работы с ключом

  if (!digitalRead (dotpin)) {     //Если DOTpin, то:
    cwkeycount++;     // счетчик ключа +1
  }
  else {              // Если нет контакта
    cwkeycount = 0;    // Сбросить счетчики
  }

  if (cwkeycount > 10)  {   // Если счетчик непрерывно нажатого ключа больше 10 то:
    cwkeycount = 10;    // поддерживаем счетчик
    if (txen && !cwtxen && cwkeydown) {
      cwtxen = true;   // Если перевелся на TX но нет флага cw tx запоминаем Трансивер на передаче в CW
      vfosetup();
    }
    if (!cwkeydown) {                   // Если ключ НЕ БЫЛ НАЖАТ, то
      cwkeydown = true;                  // Запоминаем что ключ нажат
      if (!cwtxen) {                     // Если не на CW передаче, то
        // digitalWrite(cwtxpin, HIGH);        //Пытаемся перевести трансивер на передачу
      }
      vfosetup();
    }
  }

  if (!cwkeycount) {    // Если ключ сейчас отпущщен

    if (cwkeydown) {                 // Если ключ БЫЛ НАЖАТ раньше, то
      keyupmillis = millis();                             // Запоминаем время последнего отпускания ключа
      cwkeydown = false;                                  // Флаг что ключ отпущен
      vfosetup();
    }

    if (!cwkeydown) {                 // ключ НЕ БЫЛ нажат
      long keymillis = millis();          // проверяем текущее время
      if (keymillis - keyupmillis >= (cwdelay * 10)) {   // Если прошло больше чем CW delay
        //        digitalWrite(cwtxpin, LOW);                          //Пытаемся перевести трансивер на прием                                            // Если трансивер перешёл прием, то
        cwtxen = false;                                    // опускаем флаг передачи CW
        vfosetup();
      }
    }
  }
}

void pttsensor() {
  if (!digitalRead (pttpin)) {     //Если PTT на земле, то:
    pttcount++;     // увеличить счетчик PTT на 1
  }
  else {              // Если нет контакта
    pttcount = 0;    // Сбросить счетчик
    if (ptten) {  //  Если флаг ptten поднят, то:
      ptten = false;   //  опускаем флаг ptten
      pttreleasetimer = millis(); // запоминаем момент отпускания PTT.
    }

  }

  if (pttcount > 10)  {   // Если счетчик непрерывно нажатого PTT больше 10 то:
    if (!ptten) ptten = true;   //  Если флаг ptten не поднят поднимаем флаг ptten
    pttcount = 10;    // 1 - поддерживаем счетчик pttcount на 10
  }
  rxtxcontrol();
}

void rxtxcontrol() {
  if (!txen && ptten) { // Если на приеме и нажали PTT то перевести на передачу!
    digitalWrite (rxenpin, LOW);
    digitalWrite (txenpin, HIGH);
  }
  if (txen && !ptten && (millis() - pttreleasetimer >= (pttdelay * 10))) { //Если на передаче и отпустили PTT больше чем 500мс назад, то перевести на прием!
    digitalWrite (txenpin, LOW);
    digitalWrite (rxenpin, HIGH);
  }
}
