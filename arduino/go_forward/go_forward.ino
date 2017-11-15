#include <ESP8266WiFi.h>
#include <WiFiClient.h>

/* You can remove the password parameter if you want the AP to be open. */
const char *ssid = "myROBOT";
//const char *password = "passwod";

// Create an instance of the server
// specify the port to listen on as an argument

WiFiServer server(80);
int Speed = 800; // max 1024
int TurnSpeed = 600;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point…");
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("server started");
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);

  digitalWrite(5, 0);
  digitalWrite(4, 0);
  digitalWrite(0, 1);
  digitalWrite(2, 1);

  forward();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  int direction;
  if (req.indexOf("/key/1") != -1)
    direction = 1;
  else if (req.indexOf("/key/2") != -1)
    direction = 2;
  else if (req.indexOf("/key/3") != -1)
    direction = 3;
  else if (req.indexOf("/key/4") != -1)
    direction = 4;
  else if (req.indexOf("/key/5") != -1)
    direction = 5;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  String html;
  switch (direction) {
    case 1:
      Serial.println("Stop");
      stopp();
      html = response("stopping");
      break;
    case 2:
      Serial.println("forward");
      forward();
      html = response("going forward");
      break;
    case 3:
      Serial.println("back");
      backward();
      html = response("going back");
      break;
    case 4:
      Serial.println("right");
      right();
      html = response("turning right");
      break;
    case 5:
      Serial.println("left");
      left();
      html = response("turning left");
      break;
  }

  client.flush();
  // Send the response to the client
  client.print(html);
  delay(1);
  Serial.println("Client disonnected");
  delay(200);
}

String response(String r) {
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nCar is now going";
  s += r;
  s += "</html>\n";
  return s;
}

void stopp(void)
{ analogWrite(5, 0);
  analogWrite(4, 0);
}

void forward(void)
{ analogWrite(5, Speed);
  analogWrite(4, Speed);
  digitalWrite(0, HIGH);
  digitalWrite(2, HIGH);
}

void backward(void)
{ analogWrite(5, Speed);
  analogWrite(4, Speed);
  digitalWrite(0, LOW);
  digitalWrite(2, LOW);
}

void left(void)
{ analogWrite(5, TurnSpeed);
  analogWrite(4, TurnSpeed);
  digitalWrite(0, LOW);
  digitalWrite(2, HIGH);
}

void right(void)
{ analogWrite(5, TurnSpeed);
  analogWrite(4, TurnSpeed);
  digitalWrite(0, HIGH);
  digitalWrite(2, LOW);
}
