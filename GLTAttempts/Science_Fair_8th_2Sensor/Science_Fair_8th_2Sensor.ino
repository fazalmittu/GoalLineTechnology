
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
const unsigned int MAX_DIST = 23200; //no need to repeat

//
const int STABILISATION_TIME_SECS = 10 ; //no need to repeat

const int SAMPLE_BUFFER_ENTRIES = 20 ; //no need to repeat
unsigned long sample_buffer_1[ SAMPLE_BUFFER_ENTRIES ] = {0};
unsigned long sample_buffer_2[ SAMPLE_BUFFER_ENTRIES ] = {0};
int sample_buffer_ptr_1 = 0;
int sample_buffer_ptr_2 = 0;

unsigned long stabilisation_period_start_ms_1;
unsigned long stabilisation_period_start_ms_2;

unsigned long alarm_triggered_ms_1;
unsigned long alarm_triggered_ms_2;


// boolean lastValidPulseOutOfAverage = false ;
int consecutiveOutOfAveragePulseCount_1 = 0;
int consecutiveOutOfAveragePulseCount_2 = 0;
const int TRIGGER_LEVEL_OUT_OF_AVERAGE_PULSES = 0 ;  // consecutive //no need to repeat




void addSample_1( unsigned long pulseLengthUs_1 ) {
  sample_buffer_1[ sample_buffer_ptr_1 ] = pulseLengthUs_1 ;
  sample_buffer_ptr_1 ++ ;
  if ( sample_buffer_ptr_1 >= SAMPLE_BUFFER_ENTRIES ) sample_buffer_ptr_1 = 0 ;
}

void addSample_2( unsigned long pulseLengthUs_2 ) {
  sample_buffer_2[ sample_buffer_ptr_2 ] = pulseLengthUs_2 ;
  sample_buffer_ptr_2 ++ ;
  if ( sample_buffer_ptr_2 >= SAMPLE_BUFFER_ENTRIES ) sample_buffer_ptr_2 = 0 ;
}

unsigned long averageSampleUs_1() {
  unsigned long av_1 = 0 ;
  for ( int i = 0 ; i < SAMPLE_BUFFER_ENTRIES ; i ++ ) {
    av_1 += ( sample_buffer_1[ i ] / SAMPLE_BUFFER_ENTRIES ) ;
  }
  return av_1 ;
}

unsigned long averageSampleUs_2() {
  unsigned long av_2 = 0 ;
  for ( int j = 0 ; j < SAMPLE_BUFFER_ENTRIES ; j ++ ) {
    av_2 += ( sample_buffer_2[ j ] / SAMPLE_BUFFER_ENTRIES ) ;
  }
  return av_2 ;
}


void setup() {
  // The Trigger pin will tell the sensor to range find
  pinMode(trigPin1, OUTPUT);
  digitalWrite(trigPin1, LOW);
  pinMode(trigPin2, OUTPUT);
  digitalWrite(trigPin2, LOW);

  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);

  stabilisation_period_start_ms_1 = millis() ;
  alarm_triggered_ms_1 = 0 ;

  stabilisation_period_start_ms_2 = millis() ;
  alarm_triggered_ms_2 = 0 ;
}


