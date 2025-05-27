// This code is part of the IRRIOTA prototype presented in the article:
// "Integrating Low-Cost IoT Sensors with Community-Accessible
// Online Tools for Irrigation Management: a Case Study in
// Actinidia Orchards".

// This part of the code performs:
// - Wi-Fi connection.
// - Time verification (checks if minutes are 00, 30, or time is 23:59).
// - Sends sensor data to Google Sheets only at these specified times.
// - Resets the rain gauge at midnight (00:00).

// The first column in Google Sheets holds the date and time.
// The other columns correspond to sensor data and must be populated accordingly.

// IMPORTANT: The user must modify the initial part of the code and follow the video tutorial
// to properly configure their Google account and enable data reception.


#include <Arduino.h>
#include <WiFi.h> //WiFi library.
#include "time.h" //Library to get Date and Time.
#include <ESP_Google_Sheet_Client.h> // Library to send data to Google Sheets via Google Cloud.
#include <math.h> //Library for mathematical functions
#include "DHT.h" //DHT11 Sensor Library
#include <OneWire.h> //DS18B20 Library (1/2)
#include <DallasTemperature.h> //DS18B20 Library (2/2)


//============================= TO BE MODIFIED BY THE USER ===================================//

// To configure the ESP32/Google Sheets connection, we recommend carefully following the steps in this video:
// https://www.youtube.com/watch?v=ow5ZIrz60_4

// Wi-Fi credentials
#define WIFI_SSID "CHANGE_HERE_WIFI_SSID"
#define WIFI_PASSWORD "CHANGE_HERE_WIFI_PASSWORD"
// Google Cloud credentials
#define PROJECT_ID "CHANGE_HERE_GOOGLE_SHEETS_PROJECT_ID"
#define CLIENT_EMAIL "CHANGE_HERE_GOOGLE_SHEETS_CLIENT_EMAIL"
#define PAGE "CHANGE_HERE_GOOGLE_SHEETS_PAGE"
const char spreadsheetId[] = "CHANGE_HERE_GOOGLE_SHEETS_spreedsheetID";

//============================= AFTER HERE, DO NOT MODIFY ===================================//
// If you assemble exactly as described in the project, connecting the ESP pins as specified in the article,
// from this point onwards the code should not be modified.


