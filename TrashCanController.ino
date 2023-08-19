
// Adding A Header File For Using Of Display
#include <LiquidCrystal.h>

// Adding A Header File For Using Of Electrical Servo Motor
#include <Servo.h>

// Adding A Header File For Using Of GSM Module
#include <SoftwareSerial.h>

// Declaring Some Macros For Using Display
#define Display_RS_Pin 2
#define Display_EN_Pin 4
#define Display_D4_Pin 6
#define Display_D5_Pin 7
#define Display_D6_Pin 8
#define Display_D7_Pin 9
#define Display_NumberOfRows 4
#define Display_NumberOdColumns 20

// Declaring Some Macros For Using Keys
#define GlassKey_Pin 14
#define GlassKey_DefaultStatus HIGH
#define GlassKey_TriggeredStatus LOW
#define PlasticKey_Pin 15
#define PlasticKey_DefaultStatus HIGH
#define PlasticKey_TriggeredStatus LOW
#define OtherKey_Pin 16
#define OtherKey_DefaultStatus HIGH
#define OtherKey_TriggeredStatus LOW

// Declaring Some Macros For Using The Electrical Motor
#define Motor_Pin_One 3
#define Motor_Pin_Two 5
#define Motor_PowerOffValue 0
#define Motor_PowerOnValue 200

// Declaring Some Macros For Using The Electrical Servo Motor
#define ServoMotor_Pin 10
#define ServoMotor_StartPointDegree 0
#define ServoMotor_EndPointDegree 90

// Declaring Some Macros For Moving The Box With The Electrical Motor
#define Home_SectionOfBox 1383
#define Glass_SectionOfBox 1383
#define Plastic_SectionOfBox 1384
#define Other_SectionOfBox 1385

#if Home_SectionOfBox == Glass_SectionOfBox
#define Time_FromHomeSection_ToGlassSection 0
#define Time_FromGlassSection_ToHomeSection 0
#define Time_FromHomeSection_ToPlasticSection 5000
#define Time_FromPlasticSection_ToHomeSection 5000
#define Time_FromHomeSection_ToOtherSection 10000
#define Time_FromOtherSection_ToHomeSection 10000

#elif Home_SectionOfBox == Plastic_SectionOfBox
#define Time_FromHomeSection_ToGlassSection 0
#define Time_FromGlassSection_ToHomeSection 0
#define Time_FromHomeSection_ToPlasticSection 5000
#define Time_FromPlasticSection_ToHomeSection 5000
#define Time_FromHomeSection_ToOtherSection 10000
#define Time_FromOtherSection_ToHomeSection 10000

#elif Home_SectionOfBox == Other_SectionOfBox
#define Time_FromHomeSection_ToGlassSection 0
#define Time_FromGlassSection_ToHomeSection 0
#define Time_FromHomeSection_ToPlasticSection 5000
#define Time_FromPlasticSection_ToHomeSection 5000
#define Time_FromHomeSection_ToOtherSection 10000
#define Time_FromOtherSection_ToHomeSection 10000

#else
#endif

//Declaring Some Macros For Using GSM Module
#define GSM_RX_Pin 18
#define GSM_TX_Pin 19
#define GSM_BaudRate 9600

// Declaring Some Macros For Working With PIR Sensor
#define PIR_Pin 20
#define PIR_DefaultStatus LOW
#define PIR_TriggeredStatus HIGH

// Declaring Some Macros For Working With IR Sensor
#define IR_Pin 21
#define IR_DefaultStatus HIGH
#define IR_TriggeredStatus LOW

// Declaring A Display And Configuring Its Pins
LiquidCrystal Display((uint8_t)Display_RS_Pin, (uint8_t)Display_EN_Pin, (uint8_t)Display_D4_Pin, (uint8_t)Display_D5_Pin, (uint8_t)Display_D6_Pin, (uint8_t)Display_D7_Pin);

// Declaring An Electrical Servo Motor
Servo ServoMotor;

// Declaring A GSM Module
SoftwareSerial GSM((uint8_t)GSM_RX_Pin, (uint8_t)GSM_TX_Pin);

// Declaring A Function For Opening The Head Of The Box With The Electrical Servo Motor
void Opening_HeadOfBox(void);

// Declaring A Function For Closing The Head Of The Box With The Electrical Servo Motor
void Closing_HeadOfBox(void);

// Declaring A Function For Moving The Box From Any Section To Home Section With Electrical Motor
void Moving_BoxToHomeSection(unsigned short int);

