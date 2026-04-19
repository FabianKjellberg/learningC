#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "fizzbusspop.h"

fizzBuzzPop newFizzBuzzPop(int nr) {
  fizzBuzzPop result;
  result.count = nr;

  bool fizz = nr % 3 == 0;
  bool buzz = nr % 5 == 0;
  bool pop = nr % 7 == 0;

  if(fizz || buzz || pop) {
    snprintf(result.text, sizeof(result.text), "%s%s%s%s" ,
      (fizz) ? "fizz" : "", 
      (buzz) ? "buzz" : "", 
      (pop) ? "pop" : "", 
      ":"
    );
  }
  else {
    snprintf(result.text, sizeof(result.text), "number:");
  }

  return result;
}

void writeFizzBuzzPop(fizzBuzzPop fbp) {
  printf("%s %d\n", fbp.text, fbp.count);
}

void fizzBussPopTask(void) {
  int count = 0;
  while(1) {
    fizzBuzzPop c = newFizzBuzzPop(count);
    writeFizzBuzzPop(c);

    count++;
    vTaskDelay(pdMS_TO_TICKS(1000));
 }
}
