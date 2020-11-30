/*unsigned int trigPin1 = 40;
unsigned int echoPin1 = 41;

unsigned int trigPin2 = 42;
unsigned int echoPin2 = 43;

unsigned int trigPin3 = 44;
unsigned int echoPin3 = 45;

unsigned int trigPin4 = 46;
unsigned int echoPin4 = 47;

*/

unsigned int trigPin1 = 2;
unsigned int echoPin1 = 3;

unsigned int trigPin2 = 4;
unsigned int echoPin2 = 5;

unsigned int trigPin3 = 6;
unsigned int echoPin3 = 7;

unsigned int trigPin4 = 8;
unsigned int echoPin4 = 9;

int LED_1 = 24;
int LED_2 = 25;
int BUZZER = 13;

const int num_sensor = 4;

long duration, distance;
int arg_sensor;
unsigned int distance_array[num_sensor];
unsigned int pinArray[num_sensor] = {trigPin1, trigPin2, trigPin3, trigPin4};
unsigned int echoPinArray[num_sensor] = {echoPin1, echoPin2, echoPin3, echoPin4};
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
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);

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
