// original sketch is at https://github.com/bmzstore/Analog-Clock-NTP-ESP-32-With-Panel-LED-P3
// niq_ro's info: https://nicuflorica.blogspot.com/2022/12/ceas-analogic-ntp-pe-afisaj-p3-64x64-cu.html
// v.1 - colours changed every minutes not seconds
// v.1.a - change brightness in 2 steps(max and min) by default one
// v.1.b - addeed resistor + 10k resistor for real ccontrol for brightness in 2 steps - https://esp32io.com/tutorials/esp32-light-sensor
// v.1.b.1 - added histeresys at brightness changes
// v.1.c - changed order of tongues of the clock (hours upside)

#include "WiFi.h"
#include <RGBmatrixPanelGP.h>  // RGB matrix Panel Library: https://github.com/3tawi/RGBmatrixPanelGP
#include <DS1307ESP.h>         // DS1307ESP Library: https://github.com/3tawi/DS1307ESP
#include <font/fontclock.h>    // included font in RGB matrix Panel Library: https://github.com/3tawi/RGBmatrixPanelGP
#include <font/bmz5x7.h>       // special font: https://github.com/bmzstore/Analog-Clock-NTP-ESP-32-With-Panel-LED-P3
DS1307ESP rtc;

char ssid[] = "niq_ro"; // Nama Wifi Anda
char pass[] = "coldbeer!"; // Password Wifi Anda
#define TZ (+2*60*60) //Timezone UTC+2

uint8_t sdapin = 21, sclpin = 22; 
//                        R1, G1, B1, R2, G2, B2  RGB Pins
uint8_t listrgbpins[6] = {19, 13, 18,  5, 12, 17};

#define P_A    16
#define P_B    14
#define P_C    4
#define P_D    27
#define P_E    25
#define P_CLK  2
#define P_LAT  26 
#define P_OE   15

RGBmatrixPanelGP matrix(P_A, P_B, P_C, P_D, P_E, P_CLK, P_LAT, P_OE, true, 64, listrgbpins);

#define WIDTH 64
#define HEIGHT 64
const byte CENTRE_X = (WIDTH / 2) - 1;
const byte CENTRE_Y = (HEIGHT / 2) - 1;
#define R_HOUR 17
#define R_MIN  25
#define R_SEC   29
uint32_t lastTime; 

uint16_t lineColor1;
uint16_t lineColor2;
uint16_t lineColor3;
uint16_t lineColor4;
uint16_t lineColor5;
uint16_t lineColor6;

byte luminozitate = 255;
int cul1, cul2, cul3, cul4;
byte secunda;

#define LIGHT_SENSOR_PIN 36 // ESP32 pin GIOP36 (ADC0) - #define LIGHT_SENSOR_PIN 36 // ESP32 pin GIOP36 (ADC0)
int analogValue = 127;

void setup() {
 Serial.begin(115200);

  Serial.print("Attempting to connect to Network named: ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  configTime(TZ, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp"); 
  
  matrix.begin();
  delay(1000);

cul1 = random(0, 1536);
cul2 = random(0, 1536);
cul3 = random(0, 1536);
cul4 = random(0, 1536);

lineColor1 = matrix.ColorHSV(cul1, 255, 255, true);
lineColor2 = matrix.ColorHSV(cul2, 255, 255, true);
lineColor3 = matrix.ColorHSV(cul3, 255, 255, true);
lineColor4 = matrix.ColorHSV(cul4, 255, 255, true);
lineColor5 = matrix.ColorHSV(0xf000, 255, 255, true); 
lineColor6 = matrix.ColorHSV(0x0f00, 255, 255, true); 
}

void loop() {
  if (WiFi.status() != WL_CONNECTED)
    ESP.restart();

analogValue = analogRead(LIGHT_SENSOR_PIN);  // reads the input on analog pin (value between 0 and 4095)
if (analogValue < 800)
    luminozitate = 127;  // low brightness
if (analogValue > 850)
    luminozitate = 255;  // high brightness
  
  drawtime();
  matrix.swapBuffers(false);
  delay(1000);
} // end main loop

