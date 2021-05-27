/*
 * Created Date: Monday May 24th 2021
 * Author: Jefferson Ding
 * -----
 * Last Modified: Monday May 24th 2021 10:30:33 pm
 * Modified By: Jefferson Ding at <tyding2004@outlook.com>
 * -----
 * Copyright (c) 2021 Jefferson Ding
 */

// Import required libraries
#include <Arduino.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <aREST.h>

// Create aREST instance
aREST rest = aREST();

// WiFi parameters
const char *ssid = "";
const char *password = "";

// The port to listen for incoming TCP connections
#define LISTEN_PORT 80

//Initiate Servo
Servo servo;

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

int trigger(String command);

void setup(void)
{
  // Start Serial
  Serial.begin(115200);

  // Setup Servo
  servo.attach(2); //D4
  servo.write(0);

  // Function to be exposed
  rest.function("trigger", trigger);

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id("schbot");
  rest.set_name("j3of0");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop()
{

  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  while (!client.available())
  {
    delay(1);
  }
  rest.handle(client);
}

// Custom function accessible by the API
int trigger(String command)
{
  int control = command.toInt();
  servo.write(control);
  delay(1000);
  return 1;
}