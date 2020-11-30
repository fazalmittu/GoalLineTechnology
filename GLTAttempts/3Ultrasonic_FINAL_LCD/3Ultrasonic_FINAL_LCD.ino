int trigPin1 = 2;
int echoPin1 = 3;

int trigPin2 = 4;
int echoPin2 = 5;

int trigPin3 = 6;
int echoPin3 = 7;

int LED = 13;

long duration, distance;
int arg_sensor;
unsigned int distance_array[3];
unsigned int pinArray[3] = {trigPin1, trigPin2, trigPin3};
unsigned int echoPinArray[3] = {echoPin1, echoPin2, echoPin3};
bool goalArray[3];
int maxDist = 28;


void goal_scored() {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    Serial.println ("GOAL");
    Serial.println();
    Serial.println();

}
int sensor_trigger( int arg_sensor ) {
    digitalWrite(pinArray[arg_sensor], LOW);
    delayMicroseconds(2);
    digitalWrite(pinArray[arg_sensor], HIGH);
    delayMicroseconds(10);
    digitalWrite(pinArray[arg_sensor], LOW);
    duration = pulseIn(echoPinArray[arg_sensor], HIGH);
    distance = (duration/2) / 29.1;
    if (distance >= 500 || distance <= 0) {
      Serial.println(" OUT OF RANGE ");
    }
    else {
      Serial.print("Sensor ");
      Serial.print(arg_sensor+1);
      Serial.print(" - ");
      Serial.print(distance_array[arg_sensor]);
      Serial.println(" cm ");
    }
    return distance;
}

void setup() {
  Serial.begin (9600);
  pinMode(LED, OUTPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

}


void loop() {
  int i, j, k;
  int counter = 0;
  for (k = 0; k <= 2; k++) {
    goalArray[k] = 0;
  }
  for (i = 0; i <= 2 ; i++  ) {
    distance_array[i] = sensor_trigger(i);
    if (distance < (maxDist - 3)) {
      counter ++ ;
      goalArray[i] = 1;
     }
   }
   if (counter == 2) {
    for (j = 0; j<=2; j++) {
      if (goalArray[j] == 1) {
        if (goalArray[j+1] == 1) {
          goal_scored();
          break;
        }
        else {
          break; // or you can just set j to a number greater than num_sensors
        }
      }
     }
   }
  counter = 0;
  delay(100);

}
