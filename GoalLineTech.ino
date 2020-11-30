/*
 * 
 * Fazal Mittu, 2019
 * 
 */


/*
 * 
 * POST
 * 
 */

int POSTtrigPin1 = 2;
int POSTechoPin1 = 3;

int POSTtrigPin2 = 4;
int POSTechoPin2 = 5;

int POSTtrigPin3 = 6;
int POSTechoPin3 = 7;

int POSTtrigPin4 = 8;
int POSTechoPin4 = 9;

/*
 * 
 * TOP
 * 
 */

int TOPtrigPin1 = 40;
int TOPechoPin1 = 41;

int TOPtrigPin2 = 42;
int TOPechoPin2 = 43;

int TOPtrigPin3 = 44;
int TOPechoPin3 = 45;

int TOPtrigPin4 = 46;
int TOPechoPin4 = 47;

int TOPtrigPin5 = 48;
int TOPechoPin5 = 49;

int TOPtrigPin6 = 50;
int TOPechoPin6 = 51;


int LED_1 = 24;
int LED_2 = 25;
int BUZZER = 13;

const int num_sensor = 10;

const int post_sensor = 4;
const int top_sensor = 6;

float POSTdistance_array[post_sensor];
float TOPdistance_array[top_sensor];

int POSTtrigPinArray[post_sensor] = {POSTtrigPin1, POSTtrigPin2, POSTtrigPin3, POSTtrigPin4};
int TOPtrigPinArray[top_sensor] = {TOPtrigPin1, TOPtrigPin2, TOPtrigPin3, TOPtrigPin4, TOPtrigPin5, TOPtrigPin6};

int POSTechoPinArray[post_sensor] = {POSTechoPin1, POSTechoPin2, POSTechoPin3, POSTechoPin4};
int TOPechoPinArray[top_sensor] = {TOPechoPin1, TOPechoPin2, TOPechoPin3, TOPechoPin4, TOPechoPin5, TOPechoPin6};

bool POSTgoalArray[post_sensor];
bool TOPgoalArray[top_sensor];

float maxDistPOST = 56;
float maxDistTOP = 35;


