/* ______________________________________Libraries______________________________________ */
//For Firebase & WiFi Connectivity
#include <ESP8266WiFi.h> 
#include <FirebaseESP8266.h>
#include <FirebaseESP8266HTTPClient.h>
#include <FirebaseFS.h>
#include <FirebaseJson.h>
#include <deprecated.h>

//For Firebase & WiFi Connectivity
#define FIREBASE_HOST "*********"  
#define FIREBASE_AUTH "*********"  
#define WIFI_SSID "*********"
#define WIFI_PASSWORD "*********"
#define AXpin D3//Red
#define AYpin D4//Red
#define BXpin D5//Green
#define BYpin D6//Green
#define CXpin D7//Blue
#define CYpin D8//Blue

FirebaseData firebaseData;
FirebaseJson json;
double A_lat, A_long, B_lat, B_long, C_lat, C_long; //Users, ie, Current Location
String x_lat, x_long, y_lat, y_long; //Saved Locations in String
double X_lat, X_long, Y_lat, Y_long; //Saved Locations in String
int AX, AY, AT, BX, BY, BT, CX, CY, CT;

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

  pinMode(AXpin, OUTPUT);
  pinMode(AYpin, OUTPUT);
  pinMode(BXpin, OUTPUT);
  pinMode(BYpin, OUTPUT);
  pinMode(CXpin, OUTPUT);
  pinMode(CYpin, OUTPUT);
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
  //aadifernandes9 Latitude
  if(Firebase.getDouble(firebaseData, "/Current Location/aadifernandes9/Current Latitude")) {
      if (firebaseData.dataType() == "double") {
        C_lat = firebaseData.doubleData();
        //Serial.println(B_lat,7);
      }
  } 
  else {
      Serial.println(firebaseData.errorReason());
  } 
  //aadifernandes9 Longitude
  if(Firebase.getDouble(firebaseData, "/Current Location/aadifernandes9/Current Longitude")) {
      if (firebaseData.dataType() == "double") {
        C_long = firebaseData.doubleData();
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
      AY = 0;
      AT = 0;
    }
  }
  else if(int(A_lat*10000) == int(Y_lat*10000)){
    if(int(A_long*10000) == int(Y_long*10000)){
      Serial.println("aaryaapg is at VESIT");
      AY = 1;
      AX = 0;
      AT = 0;
    }
  }
  else {
    Serial.println("aaryaapg is travelling");
    AT = 1;
    AX = 0;
    AY = 0;
    
  }

  //Compare B
  if(int(B_lat*10000) == int(X_lat*10000)){
    if(int(B_long*10000) == int(X_long*10000)){
      Serial.println("aashree2019 is at AaryaaHome");
      BX = 1;
      BY = 0;
      BT = 0;
    }
  }
  else if(int(B_lat*10000) == int(Y_lat*10000)){
    if(int(B_long*10000) == int(Y_long*10000)){
      Serial.println("aashree2019 is at VESIT");
      BY = 1;
      BX = 0;
      BT = 0;
    }
  }
  else {
    Serial.println("aashree2019 is travelling");
    BT = 1;
    BX = 0;
    BY = 0;
  }
  //Compare C
  if(int(C_lat*10000) == int(X_lat*10000)){
    if(int(C_long*10000) == int(X_long*10000)){
      Serial.println("aadifernandes9 is at AaryaaHome");
      CX = 1;
      CY = 0;
      CT = 0;
    }
  }
  else if(int(C_lat*10000) == int(Y_lat*10000)){
    if(int(C_long*10000) == int(Y_long*10000)){
      Serial.println("aadifernandes9 is at VESIT");
      CY = 1;
      CX = 0;
      CT = 0;
    }
  }
  else {
    Serial.println("aadifernandes9 is travelling");
    CT = 1;
    CX = 0;
    CY = 0;
  }
}

void glowLED() {
  //For User A (aaryaapg)
  if(AX){ 
    digitalWrite(AXpin, HIGH);   
    digitalWrite(AYpin, LOW);
  }
  else if(AY){
    digitalWrite(AXpin, LOW);   
    digitalWrite(AYpin, HIGH);
  }
  else{ //Travelling
    digitalWrite(AXpin, LOW);   
    digitalWrite(AYpin, LOW);
  }

  //For User B (aashree2019)
  if(BX){ 
    digitalWrite(BXpin, HIGH);   
    digitalWrite(BYpin, LOW);
  }
  else if(BY){
    digitalWrite(BXpin, LOW);   
    digitalWrite(BYpin, HIGH);
  }
  else{ //Travelling
    digitalWrite(BXpin, LOW);   
    digitalWrite(BYpin, LOW);
  }

  //For User C (aadifernandes9)
  if(CX){ 
    digitalWrite(CXpin, HIGH);   
    digitalWrite(CYpin, LOW);
  }
  else if(CY){
    digitalWrite(CXpin, LOW);   
    digitalWrite(CYpin, HIGH);
  }
  else{ //Travelling
    digitalWrite(CXpin, LOW);   
    digitalWrite(CYpin, LOW);
  }
}
//https://github.com/mobizt/Firebase-ESP8266/blob/master/README.md
//http://www.lib4dev.in/info/mobizt/Firebase-ESP32/144101498
  
