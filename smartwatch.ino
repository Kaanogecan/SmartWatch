void serialEvent() {
}
#include <LCD5110_Basic.h>
int isik = 0;
int currentMenuIndex = 0;
int leftButtonPin = 4;
int rightButtonPin = 2;
int middleButtonPin = 3;
int middlePressed, rightPressed, leftPressed;
LCD5110 myGLCD(8, 9, 10, 11, 12);
String menuItems[] = {"-Hello Kaan-", "Saat Modu", "Arka Isik", "Notlar", "Led", "Uyku Modu", "Metronome", "Kronometre", "B Acik", "", "", ""};
extern uint8_t SmallFont[];
bool getNotification = true;
String note = "";
void setup()
{
  pinMode(rightButtonPin, INPUT);
  pinMode(leftButtonPin, INPUT);
  pinMode(middleButtonPin, INPUT);
  pinMode(5, OUTPUT);
  pinMode(A3, OUTPUT);
  Serial.begin(9600);
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  digitalWrite(5, HIGH);
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);
  Initalize();
}

void loop()
{
  Serial.begin(9600);
  myGLCD.disableSleep();
  rightPressed = digitalRead(rightButtonPin);
  leftPressed = digitalRead(leftButtonPin);
  middlePressed = digitalRead(middleButtonPin);
  if (rightPressed == 1 || leftPressed == 1 || middlePressed == 1 ) {
    if (rightPressed == 1)
    {
      if (currentMenuIndex < 8)
      {
        currentMenuIndex = currentMenuIndex + 1;
      }
    }
    if (leftPressed == 1)
    {
      if (currentMenuIndex > 0)
        currentMenuIndex = currentMenuIndex - 1;
    }
    int showMenuItemsPixel = 1;
    myGLCD.clrScr();
    for (int i = currentMenuIndex; i < currentMenuIndex + 4; i++)
    {
      if (currentMenuIndex == i)
      {
        myGLCD.print(">" + menuItems[i], LEFT, 8 * showMenuItemsPixel);
      }
      else
      {
        myGLCD.print(menuItems[i], LEFT, (8 * showMenuItemsPixel));
      }
      showMenuItemsPixel++;
    }

    if (middlePressed == 1)
    {
      switch (currentMenuIndex)
      {
        case 0:
          break;
        case 1:
          Watch();
          break;
        case 2:
          LedControl(5, "Arka");
          break;
        case 3:
          Notes();
          break;
        case 4:
          LedControl(A3, "Kirmizi");
          break;
        case 5:
          Sleep();
          break;
        case 6:
          Metronome();
          break;
        case 7:
          Stopwatch();
          break;
        case 8:
          switch (getNotification)
          {
            case true:
              menuItems[8] = "B>Kapali";
              getNotification = false;
              delay(150);
              break;
            case false:
              menuItems[8] = "B>Acik";
              getNotification = true;
              delay(150);
              break;
          }
          break;
      }

    }
  }
  else
  {
    delay(150);
  }

}
void Initalize()
{
  String animationText = "#          ";
  for (int i = 0; i <= 9; i++)
  {
    for (int j = 0; j <= 5; j++)
    {
      myGLCD.print("Baslatiliyor" , CENTER, 8);
      myGLCD.print("%" + String((i * 10) + (j * 2)) , CENTER , 24);
      myGLCD.print("[" + animationText + "]" , LEFT, 32);
      delay(25);
      myGLCD.clrScr();
    }
    animationText[i] = '#';

  }
  myGLCD.print("Tamamlandı" , CENTER, 8);
  myGLCD.print("%100", CENTER , 24);
  myGLCD.print("[" + animationText + "]" , LEFT, 32);
  delay(75);
}
void Watch()
{
  myGLCD.clrScr();
  delay(200);
  middlePressed = 0;
  while (middlePressed == 0) {
    String serialA;
    serialA = Serial.readString();
    if (serialA != "")
    {
      String btmsg = serialA + " ";
      String txt = "";
      int line = 1;
      myGLCD.clrScr();
      int getMessageln = btmsg.length();
      for (int i = 0; i < getMessageln; i++)
      {
        if (String(btmsg[i]) != " ")
        {
          txt = txt + String(serialA[i]);
        }
        else
        {
          if (txt == "N")
          {
            note = note + btmsg.substring(i, getMessageln);
            Serial.println(note);
            myGLCD.print("Not eklendi", CENTER, 16);
            break;
          }
          if (txt == "ND")
          {
            note = "";
            myGLCD.print("Notlar silindi", CENTER, 16);
            break;
          }
          if (txt == "B")
          {
            String notification = btmsg.substring(i, getMessageln);
            String parseForScreen = "";
            int lineCount = 0;
            for (int i = 0; i < notification.length(); i++)
            {
              if (i % 12 != 0)
              {
                parseForScreen = parseForScreen + notification[i];
              }
              else
              {
                parseForScreen = parseForScreen + notification[i];
                myGLCD.print(parseForScreen, LEFT, 8 * lineCount);
                lineCount++;
                parseForScreen = "";
              }
            }
            if (notification.length() % 12 != 0)
            {
              parseForScreen = parseForScreen + notification[notification.length()];
              myGLCD.print(parseForScreen, LEFT, 8 * lineCount);
            }
            while (middlePressed == 0)
            {
              Alert();
              middlePressed = digitalRead(middleButtonPin);
            }
            break;
          }
          myGLCD.print(txt, CENTER, (8 * line));
          txt = "";
          line++;
        }

      }
      if (btmsg.substring(0, 5) == "Arama")
      {
        while (middlePressed == 0)
        {
          Alert();
          middlePressed = digitalRead(middleButtonPin);
        }
        delay(300);
      }
    }
    middlePressed = digitalRead(middleButtonPin);
    delay(200);
    currentMenuIndex = 0;
  }
  myGLCD.clrScr();
}
void Notes()
{
  myGLCD.clrScr();
  delay(200);
  middlePressed = 0;
  int noteLn = note.length();
  String message = "";
  int mLineCount = 0;
  for (int i = 0; i < noteLn; i++)
  {
    if (i % 13 != 0)
    {
      message = message + note[i];
    }
    else
    {
      message = message + note[i];
      myGLCD.print(message, CENTER, 8 * mLineCount);
      mLineCount++;
      message = "";
    }
  }
  if (noteLn % 13 != 0)
  {
    message = message + note[noteLn];
    myGLCD.print(message, CENTER, 8 * mLineCount);
  }
  while (middlePressed == 0) {
    middlePressed = digitalRead(middleButtonPin);
    delay(200);
    currentMenuIndex = 0;
  }
}

