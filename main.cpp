// Harshdeep Singh
// 2320006
// Batch 2

# include <avr/io.h>            
# include <avr/interrupt.h>

void init();
void pwm_init();
void pwm_on();
void timer0_init();
void timer_off();
void pwm_off();

int Count;
int seat_occupied=0;
int seat_belt=0;
int req_delay = 0;

int main()
{ 
  sei();
  Serial.begin(9600);
  DDRD |= (1 << PD7);
  init();
  
  while(1)
  {
    if (seat_occupied == 1) 
    {
      	Serial.println(" seat occupied");
        if (seat_belt == 0)
        {
      		timer0_init();
       		if(req_delay == 1)
      		{
              PORTD |= (1 << PD7);
              pwm_init();
        	pwm_on();
        	Serial.println("buzzer ON");
        	}
    	}
      else
      {
        Serial.println("seat belt on");
        PORTD &= ~(1 << PD7);
     	timer_off();
      	pwm_off();
      }
    }
    else
    {
      Serial.println("In else");
      PORTD &= ~(1 << PD7);
      timer_off();
      pwm_off();
    }
  }
  
}

void init()
{
  EICRA |=  (1 << ISC00);
  EICRA &=~ (1 << ISC11);
  EIMSK |= (1 << INT0);
  EIMSK |= (1 << INT1);
  
}
void pwm_init()
{
    TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10);
  	OCR1A = 512;
  	OCR1B = 0;
	TIMSK1 |= (1 << OCIE1A)|(1<<OCIE1B);
}

void pwm_on()
{
  TIMSK1 |= (1 << OCIE1A)|(1<<OCIE1B);
  OCR1A = 256;
  OCR1B = 128;
}
void timer0_init()
{
	TCCR0A |=(1 << WGM01); 
	OCR0A= 81;                            
	TIMSK0 |= (1 <<  OCIE0A) ; 
  	TCCR0B |= (1 << CS02)|(1 << CS00);
  
}
void timer_off()
{
  TIMSK1 &=~ (1 << OCIE1A)& ~(1<<OCIE1B);
  TCCR0A = 0x00;
  TIMSK0 = 0x00;
  req_delay = 0;
}


void pwm_off()
{
  
  OCR1A = 256;
  OCR1B = 0;
}

ISR (TIMER1_COMPA_vect)
{
	PORTD |= (1 << PD7);                                  
}
ISR (TIMER1_COMPB_vect)
{
	PORTD &= ~(1 << PD7);
}

ISR (INT0_vect)
{
  seat_occupied++;
  seat_occupied = seat_occupied % 2 ;
}

ISR (INT1_vect)
{PORTD |= (1 << PORTD7);
 seat_belt++;
 seat_belt = seat_belt % 2 ;
}

ISR(TIMER0_COMPA_vect)
{
	Count++;
	if (Count >= 100)           
	{
      req_delay = 1;
      Count=0;
    }
}