// Declaring A Function For Moving The Box From Home Section To Any Section With Electrical Motor
void Moving_BoxToAnySection(unsigned short int);

// Declaring A Function For Moving The Trash To The Glass Section Of Box
void Moving_TrashToGlassSectionOfBox(void);

// Declaring A Function For Moving The Trash To The Plastic Section Of Box
void Moving_TrashToPlasticSectionOfBox(void);

// Declaring A Function For Moving The Trash To The Other Section Of Box
void Moving_TrashToOtherSectionOfBox(void);

// Declaring A Function For Showing Welcome Message To The User
void Showing_WelcomeMessageToUser(void);

// Declaring A Function For Showing Help Message For Using To User
void Showing_HelpMessageToUser(void);

// Declaring A Function For Sending Message With GSM Module
void Sending_Message(const char *, const char *);

// Declaring Some Useful Variable For Main Program
bool IsAvailable = false;
bool IsFull = false;

unsigned long int TheTime = (unsigned long int)0;
const unsigned long int ProgramTime_Empty = (unsigned long int)60000;
const unsigned long int ProgramTime_Full = (unsigned long int)10000;
const unsigned long int ProgramTime_Initializing = (unsigned long int)10000;

const char *PhoneNumber = "+989202969269";
const char *Message = "The Trash Can Is Full. Please Empty It.";

void setup(void) {
  // Configuring Display Information(Number Of Columns And Rows)
  Display.begin((uint8_t)Display_NumberOdColumns, (uint8_t)Display_NumberOfRows);
  // Clearing Everything Is Available On The Display
  Display.clear();
  // Configuring The Electrical Servo Motor Pin
  ServoMotor.attach((int)ServoMotor_Pin);
  ServoMotor.write((int)ServoMotor_StartPointDegree);
  // Configuring The GSM Module BaudRate
  GSM.begin((long int)GSM_BaudRate);
  // Clearing Everything Is Avilable On Buffer Of The GSM Module
  GSM.flush();
  // Configuring The Electrical Motor Pin
  pinMode((uint8_t)Motor_Pin_One, (uint8_t)OUTPUT);
  pinMode((uint8_t)Motor_Pin_Two, (uint8_t)OUTPUT);
  digitalWrite((uint8_t)Motor_Pin_One, (uint8_t)LOW);
  digitalWrite((uint8_t)Motor_Pin_Two, (uint8_t)LOW);
  // Configuring Keys
  pinMode((uint8_t)GlassKey_Pin, (uint8_t)INPUT_PULLUP);
  pinMode((uint8_t)PlasticKey_Pin, (uint8_t)INPUT_PULLUP);
  pinMode((uint8_t)OtherKey_Pin, (uint8_t)INPUT_PULLUP);
  // Configuring PIR Sensor
  pinMode((uint8_t)PIR_Pin, (uint8_t)INPUT);
  // Configuring IR Sensor
  pinMode((uint8_t)IR_Pin, (uint8_t)INPUT);
  // Showing Current Operation Message And Waiting 10 Seconds For Initializing Program
  Display.setCursor((uint8_t)0, (uint8_t)0);
  Display.print("                    ");
  Display.setCursor((uint8_t)0, (uint8_t)1);
  Display.print("   Initializing...  ");
  Display.setCursor((uint8_t)0, (uint8_t)2);
  Display.print("                    ");
  Display.setCursor((uint8_t)0, (uint8_t)3);
  Display.print("                    ");
  delay(ProgramTime_Initializing);
  Display.clear();
}

