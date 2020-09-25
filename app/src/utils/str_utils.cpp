#include "protothreads.h"
#include "Arduino.h"
#include "str_utils.h"

char *subString(const char *g, int start, int size) {
  char *substr = (char*) malloc(size + 1);
  if (substr) {
    int i;
    for(i = 0; i < size; i++) {
      substr[i] = g[start + i];
    }
    substr[i] = '\0';
  }
  return substr;
}


void append(char* s, char c) {
  int len = strlen(s);
  s[len] = c;
  s[len+1] = '\0';
}