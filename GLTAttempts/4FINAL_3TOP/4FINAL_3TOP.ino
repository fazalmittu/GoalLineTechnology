
/* 
 *  POST ROW
 */

unsigned int trigPin1 = 2;
unsigned int echoPin1 = 3;

unsigned int trigPin2 = 4;
unsigned int echoPin2 = 5;

unsigned int trigPin3 = 6;
unsigned int echoPin3 = 7;

unsigned int trigPin4 = 8;
unsigned int echoPin4 = 9;

/*
 * TOP ROW
 */

unsigned int trigPin5 = 40;
unsigned int echoPin5 = 41;

unsigned int trigPin6 = 42;
unsigned int echoPin6 = 43;

unsigned int trigPin7 = 44;
unsigned int echoPin7 = 45;

unsigned int trigPin8 = 46;
unsigned int echoPin8 = 47;

unsigned int trigPin9 = 48;
unsigned int echoPin9 = 49;

unsigned int trigPin10 = 50;
unsigned int echoPin10 = 51;


int LED_1 = 12;
int LED_2 = 13;
int BUZZER = 33;

const int num_sensor = 10;

long duration, distance;
int arg_sensor;
unsigned int distance_array[num_sensor];
unsigned int pinArray[num_sensor] = {trigPin1, trigPin2, trigPin3, trigPin4, trigPin5, trigPin6, trigPin7, trigPin8, trigPin9, trigPin10};
unsigned int echoPinArray[num_sensor] = {echoPin1, echoPin2, echoPin3, echoPin4, echoPin5, echoPin6, echoPin7, echoPin8, echoPin9, echoPin10};
bool goalArray[num_sensor];
int maxDistPOST = 56;
int maxDistTOP = 42;


void goal_scored() {
    digitalWrite(LED_2, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(LED_2, LOW);
    digitalWrite(BUZZER, LOW);
    Serial.println ("GOAL");
    Serial.println();
    Serial.println();
    digitalWrite(LED_1, LOW);
    delay(10000);
    digitalWrite(LED_1, HIGH);

}

void print_distance(int arg_print) {
  if (distance >= 500 /*|| distance <= 0*/) {
    Serial.println(" OUT OF RANGE ");
  }
  else {
    Serial.print("Sensor ");
    Serial.print(arg_print+1);
    Serial.print(" - ");
    Serial.print(distance_array[arg_print]);
    Serial.println(" cm ");
  }
}

int sensor_trigger(int arg_sensor) {
    digitalWrite(pinArray[arg_sensor], LOW);
    delayMicroseconds(2);
    digitalWrite(pinArray[arg_sensor], HIGH);
    delayMicroseconds(10);
    digitalWrite(pinArray[arg_sensor], LOW);
    duration = pulseIn(echoPinArray[arg_sensor], HIGH);
    distance = (duration/2) / 29.1;
    /*if (distance >= 500 || distance <= 0) {
      Serial.println(" OUT OF RANGE ");
    }
    else {
      Serial.print("Sensor ");
      Serial.print(arg_sensor+1);
      Serial.print(" - ");
      Serial.print(distance_array[arg_sensor]);
      Serial.println(" cm ");
    }*/
    return distance;
}

void setup() {
  Serial.begin (9600);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);
  pinMode(trigPin5, OUTPUT);
  pinMode(echoPin5, INPUT);
  pinMode(trigPin6, OUTPUT);
  pinMode(echoPin6, INPUT);
  pinMode(trigPin7, OUTPUT);
  pinMode(echoPin7, INPUT);
  pinMode(trigPin8, OUTPUT);
  pinMode(echoPin8, INPUT);
  pinMode(trigPin9, OUTPUT);
  pinMode(echoPin9, INPUT);
  pinMode(trigPin10, OUTPUT);
  pinMode(echoPin10, INPUT);
  

}


void loop() {
  int i, j, k, l;
  int counter, counter_1 = 0;
  digitalWrite(LED_1, HIGH);
  for (k = 0; k <= (num_sensor-1); k++) {
    goalArray[k] = 0;
  }
  for (i = 5; i <= (num_sensor); i++) {   //POST
    distance_array[i] = sensor_trigger(i);
    print_distance(i-5);
    if (distance < (maxDistTOP - 3)) {
      counter ++ ;
      goalArray[i] = 1;
     }
  }
  
  for (l = 0; l <= (num_sensor-5); l++) {     //TOP
    distance_array[l] = sensor_trigger(l);
    print_distance(l);
    if (distance < (maxDistPOST - 3)) {
      counter_1 ++ ;
      goalArray[l] = 1;
    }
  }
  
   if (counter_1 == 2) {
    for (j = 0; j <= (num_sensor-5); j++) {
      if (goalArray[j] == 1) {
        if (goalArray[j+1] == 1) {
          if (counter == 2)
            for (j = 0; j <= (num_sensor-7); j++) { 
              if (goalArray[j] == 1) {
                if (goalArray[j+1] == 1) {
                  goal_scored();
                  break;
                }
              }
            }
          }
        }
      }
    }
}
