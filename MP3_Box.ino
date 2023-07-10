#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 5;    
constexpr uint8_t SS_PIN = 4;
const int buttonPause = 3; //button Pause/Start    
//const int buttonNext = 14;   not implemented
int folderCnt = 25;
bool bpPause = false;
bool bpFolder = false;
bool btnPauseHigh = true;
int savedFolder = 0;
int selFolder = 0;

struct storedUID {
  String uid;
  int folderID;
};
storedUID listUID[25];

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
  int buttonStatePause = digitalRead(buttonPause);
  
  if (buttonStatePause == LOW && btnPauseHigh){
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
    btnPauseHigh = false;
  }
  if (buttonStatePause == HIGH && !btnPauseHigh)
    btnPauseHigh = true;
  
  if (Serial.available())
    readCommand();
  
  if (myDFPlayer.available())
    printDetail(myDFPlayer.readType(), myDFPlayer.read());

  
  if (rfid.PICC_IsNewCardPresent())
    detectedRFIDCard();

  if(bpFolder)
      selectFolder();
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
  Serial.println(F("DFPlayer Mini online."));

  printHelp();


  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms

  //----Set volume----
  myDFPlayer.volume(20);  //Set volume value (0~30).
  // myDFPlayer.volumeUp(); //Volume Up
  // myDFPlayer.volumeDown(); //Volume Down

  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  //  myDFPlayer.EQ(DFPLAYER_EQ_POP);
  //  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
  //  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
  //  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
  //  myDFPlayer.EQ(DFPLAYER_EQ_BASS);

  //----Set device we use SD as default----
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_AUX);
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SLEEP);
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_FLASH);

  //----Mp3 control----
  //  myDFPlayer.sleep();     //sleep
  //  myDFPlayer.reset();     //Reset the module
  //  myDFPlayer.enableDAC();  //Enable On-chip DAC
  //  myDFPlayer.disableDAC();  //Disable On-chip DAC
  //  myDFPlayer.outputSetting(true, 15); //output setting, enable the output and set the gain to 1
}

void detectedRFIDCard()
{
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
  
  //Serial.println("RFID: "+uidString);

  selFolder = getUIDFolder(uidString);
  bpFolder = true;
}

void selectFolder()
{
  uint8_t folderIDUint8 = static_cast<uint8_t>(selFolder);
  myDFPlayer.loopFolder(folderIDUint8);
  bpFolder = false;
}

void saveRFIDTable()
{
  int i = 0;
  listUID[i].uid = "04881422110189";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "04504f21110189";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "04c5f521110189";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "04956422110189";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "04bbb921110189";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "04f98a21110189";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "048bf621110189";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "04b8c821110189";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "04ce1d22110189";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "041c7321110189";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "9673df5f";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "9e496b1d";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "11aeb789";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "808be112";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "1dc0b394680000";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "1dd8b394680000";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "1dd5b394680000";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "1dfbb394680000";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "1dfab394680000";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "1de4b394680000";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "1de0b394680000";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "1dfdb394680000";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "1d82b394680000";
  listUID[i].folderID = i + 1;
  i++;
  listUID[i].uid = "1d7fb394680000";
  listUID[i].folderID = i + 1;
  //Folder 0 - 23 total 24 Folders
}

void selectFolder(int folderID)
{
  uint8_t folderIDUint8 = static_cast<uint8_t>(folderID);
  myDFPlayer.loopFolder(folderIDUint8);
}

void readCommand() {
  char cmd = ' ';
  int value1, value2 = 0;
  cmd = Serial.read();
  value1 = Serial.parseInt();
  value2 = Serial.parseInt();

  switch (cmd) {
    case 'h':
      printHelp();
      break;

    case '>':
      myDFPlayer.next();
      break;

    case '<':
      myDFPlayer.previous();
      break;

    case 'p':
      if (value2 == 0) myDFPlayer.play(value1);
      else if (value1 != 0) myDFPlayer.playFolder(value1, value2);
      break;

    case 'P':
      if (value2 == 0) myDFPlayer.playMp3Folder(value1);
      else if (value1 != 0)myDFPlayer.playLargeFolder(value1, value2);
      break;

    case '+':
      myDFPlayer.volumeUp();
      break;

    case '-':
      myDFPlayer.volumeDown();
      break;

    case 'v':
      myDFPlayer.volume(value1);
      break;

    case 'b':
      myDFPlayer.pause();
      break;

    case 's':
      myDFPlayer.start();
      break;

    case 'z':
      myDFPlayer.sleep();
      break;

    case 'L':
      if (value1 == 0) myDFPlayer.enableLoopAll();
      else myDFPlayer.loopFolder(value1);
      break;

    case 'l':
      if (value1 == 0) myDFPlayer.disableLoopAll();
      else myDFPlayer.loop(value1);
      break;

    case 'A':
      myDFPlayer.advertise(value1);
      break;

    case 'a':
      myDFPlayer.stopAdvertise();
      break;

    case 'q':
      if (value1 == 1) Serial.println(myDFPlayer.readState());
      else if (value1 == 2) Serial.println(myDFPlayer.readVolume());
      else if (value1 == 3) Serial.println(myDFPlayer.readEQ());
      else if (value1 == 4) Serial.println(myDFPlayer.readFileCounts());
      else if (value1 == 5) Serial.println(myDFPlayer.readFolderCounts());
      else if (value1 == 6) Serial.println(myDFPlayer.readCurrentFileNumber());
      break;
    default:
      Serial.println("Ungültiges Kommando");
      break;
  }

}

void printHelp() {
  Serial.println("DFPlayer Commands:");
  Serial.println(" h - help");
  Serial.println(" > - next ");
  Serial.println(" < - previous");
  Serial.println(" p3 - play");
  Serial.println(" p3,5 - play folder 3, file 5");
  Serial.println(" P3,5 - play large folder 3, file 5");
  Serial.println(" P3 - play file 3 in MP3 folder");
  Serial.println(" + - volume up");
  Serial.println(" - - volume down");
  Serial.println(" v10 - set volume to 10");
  Serial.println(" b - Pause");
  Serial.println(" s - start ");
  Serial.println(" z - sleep ");
  Serial.println(" L - enable loop all");
  Serial.println(" l - disable loop all");
  Serial.println(" L3 - loop folder 3");
  Serial.println(" l3 - loop file 3");
  Serial.println(" A3 - advertise file 3");
  Serial.println(" a - stop advertise ");
  Serial.println(" qx - query No. x");
  Serial.println("     x = 1 - read state");
  Serial.println("     x = 2 - read volume");
  Serial.println("     x = 3 - read equalizer");
  Serial.println("     x = 4 - read file counts");
  Serial.println("     x = 5 - read current file number");
  Serial.println("     x = 6 - read file counts in folder");
  Serial.println("     x = 7 - read folder counts");
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
  Serial.println("FNF");
  return -1;
}
