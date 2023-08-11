#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 5;    
constexpr uint8_t SS_PIN = 4;
const int buttonPause = 3; //button Pause/Start    
//const int buttonNext = 14;   not implemented
int folderCnt = 36;
bool bpPause = false;
bool bpFolder = false;
bool btnPauseHigh = true;
bool enableStop = false;
int savedFolder = 0;
int selFolder = 0;
bool initBox = false;
int time1;
int time2;
int timeDiv;
int sleepTimer = 90 * 60 * 1000;
int sleepTimerStartTime;
int sleepTimeGone = 0;

struct storedUID {
  String uid;
  int folderID;
};
storedUID listUID[36];

SoftwareSerial mySoftwareSerial(13, 15); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
MFRC522 rfid(SS_PIN, RST_PIN); 
MFRC522::MIFARE_Key key;

void printDetail(uint8_t type, int value);
void setupMP3();
void selectFolder();
void setupRFID();
void saveRFIDTable();
void detectedRFIDCard();
int getUIDFolder(String uid);

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  pinMode(buttonPause, INPUT_PULLUP);
  setupMP3();
  setupRFID();
  saveRFIDTable();
}

void loop() {
  if (rfid.PICC_IsNewCardPresent())
    detectedRFIDCard();
  
  int buttonStatePause = digitalRead(buttonPause);
  
  if (buttonStatePause == LOW && btnPauseHigh){ 
    btnPauseHigh = false;
    time1 = millis();
  }
  if (buttonStatePause == HIGH && !btnPauseHigh){
    //go in deepsleep.
    //sleepTimerStartTime = millis();
    btnPauseHigh = true;
    time2 = millis();
    timeDiv = time1 - time2;
    timeDiv = -1 * timeDiv;

    Serial.println(timeDiv);
    
    if (timeDiv < 500)
    {
      if (!bpPause)
      {
        myDFPlayer.pause();
        bpPause = true;
      }
      else
      {
        myDFPlayer.start();
        bpPause = false;
      }
    }
    else
    {
      //stop tanz ein
      Serial.println("Stop Tanz <.<  >.> Dance");
      if (enableStop)
        enableStop = false;
      else
        enableStop = true;
    }    
  }
  
  if(bpFolder)
      selectFolder();
  
  if (myDFPlayer.available() && !initBox)
  {
    //myDFPlayer.play(1);
    initBox = true;
  }

  if (enableStop)
  {
    if (!bpPause)
    {
      myDFPlayer.pause();
      bpPause = true;
      delay(random(3,10)*1000);
    }
    else
    {
      myDFPlayer.start();
      bpPause = false;
      delay(random(20,30)*1000);
    }
  }
    
 
  if (myDFPlayer.available() && Serial.available())
    printDetail(myDFPlayer.readType(), myDFPlayer.read());

//  sleepTimeGone = millis() - sleepTimerStartTime;
//  if (sleepTimeGone < sleepTimer)
//    ESP.deepSleep(1000);
//  else
//    Serial.println(sleepTimeGone);
}

void setupRFID()
{
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  Serial.println("RFID reading enabled");
}

void setupMP3()
{
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }

  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms

  //----Set volume----
  myDFPlayer.volume(15);  //Set volume value (0~30).

  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

  //----Set device we use SD as default----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  //----Mp3 control----
  //  myDFPlayer.sleep();     //sleep
  //  myDFPlayer.reset();     //Reset the module
  //  myDFPlayer.enableDAC();  //Enable On-chip DAC
  //  myDFPlayer.disableDAC();  //Disable On-chip DAC
  // Serial.println(myDFPlayer.readCurrentFileNumber())
  //  myDFPlayer.outputSetting(true, 15); //output setting, enable the output and set the gain to 1
}

void detectedRFIDCard()
{
  Serial.println(".");
  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;
  
  Serial.println("New RFID detected");
    
  String uidString = "";

  for (byte i = 0; i < rfid.uid.size; i++) {
    uidString.concat(String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""));
    uidString.concat(String(rfid.uid.uidByte[i], HEX));
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  selFolder = getUIDFolder(uidString);
  bpFolder = true;
}

void selectFolder()
{
  uint8_t folderIDUint8 = static_cast<uint8_t>(selFolder);
  myDFPlayer.loopFolder(folderIDUint8);
  bpFolder = false;
//  enableSleep = false;
}