void drawtime() {
  time_t t;
  static time_t last_t;
  struct tm *tm;
  t = time(NULL);
  if (last_t == t) return;
  last_t = t;
  tm = localtime(&t);
 
            double radians;
            int x;
            int y;
            char jam[9];
            char menit[9];
            int j = 0;
            rtc.DSread();
            uint8_t hour = tm->tm_hour;
            if (hour > 12)
                hour -= 12;

lineColor1 = matrix.ColorHSV(cul1, 255, luminozitate, true);
lineColor2 = matrix.ColorHSV(cul2, 255, luminozitate, true);
lineColor3 = matrix.ColorHSV(cul3, 255, luminozitate, true);
lineColor4 = matrix.ColorHSV(cul4, 255, luminozitate, true);
lineColor5 = matrix.ColorHSV(0xf000, 255, luminozitate, true);
lineColor6 = matrix.ColorHSV(0x1f00, 255, luminozitate, true);         
if (tm->tm_sec == 0)
{
cul1 = random(0, 1536);
cul2 = random(0, 1536);
cul3 = random(0, 1536);
cul4 = random(0, 1536);         
}
            matrix.fillScreen(0);
            matrix.drawCircle(CENTRE_X,CENTRE_Y, 31, lineColor1);
            for (int i = 0; i < 60; i+=15) {
            radians = (90.0F - (i * 6.0F)) * M_PI / 180.0;
            x = round(CENTRE_X + 30 * cos(radians));
            y = round(CENTRE_Y - 30 * sin(radians));
            //matrix.fillCircle(x,y, 2, lineColor2);

            //Panah 9
            matrix.drawLine(0,29, 0,33, lineColor2);
            matrix.drawLine(1,29, 1,33, lineColor2);
            matrix.drawLine(2,30, 2,32, lineColor2);
            matrix.fillCircle(2,31, 1, lineColor2);

            //Panah 3
            matrix.drawLine(62,29, 62,33, lineColor2);
            matrix.drawLine(61,29, 61,33, lineColor2);
            matrix.drawLine(60,30, 60,32, lineColor2);
            matrix.fillCircle(60,31, 1, lineColor2);
            }

            //Panah 12
            matrix.drawLine(29,0, 33,0, lineColor2);
            matrix.drawLine(29,1, 33,1, lineColor2);
            matrix.drawLine(30,2, 32,2, lineColor2);
            matrix.fillCircle(31,2, 1, lineColor2);

            //Panah 6
            matrix.drawLine(29,62, 33,62, lineColor2);
            matrix.drawLine(29,61, 33,61, lineColor2);
            matrix.drawLine(30,60, 32,60, lineColor2);
            matrix.fillCircle(31,60, 1, lineColor2);
            
            
            matrix.setFont(&bmz5x7);
            matrix.setTextColor(lineColor3);
            //Jam 1
            matrix.setCursor(43,9);
            matrix.print(char(1));

            //Jam 2
            matrix.setCursor(51,19);
            matrix.print(char(2));
            
            //Jam 3
            matrix.setCursor(54,30);
            matrix.print(char(3));
            
            //Jam 4
            matrix.setCursor(51,42);
            matrix.print(char(4));

            //Jam 5
            matrix.setCursor(42,51);
            matrix.print(char(5));
            
            //Jam 6
            matrix.setCursor(29,54);
            matrix.print(char(6));

            //Jam 7
            matrix.setCursor(16,51);
            matrix.print(char(7));
            
            //Jam 8
            matrix.setCursor(8,42);
            matrix.print(char(8));
            
            //Jam 9
            matrix.setCursor(4,30);
            matrix.print(char(9));

            //Jam 10
            matrix.setCursor(6,19);
            matrix.print(char(1));
            matrix.setCursor(11,19);
            matrix.print(char(0));

            //Jam 11
            matrix.setCursor(17,8);
            matrix.print(char(11));

            //Jam 12
            matrix.setCursor(28,6);
            matrix.print(char(12));
            
           //Jam Digital
            matrix.setFont(&bmz5x7);
            matrix.setTextColor(lineColor3);
            
            

            for (int i = 0; i < 60; i+=5) {
            radians = (90.0F - (i * 6.0F)) * M_PI / 180.0;
            x = round(CENTRE_X + 30 * cos(radians));
            y = round(CENTRE_Y - 30 * sin(radians));
            matrix.fillCircle(x,y, 1, lineColor2);
            x = round(CENTRE_X + 26 * cos(radians));
            y = round(CENTRE_Y - 26 * sin(radians));
            matrix.setCursor(x,y);
            //matrix.print(char(j));
            j++;
            if (j == 10) { j = 11;}
            }
            
            matrix.setFont();

            
           

            
// second tongue          
            // Serial.println(tm->tm_sec);
            secunda = tm->tm_sec;
            radians = (90.0F - ((float)tm->tm_sec * 6.0F)) * M_PI / 180.0F;
            x = round(CENTRE_X + R_SEC * cos(radians));
            y = round(CENTRE_Y - R_SEC * sin(radians));
            matrix.drawLine(x,y, CENTRE_X-1, CENTRE_Y, lineColor4);
            matrix.drawLine(x,y, CENTRE_X+1, CENTRE_Y, lineColor4);
            matrix.drawLine(x,y, CENTRE_X, CENTRE_Y-1, lineColor4);
            matrix.drawLine(x,y, CENTRE_X, CENTRE_Y+1, lineColor4);
            matrix.fillCircle(x,y, 1, lineColor4);

// minute tongue
            radians = (90.0F - (tm->tm_min * 6.0F)) * M_PI / 180.0;
            x = round(CENTRE_X + R_MIN * cos(radians));
            y = round(CENTRE_Y - R_MIN * sin(radians));
            matrix.drawLine(x,y, CENTRE_X, CENTRE_Y-1, lineColor6);
            matrix.drawLine(x,y, CENTRE_X, CENTRE_Y+1, lineColor6);
            matrix.drawLine(x,y, CENTRE_X-1, CENTRE_Y, lineColor6);
            matrix.drawLine(x,y, CENTRE_X+1, CENTRE_Y, lineColor6);

 // hour tongue
            radians = (90.0F - ((hour * 30.0F) + ((tm->tm_min * 29.0F) / 60.0F))) * M_PI / 180.0F;
            x = round(CENTRE_X + R_HOUR * cos(radians));
            y = round(CENTRE_Y - R_HOUR * sin(radians));
            matrix.drawLine(x,y, CENTRE_X, CENTRE_Y-1, lineColor5);
            matrix.drawLine(x,y, CENTRE_X, CENTRE_Y+1, lineColor5);
            matrix.drawLine(x,y, CENTRE_X-1, CENTRE_Y, lineColor5);
            matrix.drawLine(x,y, CENTRE_X+1, CENTRE_Y, lineColor5);           
    }
