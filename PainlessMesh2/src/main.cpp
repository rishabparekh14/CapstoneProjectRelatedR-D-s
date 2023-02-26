#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;


String mainMsg;
// You will need an internet connection for this...
const char* ssid = "Apt247";
const char* password = "kukukamagic";

// The endpoint is broker address
const char* awsEndpoint = "a2qx0h90p3xwh8-ats.iot.us-west-2.amazonaws.com";

// This is the arn
const char* topic = "arn:aws:iot:us-west-2:068820307922:thing/CapstoneProjectThing";

// Right now we are also subscribing to our own publication... 
// You can create another topic on AWS to subscribe to if you wish...
const char* subscribeTopic = topic;

// Update the two certificate strings below. Paste in the text of your AWS 
// device certificate and private key. Add a quote character at the start
// of each line and a backslash, n, quote, space, backslash at the end 
// of each line:

// xxxxxxxxxx-certificate.pem.crt
const char* certificate_pem_crt = \

"-----BEGIN CERTIFICATE-----\n" \
"MIIDWTCCAkGgAwIBAgIUOYl7VjzHR9Nnlzy1ZAngwfiLTtkwDQYJKoZIhvcNAQEL\n" \
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n" \
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIzMDIyNDA2MTU1\n" \
"MVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n" \
"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAM9yCjD0yQB7WAzpkbou\n" \
"4L7RyGuokzTnQaTvxth2TufAIOT4XB9ojYhHOpdHWVDDfLgVHlLBqAJJD6tgaV6A\n" \
"0gCsiVIhOuWqDx7wXQoAcsYO/w5bAT/fkkPzFmlf34BMABto6X1EHQUdegrxh3Rf\n" \
"TddbUuzLoJ4CrnjFP0+hzNk0PZzUupyzYQf3ZKlgSCAjnFO71DpnKqUza9UTLHjs\n" \
"1ddD3pX8CXE3F4tU8ToXKMVpSAl0CTSShzTzEQTLSmM05DSERj2Hl4Q6+ALIdOWK\n" \
"LM1AhGMAOWWRCf5GSTOK3hPrSMw9P3bG2E33SojdcEIV15YuoQdAGCM1fokHLTQV\n" \
"tdsCAwEAAaNgMF4wHwYDVR0jBBgwFoAULoi2p8gLiBfh/dvMKu9/5BuisnwwHQYD\n" \
"VR0OBBYEFFUM4IWI+LVgOK3mXuJRj6oar735MAwGA1UdEwEB/wQCMAAwDgYDVR0P\n" \
"AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQB0uGTNETBGsNZYRqjK+2oKtB9U\n" \
"yNcgJnb1jAw+J5nw/wOoL4jGfOMRk6F+IC+p2tWJz+dpZHJYgV34b7ztTYgctQYM\n" \
"1TUk/N+AyHlB0QILS4zvvk1Ws3Msus+dlZr/ubHZdWzZxUN811blVZfJFmxLjqoM\n" \
"6xxji2uPJDN9mF91cW+Od3AIS/U3O5TFhQB5sKjoRLqRpTdHIf2HpDO527fvRC4F\n" \
"xVuxubBsTckFcAHqm8HHNDEh1z0X4qs6kzv84kBlu/N+OHInuF4VfsCUQc0P2YXK\n" \
"iWwGeue+7Eha1qjijko/L3Zck03lXBI/XSet1jadteKNqBjS+twunUCRyRWa\n" \
"-----END CERTIFICATE-----\n";

// xxxxxxxxxx-private.pem.key
const char* private_pem_key = \