void loop(void) {
  // Checks If PIR Sensor Triggered, So If It Triggered Changes The Value Of A Variable
  if (digitalRead((uint8_t)PIR_Pin) == (int)PIR_TriggeredStatus) {
    IsAvailable = true;
    // This Code Checks The Trash Can Is Full Or Not
    if (digitalRead((uint8_t)IR_Pin) == (int)IR_TriggeredStatus) {
      IsFull = true;
    } else {
      IsFull = false;
    }
    // This Code Will Run When The PIR Sensor Be Triggered And The Trash Can Doesn't Be Full
    if (IsAvailable == true) {
      IsAvailable = false;
      Showing_WelcomeMessageToUser();
      // This Code Will Run If The Trash Can Is Not Fulled
      if ((!IsFull)) {
        // This Code Will Run For 30 Seconds
        TheTime = millis();
        while ((millis() <= (TheTime + ProgramTime_Empty))) {
          // For Handling Action Of GlassKey
          if (digitalRead((uint8_t)GlassKey_Pin) == (int)GlassKey_TriggeredStatus) {
            delay((unsigned long int)250);
            if (digitalRead((uint8_t)GlassKey_Pin) == (int)GlassKey_DefaultStatus) {
              Moving_TrashToGlassSectionOfBox();
            }
          }
          // For Handling Action Of PlasticKey
          else if (digitalRead((uint8_t)PlasticKey_Pin) == (int)PlasticKey_TriggeredStatus) {
            delay((unsigned long int)250);
            if (digitalRead((uint8_t)PlasticKey_Pin) == (int)PlasticKey_DefaultStatus) {
              Moving_TrashToPlasticSectionOfBox();
            }
          }
          // For Handling Action Of OtherKey
          else if (digitalRead((uint8_t)OtherKey_Pin) == (int)OtherKey_TriggeredStatus) {
            delay((unsigned long int)250);
            if (digitalRead((uint8_t)OtherKey_Pin) == (int)OtherKey_DefaultStatus) {
              Moving_TrashToOtherSectionOfBox();
            }
          }
          Showing_HelpMessageToUser();
        }
        // Clearing Everything Available On Display
        Display.clear();
      }
      // This Code Will Run If The Trash Can Is Fulled
      else {
        // This Code Will Run For 10 Seconds
        TheTime = millis();
        while ((millis() <= (TheTime + ProgramTime_Full))) {
          Display.setCursor((uint8_t)0, (uint8_t)0);
          Display.print("                    ");
          Display.setCursor((uint8_t)0, (uint8_t)1);
          Display.print("      Excuse Me     ");
          Display.setCursor((uint8_t)0, (uint8_t)2);
          Display.print("  TrashCan Is Full  ");
          Display.setCursor((uint8_t)0, (uint8_t)3);
          Display.print("                    ");
        }
        // This Code Sends A Message To Operator
        Sending_Message(PhoneNumber, Message);
        // Clearing Everything Available On Display
        Display.clear();
      }
    }
  }
}

// Configuring Body Of The Declared Function
void Opening_HeadOfBox(void) {
  for (int Counter = (int)ServoMotor_StartPointDegree; Counter <= (int)ServoMotor_EndPointDegree; Counter += (int)1) {
    ServoMotor.write((int)Counter);
    delay((unsigned long int)50);
  }
}

// Configuring Body Of The Declared Function
void Closing_HeadOfBox(void) {
  for (int Counter = (int)ServoMotor_EndPointDegree; Counter >= (int)ServoMotor_StartPointDegree; Counter -= (int)1) {
    ServoMotor.write((int)Counter);
    delay((unsigned long int)50);
  }
}

// Configuring Body Of The Declared Function
void Moving_BoxToHomeSection(unsigned short int Any_Section) {
  switch (Any_Section) {
    case ((unsigned short int)Glass_SectionOfBox):
      {
        if ((unsigned short int)Glass_SectionOfBox == (unsigned short int)Home_SectionOfBox) {
          break;
        } else {
          digitalWrite((uint8_t)Motor_Pin_One, (uint8_t)LOW);

          analogWrite((uint8_t)Motor_Pin_Two, (int)Motor_PowerOnValue);
          delay((unsigned long int)Time_FromGlassSection_ToHomeSection);
          analogWrite((uint8_t)Motor_Pin_Two, (int)Motor_PowerOffValue);

          break;
        }
      }
    case ((unsigned short int)Plastic_SectionOfBox):
      {
        if ((unsigned short int)Plastic_SectionOfBox == (unsigned short int)Home_SectionOfBox) {
          break;
        } else {
          digitalWrite((uint8_t)Motor_Pin_One, (uint8_t)LOW);

          analogWrite((uint8_t)Motor_Pin_Two, (int)Motor_PowerOnValue);
          delay((unsigned long int)Time_FromPlasticSection_ToHomeSection);
          analogWrite((uint8_t)Motor_Pin_Two, (int)Motor_PowerOffValue);

          break;
        }
      }
    case ((unsigned short int)Other_SectionOfBox):
      {
        if ((unsigned short int)Other_SectionOfBox == (unsigned short int)Home_SectionOfBox) {
          break;
        } else {
          digitalWrite((uint8_t)Motor_Pin_One, (uint8_t)LOW);

          analogWrite((uint8_t)Motor_Pin_Two, (int)Motor_PowerOnValue);
          delay((unsigned long int)Time_FromOtherSection_ToHomeSection);
          analogWrite((uint8_t)Motor_Pin_Two, (int)Motor_PowerOffValue);

          break;
        }
      }
  }
}

