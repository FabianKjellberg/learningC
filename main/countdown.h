#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <stdbool.h>

typedef struct {
  bool down;
  int count;
} countdown;

void countdown_task(void);

#endif