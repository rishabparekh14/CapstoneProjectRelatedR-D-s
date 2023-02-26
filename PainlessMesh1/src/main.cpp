//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
# define SensorPin1 36
# define SensorPin2 37
# define SensorPin3 38
# define SensorPin4 39
# define trigPin 5
# define echoPin 18
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const int AirValue = 3550;   //you need to replace this value with Value_1
const int WaterValue = 1000;  //you need to replace this value with Value_2
//const int SensorPin = 36;
int soilMoistureValue1 = 0;
int soilmoisturepercent1=0;

int soilMoistureValue2 = 0;
int soilmoisturepercent2=0;

int soilMoistureValue3 = 0;
int soilmoisturepercent3=0;

int soilMoistureValue4 = 0;
int soilmoisturepercent4=0;


long duration;
float distanceCm;
float distanceInch;


Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage1() ; // Prototype so PlatformIO doesn't complain
void sendMessage2();
void sendMessage3();
void sendMessage4();
Task taskSendMessage1( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage1 );
Task taskSendMessage2( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage2 );
Task taskSendMessage3( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage3 );
Task taskSendMessage4( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage4 );

void sendMessage1() {
  String msg = "Soil Moisture Sensor 1:";
  msg += String(soilmoisturepercent1);
  mesh.sendBroadcast( msg );
  taskSendMessage1.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

void sendMessage2() {
  String msg = "Soil Moisture Sensor 2:";
  msg += String(soilmoisturepercent2);
  mesh.sendBroadcast( msg );
  taskSendMessage2.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}
void sendMessage3() {
  String msg = "Soil Moisture Sensor 3:";
  msg += String(soilmoisturepercent3);
  mesh.sendBroadcast( msg );
  taskSendMessage3.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}
void sendMessage4() {
  String msg = "Soil Moisture Sensor 4:";
  msg += String(soilmoisturepercent4);
  mesh.sendBroadcast( msg );
  taskSendMessage4.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage1 );
  userScheduler.addTask( taskSendMessage2 );
  userScheduler.addTask( taskSendMessage3 );
  userScheduler.addTask( taskSendMessage4 );
  taskSendMessage1.enable();
  taskSendMessage2.enable();
  taskSendMessage3.enable();
  taskSendMessage4.enable();
}

void loop() {
  // it will run the user scheduler as well
   soilMoistureValue1 = analogRead(SensorPin1);  //put Sensor insert into soil
  Serial.print("SoilMositureValue1:");
  Serial.println(soilMoistureValue1);
  soilmoisturepercent1 = map(soilMoistureValue1, AirValue, WaterValue, 0, 100);
  Serial.println("SoilMoisturePercent1:");
  Serial.println(soilmoisturepercent1);
  delay(100);

  soilMoistureValue2 = analogRead(SensorPin2);  //put Sensor insert into soil
  Serial.print("SoilMositureValue2:");
  Serial.println(soilMoistureValue2);
  soilmoisturepercent2 = map(soilMoistureValue2, AirValue, WaterValue, 0, 100);
  Serial.println("SoilMoisturePercent2:");
  Serial.println(soilmoisturepercent2);
  delay(100);

  soilMoistureValue3 = analogRead(SensorPin3);  //put Sensor insert into soil
  Serial.print("SoilMositureValue3:");
  Serial.println(soilMoistureValue3);
  soilmoisturepercent3 = map(soilMoistureValue3, AirValue, WaterValue, 0, 100);
  Serial.println("SoilMoisturePercent3:");
  Serial.println(soilmoisturepercent3);
  delay(100);

  soilMoistureValue4 = analogRead(SensorPin4);  //put Sensor insert into soil
  Serial.print("SoilMositureValue4:");
  Serial.println(soilMoistureValue4);
  soilmoisturepercent4 = map(soilMoistureValue4, AirValue, WaterValue, 0, 100);
  Serial.println("SoilMoisturePercent4:");
  Serial.println(soilmoisturepercent4);
  delay(100);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
  
  


  mesh.update();
}