const char PRIVATE_KEY[] PROGMEM = R"EOF(
-----BEGIN PRIVATE KEY-----
MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQChcttgMFpKk6s0
NRcAsmxWyBDX3Eoxsp5alXbeP3sSB6IRjkiU7q5/G2wnYPb6E4G1iY32pJ84QtIc
PF+ZmeKxmn0WtIYw4UrLlsZBQdE8tOXmWBZqmeMXy9Y2e1zAgLuVwOyEbVEli37z
2KczcevgjVh3uyEhl43hpcLMKJD9J/lPBXqqCzC/WgFQfJQ4w3+64nyr7SNHPrxJ
4ZWHMKVr9Tl/KZXsS8PIGCeJza9ec96KuKV0IOMvdXiEAii/26+nuwSR3yCnsiMO
u52p6aXcXjcbjLmVbAQAnkDPSjkwT5RFq2G2I2OrdED1Q88hiBhS/bsP8Q2kPJpU
18Ho63U/AgMBAAECggEAUK1yNcuD2z+WrqSXBZijrewVCqGBC/uryS1mdKI5UGWz
Fd3JpG7vkQH1lmoMkSQdTcfawrU6AWXW1BhRV5MAJutB9g4o/pUeiMUj9ik7M4Bc
YDKFpSiYRGRue9RCocnSs39wUa7E/aOLQbRyvTEBA7nuSwqegEhZNLjeMqJU7cP4
S45blRZqB5M5hARg38BzKE9WoFx7f8Cdwd+JKgQ5v9DKj6/I9jOtF53SpIeqTyb2
unHlW1ojsV6nyBZsKtO2DDBIpVwV/WWqLXo5v3mlydC05J25Q1jjmDA7hW1TN0qt
WAyfFxD521W3Pf+fDWYSXkZsfgvZBsPmLGNC+twBUQKBgQDOKLF55b4LNBtiK50I
ukb56oLAvlkDNazoFLc932Ig0+h72HvWu0y2wbn/eVHmxaHbIHhkFDRk+F6Tntrb
n4i2FdGwhrDitt/Fz7E69HlAH6+XZpr/6OAan2aT0/bHKJO8qE8PotlSFzn8Ihkm
IszzZEqzB70hmYcm/zDRBV0dzwKBgQDIewPv6vm+QVZuZ1hdLIgHqJ7/MubtUMO7
ZpnmtGCOK1nzDnENH0k3J98rMTrmioNmSrlzSaRHsDCzUoCyiXtqqnU2l6MUIM/9
wPRbEu8ehnbMOLuKFYbHwTPlBBsFltO8zAgioQ5tHNXeKOZTBEOyImnr7jN71UPg
CspAQcj9kQKBgBjXbHNMzLaa48XTjYuUrqLEsFa7WQx69kfM3zu0ZCWigzwRHPMG
BLts4pW3L7hIDjXSVs9wY9Pa3cKZ/LNEKPwP6wGDaWMYOCSk3y1EcivY5ZavxXOa
ww5SMNEVVIIxEIRGH6i5rcgnhlQsY61luh6F8j5aa17WGiQH0gENzWefAoGBAIqp
0zpRKvX12SM5pbukPl4VugtHGMRR1V+8Any4jAQpZeigN39dAVX95PTJ8zRb37CT
xJtB9zeCXNzbFprmTo+jYLkmvqwAWfvoRtrrSKqzVF3Ca6yBYRyuvAcOBOajwMXX
1VSmUwUTKbWkUlJfI7eE6ABAXCmtg2nl7KfvGGaRAoGAeW1TXj21/EiNsEAHsrkt
ykny3C1AYdHsQVXrUciU19rEVt4dpnAdTcOm6FaVG8b9HftcAnRDKH+eocb2dZBL
iZ8sSTJcHVami1sJ2LZoF9PVJcn7fXSFDBwzUoGNjRGB6Z/jwruEuchlBrzk8KPj
GagRF9kdnRRJBjaBJppwSt8=
-----END PRIVATE KEY-----
)EOF";


//============================= SENSOR OUTPUT VARIABLES ===================================//

// Total accumulated daily precipitation in millimeters. Calculated by multiplying the number of tips by the water quantity per tip.
float pluv_valor = 0; 
// Current air relative humidity value (in %)
int hum_valor = 0;
// Current air temperature value (in °C)
float temp_valor = 0; 
// Soil water content values measured by capacitive sensors at different depths
// cap1_valor = Sensor 1; cap2_valor = Sensor 2; cap3_valor = Sensor 3; cap4_valor = Sensor 4;
float cap1_valor, cap2_valor, cap3_valor, cap4_valor = 0; 
// Intermediate depth points between sensors (estimates by interpolation)
float vazio1, vazio2, vazio3 = 0; 
// Effective root zone thickness with available water
float h_esp_efetiva = 0; 
// Stable version of the final value (for sending or logging)
int WM1_CB_VALOR = 0; 
// Calculated sensor resistance (in ohms), based on readings
int WM1_R_VALOR = 0;        
// Value that appears when there are reading errors or when the sensor is not working.
int READING_ERROR = -999;

//============================= SENSOR CONFIGURATION ===================================//

//==========================================//
//        RAIN GAUGE CONFIGURATION          //
//==========================================//
// This sensor (rain gauge) measures daily precipitation.

// ============ RAIN GAUGE VARIABLES ===========//
// Constants
const int buttonPin = 39; 
// Digital pin where the rain gauge is connected (tipping bucket type).
// This type of sensor has two small buckets that alternate when collecting a small amount of water.
// Each time the bucket tips, it temporarily closes a circuit (like a switch), generating a digital pulse.

// State variables
int buttonPushCounter = 0; 
// Counts the number of tips registered by the sensor. 

float pluv_single = 0.2794;
// Each tip corresponds to a fixed amount of precipitation (0.2794 mm), defined by the sensor manufacturer.

int buttonState = 0;       
// Current pin state (HIGH or LOW), used to detect if a new tip occurred.

int lastButtonState = 0;   
// Previous pin state. Used to compare with the current state and identify changes.

// Debounce variables
long lastDebounceTime = 0; 
// Time of the last stable state change. Used to avoid false readings caused by vibrations or electrical noise.

