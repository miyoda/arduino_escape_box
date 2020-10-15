#include <protothreads.h>
#include "lcd.h"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string.h>
#include "../outputs/serial.h"
#include "../utils/str_utils.h"

using namespace std;  

/*
  LiquidCrystal Library - Hello World

 Using a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 */


int const PIN_LCD_D7 = 29;
int const PIN_LCD_D6 = 27;
int const PIN_LCD_D5 = 28;
int const PIN_LCD_D4 = 26;
int const PIN_LCD_ENABLE = 25;
int const PIN_LCD_RS = 24;

int const LCD_WIDTH = 16;
int const LCD_HEIGHT = 2;

static struct pt pt_lcd_line0, pt_lcd_line1;
static char *line0Text = "Starting...", *line1Text = "...";

LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_ENABLE, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);

void setLcdLine0Text(const char *text) {
  if (strcmp(line0Text, text) != 0) {
    Serial.print("lcd line0 = "); Serial.print(text); Serial.println("|");
    line0Text = text;
  } else {
    Serial.print("lcd line0 = "); Serial.print(text); Serial.println("| (no changes)");
  }
}

void setLcdLine1Text(const char *text) {
  if (strcmp(line1Text, text) != 0) {
    Serial.print("lcd line1 = "); Serial.print(text); Serial.println("|");
    line1Text = text;
  } else {
    Serial.print("lcd line1 = "); Serial.print(text); Serial.println("| (no changes)");
  }
}

void setup_lcd() {
  PT_INIT(&pt_lcd_line0);
  PT_INIT(&pt_lcd_line1);

  lcd.noBlink();
  lcd.noCursor();
  lcd.noAutoscroll();
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
}

void paddingRight(char *text, int maxSize, char c) {
  int len = strlen(text);
  text[maxSize] = '\0';
  for (int i=len; i<maxSize; i++) {
    text[i] = c;
  }
}

void lcdPrintFullLine(int lineNum, char *text) {
  char lineText[LCD_WIDTH+1];
  strcpy(lineText, text);
  paddingRight(lineText, LCD_WIDTH, ' ');
  lcd.setCursor(0,lineNum);
  lcd.print(lineText);
  //Serial.print("lcdPrintFullLine = "); Serial.print(lineNum); Serial.print(" = "); Serial.print(lineText); Serial.println("| (no changes)");
}

void substr(char *__dst, char *__src , int start, int len){
    memcpy(__dst, &__src[start], len);
    __dst[len] = '\0';
}

int schedule_lcd_line0(struct pt *pt) {
  static int scrollPos;
  static char *currentContent;
  PT_BEGIN(pt);
  for(;;) {
    currentContent = (char*) malloc(sizeof(char)*(strlen(line0Text)+1));
    strcpy(currentContent, line0Text);
    if (strlen(currentContent) <= LCD_WIDTH) {
      lcdPrintFullLine(0, currentContent);
      PT_WAIT_UNTIL(pt, strcmp(line0Text, currentContent));
    } else {
      scrollPos = 1-LCD_WIDTH;
      while (strcmp(currentContent, line0Text) == 0) {
        char scolledLineContent[LCD_WIDTH+1];
        scolledLineContent[0] = '\0';
        if (scrollPos < 0) {
          paddingRight(scolledLineContent, -scrollPos, ' ');
        }
        int visibleLen = LCD_WIDTH + min(0, scrollPos);
        char *visibleText = subString(currentContent, max(0, scrollPos), visibleLen);
        strcat(scolledLineContent, visibleText);
        free(visibleText);
        lcdPrintFullLine(0, scolledLineContent);
        int currentContentLen = strlen(currentContent);
        if (scrollPos > currentContentLen) {
          scrollPos = 1-LCD_WIDTH;
        } else {
          scrollPos++;
        }
        PT_SLEEP(pt, 300);
      }
    }
    free(currentContent);
  }
  PT_END(pt);
}

int schedule_lcd_line1(struct pt *pt) {
  static int scrollPos;
  static char *currentContent;
  PT_BEGIN(pt);
  for(;;) {
    currentContent = (char*) malloc(sizeof(char)*(strlen(line1Text)+1));
    strcpy(currentContent, line1Text);
    if (strlen(currentContent) <= LCD_WIDTH) {
      lcdPrintFullLine(1, currentContent);
      PT_WAIT_UNTIL(pt, strcmp(line1Text, currentContent));
    } else {
      scrollPos = 1-LCD_WIDTH;
      while (strcmp(currentContent, line1Text) == 0) {
        char scolledLineContent[LCD_WIDTH+1];
        scolledLineContent[0] = '\0';
        if (scrollPos < 0) {
          paddingRight(scolledLineContent, -scrollPos, ' ');
        }
        int visibleLen = LCD_WIDTH + min(0, scrollPos);
        char *visibleText = subString(currentContent, max(0, scrollPos), visibleLen);
        strcat(scolledLineContent, visibleText);
        free(visibleText);
        lcdPrintFullLine(1, scolledLineContent);
        int currentContentLen = strlen(currentContent);
        if (scrollPos > currentContentLen) {
          scrollPos = 1-LCD_WIDTH;
        } else {
          scrollPos++;
        }
        PT_SLEEP(pt, 300);
      }
    }
    free(currentContent);
  }
  PT_END(pt);
}

void loop_lcd() {
  PT_SCHEDULE(schedule_lcd_line0(&pt_lcd_line0));
  PT_SCHEDULE(schedule_lcd_line1(&pt_lcd_line1));
}

