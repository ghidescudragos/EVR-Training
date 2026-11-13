#include "task5.h"

void setup_task5(void) {
  // folosesc TCNT2 si numar 16.000 de cloci interni OCR2A = 250 cu un prescaler de 64
  // folosesc modul fast PWM si imi setez pinul ca iesire OC2B -> PD3
  // in timpul intreruperii eu cresc OCR2B cu ajutorul careia generez semnalul pentru aprinderea led ului gradual
  OCR2A = 250;
  OCR2B = 0;

  // resetez atat timer - ul cat si registrii de control (configurarile)
  TCNT2 = 0;
  TCCR2A = 0;
  TCCR2B = 0;

  // setez pin ul PD3 (OC2B) ->Clear OC2B on compare match when up-counting
  // cu cat cresc OCR2B cu atat ii dau clear mai tarziu si deci creste duty cycle ul
  TCCR2A |= (1 << COM2B1 | 1 << WGM21 | 1 << WGM20);
  TCCR2A &= ~(1 << COM2B0);
  TCCR2B |= 1 << WGM22;

  DDRD |= (1 << DDD3);

  // setez prescaler ul la 64
  TCCR2B |= 1 << CS22;
  TCCR2B &= ~(1 << CS21 | 1 << CS20);

  TIFR2 |= (1 << OCF2A);  // ma asigur ca dau clear la flag sa nu intre din prima in ISR

  TIMSK2 &= ~(1 << TOIE2);
  TIMSK2 |= (1 << OCIE2A);
  // enable la intreruperi globale
  SREG |= (1 << 7);
}

ISR(TIMER2_COMPA_vect) {
  static uint8_t counter = 0;
  counter++;
  if (counter > 10) {
    if (OCR2B + 1 > 250) {
      OCR2B = 0;
    } else {
      OCR2B++;
    }
    counter = 0;
  }
}

void task5(void) {
}