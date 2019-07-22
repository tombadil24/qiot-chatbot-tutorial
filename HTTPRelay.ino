#include <LWiFi.h>

#define RLY1 3                      // what digital pin we're connected to
#define RLY2 4

char ssid[] = "TP-LINK_1A9F";       // your network SSID (name)
char pass[] = "04210252";           // your network password
int keyIndex = 0;                   // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);    // Wait on port 80

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
  }
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();

  pinMode(RLY1, OUTPUT);
  pinMode(RLY2, OUTPUT);
  digitalWrite(RLY1, LOW);
  digitalWrite(RLY2, LOW);
}

void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new connection");

    String msg;
    String param;

    // an http request ends with a blank line
    if (client.connected()) {
      while (client.available()) {
        char c = client.read();
        msg += String(c);
      }
    }

    Serial.print(msg);

    int posi = msg.indexOf("Referer: ");
    if(posi > 0)
    {
      String sb = msg.substring(posi);
      int p = sb.indexOf("/");
      param = sb.substring(p+2);
      p = param.indexOf("/");
      param = param.substring(p, p+5);
    }
    else
    {
      posi = msg.indexOf("GET");
      param = msg.substring(posi+4, posi+9);
    }

    Serial.println(param);

    // send a standard http response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");  // the connection will be closed after completion of the response
    client.println();

    if(param == "/ON_1")
    {
      digitalWrite(RLY1, HIGH);
      client.println("done");
    }
    if(param == "/OFF1")
    {
      digitalWrite(RLY1, LOW);
      client.println("done");
    }
    if(param == "/ON_2")
    {
      digitalWrite(RLY2, HIGH);
      client.println("done");
    }
    if(param == "/OFF2")
    {
      digitalWrite(RLY2, LOW);
      client.println("done");
    }

    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }

  // give the web browser timev to receive the data
  delay(100);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
