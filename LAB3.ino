



#define ENABLE 5
#define DIRA 3
#define DIRB 4


#include <LiquidCrystal.h>
#include <Wire.h>
#include <DS3231.h>
#include "IRremote.h"

DS3231 clock;
RTCDateTime dt;
int receiver = 2; // Signal Pin of IR receiver to Arduino Digital Pin 11
/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

int buttonApin = 13;
int buttonBpin = 6;

byte leds = 0;
LiquidCrystal lcd(7,8,9,10,11,12);//LCD Interface Pins

 int i;
void setup() {
  //---set pin direction
  //Button Digital Inputs Setup
    //pinMode(ledPin, OUTPUT);
  pinMode(buttonApin, INPUT_PULLUP);  
  pinMode(buttonBpin, INPUT_PULLUP);  
  //DC Motor Setup
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  Serial.begin(9600);
  //RTC Setup
   Serial.println("Initialize RTC module");
  // Initialize DS3231
  clock.begin();
  // Send sketch compiling time to Arduino
  clock.setDateTime(__DATE__, __TIME__);  
  //IR Setup
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the receiver
  // set up the LCD's number of columns and rows:
  
  
}




void translateIR() // takes action based on IR code received

// describing Remote IR codes 

{

  switch(results.value)

  {
  case 0xFFA25D: Serial.println("POWER"); break;
  case 0xFFE21D: Serial.println("FUNC/STOP"); break;
  case 0xFF629D: Serial.println("VOL+"); break;
  case 0xFF22DD: Serial.println("FAST BACK");    break;
  case 0xFF02FD: Serial.println("PAUSE");    break;
  case 0xFFC23D: Serial.println("FAST FORWARD");   break;
  case 0xFFE01F: Serial.println("DOWN");    break;
  case 0xFFA857: Serial.println("VOL-");    break;
  case 0xFF906F: Serial.println("UP");    break;
  case 0xFF9867: Serial.println("EQ");    break;
  case 0xFFB04F: Serial.println("ST/REPT");    break;
  case 0xFF6897: Serial.println("0"); digitalWrite(ENABLE,LOW);   break;
  case 0xFF30CF: Serial.println("1");  analogWrite(ENABLE,80); lcd.setCursor(0,1) ;
  lcd.print("1/4 Speed"); break;
  case 0xFF18E7: Serial.println("2");  analogWrite(ENABLE,128);
  lcd.setCursor(0,1) ;
  lcd.print("1/2 Speed");  break;
  case 0xFF7A85: Serial.println("3");  analogWrite(ENABLE,180);
  lcd.setCursor(0,1) ;
  lcd.print("3/4 Speed");  break;
  case 0xFF10EF: Serial.println("4"); analogWrite(ENABLE,255);  lcd.setCursor(0,1) ;
  lcd.print("Full Speed"); break;
  case 0xFF38C7: Serial.println("5"); digitalWrite(ENABLE,HIGH);  lcd.setCursor(0,1) ;
  lcd.print("HIGH SPEED");   break;
  case 0xFF5AA5: Serial.println("6");    break;
  case 0xFF42BD: Serial.println("7");    break;
  case 0xFF4AB5: Serial.println("8");    break;
  case 0xFF52AD: Serial.println("9");    break;
  case 0xFFFFFFFF: Serial.println(" REPEAT");break;  

  default: 
    Serial.println(" other button   ");

  }// End Case

  delay(500); // Do not get immediate repeat


} //END translateIR




 
  void updateDisplay() {
   lcd.setCursor(0,0);
   lcd.print(dt.year);   lcd.print("-");
 lcd.print(dt.month);  lcd.print("-");
  lcd.print(dt.day);    lcd.print(" ");
  lcd.print(dt.hour);   lcd.print(":");
  lcd.print(dt.minute); lcd.print(":");
  lcd.print(dt.second); lcd.println("");
  if(digitalRead(DIRA) == HIGH )
  {
    lcd.setCursor(0,1);
    lcd.print("C");
    delay(100);
  }
  if(digitalRead(DIRA) == LOW )
  {
    lcd.setCursor(0,1);
    lcd.print("CC");
    delay(100);
  }
  if(results.value == 0xFF6897)
  {
    lcd.setCursor(5,1);
    lcd.print("0 SPEED");
    delay(100);
    } 
    if(results.value == 0xFF30CF)
  {
    lcd.setCursor(5,1);

    lcd.print("1/4 SPEED");
    delay(100);
    }
     if(results.value == 0xFF18E7)
  {
    lcd.setCursor(5,1);

    lcd.print("1/2 SPEED");
    delay(100);
    }  
   if(results.value == 0xFF7A85)
  {
    lcd.setCursor(5,1);

    lcd.print("3/4 SPEED");
    delay(100);
    }  
   if(results.value == 0xFF10EF)
  {
    lcd.setCursor(5,1);

    lcd.print("FULL SPEED");
    delay(100);
    }

     if(digitalRead(ENABLE) == LOW)
  {
    lcd.setCursor(5,1);

    lcd.print("0 Speed");
    delay(100);
    }
  }



  
void loop() { 
   updateDisplay(); 
   dt = clock.getDateTime();
   lcd.begin(16,2);
   lcd.setCursor(2,0) ;

  if (digitalRead(buttonApin) == LOW)
  {
   // digitalWrite(ledPin, HIGH);
    Serial.println("BUTTON LOW");
    digitalWrite(DIRA,HIGH); //one way
    digitalWrite(DIRB,LOW);
    lcd.setCursor(0,1) ;
    lcd.print("CLOCKWISE");
    delay(1000);
    
    
  }
  if (digitalRead(buttonBpin) == LOW)
  {
  //  digitalWrite(ledPin, LOW);
    Serial.println("BUTTON HIGH");
    digitalWrite(DIRA,LOW);  //reverse
    digitalWrite(DIRB,HIGH);
    lcd.setCursor(0,1) ;
    lcd.print("COUNTERCLOCK");
    delay(1000);
  }
  
  if (irrecv.decode(&results)) // have we received an IR signal?

  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
   
}
   
