#include <SoftwareSerial.h>
#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>

DHT dht(22, 22);
DHT dht_2(24, 22);

SoftwareSerial lcd (10, 11);

int alcohol = 0;
int alcohol_2 = 0;
int diox_carbono = 0;

const int led_verde_temp = 26;
const int led_rojo_temp = 27;
const int led_verde_gas = 28;
const int led_rojo_gas = 29;
const int led_verde_temp2 = 30;
const int led_rojo_temp2 = 31;
const int led_verde_gas2 = 32;
const int led_rojo_gas2 = 33;
const int led_verde_diox = 34;
const int led_rojo_diox = 35;


// MAC - IP para la red

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 100);


//Inicia la bilbioteca Ethernet en el puerto 80 (HTTP)

EthernetServer server(80);

void setup() {
  
  Serial.begin(9600);
  lcd.begin(9600);
  while (!Serial) {
    ;
  }


  // Inicio del server y la coneccion ethernet
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  
  pinMode(led_verde_temp, OUTPUT);
  pinMode(led_rojo_temp, OUTPUT);
  pinMode(led_verde_gas, OUTPUT);
  pinMode(led_rojo_gas, OUTPUT);
  pinMode(led_verde_temp2, OUTPUT);
  pinMode(led_rojo_temp2, OUTPUT);
  pinMode(led_verde_gas2, OUTPUT);
  pinMode(led_rojo_gas2, OUTPUT);
  pinMode(led_verde_diox, OUTPUT);
  pinMode(led_rojo_diox, OUTPUT);
}