"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEpAIBAAKCAQEAz3IKMPTJAHtYDOmRui7gvtHIa6iTNOdBpO/G2HZO58Ag5Phc\n" \
"H2iNiEc6l0dZUMN8uBUeUsGoAkkPq2BpXoDSAKyJUiE65aoPHvBdCgByxg7/DlsB\n" \
"P9+SQ/MWaV/fgEwAG2jpfUQdBR16CvGHdF9N11tS7MugngKueMU/T6HM2TQ9nNS6\n" \
"nLNhB/dkqWBIICOcU7vUOmcqpTNr1RMseOzV10PelfwJcTcXi1TxOhcoxWlICXQJ\n" \
"NJKHNPMRBMtKYzTkNIRGPYeXhDr4Ash05YoszUCEYwA5ZZEJ/kZJM4reE+tIzD0/\n" \
"dsbYTfdKiN1wQhXXli6hB0AYIzV+iQctNBW12wIDAQABAoIBAQCkKoOWKqHHF9We\n" \
"olkJFoAVgr8xM78LZte0R2z9cOMQ1CM0ELgWww7+giAECBrOp2XT6CKPK2JiLJ+R\n" \
"6EJ630dR8TjbHiEWD3bX7TAz8hbiVovSOILMoYmG7KEX0x900LBlAKmG0JiGq8Uh\n" \
"U/L2TVW00DjX3G3Hnieku7RWuJz8ZcSTtDXchdAQF/JesZO0lD0L8uhGo1HSdTlS\n" \
"wFcQrHFPuF8imo9vMe7hEB7P/uibcGH4arHxuTgTdNb0llnJe0HjYcKMwZ9sgd7t\n" \
"z5a+KGvyGK/MqpR7UhEJ7fNM9Nbag23HZsTrLQELzD40ku9dP+ab0tSyIuOV10Ij\n" \
"dcXTGJ6BAoGBAP9OwptGY3UjEVLgxPISo0OdthYfiMIKS+mj4GMKdN6ci26MUZg/\n" \
"0OQJOfVa6B2X3yvyYYrSuzLlR0niaj9ge0nhzwk59rlLOpmNKajp48S6EJxMYcG8\n" \
"1DIXQBEIlw80pB1VO33NdciRB2FIAlTw4ThPG1oAcjTfB+G3VTl5aHyzAoGBANAC\n" \
"DX6eKXojzYkLNBA/S72aBpcpimBIvBe7on03AqMtuHuX0Fcpanmz8a4enNEomT8S\n" \
"jC8HeiskSf5zJSXc/TqcdA3VThLKT3xadb8U4wywAprIvLWgQUOlKdnlVaPpnNFM\n" \
"z7Q51TTkZiDE7+/XErj3Q18CvTIUnudHpVld40Y5AoGBAJv1Hsf/jY3mqBR6nZRK\n" \
"clVBqBOHm+D8rTbCFrYpkw7RjhMvwEBC63YBdNg+egBp37hWzeMhq50BljS6DLE+\n" \
"XNHbEL/QGrlCDT/zjXlTtwFbr/3qYuUYVn9q2IYPO0fcnkwU3syQj9y/S0TP2izN\n" \
"IKdnG4w1LY7Cl2v4tCTx1zQlAoGAIgST7Gc1ZVJBpO3+H8IaQvHTrhkPFlwO1trK\n" \
"4iuC9OjfSOc1HWwai7jQB6feAZ7gIR6Jao+QfUm8CC8Qnjab+1/m4vRFhnEruqyY\n" \
"oR1rXIHUizfVFOcWhVeXlpkY/muQR/KF7XWf1rybrzRGATVtTG6zx/AMvQn+Sti+\n" \
"w/TE3JECgYA9Pp2e5LcC/2n8v8+VAmyjxmY83xet2m47ax21VOZGFtF85JPBS9mM\n" \
"LJ/SPC0ZahrTK65Lpy5/bB4tR1ouy4OaL3TaaahBRUAHB4P/g5bfbLcjPLMZ2pfU\n" \
"Tjxu/A/HaSUpdv2Yihi2pm2wv65SIeuy/DaXK6pSEAQiRUfWkjinog==\n" \
"-----END RSA PRIVATE KEY-----\n";

// This key should be fine as is. It is just the root certificate.
const char* rootCA = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
"rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
"-----END CERTIFICATE-----\n";

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); 
unsigned long lastPublish;
int msgCount;
void pubSubCheckConnect();

void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = "Hello from node 2";
 // msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

void receivedCallback( uint32_t from, String &msg ) {
  mainMsg = msg.c_str();
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  pubSubCheckConnect();
  boolean rc = pubSubClient.publish(topic, msg.c_str());
    Serial.print("Published, rc="); 
    Serial.print( (rc ? "OK: " : "FAILED: ") );
    Serial.println(msg);
    lastPublish = millis();
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
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  delay(50); 
  Serial.println();
  Serial.println("ESP32 AWS IoT Example");
  Serial.printf("SDK version: %s\n", ESP.getSdkVersion());

  Serial.print("Connecting to "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); 
  Serial.println(WiFi.localIP());

  wiFiClient.setCACert(rootCA);
  wiFiClient.setCertificate(certificate_pem_crt);
  wiFiClient.setPrivateKey(private_pem_key);
  
}



void loop() {
    mesh.update();
  //pubSubCheckConnect();
  // pubSubCheckConnect();
  
  if (millis() - lastPublish > 10000) {
    String msg = String(mainMsg);
    boolean rc = pubSubClient.publish(topic, msg.c_str());
    Serial.print("Published, rc="); 
    Serial.print( (rc ? "OK: " : "FAILED: ") );
    Serial.println(msg);
    lastPublish = millis();
  }
}

void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); 
  Serial.print(topic); 
  Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
}

void pubSubCheckConnect() {
  if (!pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); 
    Serial.print(awsEndpoint);
    while (!pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect("ESPthingXXXX");
      delay(1000);
    }
    Serial.println(" connected");
    pubSubClient.subscribe(subscribeTopic);
  }
  pubSubClient.loop();
}