// Configuring Body Of The Declared Function
void Moving_BoxToAnySection(unsigned short int Any_Section) {
  switch (Any_Section) {
    case ((unsigned short int)Glass_SectionOfBox):
      {
        if ((unsigned short int)Glass_SectionOfBox == (unsigned short int)Home_SectionOfBox) {
          break;
        } else {
          digitalWrite((uint8_t)Motor_Pin_Two, (uint8_t)LOW);

          analogWrite((uint8_t)Motor_Pin_One, (int)Motor_PowerOnValue);
          delay((unsigned long int)Time_FromHomeSection_ToGlassSection);
          analogWrite((uint8_t)Motor_Pin_One, (int)Motor_PowerOffValue);

          break;
        }
      }
    case ((unsigned short int)Plastic_SectionOfBox):
      {
        if ((unsigned short int)Plastic_SectionOfBox == (unsigned short int)Home_SectionOfBox) {
          break;
        } else {
          digitalWrite((uint8_t)Motor_Pin_Two, (uint8_t)LOW);

          analogWrite((uint8_t)Motor_Pin_One, (int)Motor_PowerOnValue);
          delay((unsigned long int)Time_FromHomeSection_ToPlasticSection);
          analogWrite((uint8_t)Motor_Pin_One, (int)Motor_PowerOffValue);

          break;
        }
      }
    case ((unsigned short int)Other_SectionOfBox):
      {
        if ((unsigned short int)Other_SectionOfBox == (unsigned short int)Home_SectionOfBox) {
          break;
        } else {
          digitalWrite((uint8_t)Motor_Pin_Two, (uint8_t)LOW);

          analogWrite((uint8_t)Motor_Pin_One, (int)Motor_PowerOnValue);
          delay((unsigned long int)Time_FromHomeSection_ToOtherSection);
          analogWrite((uint8_t)Motor_Pin_One, (int)Motor_PowerOffValue);

          break;
        }
      }
  }
}

// Configuring Body Of The Declared Function
void Moving_TrashToGlassSectionOfBox(void) {
  // Showing Current Operation Message To The User
  Display.setCursor((uint8_t)0, (uint8_t)0);
  Display.print("                    ");
  Display.setCursor((uint8_t)0, (uint8_t)1);
  Display.print("Processing The Trash");
  Display.setCursor((uint8_t)0, (uint8_t)2);
  Display.print("   Please Wait...   ");
  Display.setCursor((uint8_t)0, (uint8_t)3);
  Display.print("                    ");
  // Moving The Trash To The Glass Section Of Box
  Moving_BoxToAnySection((unsigned short int)Glass_SectionOfBox);
  delay((unsigned long int)100);
  Opening_HeadOfBox();
  delay((unsigned long int)100);
  Closing_HeadOfBox();
  delay((unsigned long int)100);
  Moving_BoxToHomeSection((unsigned short int)Glass_SectionOfBox);
  delay((unsigned long int)100);
  // Showing Thanks Message To The User
  Display.setCursor((uint8_t)0, (uint8_t)0);
  Display.print("                    ");
  Display.setCursor((uint8_t)0, (uint8_t)1);
  Display.print("     Thanks For     ");
  Display.setCursor((uint8_t)0, (uint8_t)2);
  Display.print("    Selecting Us    ");
  Display.setCursor((uint8_t)0, (uint8_t)3);
  Display.print("                    ");
  delay((unsigned long int)3000);
}

// Configuring Body Of The Declared Function
void Moving_TrashToPlasticSectionOfBox(void) {
  // Showing Current Operation Message To The User
  Display.setCursor((uint8_t)0, (uint8_t)0);
  Display.print("                    ");
  Display.setCursor((uint8_t)0, (uint8_t)1);
  Display.print("Processing The Trash");
  Display.setCursor((uint8_t)0, (uint8_t)2);
  Display.print("   Please Wait...   ");
  Display.setCursor((uint8_t)0, (uint8_t)3);
  Display.print("                    ");
  // Moving The Trash To The Plastic Section Of Box
  Moving_BoxToAnySection((unsigned short int)Plastic_SectionOfBox);
  delay((unsigned long int)100);
  Opening_HeadOfBox();
  delay((unsigned long int)100);
  Closing_HeadOfBox();
  delay((unsigned long int)100);
  Moving_BoxToHomeSection((unsigned short int)Plastic_SectionOfBox);
  delay((unsigned long int)100);
  // Showing Thanks Message To The User
  Display.setCursor((uint8_t)0, (uint8_t)0);
  Display.print("                    ");
  Display.setCursor((uint8_t)0, (uint8_t)1);
  Display.print("     Thanks For     ");
  Display.setCursor((uint8_t)0, (uint8_t)2);
  Display.print("    Selecting Us    ");
  Display.setCursor((uint8_t)0, (uint8_t)3);
  Display.print("                    ");
  delay((unsigned long int)3000);
}

