unsigned int trigPin1 = 2;
unsigned int echoPin1 = 3;

unsigned int trigPin2 = 6;
unsigned int echoPin2 = 7;

unsigned int trigPin3 = 10;
unsigned int echoPin3 = 11;

int LED_1 = 12;
int LED_2 = 13;
int BUZZER = 33;

const int num_sensor = 3;

long duration, distance;
int arg_sensor;
unsigned int distance_array[num_sensor];
unsigned int pinArray[num_sensor] = {trigPin1, trigPin2, trigPin3};
unsigned int echoPinArray[num_sensor] = {echoPin1, echoPin2, echoPin3};
bool goalArray[num_sensor];
int maxDist = 56;


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
int sensor_trigger( int arg_sensor ) {
    digitalWrite(pinArray[arg_sensor], LOW);
    delayMicroseconds(2);
    digitalWrite(pinArray[arg_sensor], HIGH);
    delayMicroseconds(10);
    digitalWrite(pinArray[arg_sensor], LOW);
    duration = pulseIn(echoPinArray[arg_sensor], HIGH);
    distance = (duration/2) / 29.1;
    if (distance >= 500 /*|| distance <= 0*/) {
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
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
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
  digitalWrite(LED_1, HIGH);
  for (k = 0; k <= (num_sensor-1); k++) {
    goalArray[k] = 0;
  }
  for (i = 0; i <= (num_sensor-1); i++) {
    distance_array[i] = sensor_trigger(i);
    if (distance < (maxDist - 3)) {
      counter ++ ;
      goalArray[i] = 1;
     }
   }
   if (counter == 2) {
    for (j = 0; j <= (num_sensor-1); j++) {
      if (goalArray[j] == 1) {
        if (goalArray[j+1] == 1) {
          goal_scored();
          break;
        }
        else {
          break; 
        }
      }
     }
   }
  counter = 0;

}