long debounceDelay = 60;   
// Minimum time (in milliseconds) the signal must remain stable to be considered a valid tip.

//==========================================//
//             RAIN GAUGE FUNCTION          //
//==========================================//
// Function that operates the rain gauge

void executaPluviometro()
{
   // Read the state of the switch from the input pin:
   buttonState = digitalRead(buttonPin);
 
   // Compare the current state of the switch to its previous state:
   if (buttonState != lastButtonState) {
     if (buttonState == HIGH) {
       // If the switch state changes, due to noise or bucket tip, and the current state is high, reset the debounce timer
       lastDebounceTime = millis();
     }
     if ((millis() - lastDebounceTime) > debounceDelay) {
     // The current switch state has been stable for more than the debounce delay, so it is not noise and can be counted as a tip
       buttonPushCounter++;
     
     pluv_valor = buttonPushCounter * pluv_single;
    
     Serial.print("Total rainfall measurement: ");
     Serial.print(pluv_valor);
     Serial.println(" mm");
     } 
   // Save the current state as the last state, for the next time through the loop
   lastButtonState = buttonState;
 }
}

// CONTINUATION OF SENSOR CONFIGURATION AND FUNCTIONS

//==========================================//
//        DHT11 SENSOR CONFIGURATION         //
//==========================================//
// This sensor (DHT11) measures air temperature and relative humidity.

#define DHTPIN 35
// Digital pin to which the DHT11 sensor is connected.

#define DHTTYPE DHT11
// Define the sensor model used.
// In this case, DHT11 — a basic low-cost digital sensor with moderate accuracy.
// If you want to use the DHT22 sensor (better accuracy), change to: #define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
// Create the 'dht' object to interact with the sensor, indicating the pin and type.

const int maxTentativas = 10;
// Maximum number of read attempts.
int tentativas = 0;

void obtemDHT() {
  do {
    hum_valor = dht.readHumidity();
    delay(500);
    tentativas++;
  } while ((hum_valor > 100 || hum_valor < 0 || isnan(hum_valor)) && tentativas < maxTentativas);

  if (tentativas == maxTentativas) {
    hum_valor = READING_ERROR;
    Serial.println("Error: humidity reading failed");
  }

  tentativas = 0;

  do {
    temp_valor = dht.readTemperature();
    delay(500);
    tentativas++;
  } while ((temp_valor < -15 || temp_valor > 55 || isnan(temp_valor)) && tentativas < maxTentativas);

  if (tentativas == maxTentativas) {
    temp_valor = READING_ERROR;
    Serial.println("Error: temperature reading failed");
  }

  Serial.print("Humidity (DHT): ");
  Serial.println(hum_valor);
  Serial.print("Temperature (DHT): ");
  Serial.println(temp_valor);
}

//=====================================================//
// CONFIGURATION OF CAPACITIVE SOIL MOISTURE SENSORS  //
//=====================================================//
// For Capacitive Soil Moisture Sensor V2.0.

int ValorLeitura, LeituraCap; // Auxiliary variables for reading sensors
int ValorSensor1, ValorSensor2, ValorSensor3, ValorSensor4; // Average analog value of each sensor (0 to 4095)
float percentagemHumidade1, percentagemHumidade2, percentagemHumidade3, percentagemHumidade4; // Estimated moisture percentage via calibration formula

const int NumLeituras = 100; // Number of samples per sensor for stable reading
const int sensores[] = {7, 8, 9, 10}; // Analog pins connected to the soil moisture sensors
float* valores[] = {&cap1_valor, &cap2_valor, &cap3_valor, &cap4_valor};
int* brutos[] = {&ValorSensor1, &ValorSensor2, &ValorSensor3, &ValorSensor4};
float* percentagens[] = {&percentagemHumidade1, &percentagemHumidade2, &percentagemHumidade3, &percentagemHumidade4};

