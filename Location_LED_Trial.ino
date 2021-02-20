/* ______________________________________Libraries______________________________________ */
//For Firebase & WiFi Connectivity
#include <ESP8266WiFi.h> 
#include <FirebaseESP8266.h>
#include <FirebaseESP8266HTTPClient.h>
#include <FirebaseFS.h>
#include <FirebaseJson.h>
#include <deprecated.h>

//For Firebase & WiFi Connectivity
#define FIREBASE_HOST "location-database-25b6d-default-rtdb.firebaseio.com"  
#define FIREBASE_AUTH "XQSMRqCWI3uetqyC4WMgtq08a0VKYZmZ60MVFiI6"  
#define WIFI_SSID "Padhyegurjar"
#define WIFI_PASSWORD "mncb8tbbt8"
#define A_RedPin D1
#define A_GreenPin D2
#define A_BluePin D3


FirebaseData firebaseData;
FirebaseJson json;
double A_lat, A_long, B_lat, B_long; //Users, ie, Current Location
String x_lat, x_long, y_lat, y_long; //Saved Locations in String
double X_lat, X_long, Y_lat, Y_long; //Saved Locations in String
int AX, AY, AT, BX, BY, BT;
/* ______________________________________Setup______________________________________ */
void setup() {
  Serial.begin(9600);
  //Connecting to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  
  Serial.print("connecting");  
  while (WiFi.status() != WL_CONNECTED) {  
    Serial.print(".");   
    delay(500);  
  }  
  Serial.println();  
  Serial.print("Connected: ");  
  Serial.println(WiFi.localIP());  

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  pinMode(A_RedPin, OUTPUT);
  pinMode(A_GreenPin, OUTPUT);
  pinMode(A_BluePin, OUTPUT);
}

/* ______________________________________Loop______________________________________ */
void loop() {
  getCurrentLocations();
  getSavedLocations();
  compareCurrentSaved();
  glowLED();
}

/* ______________________________________Functions______________________________________ */
void getCurrentLocations(){
  //aaryaapg Latitude
  if(Firebase.getDouble(firebaseData, "/Current Location/aaryaapg/Current Latitude")) {
      if (firebaseData.dataType() == "double") {
        A_lat = firebaseData.doubleData();
        //Serial.println(A_lat,7);
      }
  } 
  else {
      Serial.println(firebaseData.errorReason());
  }
  //aaryaapg Longitude
  if(Firebase.getDouble(firebaseData, "/Current Location/aaryaapg/Current Longitude")) {
      if (firebaseData.dataType() == "double") {
        A_long = firebaseData.doubleData();
        //Serial.println(A_long,7);
      }
  } 
  else {
      Serial.println(firebaseData.errorReason());
  }
  //aashree2019 Latitude
  if(Firebase.getDouble(firebaseData, "/Current Location/aashree2019/Current Latitude")) {
      if (firebaseData.dataType() == "double") {
        B_lat = firebaseData.doubleData();
        //Serial.println(B_lat,7);
      }
  } 
  else {
      Serial.println(firebaseData.errorReason());
  } 
  //aashree2019 Longitude
  if(Firebase.getDouble(firebaseData, "/Current Location/aashree2019/Current Longitude")) {
      if (firebaseData.dataType() == "double") {
        B_long = firebaseData.doubleData();
        //Serial.println(B_long,7);
      }
  } 
  else {
      Serial.println(firebaseData.errorReason());
  }
}

void getSavedLocations() {
  //AaryaaHome Latitude
  if(Firebase.getString(firebaseData, "/Saved Locations/AaryaaHome/LATITUDE")) {
      if (firebaseData.dataType() == "string") {
        x_lat = firebaseData.stringData();
        X_lat = x_lat.toDouble();
        //Serial.println(X_lat,7);
      }
  } 
  else {
      Serial.println(firebaseData.errorReason());
  }
  //AaryaaHome Longitude
  if(Firebase.getString(firebaseData, "/Saved Locations/AaryaaHome/LONGITUDE")) {
      if (firebaseData.dataType() == "string") {
        x_long = firebaseData.stringData();
        X_long = x_long.toDouble();
        //Serial.println(X_long,7);
      }
  } 
  else {
      Serial.println(firebaseData.errorReason());
  }
  //VESIT Latitude
  if(Firebase.getString(firebaseData, "/Saved Locations/VESIT/LATITUDE")) {
      if (firebaseData.dataType() == "string") {
        y_lat = firebaseData.stringData();
        Y_lat = y_lat.toDouble();
        //Serial.println(Y_lat,7);
      }
  } 
  else {
      Serial.println(firebaseData.errorReason());
  }
  //VESIT Longitude
  if(Firebase.getString(firebaseData, "/Saved Locations/VESIT/LONGITUDE")) {
      if (firebaseData.dataType() == "string") {
        y_long = firebaseData.stringData();
        Y_long = y_long.toDouble();
        //Serial.println(Y_long,7);
      }
  } 
  else {
      Serial.println(firebaseData.errorReason());
  }
}

void compareCurrentSaved() {
  //Compare A
  if(int(A_lat*10000) == int(X_lat*10000)){
    if(int(A_long*10000) == int(X_long*10000)){
      Serial.println("aaryaapg is at AaryaaHome");
      AX = 1;
    }
  }
  else if(int(A_lat*10000) == int(Y_lat*10000)){
    if(int(A_long*10000) == int(Y_long*10000)){
      Serial.println("aaryaapg is at VESIT");
      AY = 1;
    }
  }
  else {
    Serial.println("aaryaapg is travelling");
    AT = 1;
  }

  //Compare B
  if(int(B_lat*10000) == int(X_lat*10000)){
    if(int(B_long*10000) == int(X_long*10000)){
      Serial.println("aashree2019 is at AaryaaHome");
      BX = 1;
    }
  }
  else if(int(B_lat*10000) == int(Y_lat*10000)){
    if(int(B_long*10000) == int(Y_long*10000)){
      Serial.println("aashree2019 is at VESIT");
      BY = 1;
    }
  }
  else {
    Serial.println("aashree2019 is travelling");
    BT = 1;
  }
}

void glowLED(){
  if(AX) { 
    analogWrite(A_RedPin, 0);
    analogWrite(A_GreenPin, 10);
    analogWrite(A_BluePin, 10);
  }
  else if (AY) { analogWrite(A_GreenPin, 100); }
  else { analogWrite(A_BluePin, 100); }
}
//https://github.com/mobizt/Firebase-ESP8266/blob/master/README.md
//http://www.lib4dev.in/info/mobizt/Firebase-ESP32/144101498
  
