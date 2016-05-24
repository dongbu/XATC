#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

char* ssid = ".........";
char* pass = ".........";

int position  = 0;
int target    = 0;
int pin       = 5; // Servo pin


ESP8266WebServer server(80);

Servo myservo;

void setup(void){
  Serial.begin(115200);
  Serial.println("");
  
  WiFi.begin(ssid, pass);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.on("/", [](){
    server.send(200, "text/plain", "Open /servo?value=90 to control servo or set the target value via /target?value=512");
  });

  server.on("/target", [](){
    String sval = server.arg("value");
    target = sval.toInt();
    server.send(200, "text/plain", info(position, target));
  });

  server.on("/servo", [](){
    String sval = server.arg("value");
    position = sval.toInt();
    myservo.write(position);
    server.send(200, "text/plain", info(position, target));
  });

  server.begin();
  Serial.println("HTTP server started");
  
  myservo.attach(pin);   // Servo attached to D5 pin on NodeMCU board
  myservo.write(0);
}
 
void loop(void){
  server.handleClient();
}

String info(int position, int target){
  String text = String("Position: " + String(position, 'DEC') + "\tTarget: " + String(target, 'DEC') + "\n");
  Serial.print(text);
  return text;
}
 