void saveRFIDTable()
{
  listUID[0].uid = "1dfdf2cb930000";
  listUID[0].folderID = 1;          //Mulan

  listUID[1].uid = "1dbac8cb930000";
  listUID[1].folderID = 2;        // tkkg
  
  listUID[2].uid = "04c5f521110189";
  listUID[2].folderID = 3;          //gloeckner
   
  listUID[3].uid = "04956422110189";
  listUID[3].folderID = 4;          //peterpan
   
  listUID[4].uid = "04bbb921110189";
  listUID[4].folderID = 5;          //school/sally
   
  listUID[5].uid = "04f98a21110189";
  listUID[5].folderID = 6;          //pocahontas
   
  listUID[6].uid = "048bf621110189";
  listUID[6].folderID = 7;          //barbie
   
  listUID[7].uid = "04b8c821110189";
  listUID[7].folderID = 8;          //dornroesschen
   
  listUID[8].uid = "04ce1d22110189";
  listUID[8].folderID = 9;          //raya
   
  listUID[9].uid = "041c7321110189";
  listUID[9].folderID = 10;         //rapunzel
   
  listUID[10].uid = "9673df5f";
  listUID[10].folderID = 11;        //die Schoene und das Biest
   
  listUID[11].uid = "9e496b1d";
  listUID[11].folderID = 12;        //aladin
   
  listUID[12].uid = "11aeb789";
  listUID[12].folderID = 13;        //tarzan
   
  listUID[13].uid = "808be112";
  listUID[13].folderID = 14;        //der Prinz von Agypten
   
  listUID[14].uid = "1dc0b394680000";
  listUID[14].folderID = 15;        //vaiana
   
  listUID[15].uid = "1dd8b394680000";
  listUID[15].folderID = 16;        //koenig der loewen
   
  listUID[16].uid = "1dd5b394680000";
  listUID[16].folderID = 17;        //luca
   
  listUID[17].uid = "1dfbb394680000";
  listUID[17].folderID = 18;        //Casper
   
  listUID[18].uid = "1dfab394680000";
  listUID[18].folderID = 19;        //Harry Potter
   
  listUID[19].uid = "1de4b394680000";
  listUID[19].folderID = 20;        //Black Pink
   
  listUID[20].uid = "1de0b394680000";
  listUID[20].folderID = 21;        //lea
   
  listUID[21].uid = "1dfdb394680000";
  listUID[21].folderID = 22;        //lina
   
  listUID[22].uid = "1d82b394680000";
  listUID[22].folderID = 23;        //Bibi und Tina
   
  listUID[23].uid = "1d7fb394680000";
  listUID[23].folderID = 24;        //Herkules

  listUID[24].uid = "1d2bdbcb930000";               // 
  listUID[24].folderID = 25;          // Encanto
  
  listUID[25].uid = "04504f21110189"; // 
  listUID[25].folderID = 26;          // Emmi

  listUID[26].uid = "04881422110189"; // 
  listUID[26].folderID = 27;          // Bobo

  listUID[27].uid = "1d49d4cb930000";               // 
  listUID[27].folderID = 28;          // Gute Nacht mein Schatz

  listUID[28].uid = "1d4fe5cb930000";               // 
  listUID[28].folderID = 29;          // Party

  listUID[29].uid = "1227ae89";       // 
  listUID[29].folderID = 30;          // Super Simple Songs

  listUID[30].uid = "eeac901d";       // 
  listUID[30].folderID = 31;          // Worship
  
  listUID[31].uid = "1d00cbcb930000"; // 
  listUID[31].folderID = 32;          // Workout

  listUID[32].uid = "1d73edcb930000"; // 
  listUID[32].folderID = 33;          // 5Geschwister

  listUID[33].uid = "1da4d4cb930000"; // 
  listUID[33].folderID = 34;          // Gänsehaut
  
  listUID[34].uid = "1dd4e3cb930000"; // 
  listUID[34].folderID = 35;          // 5Freunde
  
  listUID[35].uid = "1dc1dacb930000"; // 
  listUID[35].folderID = 36;          // Die 3 ???
}

void selectFolder(int folderID)
{
  uint8_t folderIDUint8 = static_cast<uint8_t>(folderID);
  myDFPlayer.loopFolder(folderIDUint8);
  Serial.println(folderIDUint8);
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

int getUIDFolder(String uid)
{
  for (int i = 0; i < folderCnt; i++)
  {
    if (listUID[i].uid.equals(uid))
    {
      return listUID[i].folderID;
    }
  }
  Serial.println(uid);
  Serial.println("FNF");
  return -1;
}
