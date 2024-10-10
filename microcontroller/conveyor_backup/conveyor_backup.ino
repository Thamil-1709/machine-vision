
// Load Wi-Fi library
#include <WiFi.h>
#include <vector>

// Replace with your network credentials

// kumar - 8508794322
// Airtel_thir_7804 - air62949
const char* ssid = "Airtel_thir_7804";
const char* password = "air62949";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Motor control pins

// Motor A, Left Side 
#define ENA 9
#define IN1 8
#define IN2 7

// Motor B , Right Side
#define IN3  6
#define IN4  5
#define ENB  3

String substring =  String();
int startIndex;
int endIndex;
int i=0;


// Vector of String
std::vector<int> values;


// struct for motor number mapping with corresponding pins
struct Motor {
  int motorNumber;
  int rpm;
  int direction; // Could be FORWARD, BACKWARD, or a defined enum
  int pin1;
  int pin2;
  int enable;
};

struct Motor motors[] = {
    {1, 225, 1, IN1, IN2, ENA},
    {2, 255, 1, IN3, IN3, ENB}
};


//   -------------------------------

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

//
const int min_RPM = 0; // Assuming the motor can start from 0 RPM
const int max_RPM = 100;
const int min_PWM = 0;
const int max_PWM = 255;

int mapRPMtoPWM(int RPM) {
  int PWM_value = (RPM - min_RPM) * (max_PWM - min_PWM) / (max_RPM - min_RPM) + min_PWM;
  return PWM_value;
}



// parse the http request
std::vector<int> parseQueryStringIntegers(const String& queryString) {
  std::vector<int> values;

  // Parse the query string into integer values
  int ampersandPos = queryString.indexOf('&');
  while (ampersandPos != -1) {
    // Extract the value
    String valueString = queryString.substring(queryString.indexOf('=') + 1, ampersandPos);

    // Convert the value to an integer
    int value = valueString.toInt();

    // Add the value to the array
    values.push_back(value);

    // Remove the parsed value from the query string
   String queryString = queryString.substring(ampersandPos + 1);

    // Yield to allow other tasks to run
    yield();

    // Find the next ampersand
    ampersandPos = queryString.indexOf('&');
  }

  // Parse the last value
  String valueString = queryString.substring(queryString.indexOf('=') + 1);
  int value = valueString.toInt();
  values.push_back(value);

  return values;
}

void setup() {
  Serial.begin(115200);
  // Initialize the MOtor pins


  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();  // Listen for incoming clients

  if (client) {  // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");                                             // print a message out in the serial port
    String currentLine = "";                                                   // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {  // if there's bytes to read from the client,
        char c = client.read();  // read a byte, then
        Serial.write(c);         // print it out the serial monitor
        header += c;
        if (c == '\n') {  // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();


            if (header.indexOf("GET /motor/set") >= 0) {
              // request with motor number , rpm ,direction
              startIndex = header.indexOf('/motor/set?');
              endIndex = header.indexOf(' HTTP')-4;
              substring = header.substring(startIndex,endIndex);
              Serial.println(substring);
              values = parseQueryStringIntegers(substring);
              setMotorTo(values[0]-1,values[1],values[2]);

            } else if (header.indexOf("GET /motor/stop") >= 0) {
              //request with motor number
              startIndex = header.indexOf('/motor/stop?');
              endIndex = header.indexOf(' HTTP')-4;
              substring = header.substring(startIndex,endIndex);
              Serial.println(substring);
              
              stop(0);
              stop(1);

            }

            // The HTTP response ends with another blank line
            client.println();
            break;
          } else {  // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    substring="";
    
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}


// Motor control functions

void increaseSpeedTo(int motor,int targetRPM){
    int Current_PWM = mapRPMtoPWM(motors[motor].rpm);
    int targetPWM = mapRPMtoPWM(targetRPM);
    if((Current_PWM + targetPWM) > 255){
        targetPWM =255;
    }
    for(i=Current_PWM;i>targetPWM;i++){
        digitalWrite(motors[motor].enable,i);
    } 
    Serial.println("Increased the Speed of the motor ");    
}

void decreaseSpeedTo(int motor,int targetRPM){
    int Current_PWM = mapRPMtoPWM(motors[motor].rpm);
    int targetPWM = mapRPMtoPWM(targetRPM);
    if((Current_PWM - targetPWM) < 0){
        targetPWM =0;
    }
    for(i=Current_PWM;i>targetPWM;i--){
        digitalWrite(motors[motor].enable,i);
    }    
    Serial.println("Decreased the speed of the motor");  
}

void reverse(int motor){ 
    decreaseSpeedTo(motor,0);
    delay(25);
    digitalWrite(motors[motor].pin1,HIGH);
    digitalWrite(motors[motor].pin2,LOW);
    increaseSpeedTo(motor,mapRPMtoPWM(motors[motor].rpm));
    Serial.println("reversed the direction of DC motor");
}

void forward(int motor){
  decreaseSpeedTo(motor,0);
  delay(25);
  digitalWrite(motors[motor].pin1,LOW);
  digitalWrite(motors[motor].pin2,HIGH);
  increaseSpeedTo(motor,mapRPMtoPWM(motors[motor].rpm));
  Serial.println("Moved it in forward direction");
}

void stop(int motor){
    digitalWrite(motors[motor].pin1,LOW);
    digitalWrite(motors[motor].pin2,LOW);
    digitalWrite(motors[motor].enable,LOW);
    Serial.println("Stop the DC motor!!");
}


void setMotorTo(int motorNumber , int rpm,int direction){
  int PWM = mapRPMtoPWM(rpm);
  if(rpm != motors[motorNumber].rpm && direction != motors[motorNumber].direction){
    // if we have to change both Speed and Direction
    motors[motorNumber].rpm=rpm;
    if(motors[motorNumber].direction==1){
        forward(motorNumber);
    }else{
        reverse(motorNumber);
    }

  } else if(rpm !=motors[motorNumber].rpm){  
      // if we only have to change the RPM of the Motor 
      if(rpm > motors[motorNumber].rpm ){
          increaseSpeedTo(motorNumber,rpm);
      }else{
        decreaseSpeedTo(motorNumber,rpm);
      }

  }
}