void Metronome()
{
  int tap = 60;
  myGLCD.clrScr();
  delay(100);
  middlePressed = digitalRead(middleButtonPin);
  rightPressed = digitalRead(rightButtonPin);
  leftPressed = digitalRead(leftButtonPin);
  while (middlePressed == 0)
  {
    myGLCD.clrScr();
    myGLCD.print("Metronom", CENTER, 8);
    myGLCD.print(String(tap) , CENTER, 16);
    middlePressed = digitalRead(middleButtonPin);
    rightPressed = digitalRead(rightButtonPin);
    leftPressed = digitalRead(leftButtonPin);
    if (rightPressed == 1)
    {
      tap += 1;
      delay(10);
    }
    if (leftPressed == 1)
    {
      tap -= 1;
      delay(10);
    }
    delay(50);
  }
  delay(300);
  middlePressed = 0;
  while (middlePressed == 0)
  {
    int bpmTap = 60000 / tap;
    myGLCD.print(String(bpmTap) , CENTER, 24);
    digitalWrite(A3, HIGH);
    delay(10);
    digitalWrite(A3, LOW);
    delay(bpmTap - 10);
    middlePressed = digitalRead(middleButtonPin);
    currentMenuIndex = 0;
  }
}
void Sleep()
{
  myGLCD.enableSleep();
  digitalWrite(5, LOW);
  delay(300);
  middlePressed = 0;
  while (middlePressed == 0)
  {
    middlePressed = digitalRead(middleButtonPin);
    currentMenuIndex = 0;
    delay(1000);
  }
  digitalWrite(5, HIGH);
}
void LedControl(int pin, String ledName)
{
  myGLCD.clrScr();
  myGLCD.print(ledName + " Isik", CENTER, 8);
  myGLCD.print("Yaniyor", CENTER, 16);
  digitalWrite(pin, HIGH);
  delay(200);
  middlePressed = 0;
  while (middlePressed == 0)
  {
    middlePressed = digitalRead(middleButtonPin);
    rightPressed = digitalRead(rightButtonPin);
    leftPressed = digitalRead(leftButtonPin);
    if (rightPressed == 1)
    {
      myGLCD.clrScr();
      myGLCD.print(ledName + " isik", CENTER, 8);
      myGLCD.print("Yaniyor", CENTER, 16);
      digitalWrite(pin, HIGH);
    }
    if (leftPressed == 1)
    {
      myGLCD.clrScr();
      myGLCD.print(ledName + " isik", CENTER, 8);
      myGLCD.print("Yanmiyor", CENTER, 16);
      digitalWrite(pin, LOW);
    }
    delay(200);
  }
  currentMenuIndex = 0;
}
void Stopwatch ()
{
  int second = 0;
  delay(200);
  middlePressed = 0;
  int minute = 0;
  while (middlePressed == 0)
  {
    myGLCD.clrScr();
    middlePressed = digitalRead(middleButtonPin);
    if (second == 60)
    {
      minute += 1;
      second = 0;
      myGLCD.print((String(minute) + ":" + String(second)), CENTER, 16);
    }
    else
    {
      myGLCD.print((String(minute) + ":" + String(second)), CENTER, 16);
    }
    second += 1;
    delay(1000);
  }
  currentMenuIndex = 0;
}
void Alert()
{
  for (int i = 0; i < 10; i++)
  {
    digitalWrite(5, HIGH);
    if (getNotification)
    {
      digitalWrite(A3, HIGH);
    }
    delay(50);
    digitalWrite(5, LOW);
    if (getNotification)
    {
      digitalWrite(A3, LOW);
    }
    delay(50);
  }
  digitalWrite(5, HIGH);
}
