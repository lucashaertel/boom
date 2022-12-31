// Load Wi-Fi library
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Network credentials Here
const char* ssid     = "BoomBoom";
const char* password = "BoomBoom0211";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//variables to store the current LED states
String stateRelaisPin1 = "off";
String stateRelaisPin2 = "off";
String stateRelaisPin3 = "off";
String stateRelaisPin4 = "off";
String stateRelaisPin5 = "off";
String stateRelaisPin6 = "off";
String stateRelaisPin7 = "off";
String stateRelaisPin8 = "off";
//Output variable to GPIO pins
const int relaisPin1 = 16;
const int relaisPin2 = 17;
const int relaisPin3 = 17;
const int relaisPin4 = 17;
const int relaisPin5 = 17;
const int relaisPin6 = 17;
const int relaisPin7 = 17;
const int relaisPin8 = 17;
// oled pins
const int sclPin = 22;
const int sdaPin = 21;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds
const long timeoutTime = 2000;

// Define oled display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // -1 as reset pin, becuase oled has none

void setup() {
  Serial.begin(115200);
  // relais pinmodes and defaults
  setRelaisPinmodeAndDefault();
  WiFi.softAP(ssid, password);
  // Print IP address and start web server
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}

void loop() {
  delay(2000);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  displayText();

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            evaluateHeaderData();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: yellowgreen; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 32px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: gray;}</style></head>");

            client.println("<body><h1>ESP32 Web Server</h1>");
            client.println("<p>Control LED State</p>");

            printButtons(client);
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void displayText() {
  //display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 20);
  // Display static text
  display.println("Hello, world!");
  display.display();
}

void setRelaisPinmodeAndDefault() {
  pinMode(relaisPin1, OUTPUT);
  digitalWrite(relaisPin1, 0);
  pinMode(relaisPin2, OUTPUT);
  digitalWrite(relaisPin2, 0);
  pinMode(relaisPin3, OUTPUT);
  digitalWrite(relaisPin3, 0);
  pinMode(relaisPin4, OUTPUT);
  digitalWrite(relaisPin4, 0);
  pinMode(relaisPin5, OUTPUT);
  digitalWrite(relaisPin5, 0);
  pinMode(relaisPin6, OUTPUT);
  digitalWrite(relaisPin6, 0);
  pinMode(relaisPin7, OUTPUT);
  digitalWrite(relaisPin7, 0);
  pinMode(relaisPin8, OUTPUT);
  digitalWrite(relaisPin8, 0);
}

void evaluateHeaderData() {
  if (header.indexOf("GET /1/on") >= 0) {
    stateRelaisPin1 = "on";
    digitalWrite(relaisPin1, HIGH);
  } else if (header.indexOf("GET /1/off") >= 0) {
    stateRelaisPin1 = "off";
    digitalWrite(relaisPin1, LOW);
  }
  if (header.indexOf("GET /2/on") >= 0) {
    stateRelaisPin2 = "on";
    digitalWrite(relaisPin2, HIGH);
  } else if (header.indexOf("GET /2/off") >= 0) {
    stateRelaisPin2 = "off";
    digitalWrite(relaisPin2, LOW);
  }
  if (header.indexOf("GET /3/on") >= 0) {
    stateRelaisPin3 = "on";
    digitalWrite(relaisPin3, HIGH);
  } else if (header.indexOf("GET /3/off") >= 0) {
    stateRelaisPin3 = "off";
    digitalWrite(relaisPin3, LOW);
  }
  if (header.indexOf("GET /4/on") >= 0) {
    stateRelaisPin4 = "on";
    digitalWrite(relaisPin4, HIGH);
  } else if (header.indexOf("GET /4/off") >= 0) {
    stateRelaisPin4 = "off";
    digitalWrite(relaisPin4, LOW);
  }
  if (header.indexOf("GET /5/on") >= 0) {
    stateRelaisPin5 = "on";
    digitalWrite(relaisPin5, HIGH);
  } else if (header.indexOf("GET /5/off") >= 0) {
    stateRelaisPin5 = "off";
    digitalWrite(relaisPin5, LOW);
  }
  if (header.indexOf("GET /6/on") >= 0) {
    stateRelaisPin6 = "on";
    digitalWrite(relaisPin6, HIGH);
  } else if (header.indexOf("GET /6/off") >= 0) {
    stateRelaisPin6 = "off";
    digitalWrite(relaisPin6, LOW);
  }
  if (header.indexOf("GET /7/on") >= 0) {
    stateRelaisPin7 = "on";
    digitalWrite(relaisPin7, HIGH);
  } else if (header.indexOf("GET /7/off") >= 0) {
    stateRelaisPin7 = "off";
    digitalWrite(relaisPin7, LOW);
  }
  if (header.indexOf("GET /8/on") >= 0) {
    stateRelaisPin8 = "on";
    digitalWrite(relaisPin8, HIGH);
  } else if (header.indexOf("GET /8/off") >= 0) {
    stateRelaisPin8 = "off";
    digitalWrite(relaisPin8, LOW);
  }
}

void printButtons(WiFiClient client) {
  if (stateRelaisPin1 == "off") {
    client.println("<p><a href=\"/1/on\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/1/off\"><button class=\"button button2\">OFF</button></a></p>");
  }
  if (stateRelaisPin2 == "off") {
    client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
  }
  if (stateRelaisPin3 == "off") {
    client.println("<p><a href=\"/3/on\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/3/off\"><button class=\"button button2\">OFF</button></a></p>");
  }
  if (stateRelaisPin4 == "off") {
    client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
  }
  if (stateRelaisPin5 == "off") {
    client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
  }
  if (stateRelaisPin6 == "off") {
    client.println("<p><a href=\"/6/on\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/6/off\"><button class=\"button button2\">OFF</button></a></p>");
  }
  if (stateRelaisPin7 == "off") {
    client.println("<p><a href=\"/7/on\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/7/off\"><button class=\"button button2\">OFF</button></a></p>");
  }
  if (stateRelaisPin8 == "off") {
    client.println("<p><a href=\"/8/on\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/8/off\"><button class=\"button button2\">OFF</button></a></p>");
  }
}