void loop() {
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();
  float temperatura_2 = dht_2.readTemperature();
  float humedad_2 = dht_2.readHumidity();
  
  alcohol = analogRead(A8);
  alcohol_2 = analogRead(A9);
  diox_carbono = analogRead(A10);

  if (lcd.available()){
    lcd.write(temperatura);
    lcd.write(humedad);
    lcd.write(alcohol);
  }
  
 //Escucha de los clientes
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        
        if (c == '\n' && currentLineIsBlank) {
          
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  
          client.println("Refresh: 2");  
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<meta charset='utf-8'>");
          client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
          client.println("<title>Robotica Educativa</title>");
          client.println("<style>*{padding:0; margin:0;font-family:sans-serif;font-weight:bold;}body{background:#fff;}header{width:80%;border-radius:20px; background:#222; overflow:hidden;padding:25px;margin: 0 auto;}h1, h2{text-align:center; color: #fff; width: 100%; font-size:150%;cursor:pointer;}.contenedor{width:80%; margin: 0 auto; background:#90CAF9; padding: 25px; position:relative; top:-15px; border-radius: 0px 0px 20px 20px; }h2{width:90%;  margin: 0 auto;}table{margin:0 auto; cursor:pointer; border-spacing:  0;}.centro{font-size:18px; font-weight: bolder; width:100%; text-align:center; display: inline-block; color:#fff;}td{padding-left:10px; transition: all .4s ease;}td:hover{background:#283747; color:#fff;}.fondo{background-color:#222;}.niveles{list-style: none; width:400px;}tr, td{border: 1px solid;}.cl{border:1px solid; display: inline-block; border-radius: 20px;} li{list-style:none; text-align:left;  width:50%; cursor:pointer;</style>");
          client.println("</head>");
          client.println("<body>");
          client.println("<header><h1>Instituto Nacional Doctor Sarbelio Navarrete</h2><br><h2><i>Robotica Educativa - MINED San Vicente</i></h2></header>");
          //Inicio contenedor
          client.println("<div class='contenedor'>");
          client.println("<table width='70%'>");
          client.println("<td colspan='2' class='fondo'><span class='centro'>Sensores en un ambiente normal</span></tr></td>");

          // normal
          client.println("<tr><td>La Temperatura es: </td><td>");
          if(temperatura < 31){
            digitalWrite(led_rojo_temp, LOW);
            digitalWrite(led_verde_temp, HIGH);
            client.print(temperatura);
            client.print("°C</td><td class='cl' style='background:#76FF03'><span>&nbsp;&nbsp;</span></td><td class='cl'><span>&nbsp;&nbsp;</span></td></td></tr>");
          }
          else{
            digitalWrite(led_verde_temp, LOW);
            digitalWrite(led_rojo_temp, HIGH);
            client.print(temperatura);
            client.print("°C</td><td class='cl'><span>&nbsp;&nbsp;</span></td><td class='cl' style='background:#D32F2F;'><span>&nbsp;&nbsp;</span></td></td></tr>");
          }
          client.println("<tr><td>La humedad es: </td><td>");
         if(humedad < 60){
          client.print(humedad);
          client.print(" g/m<sup>3</sup></td><td class='cl' style='background:#76FF03'><span>&nbsp;&nbsp;</span></td><td class='cl'><span>&nbsp;&nbsp;</span></td></td></tr>");
          }
          else{
            client.print(humedad);
            client.print(" g/m<sup>3</sup></td><td class='cl'><span>&nbsp;&nbsp;</span></td><td class='cl' style='background:#D32F2F;'><span>&nbsp;&nbsp;</span></td></td></tr>");
          }
          client.println("<tr><td>Niveles de gases sensados en el ambiente: </td><td>");
          if(alcohol < 250){
            digitalWrite(led_rojo_gas, LOW);
            digitalWrite(led_verde_gas, HIGH);
            client.print(alcohol);
            client.print("</td><td class='cl' style='background:#76FF03'><span>&nbsp;&nbsp;</span></td><td class='cl'><span>&nbsp;&nbsp;</span></td></td></tr>");
          }
          else{
            digitalWrite(led_verde_gas, LOW);
            digitalWrite(led_rojo_gas, HIGH);
            client.print(alcohol);
            client.print("</td><td class='cl'><span>&nbsp;&nbsp;</span></td><td class='cl' style='background:#D32F2F;'><span>&nbsp;&nbsp;</span></td></td></tr>");
          }

          // critico
          client.println("<td colspan='2' class='fondo'><span class='centro'>Sensores en un ambiente critico</span></tr></td>");
          client.println("<tr><td>La Temperatura es: </td><td>");
          if(temperatura_2 < 30){
            digitalWrite(led_rojo_temp2, LOW);
            digitalWrite(led_verde_temp2, HIGH);
            client.print(temperatura_2);
            client.print("°C</td><td class='cl' style='background:#76FF03'><span>&nbsp;&nbsp;</span></td><td class='cl'><span>&nbsp;&nbsp;</span></td></td></tr>");
          }
          else{
            digitalWrite(led_verde_temp2, LOW);
            digitalWrite(led_rojo_temp2, HIGH);
            client.print(temperatura_2);
            client.print("°C</td><td class='cl'><span>&nbsp;&nbsp;</span></td><td class='cl' style='background:#D32F2F;'><span>&nbsp;&nbsp;</span></td></td></tr>");
          }
          client.println("<tr><td>La humedad es: </td><td>");
         if(humedad_2 < 60){
          client.print(humedad_2);
          client.print(" g/m<sup>3</sup></td><td class='cl' style='background:#76FF03'><span>&nbsp;&nbsp;</span></td><td class='cl'><span>&nbsp;&nbsp;</span></td></td></tr>");
          }
          else{
            client.print(humedad_2);
            client.print(" g/m<sup>3</sup></td><td class='cl'><span>&nbsp;&nbsp;</span></td><td class='cl' style='background:#D32F2F;'><span>&nbsp;&nbsp;</span></td></td></tr>");
          }
          client.println("<tr><td>Niveles de gases sensados en el ambiente: </td><td>");
          if(alcohol_2 < 250){
            digitalWrite(led_rojo_gas2, LOW);
            digitalWrite(led_verde_gas2, HIGH);
            client.print(alcohol_2);
            client.print("</td><td class='cl' style='background:#76FF03'><span>&nbsp;&nbsp;</span></td><td class='cl'><span>&nbsp;&nbsp;</span></td></td></tr>");
          }
          else{
            digitalWrite(led_verde_gas2, LOW);
            digitalWrite(led_rojo_gas2, HIGH);
            client.print(alcohol_2);
            client.print("</td><td class='cl'><span>&nbsp;&nbsp;</span></td><td class='cl' style='background:#D32F2F;'><span>&nbsp;&nbsp;</span></td></td></tr>");
          }
          client.println("<tr><td>Nivel de Dioxido de Carbono en el ambiente: </td><td>");
          if(diox_carbono < 200){
            digitalWrite(led_rojo_diox, LOW);
            digitalWrite(led_verde_diox, HIGH);
          client.print(diox_carbono);
          client.print(" PPM</td><td class='cl' style='background:#76FF03'><span>&nbsp;&nbsp;</span></td><td class='cl'><span>&nbsp;&nbsp;</span></td></td></tr>");
          }
          else{
            digitalWrite(led_verde_diox, LOW);
            digitalWrite(led_rojo_diox, HIGH);
            client.print(diox_carbono);
            client.print(" PPM</td><td class='cl'><span>&nbsp;&nbsp;</span></td><td class='cl' style='background:#D32F2F;'><span>&nbsp;&nbsp;</span></td></td></tr>");
          }
          
          client.println("</table>");

          client.println("<br>");
          client.println("<center>");
          client.println("<ul>");
          client.println("<li>Parametros normales <span style='background:#76FF03; display:inline-block; border-radius: 20px;'>&nbsp;&nbsp;&nbsp;&nbsp;</span></li>");
          client.println("<li>Parametros fuera de lo normal <span style='background:#D32F2F; display:inline-block; border-radius: 20px;'>&nbsp;&nbsp;&nbsp;&nbsp;</span></li>");
          client.println("</ul>");
          client.println("</center>");
          //Fin contenedor
          client.println("</div>");
          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          
          currentLineIsBlank = true;
        } else if (c != '\r') {
         
          currentLineIsBlank = false;
        }
      }
    }
    
    delay(1);
    
    client.stop();
    Serial.println("client disconnected");
  }
}