void goal_scored() { .                //This function says what happens when a goal is scored. (LED turns on, Buzzer sounds, and SMS sent)
    digitalWrite(LED_2, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(LED_2, LOW);
    digitalWrite(BUZZER, LOW);
    digitalWrite(31, HIGH);           //This is done to trigger my ESP32 module to send me an SMS when a goal is scored.
    delay(1000);                      
    digitalWrite(31, LOW);
    Serial.println ("GOAL");
    Serial.println();
    Serial.println();
    digitalWrite(LED_1, LOW);
    delay(5000);
    digitalWrite(LED_1, HIGH);

}

int goal_check( int counter_val, int array_choice) {      //This function checks whether a goal was scored or not (if consecutive sensors were triggered)
  Serial.print(array_choice);
  Serial.print(" ");
  Serial.println(counter_val);
  if (array_choice == 1) {                                //This loop is for the TOP ROW of sensors
    if (counter_val == 3 || counter_val == 2) {
      for (int j = 0; j <= (top_sensor-1); j++) {
        if (TOPgoalArray[j] == 1) {                       //This loop is to check whether 2 or 3 consecutive sensors were triggered in the top row
          if (TOPgoalArray[j+1] == 1) {
            return 1;                                     //Returns 1 if 2 or 3 consecutive sensors were triggered
          }
        }
      }  
    }
  }
  if (array_choice == 2) {                                //This loop is for the POST ROW of sensors
    if (counter_val == 3 || counter_val == 2) {
      for (int j = 0; j <= (post_sensor-1); j++) {
        if (POSTgoalArray[j] == 1) {                      //This loop is to check whether 2 or 3 consecutive sensors were triggered in the top row
          if (POSTgoalArray[j+1] == 1) {
            return 1;                                     //Returns 1 if 2 or 3 consecutive sensors were triggered
          }
        }
      }  
    }
  }
  return 0;
  
}

float sensor_trigger( int arg_sensor, int array_num ) {            //This function is to get a distance reading from each sensor
  float duration = 0;
  float distance = 0;
  if (array_num == 1) {                                            //This loop is for the TOP ROW of sensors
    digitalWrite(TOPtrigPinArray[arg_sensor], LOW);
    delayMicroseconds(2);
    digitalWrite(TOPtrigPinArray[arg_sensor], HIGH);
    delayMicroseconds(10);
    digitalWrite(TOPtrigPinArray[arg_sensor], LOW);
    duration = pulseIn(TOPechoPinArray[arg_sensor], HIGH);
    distance = (duration/2) / 29.1;
  }
  if (array_num == 2) {                                            //Returns 1 if 2 or 3 consecutive sensors were triggered
    digitalWrite(POSTtrigPinArray[arg_sensor], LOW);
    delayMicroseconds(2);
    digitalWrite(POSTtrigPinArray[arg_sensor], HIGH);
    delayMicroseconds(10);
    digitalWrite(POSTtrigPinArray[arg_sensor], LOW);
    duration = pulseIn(POSTechoPinArray[arg_sensor], HIGH);        //This line is what measures the distance
    distance = (duration/2) / 29.1;                                //This line converts the measurement to centimeters
  }
  return distance;                                                 //This returns the distance that the sensor picked up
}

void setup() {
  Serial.begin (9600);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(POSTtrigPin1, OUTPUT);
  pinMode(POSTechoPin1, INPUT);
  pinMode(POSTtrigPin2, OUTPUT);
  pinMode(POSTechoPin2, INPUT);
  pinMode(POSTtrigPin3, OUTPUT);
  pinMode(POSTechoPin3, INPUT);
  pinMode(POSTtrigPin4, OUTPUT);
  pinMode(POSTechoPin4, INPUT);
  pinMode(TOPtrigPin1, OUTPUT);
  pinMode(TOPechoPin1, INPUT);
  pinMode(TOPtrigPin2, OUTPUT);
  pinMode(TOPechoPin2, INPUT);
  pinMode(TOPtrigPin3, OUTPUT);
  pinMode(TOPechoPin3, INPUT);
  pinMode(TOPtrigPin4, OUTPUT);
  pinMode(TOPechoPin4, INPUT);
  pinMode(TOPtrigPin5, OUTPUT);
  pinMode(TOPechoPin5, INPUT);
  pinMode(TOPtrigPin6, OUTPUT);
  pinMode(TOPechoPin6, INPUT);
  pinMode(31, OUTPUT);
  for (int i = 0; i < post_sensor; i++) {
    POSTdistance_array[i] = maxDistPOST;
  }

  for (int i = 0; i < top_sensor; i++) {
    TOPdistance_array[i] = maxDistTOP;
  }

}


void loop() {
  int counter = 0;
  int counter_1 = 0;
  int sensor_counter = 0;
  digitalWrite(LED_1, HIGH);
  for (int k = 0; k <= (top_sensor-1); k++) {
    TOPgoalArray[k] = 0;
  }
  for (int k = 0; k <= (post_sensor-1); k++) {
    POSTgoalArray[k] = 0;
  }
  for (int j = 0; j <= (top_sensor-1); j++) {
    TOPdistance_array[j] = sensor_trigger(j, 1);
    if (TOPdistance_array[j] < maxDistTOP) {
      counter ++ ;
      TOPgoalArray[j] = 1;     
    }
  }
  
  sensor_counter = goal_check(counter, 1);
  
  if (sensor_counter == 1) {
    sensor_counter = 0;
    for (int h = 0; h <= (post_sensor-1); h++) {
      POSTdistance_array[h] = sensor_trigger(h, 2);
      if (POSTdistance_array[h] < maxDistPOST) {
        counter_1 ++ ;
        POSTgoalArray[h] = 1;
      }
      sensor_counter = goal_check(counter_1, 2);
    }  
  }

  if (sensor_counter == 1) {
    goal_scored();
  }
  

  
  

}
