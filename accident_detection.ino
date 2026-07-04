#include<SoftwareSerial.h> 
#include <TinyGPS++.h>

// Software Serialpins
SoftwareSerialgpsSerial(4, 3); //RX, TX for GPS SoftwareSerialgsmSerial(7, 8); //RX, TX for GSM

TinyGPSPlusgps;

// ADXL335 pins constintxPin= A0; constintyPin= A1; constintzPin= A2;

// Thresholdforaccidentdetection(tunebased on testing) float threshold= 1.8; // g-forcethreshold

void setup() { Serial.begin(9600);
 gpsSerial.begin(9600); gsmSerial.begin(9600);

Serial.println("AccidentDetection System Starting..."); delay(2000);

// GSM Initialization gsmSerial.println("AT"); delay(1000);
gsmSerial.println("AT+CMGF=1"); // Set SMStextmode delay(1000);
gsmSerial.println("AT+CNMI=1,2,0,0,0"); delay(1000);
Serial.println("GSM Module Ready");
}

voidloop(){
// Readsensorvalues
int xRead = analogRead(xPin); 
int yRead = analogRead(yPin); intzRead=analogRead(zPin);

// Convert to g-force(approximation)
 
float X = (xRead - 512) / 102.3; float Y = (yRead - 512) / 102.3; float Z=(zRead- 512) / 102.3;

// Calculatetotalaccelerationmagnitude float totalAcc=sqrt(X* X+Y* Y+Z* Z);

Serial.print("Acceleration:"); Serial.println(totalAcc);

// Check if impactexceedsthreshold if (totalAcc > threshold) { Serial.println("AccidentDetected!"); sendAlertWithLocation(); delay(15000); // Avoidmultiplealerts
}

// ContinuouslyreadGPSdata 
while(gpsSerial.available()) 
{ gps.encode(gpsSerial.read());
}
}

voidsendAlertWithLocation(){ if (gps.location.isValid()) { float lat =gps.location.lat(); float lon=gps.location.lng();

Serial.print("Latitude:"); Serial.println(lat, 6); Serial.print("Longitude:"); Serial.println(lon, 6);

// Send SMS via GSMmodule gsmSerial.println("AT+CMGF=1"); delay(1000);
gsmSerial.println("AT+CMGS=\"+918131932857\""); // Replacewithyour phonenumber delay(1000);
gsmSerial.print(" Accident Detected!\n"); gsmSerial.print("Location:https://maps.google.com/?q="); gsmSerial.print(lat, 6);
gsmSerial.print(","); 
gsmSerial.print(lon, 6);
gsmSerial.write(26); // 
ASCIIcodefor Ctrl+Z(send SMS) delay(5000);
 
Serial.println("SMS Sent Successfully!");
} else{
Serial.println("GPSnotready...retrying");
}
}
