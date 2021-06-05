#include <TinyGPS.h>

TinyGPS gps;

char inChar = 0;
float lat, lon;
String message = "Hi";
String str;
float homeLat = 11.813102;
float homeLon = 13.205505;
float thresholdDistance = 14.00; //m
int k =0;
String online = "device is online succesfully";
String outrange = "Asset is moving out of range";
String locate = "find your asset here";
//char phone_no[]="08061683708"; // array to store phone number
char phone_no[]="07066469794";
int var = 0;
unsigned long previousMillis = 0;       
unsigned long prevMillis = 0;
// constants won't change:
const long interval = 4000;           // interval at which to blink (milliseconds)
const long inter = 8000;           // interval at which to blink (milliseconds)
const long del = 3000;
int i =0;
void setup()  
{
  Serial.begin(9600);
  Serial.println("Hello!");
  Serial1.begin(9600);
  Serial2.begin(115200);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
}
void getData();
void text(String msg);
void text2(String sg);
void loop(){
  unsigned long currentMillis = millis();
  unsigned long currMillis = millis();
if (currentMillis - previousMillis >= del) // function to send out range sms 
  {
   Serial.println("how you");   
    getData();
    previousMillis = currentMillis;
 }
   if (currMillis - prevMillis >= interval && var ==1) // function to send out range sms 
  {
    if(i==0){
    // save the last time you blinked the LED
    prevMillis = currentMillis;
    Serial.println("out of range sms");
 //send out of range sms
 text("Asset moving out of range");
//var =2;
i = 2;
}

}
 if (currMillis - prevMillis >= inter && i ==2) //function to send gps coordinate
 {
    // save the last time you blinked the LED
    prevMillis = currMillis;
 //send location sms
 Serial.println("location ");
 text2("location");
 }

if(var ==0 && i==2){
  i =0;
}

}
void getData()      
{
  while(true)
  {
    if(Serial1.available()>0)   // check for gps data
    {
       if(gps.encode(Serial1.read()))  // encode gps data
       break;
    }
  }  
  
    gps.f_get_position(&lat,&lon); // get latitude and longitude
    Serial.println("In getdata");
    checkDist(lat, lon);

}


//end displayInfo()


void checkDist(float ltd , float lon)
{

   float dist = HaverSine(homeLat,homeLon,ltd,lon);
  
  if (dist >= thresholdDistance )
  {
    
    Serial.print("Subject is ");
    Serial.print(dist - thresholdDistance, 6);
    Serial.println("M beyond threshold");   
    Serial.println("sound an alarm");
    digitalWrite(10, HIGH);
    digitalWrite(9, HIGH);  
    var =1;
    unsigned long currentMillis = millis();
  }
else
  {
        Serial.println("Subject within threshold, no worries!");
        Serial.println("go sleep");
        digitalWrite(9, HIGH);
        digitalWrite(10, HIGH);
        var =0;
  }



  
}
long HaverSine(float lat1, float lon1, float lat2, float lon2)
{
 float ToRad = PI / 180.0;
 float R = 6371000;   // radius earth in m

 float dLat = (lat2-lat1) * ToRad;
 float dLon = (lon2-lon1) * ToRad; 

 float a = sin(dLat/2) * sin(dLat/2) +
       cos(lat1 * ToRad) * cos(lat2 * ToRad) * 
       sin(dLon/2) * sin(dLon/2); 
       
 float c = 2 * atan2(sqrt(a), sqrt(1-a)); 

 float d = R * c;
 Serial.print(d, 6);
 return d;
}

void text(String msg) // function to send sms
{
var = 2;
Serial2.println("AT+CMGF=1");    
delay(2000);
Serial2.print("AT+CMGS=\"");
Serial2.print(phone_no); 
Serial2.write(0x22);
Serial2.write(0x0D);  // hex equivalent of Carraige return    
Serial2.write(0x0A);  // hex equivalent of newline
delay(2000);
Serial2.print(msg);
delay(500);
Serial2.println (char(26));//the ASCII code of the ctrl+z is 26
}


void text2(String sg) // function to send sms
{  
Serial2.println("AT+CMGF=1");    
delay(2000);
Serial2.print("AT+CMGS=\"");
Serial2.print(phone_no); 
Serial2.write(0x22);
Serial2.write(0x0D);  // hex equivalent of Carraige return    
Serial2.write(0x0A);  // hex equivalent of newline
delay(2000);
Serial2.print(sg);
Serial2.write(0x0A);
Serial2.print("https://www.google.com/maps/?q=");
Serial2.print(lat, 6);
Serial2.print(",");
Serial2.print(lon, 6);
delay(500);
Serial2.println (char(26));//the ASCII code of the ctrl+z is 26
}