void obtemCapSolo() {
  for (int s = 0; s < 4; s++) {
    long somaLeituras = 0;
    for (int j = 0; j < NumLeituras; j++) {
      somaLeituras += analogRead(sensores[s]);
    }

    *brutos[s] = somaLeituras / NumLeituras;

    float leitura = *brutos[s];
    float percentagem = 32.322428 
                        - 0.0033212 * leitura 
                        - 8.7409e-7 * pow(leitura - 5825.07, 2) 
                        - 1.3509e-9 * pow(leitura - 5825.07, 3);

    if (percentagem < 10 || percentagem > 55) {
      Serial.print("⚠️ Sensor reading error ");
      Serial.print(s + 1);
      Serial.print(" (percentage out of 10–55% range): ");
      Serial.println(percentagem);
      percentagem = READING_ERROR;
    }

    *valores[s] = percentagem;
    *percentagens[s] = percentagem;
  }

  vazio1 = cap1_valor + (cap2_valor - cap1_valor) / 2;
  vazio2 = cap2_valor + (cap3_valor - cap2_valor) / 2;
  vazio3 = cap3_valor + (cap4_valor - cap3_valor) / 2;

  h_esp_efetiva = (cap1_valor + cap2_valor + cap3_valor + vazio1 + vazio2 + vazio3) / 6;
}

//==========================================//
//       DS18B20 SENSOR CONFIGURATION       //
//==========================================//

float TempC;
const int oneWireBus = 37;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

//==========================================//
//     WATERMARK SENSOR CONFIGURATION       //
//==========================================//

#define num_of_read 10
const int Rx = 7870;
const long open_resistance = 35000;
const long short_resistance = 200;
const long short_CB = 240, open_CB = 255;
double SupplyV = 3.3;

int i, j = 0;
int SenVWM2 = 0;
int WM1_CB = 0;
double SenV10K = 0;
double SenVWM1 = 0;
double ARead_A1 = 0;
double ARead_A2 = 0;
int WM1_Calib_Resistance;

