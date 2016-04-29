/*
 * Sketch that uses an ldr sensor as a light alarm that when triggered will send out an SMS  and 
 * email to the receipient.
 */


#include <TembooAccount.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <Temboo.h>


int ldrPin = 0;  //LDR on Ananlog pin 0
int ldrValue = 0;
int lightThreshold = 500;
WiFiClient client;

void setup() {

  Serial.begin(9600);
  
  // For debugging, wait until the serial console is connected
  delay(4000);
  while(!Serial);

  int wifiStatus = WL_IDLE_STATUS;

  // Determine if the WiFi Shield is present
  Serial.print("\n\nShield:");
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("FAIL");

    // If there's no WiFi shield, stop here
    while(true);
  }

  Serial.println("OK");

  // Try to connect to the local WiFi network
  while(wifiStatus != WL_CONNECTED) {
    Serial.print("WiFi:");
    wifiStatus = WiFi.begin(WIFI_SSID, WPA_PASSWORD);

    if (wifiStatus == WL_CONNECTED) {
      Serial.println("OK");
    } else {
      Serial.println("FAIL");
    }
    delay(5000);
  }

  //Serial.println("Setup complete.\n");
}

void sendSMS()
{
    //Serial.println("Running sendSMS");
    TembooChoreo SendSMSChoreo(client);

    // Invoke the Temboo client
    SendSMSChoreo.begin();

    // Set Temboo account credentials
    SendSMSChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendSMSChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendSMSChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set Choreo inputs
    String AuthTokenValue = "Your token here";
    SendSMSChoreo.addInput("AuthToken", AuthTokenValue);
    String ToValue = "Your phone number here";
    SendSMSChoreo.addInput("To", ToValue);
    String FromValue = "You phone number from Twilio";
    SendSMSChoreo.addInput("From", FromValue);
    String BodyValue = "Safe Alert!!!";
    SendSMSChoreo.addInput("Body", BodyValue);
    String AccountSIDValue = "Account SID Value here";
    SendSMSChoreo.addInput("AccountSID", AccountSIDValue);

    // Identify the Choreo to run
    SendSMSChoreo.setChoreo("/Library/Twilio/SMSMessages/SendSMS");

    // Run the Choreo; when results are available, print them to serial
    SendSMSChoreo.run();

    while(SendSMSChoreo.available()) {
      char c = SendSMSChoreo.read();
      Serial.print(c);
    }
    SendSMSChoreo.close();
}

void sendEmail()
{
    //Serial.println("Running SendEmail");

    TembooChoreo SendEmailChoreo(client);

    // Invoke the Temboo client
    SendEmailChoreo.begin();

    // Set Temboo account credentials
    SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set Choreo inputs
    String FromAddressValue = "safeAlarm@home.com";
    SendEmailChoreo.addInput("FromAddress", FromAddressValue);
    String UsernameValue = "Your sender email here";
    SendEmailChoreo.addInput("Username", UsernameValue);
    String ToAddressValue = "Your recepient email here";
    SendEmailChoreo.addInput("ToAddress", ToAddressValue);
    String SubjectValue = "Safe Alert!!!";
    SendEmailChoreo.addInput("Subject", SubjectValue);
    String PasswordValue = "pword key here";
    SendEmailChoreo.addInput("Password", PasswordValue);
    String MessageBodyValue = "Alert!!! Light Sensor has been tripped";
    SendEmailChoreo.addInput("MessageBody", MessageBodyValue);

    // Identify the Choreo to run
    SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");

    // Run the Choreo; when results are available, print them to serial
    SendEmailChoreo.run();

    while(SendEmailChoreo.available()) {
      char c = SendEmailChoreo.read();
      Serial.print(c);
    }
    SendEmailChoreo.close(); 
}
void loop() {
  
  ldrValue = analogRead(ldrPin);
  Serial.print("Value: ");
  Serial.println(ldrValue);
  if (ldrValue < lightThreshold) {
   //Serial.println("Send msg");
    sendSMS();
    sendEmail();
  }
  delay(2000); //delay value in msec

}
