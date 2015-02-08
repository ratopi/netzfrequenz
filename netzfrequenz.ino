/**
  *
  */
  
#define PROBE_COUNT 50
#define MICROS 1000000.
#define KORREKTUR 1.001341341

int min;
int max;
int val;
int i, j;
float freq;

long start;
long end;

void setup()
{  
  Serial.begin( 9600 );

  analogReference( INTERNAL );
  
  Serial.println( "reading 10000 samples ..." );
  
  for ( i = 0; i < 10000; i++ )
  {
      analogRead( A0 );
  }
  
  Serial.println( "waiting for first period");
  
  period();

  end = micros();
  
  Serial.println( "starting..." );
}

void loop()
{
  start = end;
  min = 1024;
  max = 0;
  for ( i = 0; i < PROBE_COUNT; i++ )
  {
    period();
  }
  end = micros();
  freq = ( ( PROBE_COUNT * MICROS * KORREKTUR ) / ( end - start ) );
  Serial.print( min );
  Serial.print( " " );
  Serial.print( max );
  Serial.print( " " );
  Serial.println( freq, 3 );
}

void period()
{
  while ( analogReadPlus() > 50 ); // myDelay();
  while ( analogReadPlus() < 100 ); // myDelay();
}

int analogReadPlus() 
{
  val = analogRead( A0 );
  
  if ( val < min ) min = val;
  if ( val > max ) max = val;
  
  return val;
}

void myDelay()
{
  for ( j = 0; j < 10000; j++ );
}

