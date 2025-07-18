#ifndef CONVERTER_H

#define CONVERTER_H

#include <stdio.h>

#include <stdlib.h>

static inline char *fahrenheit_to_celsius(const char *input) {

  static char result[32];

  double f = atof(input);

  double c = (f - 32) * 5.0 / 9.0;

  snprintf(result, sizeof(result), "%g", c);

  return result;
}

#endif
