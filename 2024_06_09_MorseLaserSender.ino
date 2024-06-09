/* Elements which are configurable */

int laserLED = 13;                          // pin to which the laser is connected
int lengthOfDotMs  = 100;                   // time for . (ms)
int dashDotMult = 3;                        // multiplier of - length
int symbolDotMult = 1;                      // multiplier of break between symbols (. -)
int characterDotMult = 3;                   // how long the break between letters is
int spaceDotMult = 3;                       // how long the space is (it will be always sent after next character, so the waiting time is actually (characterDotMult + spaceDotMult) * lengthOfDotMs )

/* End of configurable parameters */

String messageString = "abc de";
const short int morseSymbols = 5; // 4 symbols + end of symbol
short int morseCode[morseSymbols];
// calculate proper lengths to save on calculations time later

int lengthOfDashMs = lengthOfDotMs * dashDotMult;                         // time for - 
int lengthOfSymbolPauseMs = lengthOfDotMs * symbolDotMult;                // length of the pause between symbols
int lengthOfCharacterPauseMs = lengthOfDotMs * characterDotMult;          // length of the  lengthOfSpaceMs between characters
int lengthOfSpaceMs = (characterDotMult + lengthOfDotMs) * spaceDotMult;  // length of the pause  between words

void setup() 
{                
  pinMode(laserLED, OUTPUT); // set laser pin as output one
  Serial.begin(9600); // open the serial port at 9600 bps, for debug purposes
}

/* This function just sends message string. In the final program, this string needs to be read from keyboard - a task for Marco */
void loop()
{ 
  SendMessage(messageString);
}

void SendMessage(String & pMessage)
{
  Serial.print("***** Starting message transmition *****");
  // Loop through the string and get  each character one at a time until the end is reached
  for (int i = 0; i < strlen(pMessage.c_str())  - 1; i++)
  {
    // make lower case (to support upper case too, number of symbols needs to be higher)
    char tmp = toLowerCase(pMessage[i]);
    GetMorseCodeFromLetter(tmp, morseCode); // Encode character to morse code (morseCode array). Currently only a-d + space are implemented
    SendMorseCode(morseCode); // send code with laser
  }
  Serial.println("\n***** Message transmitted *****");
}

/* Code for cotrolling laser */
void  SendDot()
{
  Serial.print(".");
  digitalWrite(laserLED, HIGH);    
  delay(lengthOfDotMs);            
}

void SendDash()
{
  Serial.print("-");
  digitalWrite(laserLED, HIGH);    
  delay(lengthOfDashMs);            
}

void NextSymbol()
{
  Serial.print(" ");
  LaserOff(lengthOfSymbolPauseMs);
}

void NextCharacter()
{
  Serial.print("*");
  LaserOff(lengthOfCharacterPauseMs);
}

void Space()
{
  Serial.print("#");
  LaserOff(lengthOfSpaceMs);
}

void LaserOff(int pTime)
{
  digitalWrite(laserLED, LOW);    
  delay(pTime);          
}

void SendMorseCode(short int * pSymbol)
{
    Serial.print("\nTransmitting: ");
    
    if (pSymbol[0] == 0) // good enough
    {
      Space();
      return;
    }

    short int index = 0;
    while (pSymbol[index] != 0)
    {
      if (pSymbol[index] == 1)
      {
        SendDot();
      }
      else if (pSymbol[index] == 2)
      {
        SendDash();
      }
      ++index;
      if (pSymbol[index] == 0)
        break;

      // next symbol sent only when . or - remains
      NextSymbol();
    }
    NextCharacter();

    Serial.print("\nEnd of transmission");
}

/* This functions should be filled by Francine and Marco */

void GetMorseCodeFromLetter(char pChar, short int * pSymbols)
{
  Serial.println("");
  Serial.print("Converting ");
  Serial.print(pChar);
  Serial.print(": ");
  memset(pSymbols, 0, sizeof(short int) * morseSymbols); // clear whole array first
  switch (pChar) 
  {
    case 'a': 
      pSymbols[0] = 1;
      pSymbols[1] = 2;
    break;
    case 'b':
      pSymbols[0] = 2;
      pSymbols[1] = 1;
      pSymbols[2] = 1;
      pSymbols[3] = 1;
    break;
    case 'c':
      pSymbols[0] = 2;
      pSymbols[1] = 1;
      pSymbols[2] = 2;
      pSymbols[3] = 1;
    break;
    case 'd':
      pSymbols[0] = 2;
      pSymbols[1] = 2;
      pSymbols[2] = 1;
    break; 
    case ' ':
    break;
    default: 
      Serial.println(" Undefined character found");
      // all non-matched characters are represente as space
      // not doing anything - the array will remain 0 0 0 0 0    
  }
  for (short int i = 0; i < morseSymbols; i++)
  {
      if (pSymbols[i] == 1)
        Serial.print(".");
      else if (pSymbols[i] == 2)
        Serial.print("-");
  }
}
