#include <WiFi.h>
#include <WiFiMulti.h>

#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Need to be set up in later phase
#define WIFI_SSID "..." //your username
#define WIFI_PASSWORD "..." //your password

// it use real time database
#define API_KEY "AIzaSyC9RgtCyHE2jiccavq_GrjHQkwi8YHqS3U"
#define DATABASE_URL "https://flutter-firebase-e4870-default-rtdb.asia-southeast1.firebasedatabase.app"

#define RELAY1 26
#define RELAY2 25
#define RELAY3 32
#define RELAY4 33

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

WiFiMulti WiFiMulti;

bool signupOK = false;
unsigned int din01 = 0;
unsigned int din02 = 0;
unsigned long sendDataPrevMillis = 0;

void setup() {
  
  // put your setup code here, to run once:
  //Serial.begin(11520);
  //WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // We start by connecting to a WiFi network
  //WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  WiFiMulti.addAP("username", "password"); //your username and password

  Serial.println();
  Serial.println();
  Serial.print("Waiting for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);


  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if(Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("signUp OK");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
 
 //pin mode harus di paling akhir dari fungsi setup
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // update to firebase
  Serial.println("test-loop");
  if(Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

  }
  // read from firebase
  if(Firebase.RTDB.getBool(&fbdo, "/relay-10/Ch01")) { //change your relay
    Serial.println("test-baca-firebase");
    if(fbdo.dataType() == "int") {
      din01 = fbdo.intData();
      Serial.println("Successful Read from " + fbdo.dataPath() + ": " + din01 + " (" + fbdo.dataType() + ")");
      if(din01 == 1) {
        digitalWrite(RELAY1, HIGH);  // turn the LED on (HIGH is the voltage level)
        //delay(1000);
        Serial.println("Nilai din01 = 1");
      } else {
        digitalWrite(RELAY1, LOW);
        //delay(1000);
        Serial.println("Nilai din01 = 0");
      }
    }
  }
}