void obtemWatermark() {
  sensors.requestTemperatures();
  TempC = sensors.getTempCByIndex(0);
  int tentativasTemp = 0;

  while ((TempC < -10 || TempC > 40) && tentativasTemp < 10) {
    delay(200);
    TempC = sensors.getTempCByIndex(0);
    tentativasTemp++;
  }

  if (TempC < -10 || TempC > 40) {
    TempC = 18;
    Serial.println("⚠️ Failed to read DS18B20. Default value assumed: 18 °C");
  } else {
    Serial.print("✅ Soil temperature successfully read: ");
    Serial.print(TempC);
    Serial.println(" °C");
  }

  ARead_A1 = 0;
  for (i = 0; i < num_of_read; i++) {
    digitalWrite(6, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    delay(10);
    digitalWrite(11, LOW);
    digitalWrite(5, HIGH);
    delay(0.11);
    ARead_A1 += analogRead(1);
    digitalWrite(5, LOW);
    delay(100);
  }

  SenV10K = ((ARead_A1 / 4096) * SupplyV) / num_of_read;
  double Calib_Resistance = 10000 / ((Rx * (SupplyV - SenV10K) / SenV10K));

  ARead_A1 = 0;
  ARead_A2 = 0;
  for (i = 0; i < num_of_read; i++) {
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    delay(10);
    digitalWrite(5, HIGH);
    delay(0.11);
    ARead_A1 += analogRead(1);
    digitalWrite(5, LOW);
    delay(100);
    digitalWrite(11, HIGH);
    delay(0.11);
    ARead_A2 += analogRead(1);
    digitalWrite(11, LOW);
  }

  SenVWM1 = ((ARead_A1 / 4096) * SupplyV) / num_of_read;
  SenVWM2 = ((ARead_A2 / 4096) * SupplyV) / num_of_read;
  double WM1_ResistanceA = (Rx * (SupplyV - SenVWM1) / SenVWM1);
  double WM1_ResistanceB = (Rx * SenVWM2 / (SupplyV - SenVWM2));
  double WM1_Resistance = ((WM1_ResistanceA + WM1_ResistanceB) / 2) * Calib_Resistance;

  double WM1_Calib_Resistance = 
    38414.475 
    - 8.5373679 * WM1_Resistance 
    + 0.0048515 * pow((WM1_Resistance - 3818.34), 2)
    - 0.0000023 * pow((WM1_Resistance - 3818.34), 3)
    + 0.0000000008787 * pow((WM1_Resistance - 3818.34), 4);

  digitalWrite(6, HIGH);
  delay(3000);

  if (WM1_Calib_Resistance > 550.0) {
    if (WM1_Calib_Resistance > 8000.0) {
      WM1_CB = -2.246 - 5.239 * (WM1_Calib_Resistance / 1000.0) * (1 + 0.018 * (TempC - 24.0)) - 0.06756 * pow((WM1_Calib_Resistance / 1000.0), 2) * pow((1 + 0.018 * (TempC - 24.0)), 2);
      Serial.print("Entered WM1 >8000 Loop \n");
    } else if (WM1_Calib_Resistance > 1000.0) {
      WM1_CB = (-3.213 * (WM1_Calib_Resistance / 1000.0) - 4.093) / (1 - 0.009733 * (WM1_Calib_Resistance / 1000.0) - 0.01205 * TempC);
      Serial.print("Entered WM1 >1000 Loop \n");
    } else {
      WM1_CB = ((WM1_Calib_Resistance / 1000.0) * 23.156 - 12.736) * (1.0 + 0.018 * (TempC - 24.0));
      Serial.print("Entered WM1 >550 Loop \n");
    }
  } else {
    if (WM1_Calib_Resistance > 300.0) {
      WM1_CB = 0.0;
      Serial.print("Entered 550 < WM1 > 300 Loop \n");
    } else if (WM1_Calib_Resistance >= short_resistance) {
      WM1_CB = short_CB;
      Serial.print("Entered Sensor Short Loop WM1 \n");
    }
  }

  if (WM1_Calib_Resistance >= open_resistance) {
    WM1_CB = open_CB;
    Serial.print("Entered Open or Fault Loop for WM1 \n");
  }

  WM1_R_VALOR = WM1_Calib_Resistance;
  WM1_CB_VALOR = abs(WM1_CB);
}

//============================= SENSOR SETUP CONFIGURATION ===================================//

void SetupSensors() {
  dht.begin();
  delay(50);
  sensors.begin();
  delay(50);
  pinMode(buttonPin, INPUT);
  delay(50);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(11, OUTPUT);
  delay(100);
}

//============================= WI-FI, TIME, AND GOOGLE SHEETS CONFIGURATION ===================================//

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;
bool dadosEnviados = false;
bool primeiroEnvioFeito = false;
int ultimoMinutoEnviado = -1;
bool enviou23_59 = false;
bool resetPluvioFeitoHoje = false;

void conectaWiFi() {
  Serial.print("Connecting to Wi-Fi: ");
  Serial.print(WIFI_SSID);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println();
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void configuraHoraNTP() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("🕒 NTP time configured");
}

void inicializaGoogleSheets() {
  GSheet.setTokenCallback(tokenStatusCallback);
  GSheet.setPrerefreshSeconds(10 * 60);
  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);
  Serial.println("📤 Google Sheets client initialized");
}

void tokenStatusCallback(TokenInfo info) {
  if (info.status == token_status_error) {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  } else {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
  }
}

String getDataHoraLegivel() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "ErroHora";

  char buffer[20];
  strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &timeinfo);
  return String(buffer);
}

void adicionaValorSensor(FirebaseJson &json, int coluna, String valor) {
  String caminho = "values/[0]/[" + String(coluna) + "]";
  json.set(caminho, valor);
}

//============================= SENDING SENSOR DATA TO GOOGLE SHEETS ===================================//

