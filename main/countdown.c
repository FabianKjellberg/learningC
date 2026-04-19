#include "countdown.h"
#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

countdown countdown_init(void) {
  countdown result;

  result.count = 10;
  result.down = true;

  return result;
}

void countdown_update(countdown *cd) {
  if (cd->count <= 0) cd->down = false;
  if (cd->count >= 10) cd->down = true;
  
  if (cd->down) {
    cd->count--;
  }
  else {
    cd->count++;
  }
} 

void countdown_print(countdown cd) {
  printf("%s: %d\n", cd.down ? "down" : "up", cd.count);
}

void countdown_task(void) {
  countdown cd = countdown_init();

  while(1) {
    countdown_print(cd);
    countdown_update(&cd);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
