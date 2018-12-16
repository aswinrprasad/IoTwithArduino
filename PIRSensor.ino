/*  
 * This code has been changed for the SIM800 module 
 * Bifurcated from  IOT Ver2 = SIM800_IOT_Ver2 
 * IOT_SMS code + PIR code + SWITCH code = IOT_SMS_PIR_SWITCH code 
 */

// Include elapsedMillis library 

#include <elapsedMillis.h>                  //These special variable types automatically increase as time elapses. 
                                            //It's easy to check if a certain time has elapsed, while your program performs other work or checks for user input.                                             
#define Relay 12                            // choose the output pin for Relay
#define Switch 7                            // choose the input pin for Switch
#define PIR_output 5                        // choose the input pin for PIR sensor

elapsedMillis timeElapsed;
 
unsigned int interval = 5000;               // __ seconds delay. in milliseconds if there is no motion detected by PIR
String textMessage="OFF";                   // Variable to store text message
int Switch_position=0;                      // Variable to read Switch position
int PIR_Status=0;
long  int i=0;

void setup() 
{
  Serial.begin(9600);                       // Initializing serial commmunication 
  pinMode(Relay, OUTPUT);                   // declare Relay as output
  pinMode(Switch, INPUT);                   // declare Switch as input
  pinMode(PIR_output,INPUT);               // declare pir_output as input
  
  digitalWrite(Relay,LOW);                // By default the Relay is off

  digitalWrite(0, HIGH);                    // ----------- RESET SIM800------------------------//
  delay(1000);
  digitalWrite(0, LOW);                     // ----------- RESET SIM800------------------------//
 
  delay(20000);                             // Give time to your GSM shield to log on to network
  
//  Serial.print("IOT_SMS_PIR_SWITCH");
  delay(1000);
  Serial.print("AT+CMGF=1\r");              // AT command to set SIM800 to SMS mode
  delay(100);
  Serial.print("AT+CNMI=2,2,0,0,0\r");      // Set module to send SMS data to serial out upon receipt 
  delay(100);
  Serial.print("ATE0\r");                   //Turn off echo from GSM
  delay(300);
}

//----------------START OF LOOP----------------START OF LOOP----------------START OF LOOP----------------START OF LOOP----------------START OF LOOP---------//

void loop(){

  delay(100);
  Switch_position= digitalRead(Switch);            // Read digital input value of Switch
  PIR_Status= digitalRead(PIR_output);           // Read digital input value of PIR
  
  if (Switch_position== HIGH)                         // check if Switch input is HIGH 
      {         
        if(PIR_Status == HIGH)                      // If PIR is HIGH [which means, there is somebody around]
          {
            Light_ON();                        // Call Light_ON function
//          Serial.println(Switch_position);
//          Serial.print("PIR_ON");
            timeElapsed = 0;                    // reset the counter to 0 , so that the counting starts over...
            }
            
            else if ((timeElapsed > interval)&&(PIR_Status == LOW)) // If PIR is LOW after some delay [which means, if nobody is around after a specific duration]
            {
             Light_OFF();                          // Call Light_OFF function
//           Serial.println(Switch_position);
//           Serial.print("PIR_OFF");                       
            }
      }
      
      else if(Switch_position==LOW)              // check if Switch input is LOW
      {
        if(Serial.available()>0)
        {
          delay(10);
          textMessage = "";    
          delay(10);
          textMessage = Serial.readString();
          delay(100);
          Serial.println(textMessage);    
          delay(100);
          } 
          
          if(textMessage.indexOf("ON")>=0)        //If user has requested to Switch ON through SMS
          {
           Light_ON();                         // then ,Call Light_ON function
           // Serial.println(textMessage);
           // Serial.print("SMS_ON");
           // lampState = "on";
            delay(100);
            textMessage = "";    
            }
            
            if(textMessage.indexOf("OFF")>=0)     //If user has requested to Switch OFF through SMS
            {
              Light_OFF();                         // then ,Call Light_OFF function
           // Serial.println(textMessage);
           // Serial.print("SMS_OFF");
              //lampState = "off";  
              delay(100);
               //Serial.println("Relay set to OFF"); 
              textMessage = ""; 
              }          
  }
               i++;
               
         if(i>600){      
              //Serial.println("AT+CMGL=\"ALL\",0 ");
              delSMS();
              i=0;
              delay(3000);
              }
}
//----------------END OF LOOP----------------END OF LOOP----------------END OF LOOP----------------END OF LOOP----------------END OF LOOP---------//
  
void Light_ON(){
                 //Serial.println("motion detected");
                delay(100);
               // Turn on Relay and save current state
                digitalWrite(Relay, LOW);               //Relay should be LOW to switch ON the load
                delay(500);                        
              //textMessage = "";  
              }

void Light_OFF(){
              //Serial.println("motion NOT detected");
                delay(100);
             // Turn on Relay and save current state
               digitalWrite(Relay,HIGH);                 //Relay should be HIGH to switch ON the load
               delay(500);
               //textMessage = "";   
              //delay(200);
              }

  

void delSMS() {  // Delete All messages
             //mySerial.println( "AT+CMGF=1" ); // 
            //delay(1000);                                       
             Serial.print("AT+CMGDA=\""); 
             delay(100);
             Serial.println("DEL ALL\"");
             delay(500);
            //Serial.println( "All Messages Deleted" );
              }
