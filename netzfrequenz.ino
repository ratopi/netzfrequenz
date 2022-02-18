/**
  * Bestimmung der Netzfrequenz
  * Copyright 2015 by Ralf Th. Pietsch <ratopi@abwesend.de>
  * LICENSE: MIT
  */
  
#define PROBE_COUNT 90
#define MICROS 1000000.
#define PROBE_COUNT_KORREKTUR 20

int val;
int counter = 0;
int i, j;
float freq;

long start;
long end;

long min = 30000;
long max = 0;

long adding = 0;

int myarray[PROBE_COUNT];

void setup()
{  
  Serial.begin( 115200 );

  analogReference( INTERNAL );
  
  for ( i = 0; i < 10000; i++ )
  {
      analogRead( A0 );
  }
}

void loop()
{

  while ( analogReadPlus() != 0 );
  while ( analogReadPlus() == 0 );

  adding = 0;
  min = 30000;
  max = 0;

  for ( i = 0; i < PROBE_COUNT; i++ )
  {
    period(i);
  }
 
  sort(myarray,PROBE_COUNT);

  for ( i = PROBE_COUNT_KORREKTUR; i < (PROBE_COUNT - PROBE_COUNT_KORREKTUR); i++ )
  {
    adding = adding + myarray[i];
  }
  
  freq = ( ( (PROBE_COUNT - ( 2 * PROBE_COUNT_KORREKTUR ) ) * MICROS ) / ( adding ) ) * 1000;
  // das letzte * 1000 dient nur dazu, den serial plotter benutzen zu können
  // für die korrekte Ausgabe auf dem serial Monitor muss das entfernt werden

  Serial.println( freq, 3 );
  
}

void period(int i)
{

  start = micros();
  while ( analogReadPlus() != 0 );
  delay(2);
  while ( analogReadPlus() == 0 );
  end = micros();

  myarray[i] = (end - start);

}

int analogReadPlus() 
{
  val = analogRead( A0 );
  return val;
}

void sort(int a[], int size) {
    for(int i=0; i<(size-1); i++) {
        for(int o=0; o<(size-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    int t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}
