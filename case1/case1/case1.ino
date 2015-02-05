
#include <SPI.h>
#include <MFRC522.h>


#define SS_PIN 10
#define RST_PIN 9
#define Relay 8
#define Ba 7
#define Bb 6
#define Bc 5

#define LED1 4
#define LED2 3
#define LED3 2

MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.
MFRC522::MIFARE_Key key;

class CardBank
{
  
  public:
   byte uidsize;
   byte uid[8];
   byte Bank;
   int counter;
   CardBank()
   {
     uidsize=0;
     Bank=0;
     counter=0;
   }
};

CardBank Cards[9];
int CardCounter=0;
byte Bank[3]={0,0,0};


byte BabuttonState;             // the current reading from the input pin
boolean BalastButtonState = LOW;   // the previous reading from the input pin
byte BbbuttonState;             // the current reading from the input pin
boolean BblastButtonState = LOW;   // the previous reading from the input pin
byte BcbuttonState;             // the current reading from the input pin
boolean BclastButtonState = LOW;   // the previous reading from the input pin
byte BaFlag=0;
byte BbFlag=0;
byte BcFlag=0;
byte Mode=0;
byte RelayMode=0;


// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
long RelayOnTime=0;


void setup() {
        Serial.begin(9600);        // Initialize serial communications with the PC
        while (!Serial);           // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
        SPI.begin();               // Init SPI bus
        mfrc522.PCD_Init();        // Init MFRC522 card
        mfrc522.PICC_HaltA();
        Serial.println("Warning: this example overwrites the UID of your UID changeable card, use with care!");
        
        // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
        for (byte i = 0; i < 6; i++) {
                key.keyByte[i] = 0xFF;
        }
        
        pinMode(Relay, OUTPUT); 
        pinMode(Ba, INPUT);
        pinMode(Bb, INPUT);
        pinMode(Bc, INPUT);
        pinMode(LED1, OUTPUT); 
        pinMode(LED2, OUTPUT); 
        pinMode(LED3, OUTPUT);        
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW); 
        digitalWrite(LED3, LOW);  
}

// Setting the UID can be as simple as this:
//void loop() {
//    byte newUid[] = NEW_UID;
//    if ( mfrc522.MIFARE_SetUid(newUid, (byte)4, true) ) {
//      Serial.println("Wrote new UID to card.");
//    }
//    delay(1000);
//}

