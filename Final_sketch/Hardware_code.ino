#include <TinyGPS++.h> 
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <MadgwickAHRS.h>

Madgwick filter;
TinyGPSPlus gps; 
SoftwareSerial ss(12, 13); 

const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float MagX, MagY, MagZ;
float roll, pitch, yaw;

const char* ssid = "Sahil's S23";
const char* password = "sahil2807";

const char* serverAddress = "192.168.7.219"; // or hostname
const int serverPort = 8080;

float latitude , longitude;
// int year , month , date, hour , minute , second;
// String date_str;
// String time_str;
String lat_str;
String lng_str;
String model = "Maruti swift";
String number = "CH01AA1234";
int pm;

WiFiClient client;

void setup() {
  Serial.begin(115200);

  ss.begin(9600);

  Wire.begin(4, 5); // Use GPIO 4 (D2) for SDA and GPIO 5 (D1) for SCL
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  // Configure Accelerometer Sensitivity - Full Scale Range (±16 G)
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0x18);
  Wire.endTransmission(true);

  // Configure Gyro Sensitivity - Full Scale Range (±2,000°/s)
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);
  Wire.write(0x18);
  Wire.endTransmission(true);

  // Initialize Madgwick filter
  filter.begin(100); // Sample rate of 100 Hz

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting to WI-FI");
  }

  Serial.println("");
  Serial.println("Wi-Fi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!client.connected()) {
    if (client.connect(serverAddress, serverPort)) {
      Serial.println("Connected to TCP server");
    } else {
      Serial.println("Failed to connect to TCP server");
      delay(1000);
    }
  }
  if (client.connected()) {
    while (ss.available() > 0){
    if (gps.encode(ss.read())) //read gps data
    {
      if (gps.location.isValid()) //check whether gps location is valid
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6); // latitude location is stored in a string
        longitude = gps.location.lng();
        lng_str = String(longitude , 6); //longitude location is stored in a string
      }
      // if (gps.date.isValid()) //check whether gps date is valid
      // {
      //   date_str = "";
      //   date = gps.date.day();
      //   month = gps.date.month();
      //   year = gps.date.year();
      //   if (date < 10)
      //     date_str = '0';
      //   date_str += String(date);// values of date,month and year are stored in a string
      //   date_str += " / ";

      //   if (month < 10)
      //     date_str += '0';
      //   date_str += String(month); // values of date,month and year are stored in a string
      //   date_str += " / ";
      //   if (year < 10)
      //     date_str += '0';
      //   date_str += String(year); // values of date,month and year are stored in a string
      // }
      // if (gps.time.isValid())  //check whether gps time is valid
      // {
      //   time_str = "";
      //   hour = gps.time.hour();
      //   minute = gps.time.minute();
      //   second = gps.time.second();
      //   minute = (minute + 30); // converting to IST
      //   if (minute > 59)
      //   {
      //     minute = minute - 60;
      //     hour = hour + 1;
      //   }
      //   hour = (hour + 5) ;
      //   if (hour > 23)
      //     hour = hour - 24;   // converting to IST
      //   if (hour >= 12)  // checking whether AM or PM
      //     pm = 1;
      //   else
      //     pm = 0;
      //   hour = hour % 12;
      //   if (hour < 10)
      //     time_str = '0';
      //   time_str += String(hour); //values of hour,minute and time are stored in a string
      //   time_str += " : ";
      //   if (minute < 10)
      //     time_str += '0';
      //   time_str += String(minute); //values of hour,minute and time are stored in a string
      //   time_str += " : ";
      //   if (second < 10)
      //     time_str += '0';
      //   time_str += String(second); //values of hour,minute and time are stored in a string
      //   if (pm == 1)
      //     time_str += " PM ";
      //   else
      //     time_str += " AM ";
      // }
    }
  }
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // ±16 G range
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0;
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0;

    AccX = AccX - 0.13;
    AccY = AccY - 3.99;
    AccZ = AccZ - 3.99;

    // Read gyroscope data
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyroX = (Wire.read() << 8 | Wire.read()) / 200; // ±2,000°/s range
    GyroY = (Wire.read() << 8 | Wire.read()) / 200;
    GyroZ = (Wire.read() << 8 | Wire.read()) / 200;
    
    // Read magnetometer data
    Wire.beginTransmission(MPU);
    Wire.write(0x03);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    MagX = (Wire.read() << 8 | Wire.read()) / 16384.0; // ±16 G range
    MagY = (Wire.read() << 8 | Wire.read()) / 16384.0;
    MagZ = (Wire.read() << 8 | Wire.read()) / 16384.0;

    // Apply sensor fusion using Madgwick filter
    filter.updateIMU(GyroX, GyroY, GyroZ, AccX, AccY, AccZ);
    roll = filter.getRoll();
    pitch = filter.getPitch();
    yaw = filter.getYaw();

    // Send the orientation angles to the connected client
    String data1 = String(AccX)+","+String(AccY)+","+String(AccZ)+","+String(GyroX)+","+String(GyroY)+","+String(GyroZ)+","+String(roll)+","+String(pitch)+","+String(yaw);
    String data2 = lat_str+","+lng_str+","+model+","+number;
    String to_send = data1+","+data2;
    client.println(to_send);
  }
  delay(10);
}
