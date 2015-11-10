#ifndef MyTypes_h
#define MyTypes_h

// Radio pipe addresses for the 2 nodes to communicate.


#define RADIO_SEND_PIPE 0xF0F0F0F0E1LL
#define RADIO_RECEIVE_PIPE 0xF0F0F0F0D2LL
#define RADIO_CS A5
#define RADIO_EN 10

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// Screen calibration
#define TS_MINX 176
#define TS_MINY 186
#define TS_MAXX 904
#define TS_MAXY 954

#define MINPRESSURE 1
#define MAXPRESSURE 1000

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
typedef struct {
  int x;
  int y;
  int w;
  int h;
  uint16_t color;
} Button;


#endif
