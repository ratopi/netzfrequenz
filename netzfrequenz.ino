#include <Arduino.h>

/**
  * Bestimmung der Netzfrequenz
  * Copyright 2015 by Ralf Th. Pietsch <ratopi@abwesend.de>
  * LICENSE: MIT
  */
  
#define PROBE_COUNT 50
#define MICROS 1000000.
#define KORREKTUR 1.0
#define SecSamples 100

#define DEBUG 0

int Vmin;
int Vmax;
int Vval;
int Vmid;

int i, j;
float freq, freqmin, freqmax;
long start;
long end;

//#############################################################################
//  Function declarations
//#############################################################################

void period();
int analogReadPlus() ;

//#############################################################################
//  Arduino code
//#############################################################################

void setup()
{  
  Serial.begin( 115200 );

  //analogReference( INTERNAL );
  
  Serial.println( "reading 10000 samples ..." );
  
  for ( i = 0; i < 10000; i++ )
  {
      Vval = analogRead( A0 );
      //printf("%d V\n",Vval);
      Vmin = min( Vmin, Vval);
      Vmax = max( Vmax, Vval);
  }
  
  Vmid = ( Vmax - Vmin ) / 2;

  Serial.printf("Min: %d, Max: %d\n",Vmin,Vmax);
  Serial.println( "waiting for first period");
  
  period();

  //end = micros();
  
  Serial.println( "starting..." );
}

void loop()
{
  Vmin = 1024;
  Vmax = 0;
  float Delta;

  while ( analogReadPlus() == 0 );
  for ( j = 0; j < SecSamples; j++ ) {
    analogReadPlus();
  }
  while ( analogReadPlus() > 0 );

  start = micros();
   
  for ( i = 0; i < PROBE_COUNT; i++ )
  {
    period();
  }
  end = micros();
  freq = ( PROBE_COUNT * ( MICROS / double( end - start ) ) );

  Delta = abs(freq - 50.0);

  if ( DEBUG ) 
  {
    Serial.print( Vmin );
    Serial.print( " " );
    Serial.print( Vmax );
    Serial.print( " " );
  }
  switch( Delta > 0.2 ? 2 : Delta > 0.02 ? 1 : 0 ){
    case 2: Serial.print("Error: ");break;
    case 1: Serial.print("Warning: ");break;
    }

  Serial.printf("Count: %d, Micros: %f, Frequency: %f\n",PROBE_COUNT,MICROS,freq);
}

void period()
{
  while ( analogReadPlus() == 0 );
  for ( j = 0; j < SecSamples; j++ ) {
    analogReadPlus();
  }
  while ( analogReadPlus() > 0 );
}

int analogReadPlus() 
{
  Vval = analogRead( A0 );
  
  if ( DEBUG ) 
  {
    Vmin = min( Vmin, Vval );
    Vmax = max( Vmax, Vval );
  }
 
  return Vval;
}
