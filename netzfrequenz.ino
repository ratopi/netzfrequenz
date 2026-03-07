/**
  * Determination of the mains frequency
  * Copyright 2015-2026 by Ralf Th. Pietsch <ratopi@abwesend.de>
  * LICENSE: MIT
  */


// Activate for more log output
// #define DEBUG


#define PROBE_COUNT 50
#define MICROS 1000000.
// #define KORREKTUR 1.001341341
#define KORREKTUR 1.

// Analog input port for the antenna
#define ANALOG_PORT A0

// Thresholds for period detection (depends on analogReference INTERNAL)
#define THRESHOLD_LOW 50
#define THRESHOLD_HIGH 100

int sigMin;
int sigMax;

unsigned long startTime;
unsigned long endTime;

void setup()
{
  Serial.begin( 9600 );

  analogReference( INTERNAL );

  Serial.println( "reading 10000 samples ..." );

  for ( int i = 0; i < 10000; i++ )
  {
      analogRead( ANALOG_PORT );
  }

  Serial.println( "waiting for first period");

  period();

  endTime = micros();

  Serial.println( "starting..." );
}

void loop()
{
  startTime = endTime;
  sigMin = 1024;
  sigMax = 0;
  for ( int i = 0; i < PROBE_COUNT; i++ )
  {
    period();
  }
  endTime = micros();
  float freq = ( ( PROBE_COUNT * MICROS * KORREKTUR ) / ( endTime - startTime ) );

#ifdef DEBUG
  Serial.print( sigMin );
  Serial.print( " " );
  Serial.print( sigMax );
  Serial.print( " " );
#endif

  Serial.println( freq, 3 );
}

void period()
{
  while ( analogReadPlus() > THRESHOLD_LOW );
  while ( analogReadPlus() < THRESHOLD_HIGH );
}

int analogReadPlus()
{
  int val = analogRead( ANALOG_PORT );

  if ( val < sigMin ) sigMin = val;
  if ( val > sigMax ) sigMax = val;

  return val;
}