// But of course this is a more proper approach
void loop() {

        // read the state of the switch into a local variable:
        byte reading = digitalRead(Ba);
      
        // check to see if you just pressed the button 
        // (i.e. the input went from LOW to HIGH),  and you've waited 
        // long enough since the last press to ignore any noise:  
      
        // If the switch changed, due to noise or pressing:
        if (reading != BalastButtonState) {
          // reset the debouncing timer
          lastDebounceTime = millis();
        } 
        
        if ((millis() - lastDebounceTime) > debounceDelay) {
          // whatever the reading is at, it's been there for longer
          // than the debounce delay, so take it as the actual current state:
      
          // if the button state has changed:
          if (reading != BabuttonState) {
            BabuttonState = reading;
      
            // only toggle the LED if the new button state is HIGH
            if (BabuttonState == HIGH) {
              BaFlag=1;
            }
          }
        }      
        BalastButtonState = reading;
  
        // read the state of the switch into a local variable:
        reading = digitalRead(Bb);
      
        // check to see if you just pressed the button 
        // (i.e. the input went from LOW to HIGH),  and you've waited 
        // long enough since the last press to ignore any noise:  
      
        // If the switch changed, due to noise or pressing:
        if (reading != BblastButtonState) {
          // reset the debouncing timer
          lastDebounceTime = millis();
        } 
        
        if ((millis() - lastDebounceTime) > debounceDelay) {
          // whatever the reading is at, it's been there for longer
          // than the debounce delay, so take it as the actual current state:
      
          // if the button state has changed:
          if (reading != BbbuttonState) {
            BbbuttonState = reading;
      
            // only toggle the LED if the new button state is HIGH
            if (BbbuttonState == HIGH) {
              BbFlag=1;
            }
          }
        }      
        BblastButtonState = reading;
        
        // read the state of the switch into a local variable:
        reading = digitalRead(Bc);
      
        // check to see if you just pressed the button 
        // (i.e. the input went from LOW to HIGH),  and you've waited 
        // long enough since the last press to ignore any noise:  
      
        // If the switch changed, due to noise or pressing:
        if (reading != BclastButtonState) {
          // reset the debouncing timer
          lastDebounceTime = millis();
        } 
        
        if ((millis() - lastDebounceTime) > debounceDelay) {
          // whatever the reading is at, it's been there for longer
          // than the debounce delay, so take it as the actual current state:
      
          // if the button state has changed:
          if (reading != BcbuttonState) {
            BcbuttonState = reading;
      
            // only toggle the LED if the new button state is HIGH
            if (BcbuttonState == HIGH) {
              BcFlag=1;
            }
          }
        }      
        BclastButtonState = reading;        
  
        if(BaFlag==1)
        {
          BaFlag=0;
          Mode=1;
          Serial.println("Set Groud A." );
        }
        if(BbFlag==1)
        {
          BbFlag=0;
          Mode=2;
          Serial.println("Set Groud B." );
        }
        if(BcFlag==1)
        {
          BcFlag=0;
          Mode=3;
          Serial.println("Set Groud C." );
        }        
        
        
        //Relay control
        if(RelayMode==1)
        {
          if ((millis() - RelayOnTime) > 1000*60*30)
          {
            digitalWrite(Relay, LOW); 
            RelayMode=0;
            Serial.println("Relay OFF" );
          }  
        }
        else if(RelayMode==2)
        {
          if ((millis() - RelayOnTime) > 1000*60*60)
          {
            digitalWrite(Relay, LOW); 
            RelayMode=0;
            Serial.println("Relay OFF" );
          }            
        }
        else if(RelayMode==3)
        {
          if ((millis() - RelayOnTime) > 1000*60*120)
          {
            digitalWrite(Relay, LOW); 
            RelayMode=0;
            Serial.println("Relay OFF" );
          }            
        }
  
        if(Mode==0)
        {
          // Look for new cards, and select one if present
      	  if ( ! mfrc522.PICC_IsNewCardPresent()) {
      	        	return;
      	  }
      
      	  // Select one of the cards
      	  if ( ! mfrc522.PICC_ReadCardSerial()) {
      		  return;
      	  }
          mfrc522.PICC_HaltA();
          mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
          // Now a card is selected. The UID and SAK is in mfrc522.uid.
          
          // Dump UID
          Serial.print("Card UID:");
          for (byte i = 0; i < mfrc522.uid.size; i++) {
                  Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                  Serial.print(mfrc522.uid.uidByte[i], HEX);
          } 
          Serial.println();
          
          for(byte j=0;j<9;j++)
          {
            boolean checkoldcard=0;
            if(Cards[j].Bank!=0)
            {
              for (byte i = 0; i < mfrc522.uid.size; i++) {
                  if(mfrc522.uid.uidByte[i]==Cards[j].uid[i])
                    checkoldcard=1;
                   else
                    checkoldcard=0;
              } 
              if(checkoldcard==1)
              {
                Serial.println("Card find!");
                RelayOnTime=millis();
                if(Cards[j].Bank==1)
                  {RelayMode=1;  Serial.println("Mode 1!"); digitalWrite(Relay, HIGH); Serial.println("Relay ON" );}
                if(Cards[j].Bank==2)
                  {RelayMode=2;  Serial.println("Mode 2!"); digitalWrite(Relay, HIGH); Serial.println("Relay ON" );}
                if(Cards[j].Bank==3)
                  {RelayMode=3;  Serial.println("Mode 3!"); digitalWrite(Relay, HIGH); Serial.println("Relay ON" );}
                break;
              }
            }
          }          
          
          //delay(2000);
          //digitalWrite(Relay, HIGH); 
          //delay(2000);
          //digitalWrite(Relay, LOW); 
          
        }
        else if(Mode==1)
        {
          if(Bank[0]==0)   digitalWrite(LED1, LOW);  digitalWrite(LED2, LOW);   digitalWrite(LED3, LOW);  
          if(Bank[0]==1)   digitalWrite(LED1, HIGH);  digitalWrite(LED2, LOW);   digitalWrite(LED3, LOW);  
          if(Bank[0]==2)   digitalWrite(LED1, HIGH);  digitalWrite(LED2, HIGH);   digitalWrite(LED3, LOW);  
          if(Bank[0]==3)   digitalWrite(LED1, HIGH);  digitalWrite(LED2, HIGH);   digitalWrite(LED3, HIGH);            
          
      	if ( ! mfrc522.PICC_IsNewCardPresent()) {
      		return;
      	}
      
      	// Select one of the cards
      	if ( ! mfrc522.PICC_ReadCardSerial()) {
      		return;
      	}
          mfrc522.PICC_HaltA();
          mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
          Serial.println("Get Card" );
          for(byte j=0;j<9;j++)
          {
            boolean checkoldcard=0;
            if(Cards[j].Bank!=0)
            {
              for (byte i = 0; i < mfrc522.uid.size; i++) {
                  if(mfrc522.uid.uidByte[i]==Cards[j].uid[i])
                    checkoldcard=1;
                   else
                    checkoldcard=0;
              } 
              if(checkoldcard==1)
              {
                Bank[Cards[j].Bank-1]--;
                Cards[j].Bank=0;
                Serial.println("Old Card detected" );
              }
            }
          }
          if(Bank[0]!=3)
          {
            
            for(byte j=0;j<9;j++)
            {
              if(Cards[j].Bank==0)
              {
                for (byte i = 0; i < mfrc522.uid.size; i++) {
                  Cards[j].uid[i]=mfrc522.uid.uidByte[i];
                }                 
                Cards[j].Bank=1;
                Bank[0]++;
                CardCounter++;
                Cards[j].counter=CardCounter;
                Serial.println("Save New Card to Group A" );
                break;
              }
            }
            
          }
          else
          {
            Serial.println("Del old Card and Save New Card to Group A" );
            int smallestcount=1024;
            for(byte j=0;j<9;j++)
            {
              if(Cards[j].Bank==1)
              {
                if(smallestcount>Cards[j].counter)
                {
                  smallestcount=Cards[j].counter;
                }
              }
            
            }
            for(byte j=0;j<9;j++)
            {

              if(Cards[j].Bank==1)
              {
                if(smallestcount==Cards[j].counter)
                {
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                    Cards[j].uid[i]=mfrc522.uid.uidByte[i];
                  }                 
                  Cards[j].Bank=1;
                  CardCounter++;
                  Cards[j].counter=CardCounter;
                  Serial.println("Save New Card to Group A" );
                }
              }              
            }                    
          }
          Mode=0;
          digitalWrite(LED1, LOW);  digitalWrite(LED2, LOW);   digitalWrite(LED3, LOW);  


        }
        else if(Mode==2)/////////////////////////////////////////////////////////////////////////////////////////////////////////////
        {
          if(Bank[1]==0)   digitalWrite(LED1, LOW);  digitalWrite(LED2, LOW);   digitalWrite(LED3, LOW);  
          if(Bank[1]==1)   digitalWrite(LED1, HIGH);  digitalWrite(LED2, LOW);   digitalWrite(LED3, LOW);  
          if(Bank[1]==2)   digitalWrite(LED1, HIGH);  digitalWrite(LED2, HIGH);   digitalWrite(LED3, LOW);  
          if(Bank[1]==3)   digitalWrite(LED1, HIGH);  digitalWrite(LED2, HIGH);   digitalWrite(LED3, HIGH);            
          
      	if ( ! mfrc522.PICC_IsNewCardPresent()) {
      		return;
      	}
      
      	// Select one of the cards
      	if ( ! mfrc522.PICC_ReadCardSerial()) {
      		return;
      	}
          mfrc522.PICC_HaltA();
          mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
          Serial.println("Get Card" );
          for(byte j=0;j<9;j++)
          {
            boolean checkoldcard=0;
            if(Cards[j].Bank!=0)
            {
              for (byte i = 0; i < mfrc522.uid.size; i++) {
                  if(mfrc522.uid.uidByte[i]==Cards[j].uid[i])
                    checkoldcard=1;
                   else
                    checkoldcard=0;
              } 
              if(checkoldcard==1)
              {
                Bank[Cards[j].Bank-1]--;
                Cards[j].Bank=0;
                Serial.println("Old Card detected" );
              }
            }
          }
          if(Bank[1]<3)
          {
            
            for(byte j=0;j<9;j++)
            {
              if(Cards[j].Bank==0)
              {
                for (byte i = 0; i < mfrc522.uid.size; i++) {
                  Cards[j].uid[i]=mfrc522.uid.uidByte[i];
                }                 
                Cards[j].Bank=2;
                Bank[1]++;
                CardCounter++;
                Cards[j].counter=CardCounter;
                Serial.println("Save New Card to Group B" );
                break;
              }
            }
            
          }
          else
          {
            Serial.println("Del old Card and Save New Card to Group B" );
            int smallestcount=1024;
            for(byte j=0;j<9;j++)
            {
              if(Cards[j].Bank==2)
              {
                if(smallestcount>Cards[j].counter)
                {
                  smallestcount=Cards[j].counter;
                }
              }
              
            }     
            for(byte j=0;j<9;j++)
            {

              if(Cards[j].Bank==2)
              {
                if(smallestcount==Cards[j].counter)
                {
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                    Cards[j].uid[i]=mfrc522.uid.uidByte[i];
                  }                 
                  Cards[j].Bank=2;
                  CardCounter++;
                  Cards[j].counter=CardCounter;
                  Serial.println("Save New Card to Group B" );
                }
              }              
            }      
          }
          Mode=0;
          digitalWrite(LED1, LOW);  digitalWrite(LED2, LOW);   digitalWrite(LED3, LOW);            
        }
        else if(Mode==3)/////////////////////////////////////////////////////////////////////////////////////////////////////////////
        {
          if(Bank[2]==0)   digitalWrite(LED1, LOW);  digitalWrite(LED2, LOW);   digitalWrite(LED3, LOW);  
          if(Bank[2]==1)   digitalWrite(LED1, HIGH);  digitalWrite(LED2, LOW);   digitalWrite(LED3, LOW);  
          if(Bank[2]==2)   digitalWrite(LED1, HIGH);  digitalWrite(LED2, HIGH);   digitalWrite(LED3, LOW);  
          if(Bank[2]==3)   digitalWrite(LED1, HIGH);  digitalWrite(LED2, HIGH);   digitalWrite(LED3, HIGH);            
          
      	if ( ! mfrc522.PICC_IsNewCardPresent()) {
      		return;
      	}
      
      	// Select one of the cards
      	if ( ! mfrc522.PICC_ReadCardSerial()) {
      		return;
      	}
          mfrc522.PICC_HaltA();
          mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
          Serial.println("Get Card" );
          for(byte j=0;j<9;j++)
          {
            boolean checkoldcard=0;
            if(Cards[j].Bank!=0)
            {
              for (byte i = 0; i < mfrc522.uid.size; i++) {
                  if(mfrc522.uid.uidByte[i]==Cards[j].uid[i])
                    checkoldcard=1;
                   else
                    checkoldcard=0;
              } 
              if(checkoldcard==1)
              {
                Bank[Cards[j].Bank-1]--;
                Cards[j].Bank=0;
                Serial.println("Old Card detected" );
              }
            }
          }
          if(Bank[2]<3)
          {
            
            for(byte j=0;j<9;j++)
            {
              if(Cards[j].Bank==0)
              {
                for (byte i = 0; i < mfrc522.uid.size; i++) {
                  Cards[j].uid[i]=mfrc522.uid.uidByte[i];
                }                 
                Cards[j].Bank=3;
                Bank[2]++;
                CardCounter++;
                Cards[j].counter=CardCounter;
                Serial.println("Save New Card to Group C" );
                break;
              }
            }
            
          }
          else
          {
            Serial.println("Del old Card and Save New Card to Group C" );
            int smallestcount=1024;
            for(byte j=0;j<9;j++)
            {
              if(Cards[j].Bank==3)
              {
                if(smallestcount>Cards[j].counter)
                {
                  smallestcount=Cards[j].counter;
                }
              }
              
            }     
            for(byte j=0;j<9;j++)
            {

              if(Cards[j].Bank==3)
              {
                if(smallestcount==Cards[j].counter)
                {
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                    Cards[j].uid[i]=mfrc522.uid.uidByte[i];
                  }                 
                  Cards[j].Bank=3;
                  CardCounter++;
                  Cards[j].counter=CardCounter;
                  Serial.println("Save New Card to Group C" );
                }
              }              
            }      
          }
          Mode=0;
          digitalWrite(LED1, LOW);  digitalWrite(LED2, LOW);   digitalWrite(LED3, LOW);              
        }        
}
