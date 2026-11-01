#include "task1.h"

static enum states cs = ST0;
static enum states ns = ST0;
static unsigned long previousMillis = 0;
static const unsigned long interval = 1000;

void setup_task1(void) {
  PORTB &= ~((1 << PB5) | (1 << PB4) | (1 << PB3));
  DDRB |= (1 << DDB5) | (1 << DDB4) | (1 << DDB3);
}

void task1(void) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    switch (cs) {
      case ST0: 
        PORTB &= ~((1 << PB5) | (1 << PB4) | (1 << PB3));
        ns = ST1;
        break;

      case ST1:
        PORTB &= ~((1 << PB5) | (1 << PB4) | (1 << PB3));
        PORTB |= (1 << PB5);
        ns = ST2;
        break;

      case ST2:
        PORTB &= ~((1 << PB5) | (1 << PB4) | (1 << PB3));
        PORTB |= (1 << PB5) | (1 << PB4);
        ns = ST3;
        break;

      case ST3: 
        PORTB &= ~((1 << PB5) | (1 << PB4) | (1 << PB3));
        PORTB |= (1 << PB5) | (1 << PB4) | (1 << PB3);
        ns = ST0;
        break;
    }

    cs = ns;
  }
}