void loop() {

  unsigned long t1_1;
  unsigned long t1_2;
  unsigned long t2_1;
  unsigned long t2_2;
  unsigned long pulse_width_1;
  unsigned long pulse_width_2;
  float cm_1;
  float cm_2;
  float inches_1;
  float inches_2;
  long incomingByte_1;

  if (Serial.available() > 0) {
    incomingByte_1 = Serial.read();
    if (incomingByte_1 = "score") {
      Serial.println();
      Serial.println();
      Serial.println(score);
      Serial.println();
      Serial.println();
    
    
        }
    
  }
  
  // Hold the trigger pin high for at least 10 us
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  // Wait for pulse on echo pin

  unsigned long wait1_1 = micros();
  unsigned long wait1_2 = micros();
  
  while ( digitalRead(echoPin1) == 0  ) {
    if ( micros() - wait1_1 > MAX_DIST ) {
      Serial.println("wait1_1 Out of range");
      return ;
    }
  }
  
  while ( digitalRead(echoPin2) == 0  ) {
    if ( micros() - wait1_2 > MAX_DIST ) {
      Serial.println("wait1_2 Out of range");
      return ;
    }
  }

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1_1 = micros();
  while ( digitalRead(echoPin1) == 1  ) {
    if ( micros() - t1_1 > MAX_DIST ) {
      Serial.println("wait2_1 Out of range");
      return ;
    }
  }

  t1_2 = micros();
  while ( digitalRead(echoPin2) == 1  ) {
    if ( micros() - t1_2 > MAX_DIST ) {
      Serial.println("wait2_2 Out of range");
      return ;
    }
  }

  t2_1 = micros();
  t2_2 = micros();
  
  pulse_width_1 = t2_1 - t1_1;
  pulse_width_2 = t2_2 - t1_2;
  

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed
  //of sound in air at sea level (~340 m/s).
  cm_1 = pulse_width_1 / 58.0;
  inches_1 = pulse_width_1 / 148.0;

  cm_2 = pulse_width_2 / 58.0;
  inches_2 = pulse_width_2 / 148.0;

  unsigned long int distance[] = {1, 2};
  // Print out results
  if ( pulse_width_1 > MAX_DIST ) {
    Serial.println("Sensor 1 Out of range");
  } else {

    /*
        We have a valid pulse

    */
  
    if ( millis() - stabilisation_period_start_ms_1 > ( STABILISATION_TIME_SECS * 1000UL ) ) {
      unsigned long avPulse_1 = averageSampleUs_1() ;
      if ( labs ( avPulse_1 - pulse_width_1 ) >  avPulse_1 / 7  ) {
        // circa 15% change
        if ( consecutiveOutOfAveragePulseCount_1 >=  TRIGGER_LEVEL_OUT_OF_AVERAGE_PULSES  ) {
          // initiate alarm and reset
          if ( consecutiveOutOfAveragePulseCount_2 >= TRIGGER_LEVEL_OUT_OF_AVERAGE_PULSES ) {
            distance[1] = inches_1;
            Serial.println("GOAL");
            Serial.print(inches_1);
            Serial.println(" inches");
            alarm_triggered_ms_1 = millis() ;
            consecutiveOutOfAveragePulseCount_1 = 0 ;
            stabilisation_period_start_ms_1 = millis() ;
            score ++;
            Serial.println();
            Serial.println();
            Serial.println();
          }
          else {
            consecutiveOutOfAveragePulseCount_1 ++ ;
            Serial.println("Sensor 1 - got out of average pulse. Dropping");
          }
        }
      }  
      else {
        addSample_1( pulse_width_1 ) ;
        consecutiveOutOfAveragePulseCount_1 = 0  ;
      }
    }
    else {
      Serial.println("Sensor 1 - in Stabilisation Period");
      addSample_1( pulse_width_1 ) ;
      consecutiveOutOfAveragePulseCount_1 = 0 ;
    }


    

    

    /*Serial.print(cm);
    Serial.print(" cm \t");
    Serial.print(inches);
    Serial.println(" in");*/
  }

  // Wait at least 60ms before next measurement
  delay(100);


/*
 * 
 * 
 *SENSOR 2---------------------- 
 * 
 * 
 */
/*
if ( pulse_width_2 > MAX_DIST ) {
    Serial.println("Sensor 2 Out of range");
  } else {

    /*
        We have a valid pulse

    
  
   if ( millis() - stabilisation_period_start_ms_2 > ( STABILISATION_TIME_SECS * 1000UL ) ) {
      unsigned long avPulse_2 = averageSampleUs_2() ;
      if ( labs ( avPulse_2 - pulse_width_2 ) >  avPulse_2 / 7  ) {
        // circa 15% change
        if ( consecutiveOutOfAveragePulseCount_2 >=  TRIGGER_LEVEL_OUT_OF_AVERAGE_PULSES  ) {
          // initiate alarm and reset
      
            Serial.println("GOAL");
            distance[2] = inches_2;
            Serial.print(inches_2);
            Serial.println(" inches");
            alarm_triggered_ms_2 = millis() ;
            consecutiveOutOfAveragePulseCount_2 = 0 ;
            stabilisation_period_start_ms_2 = millis() ;
            score ++;
            Serial.println();
            Serial.println();
            Serial.println();
          
                   
        }
        else {
          consecutiveOutOfAveragePulseCount_2 ++ ;
          Serial.println("Sensor 2 - got out of average pulse. Dropping");
        }
      }
      else {
        addSample_2( pulse_width_2 ) ;
        consecutiveOutOfAveragePulseCount_2 = 0  ;
      }
    }
    else {
      Serial.println("Sensor 2 - in Stabilisation Period");
      addSample_2( pulse_width_2 ) ;
      consecutiveOutOfAveragePulseCount_2 = 0 ;
    }


    *\

    

    /*Serial.print(cm);
    Serial.print(" cm \t");
    Serial.print(inches);
    Serial.println(" in");
  }

  // Wait at least 60ms before next measurement
  delay(100);*/
}
