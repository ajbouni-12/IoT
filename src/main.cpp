#include <WiFi.h>
#include <sstream>
#include <string>
#include <LiquidCrystal_I2C.h>


const char *ssid = "ISIMM-enseignants";
const char *password = "!IsImm@20Ens23!";
const char *server = "192.168.1.6";
const int port = 80;

const int gasSensorPin = 34; // Assuming the gas sensor is connected to pin 34

const int gasThreshold = 40;
const int ledpin = 4;
//// configuration afficheur lcd  
const int addr = 0x27;  // Adresse I2C de l'afficheur
const int lcdCols = 16;  // Nombre de colonnes de l'afficheur
const int lcdRows = 2;   // Nombre de lignes de l'afficheur

// Création d'une instance de la classe LiquidCrystal_I2C
LiquidCrystal_I2C lcd(addr, lcdCols, lcdRows); 

std::string urlencode(const std::string &s)
{
    static const char lookup[]= "0123456789abcdef";
    std::stringstream e;
    for(int i=0, ix=s.length(); i<ix; i++)
    {
        const char& c = s[i];
        if ( (48 <= c && c <= 57) ||//0-9
             (65 <= c && c <= 90) ||//abc...xyz
             (97 <= c && c <= 122) || //ABC...XYZ
             (c=='-' || c=='_' || c=='.' || c=='~') 
        )
        {
            e << c;
        }
        else
        {
            e << '%';
            e << lookup[ (c&0xF0)>>4 ];
            e << lookup[ (c&0x0F) ];
        }
    }
    return e.str();
}

void sendEmail(WiFiClient client, std::string message) {
  // Use WiFiClient to create a connection

  Serial.println("Sending GET request...");

  // Connect to the server
  if (client.connect(server, port)) {
    // Make a GET request
    client.print("GET /?message="); 
    client.print(message.c_str());
    client.print(" HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(server);
    client.print("\r\n");
    client.print("Connection: close\r\n\r\n");

    // Wait for the server to respond
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    // Close the connection
    client.stop();
    Serial.println("\nRequest complete");
  } else {
    Serial.println("Connection failed");
  }

}

void setup() {

  Serial.begin(115200);


  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..."); 
  }
  Serial.println("Connected to WiFi");

  // Set up gas sensor pin
  pinMode(gasSensorPin, INPUT);
  pinMode(ledpin , OUTPUT);

    // Initialisation de l'afficheur LCD
     lcd.init();
     lcd.begin(lcdCols, lcdRows);
     lcd.backlight();
  
}

void loop() {
  int gasValue = analogRead(gasSensorPin);
  //
  Serial.println(gasValue);
  delay(1000);
  // Wait for sensor stability
   WiFiClient client;
   

  if (gasValue > gasThreshold) {
    std::string m = "warning.";
    lcd.setCursor(0,1);
    lcd.print("Warning!");
    digitalWrite(ledpin,HIGH);
    delay(1000);
    sendEmail(client,m);
    }
    else {
    // Gas value is below the threshold, turn off the warning
    lcd.clear();  // Clear the LCD
    digitalWrite(ledpin,LOW);
    //digitalWrite(ledpin, LOW);
  }
  
 
  // Adjust the delay according to your requirements
}