// Configuring Body Of The Declared Function
void Moving_TrashToOtherSectionOfBox(void) {
  // Showing Current Operation Message To The User
  Display.setCursor((uint8_t)0, (uint8_t)0);
  Display.print("                    ");
  Display.setCursor((uint8_t)0, (uint8_t)1);
  Display.print("Processing The Trash");
  Display.setCursor((uint8_t)0, (uint8_t)2);
  Display.print("   Please Wait...   ");
  Display.setCursor((uint8_t)0, (uint8_t)3);
  Display.print("                    ");
  // Moving The Trash To The Other Section Of Box
  Moving_BoxToAnySection((unsigned short int)Other_SectionOfBox);
  delay((unsigned long int)100);
  Opening_HeadOfBox();
  delay((unsigned long int)100);
  Closing_HeadOfBox();
  delay((unsigned long int)100);
  Moving_BoxToHomeSection((unsigned short int)Other_SectionOfBox);
  delay((unsigned long int)100);
  // Showing Thanks Message To The User
  Display.setCursor((uint8_t)0, (uint8_t)0);
  Display.print("                    ");
  Display.setCursor((uint8_t)0, (uint8_t)1);
  Display.print("     Thanks For     ");
  Display.setCursor((uint8_t)0, (uint8_t)2);
  Display.print("    Selecting Us    ");
  Display.setCursor((uint8_t)0, (uint8_t)3);
  Display.print("                    ");
  delay((unsigned long int)3000);
}

// Configuring Body Of The Declared Function
void Showing_HelpMessageToUser(void) {
  // Showing Help Message For Using To User
  Display.setCursor((uint8_t)0, (uint8_t)0);
  Display.print("Select A TypeOfTrash");
  Display.setCursor((uint8_t)0, (uint8_t)1);
  Display.print("  Blue : Glassical  ");
  Display.setCursor((uint8_t)0, (uint8_t)2);
  Display.print(" Yellow : Plastical ");
  Display.setCursor((uint8_t)0, (uint8_t)3);
  Display.print("   Green : Other    ");
}

// Configuring Body Of The Declared Function
void Showing_WelcomeMessageToUser(void) {
  // Showing Welcome Message To The User For 5 Seconds
  Display.setCursor((uint8_t)0, (uint8_t)0);
  Display.print("     Welcome To     ");
  Display.setCursor((uint8_t)0, (uint8_t)1);
  Display.print(" The Smart TrashCan ");
  Display.setCursor((uint8_t)0, (uint8_t)2);
  Display.print("   Made In Iran By  ");
  Display.setCursor((uint8_t)0, (uint8_t)3);
  Display.print("   Alireza Nasiri   ");
  delay((unsigned long int)3000);
}

// Configuring Body Of The Declared Function
void Sending_Message(const char *PhoneNumber, const char *Message) {
  // Showing Sending Message To Operator
  Display.setCursor((uint8_t)0, (uint8_t)0);
  Display.print("                    ");
  Display.setCursor((uint8_t)0, (uint8_t)1);
  Display.print("Sending Message ....");
  Display.setCursor((uint8_t)0, (uint8_t)2);
  Display.print("                    ");
  Display.setCursor((uint8_t)0, (uint8_t)3);
  Display.print("                    ");
  // Sending Message To The Operator
  char ConfigPhoneNumber[24];
  sprintf(ConfigPhoneNumber, "AT+CMGS=\"%s\"", PhoneNumber);

  GSM.println("AT");
  delay((unsigned long int)2000);
  GSM.println("AT+CMGF=1");
  delay((unsigned long int)2000);
  GSM.println(ConfigPhoneNumber);
  GSM.print(Message);
  GSM.write((uint8_t)26);
  delay((unsigned long int)6000);

  // Showing Message Is Sent To Operator
  Display.setCursor((uint8_t)0, (uint8_t)0);
  Display.print("                    ");
  Display.setCursor((uint8_t)0, (uint8_t)1);
  Display.print("   Message Is Sent  ");
  Display.setCursor((uint8_t)0, (uint8_t)2);
  Display.print("                    ");
  Display.setCursor((uint8_t)0, (uint8_t)3);
  Display.print("                    ");
  delay((unsigned long int)3000);
}