void enviaDadosGoogleSheetsRegular() {
  FirebaseJson response;
  FirebaseJson valueRange;
  String dataHora = getDataHoraLegivel();

  obtemDHT();
  obtemCapSolo();
  obtemWatermark();

  Serial.println("\n🟢 Sending data to Google Sheets...");
  Serial.println("Date/Time: " + dataHora);

  valueRange.add("majorDimension", "ROWS");
  adicionaValorSensor(valueRange, 0, dataHora);
  adicionaValorSensor(valueRange, 1, String(temp_valor));
  adicionaValorSensor(valueRange, 2, String(hum_valor));
  adicionaValorSensor(valueRange, 3, String(pluv_valor));
  adicionaValorSensor(valueRange, 4, String(WM1_R_VALOR));
  adicionaValorSensor(valueRange, 5, String(WM1_CB_VALOR));
  adicionaValorSensor(valueRange, 6, String(cap1_valor));
  adicionaValorSensor(valueRange, 7, String(vazio1));
  adicionaValorSensor(valueRange, 8, String(cap2_valor));
  adicionaValorSensor(valueRange, 9, String(vazio2));
  adicionaValorSensor(valueRange, 10, String(cap3_valor));
  adicionaValorSensor(valueRange, 11, String(vazio3));
  adicionaValorSensor(valueRange, 12, String(cap4_valor));
  adicionaValorSensor(valueRange, 13, String(h_esp_efetiva));

  bool success = GSheet.values.append(&response, spreadsheetId, PAGE, &valueRange);

  if (success) {
    response.toString(Serial, true);
    valueRange.clear();
    dadosEnviados = true;

    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      int hora = timeinfo.tm_hour;
      int minuto = timeinfo.tm_min;

      if (hora == 23 && minuto == 59 && !resetPluvioFeitoHoje) {
        pluv_valor = 0;
        buttonPushCounter = 0;
        resetPluvioFeitoHoje = true;
        Serial.println("🔁 Rain gauge reset after 23:59 send.");
      }

      if (hora == 0 && minuto == 0) {
        resetPluvioFeitoHoje = false;
      }
    }
  } else {
    Serial.println(GSheet.errorReason());
  }

  Serial.println("Free memory: " + String(ESP.getFreeHeap()));
  Serial.println("---------------------------------------");
}

void enviaDadosInicial() {
  FirebaseJson response;
  FirebaseJson valueRange;
  String dataHora = getDataHoraLegivel();

  obtemDHT();
  obtemCapSolo();
  obtemWatermark();

  Serial.println("\n🚀 Initial test send to Google Sheets...");
  Serial.println("Date/Time: " + dataHora);

  valueRange.add("majorDimension", "ROWS");
  adicionaValorSensor(valueRange, 0, dataHora);
  adicionaValorSensor(valueRange, 1, String(temp_valor));
  adicionaValorSensor(valueRange, 2, String(hum_valor));
  adicionaValorSensor(valueRange, 3, String(pluv_valor));
  adicionaValorSensor(valueRange, 4, String(WM1_R_VALOR));
  adicionaValorSensor(valueRange, 5, String(WM1_CB_VALOR));
  adicionaValorSensor(valueRange, 6, String(cap1_valor));
  adicionaValorSensor(valueRange, 7, String(vazio1));
  adicionaValorSensor(valueRange, 8, String(cap2_valor));
  adicionaValorSensor(valueRange, 9, String(vazio2));
  adicionaValorSensor(valueRange, 10, String(cap3_valor));
  adicionaValorSensor(valueRange, 11, String(vazio3));
  adicionaValorSensor(valueRange, 12, String(cap4_valor));
  adicionaValorSensor(valueRange, 13, String(h_esp_efetiva));

  bool success = GSheet.values.append(&response, spreadsheetId, PAGE, &valueRange);

  if (success) {
    response.toString(Serial, true);
    valueRange.clear();
    primeiroEnvioFeito = true;
  } else {
    Serial.println(GSheet.errorReason());
  }

  Serial.println("✅ Initial send completed.");
  Serial.println("---------------------------------------");
}

bool deveEnviarAgora() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return false;

  int hora = timeinfo.tm_hour;
  int minuto = timeinfo.tm_min;
  int segundo = timeinfo.tm_sec;

  bool minutoCerto = (minuto == 0 || minuto == 30 || (hora == 23 && minuto == 59));
  if (!minutoCerto) return false;

  if (!(segundo == 0 || segundo == 1 || segundo == 2)) return false;

  if (minuto == ultimoMinutoEnviado) return false;

  ultimoMinutoEnviado = minuto;
  return true;
}

// ---------------------------- SETUP & LOOP ----------------------------

void setup() {
  Serial.begin(115200);
  Serial.println();

  conectaWiFi();
  delay(50);
  configuraHoraNTP(); 
  delay(50);
  inicializaGoogleSheets();
  delay(50);
  SetupSensors();
  delay(50);
  enviaDadosInicial();
  delay(50);
}

void loop() {
  executaPluviometro();

  bool deveEnviar = deveEnviarAgora();

  if (GSheet.ready() && deveEnviar) {
    enviaDadosGoogleSheetsRegular();
    delay(50);
  }
}
