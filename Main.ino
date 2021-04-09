#include <dht.h>
int ledpin=5;//Define the pin number 5
#define dataPin 8 // Defines pin number to which the sensor is connected
dht DHT; // Creats a DHT object
#define ADpin A1

#include <Arduino.h>
#include "DS1302.h"

#define Buzz_Pin 6        // Define the pin number 6 for Buzz

  int i=0;
  int j=0;
  int mins[2];
  int hs[2];
 
bool Always=false;                

int ADBuffer=0;
uint8_t ledLevel = 0;
int ledC=1;


void setup()
{
 pinMode(ledpin,OUTPUT);
int ADBuffer=0;
  Serial.begin(9600);
  pinMode(Buzz_Pin,OUTPUT);
  DS1302_Init();
  Serial.println("Welcome to use!");  
  DS1302_GetTime(&DS1302Buffer);
  Display_RTCC();
  Serial.println("Type \"help\" To Get The Help Information.");
}

//show the time
void Display_RTCC()
{
  
  if(DS1302Buffer.Year < 10)
  {
    Serial.print("200");
  }
  else
  {
    Serial.print("20");
  }
  Serial.print(DS1302Buffer.Year);
  Serial.print('-');
  Serial.print(DS1302Buffer.Month);
  Serial.print('-');
  Serial.print(DS1302Buffer.Day);

  Serial.print("   ");
// show the week
  switch(DS1302Buffer.Week)
  {
    case 1:
        Serial.println("Mon");         
        break;
    case 2:
        Serial.println("Tue");         
        break;
    case 3:
        Serial.println("Wed");         
        break;
    case 4:
        Serial.println("Thu");        
        break;
    case 5:
        Serial.println("Fri");         
        break;
    case 6:
        Serial.println("Sat");         
        break;
    case 7:
        Serial.println("Sun");         
        break;
    default : break;            
  }

  Serial.print(DS1302Buffer.Hour);
  Serial.print(':');
  Serial.print(DS1302Buffer.Minute);
  Serial.print(':');
  Serial.println(DS1302Buffer.Second);
}


//the function for setting time
void Set_Time(String &command)
{
  DS1302_ON_OFF(0);     
  DS1302Buffer.Year=command.substring(4,6).toInt();
  DS1302Buffer.Month=command.substring(7,9).toInt();
  DS1302Buffer.Day=command.substring(10,12).toInt();
  DS1302Buffer.Week=command.substring(13,14).toInt();
  DS1302Buffer.Hour=command.substring(15,17).toInt();
  DS1302Buffer.Minute=command.substring(18).toInt();
  DS1302_SetTime( DS1302_YEAR , DS1302Buffer.Year );
  DS1302_SetTime( DS1302_MONTH , DS1302Buffer.Month );
  DS1302_SetTime( DS1302_DAY , DS1302Buffer.Day );
  DS1302_SetTime( DS1302_WEEK , DS1302Buffer.Week );
  DS1302_SetTime( DS1302_HOUR , DS1302Buffer.Hour );
  DS1302_SetTime( DS1302_MINUTE , DS1302Buffer.Minute );
  DS1302_ON_OFF(1);
  Always=false;
  Serial.println("Set Done");
  Display_RTCC();   
}

void Set_Alarm(String input){
      hs[i] = input.substring(10,12).toInt();
      mins[i] = input.substring(13,15).toInt();
      i++;
}

void Show_Alarm(){
  for( int j=0;j<=1;j++){
    Serial.print(hs[j]); 
    Serial.print(":");
    Serial.println(mins[j]);
  }
}

//buzzing
void warning()
{
   unsigned char i,j,k;
   while(k<20) 
 { 
    for(i=0;i<50;i++)
    { 
      digitalWrite(Buzz_Pin,HIGH);
      delay(1);
      digitalWrite(Buzz_Pin,LOW);
      delay(1);
    } 
    for(i=0;i<80;i++)
    { 
      digitalWrite(Buzz_Pin,HIGH);
      delay(2);
      digitalWrite(Buzz_Pin,LOW);
      delay(2);
    } 
    k+=1;
  } 
}

//help function
void help()
{
  Serial.println("Type \"set YY MM DD WEEK HH MM\" To Set The Time!");
  Serial.println("Such As Type \"set 08 08 08 1 12 00\" To Set The Time As 2008-8-8 Monday 12:00");
  Serial.println("Type \"show\" To Display The Time Now!");
  Serial.println("Type \"always\" To Print The Time Every Second!");
  Serial.println("Type \"alarm\" To Check The Current Alarm");
  Serial.println("Welcome To Use!"); 
}

void loop()
{
  while(Serial.available())
    {String comdata =""; 
      while (Serial.available() > 0)  
   { 
    
     comdata += char(Serial.read());
     delay(2);
   }

      if(comdata=="A")
        {
            int readData = DHT.read11(dataPin); // Reads the data from the sensor
  float t = DHT.temperature; // Gets the values of the temperature
  float h = DHT.humidity; // Gets the values of the humidity
            Serial.print("Temperature = ");
          Serial.print(t);
  Serial.print(" *C ");
  Serial.print("    Humidity = ");
  Serial.print(h);
  Serial.println(" % ");
  
        }
      if(comdata=="B"){
            Serial.print("ledLevel= ");
    Serial.println(ledLevel);
      }
      
      if(comdata.length()==20 && comdata.startsWith("set"))
    {
      Set_Time(comdata);
    }
  if(comdata=="s")
  {
    
    Serial.println("The Time Now Is:");
    Always=false;
    Display_RTCC();
  }

//set alarm
 
  if(comdata.startsWith("set alarm "))
  {     
     Set_Alarm(comdata);
  } 
  
  if(comdata=="alarm")
  {
   Show_Alarm();
  }
    
  if(comdata=="help")
  {
    help();
  }
  
  if(comdata=="show")
  {
    Serial.println("The Time Now Is:");
    Always=false;
    Display_RTCC();
  }

  }
   DS1302_GetTime(&DS1302Buffer);       
  if(Always)
  {
    if(Flag_Time_Refresh == 1)
      {
          Flag_Time_Refresh = 0;
          Display_RTCC();
      }
  }
   
  // activate the alarm 
  if(DS1302Buffer.Hour==hs[0] && DS1302Buffer.Minute==mins[0] && DS1302Buffer.Second==0)
  {
     warning();
  }
  if(DS1302Buffer.Hour==hs[1] && DS1302Buffer.Minute==mins[1] && DS1302Buffer.Second==0)
  {
     warning();
  }

      ADBuffer=analogRead(ADpin);
 
    ledLevel = 255-(ADBuffer/4);
    pinMode(ledpin,OUTPUT);
  
        
 analogWrite(ledpin, ledLevel);



}
