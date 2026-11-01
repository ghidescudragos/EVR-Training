#include "task1.h"
#include "task2.h"

void setup() {
  // put your setup code here, to run once:
  setup_task1();
  setup_task2();
}

void loop() {
  // put your main code here, to run repeatedly:
  task2();
  task1();
}
