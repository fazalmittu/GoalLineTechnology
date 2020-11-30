
/**


   mod v0.1  20.10.2016  ex. sparkFun  timeouts on waits
   mod v0.2  21.10.2016  add smoothing and alarming


   HC-SR04 Demo
   Demonstration of the HC-SR04 Ultrasonic Sensor
   Date: August 3, 2016

   Description:
    Connect the ultrasonic sensor to the Arduino as per the
    hardware connections below. Run the sketch and open a serial
    monitor. The distance read from the sensor will be displayed
    in centimeters and inches.

   Hardware Connections:
    Arduino | HC-SR04
    -------------------
      5V    |   VCC
      7     |   Trig
      8     |   Echo
      GND   |   GND

   License:
    Public Domain
*/

// Pins
const int trigPin1=2;
const int echoPin1=3;

const int trigPin2=4;
const int echoPin2=5;

//const int trigPin3=6;
//const int echoPin3=7;

//const int trigPin4=8;
//const int echoPin4=9;

//const int TRIG_PIN = 7;
//const int ECHO_PIN = 8;


int score = 0;

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;

//
const int STABILISATION_TIME_SECS = 10 ;

const int SAMPLE_BUFFER_ENTRIES = 20 ;
unsigned long sample_buffer[ SAMPLE_BUFFER_ENTRIES ] = {0} ;
int sample_buffer_ptr = 0 ;

unsigned long stabilisation_period_start_ms ;
unsigned long alarm_triggered_ms ;
const unsigned long ALARM_DURATION_MS = 4000 ;

// boolean lastValidPulseOutOfAverage = false ;
int consecutiveOutOfAveragePulseCount = 0 ;
const int TRIGGER_LEVEL_OUT_OF_AVERAGE_PULSES = 0 ;  // consecutive




void addSample( unsigned long pulseLengthUs ) {
  sample_buffer[ sample_buffer_ptr ] = pulseLengthUs ;
  sample_buffer_ptr ++ ;
  if ( sample_buffer_ptr >= SAMPLE_BUFFER_ENTRIES ) sample_buffer_ptr = 0 ;
}


unsigned long averageSampleUs() {
  unsigned long av = 0 ;
  for ( int i = 0 ; i < SAMPLE_BUFFER_ENTRIES ; i ++ ) {
    av += ( sample_buffer[ i ] / SAMPLE_BUFFER_ENTRIES ) ;
  }
  return av ;
}


void setup() {
  // The Trigger pin will tell the sensor to range find
  pinMode(trigPin1, OUTPUT);
  digitalWrite(trigPin1, LOW);
  pinMode(trigPin2, OUTPUT);
  digitalWrite(trigPin2, LOW);

  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);

  stabilisation_period_start_ms = millis() ;
  alarm_triggered_ms = 0 ;
}


void loop() {

  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  float inches;
  long incomingByte;

  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte = "score") {
      Serial.println();
      Serial.println();
      Serial.println(score-1);
      Serial.println();
      Serial.println();
    
    
        }
    
  }
  
  // Hold the trigger pin high for at least 10 us
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  // Wait for pulse on echo pin

  unsigned long wait1 = micros();
  
  while ( digitalRead(echoPin1) == 0  ) {
    if ( micros() - wait1 > MAX_DIST ) {
      Serial.println("wait1 Out of range");
      return ;
    }
  }

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(echoPin1) == 1  ) {
    if ( micros() - t1 > MAX_DIST ) {
      Serial.println("wait2 Out of range");
      return ;
    }
  }

  t2 = micros();
  
  pulse_width = t2 - t1;
  

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed
  //of sound in air at sea level (~340 m/s).
  cm = pulse_width / 58.0;
  inches = pulse_width / 148.0;


  // Print out results
  if ( pulse_width > MAX_DIST ) {
    Serial.println("Out of range");
  } else {

    /*
        We have a valid pulse

    */


    if ( millis() - stabilisation_period_start_ms > ( STABILISATION_TIME_SECS * 1000UL ) ) {
      unsigned long avPulse = averageSampleUs() ;
      if ( labs ( avPulse - pulse_width ) >  avPulse / 7  ) {
        // circa 15% change
        if ( consecutiveOutOfAveragePulseCount >=  TRIGGER_LEVEL_OUT_OF_AVERAGE_PULSES  ) {
          // initiate alarm and reset
          Serial.println("GOAL");
          alarm_triggered_ms = millis() ;
          consecutiveOutOfAveragePulseCount = 0 ;
          stabilisation_period_start_ms = millis() ;
          score ++;
          Serial.println();
          Serial.println();
          Serial.println();
                   
        }
        else {
          consecutiveOutOfAveragePulseCount ++ ;
          Serial.println("got out of average pulse. Dropping");
        }
      }
      else {
        addSample( pulse_width ) ;
        consecutiveOutOfAveragePulseCount = 0  ;
      }
    }
    else {
      Serial.println("in Stabilisation Period");
      addSample( pulse_width ) ;
      consecutiveOutOfAveragePulseCount = 0 ;
    }

    

    Serial.print(cm);
    Serial.print(" cm \t");
    Serial.print(inches);
    Serial.println(" in");
  }

  // Wait at least 60ms before next measurement
  delay(100);
}
