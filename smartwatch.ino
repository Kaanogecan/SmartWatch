void serialEvent() {
}
#include <LCD5110_Basic.h>
int isik = 0;
int menu = 0;
int sol = 4;
int sag = 2;
int orta = 3;
int ortab, sagb, solb;
LCD5110 myGLCD(8, 9, 10, 11, 12);
String menuler[] = {"-SmartWatch-", "Saat Modu", "Arka Isik", "Notlar", "Led", "Uyku Modu", "Metronome", "Kronometre", "B Acik", "", "", ""};
extern uint8_t SmallFont[];
bool bildirimonay = true;
String msgnot = "";
void setup()
{
  pinMode(sag, INPUT);
  pinMode(sol, INPUT);
  pinMode(orta, INPUT);
  pinMode(5, OUTPUT);
  pinMode(A3, OUTPUT);
  Serial.begin(9600);
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  digitalWrite(5, HIGH);
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);
  baslatiliyor();
}

void loop()
{
  Serial.begin(9600);
  myGLCD.disableSleep();
  sagb = digitalRead(sag);
  solb = digitalRead(sol);
  ortab = digitalRead(orta);
  if (sagb == 1)
  {
    if (menu < 8)
    {
      menu = menu + 1;
    }
  }
  if (solb == 1)
  {
    if (menu > 0)
      menu = menu - 1;
  }
  int goster = 1;
  myGLCD.clrScr();
  for (int i = menu; i < menu + 4; i++)
  {
    if (menu == i)
    {
      myGLCD.print(">" + menuler[i], LEFT, 8 * goster);
    }
    else
    {
      myGLCD.print(menuler[i], LEFT, (8 * goster));
    }
    goster++;
  }

  if (ortab == 1)
  {
    switch (menu)
    {
      case 0:
        break;
      case 1:
        saat();
        break;
      case 2:
        isikcontrol(5, "Arka");
        break;
      case 3:
        notlar();
        break;
      case 4:
        isikcontrol(A3, "Kirmizi");
        break;
      case 5:
        uyku();
        break;
      case 6:
        metronom();
        break;
      case 7:
        kronometre();
        break;
      case 8:
        switch (bildirimonay)
        {
          case true:
            menuler[8] = "B Kapali";
            bildirimonay = false;
            delay(150);
            break;
          case false:
            menuler[8] = "B Acik";
            bildirimonay = true;
            delay(150);
            break;
        }
        break;
    }

  }
  delay(100);
}
void baslatiliyor()
{
  String kare = "#          ";
  for (int i = 0; i <= 9; i++)
  {
    for (int j = 0; j <= 5; j++)
    {
      myGLCD.print("Baslatiliyor" , CENTER, 8);
      myGLCD.print("%" + String((i * 10) + (j * 2)) , CENTER , 24);
      myGLCD.print("[" + kare + "]" , LEFT, 32);
      delay(25);
      myGLCD.clrScr();
    }
    kare[i] = '#';

  }
  myGLCD.print("Tamamlandı" , CENTER, 8);
  myGLCD.print("%100", CENTER , 24);
  myGLCD.print("[" + kare + "]" , LEFT, 32);
  delay(75);
}
void saat()
{
  myGLCD.clrScr();
  delay(200);
  ortab = 0;
  while (ortab == 0) {
    String serialA;
    serialA = Serial.readString();
    if (serialA != "")
    {
      String btmsg = serialA + " ";
      String metin = "";
      int satir = 1;
      myGLCD.clrScr();
      int uzunluk = btmsg.length();
      for (int i = 0; i < uzunluk; i++)
      {
        if (String(btmsg[i]) != " ")
        {
          metin = metin + String(serialA[i]);
        }
        else
        {
          if (metin == "N")
          {
            msgnot = msgnot + btmsg.substring(i, uzunluk);
            Serial.println(msgnot);
            myGLCD.print("Not eklendi", CENTER, 16);
            break;
          }
          if (metin == "ND")
          {
            msgnot = "";
            myGLCD.print("Notlar silindi", CENTER, 16);
            break;
          }
          if (metin == "B")
          {
            String bildirimn = btmsg.substring(i, uzunluk);
            String bsatir = "";
            int satirsayisi = 0;
            for (int i = 0; i < bildirimn.length(); i++)
            {
              if (i % 12 != 0)
              {
                bsatir = bsatir + bildirimn[i];
              }
              else
              {
                bsatir = bsatir + bildirimn[i];
                myGLCD.print(bsatir, LEFT, 8 * satirsayisi);
                satirsayisi++;
                bsatir = "";
              }
            }
            if (bildirimn.length() % 12 != 0)
            {
              bsatir = bsatir + bildirimn[bildirimn.length()];
              myGLCD.print(bsatir, LEFT, 8 * satirsayisi);
            }
            while (ortab == 0)
            {
              bildirim();
              ortab = digitalRead(orta);
            }
            break;
          }
          myGLCD.print(metin, CENTER, (8 * satir));
          metin = "";
          satir++;
        }

      }
      if (btmsg.substring(0, 5) == "Arama")
      {
        while (ortab == 0)
        {
          bildirim();
          ortab = digitalRead(orta);
        }
        delay(300);
      }
    }
    ortab = digitalRead(orta);
    delay(200);
    menu = 0;
  }
  myGLCD.clrScr();
}
void notlar()
{
  myGLCD.clrScr();
  delay(200);
  ortab = 0;
  int notln = msgnot.length();
  String msatir = "";
  int satirsayisi = 0;
  for (int i = 0; i < notln; i++)
  {
    if (i % 13 != 0)
    {
      msatir = msatir + msgnot[i];
    }
    else
    {
      msatir = msatir + msgnot[i];
      myGLCD.print(msatir, CENTER, 8 * satirsayisi);
      satirsayisi++;
      msatir = "";
    }
  }
  if (notln % 13 != 0)
  {
    msatir = msatir + msgnot[notln];
    myGLCD.print(msatir, CENTER, 8 * satirsayisi);
  }
  while (ortab == 0) {
    ortab = digitalRead(orta);
    delay(200);
    menu = 0;
  }
}

void metronom()
{
  int tap = 60;
  myGLCD.clrScr();
  delay(100);
  ortab = digitalRead(orta);
  sagb = digitalRead(sag);
  solb = digitalRead(sol);
  while (ortab == 0)
  {
    myGLCD.clrScr();
    myGLCD.print("Metronom", CENTER, 8);
    myGLCD.print(String(tap) , CENTER, 16);
    ortab = digitalRead(orta);
    sagb = digitalRead(sag);
    solb = digitalRead(sol);
    if (sagb == 1)
    {
      tap += 1;
      delay(10);
    }
    if (solb == 1)
    {
      tap -= 1;
      delay(10);
    }
    delay(50);
  }
  delay(300);
  ortab = 0;
  while (ortab == 0)
  {
    int bpmtap = 60000 / tap;
    myGLCD.print(String(bpmtap) , CENTER, 24);
    digitalWrite(A3, HIGH);
    delay(10);
    digitalWrite(A3, LOW);
    delay(bpmtap - 10);
    ortab = digitalRead(orta);
    menu = 0;
  }
}
void uyku()
{
  delay(300);
  ortab = 0;
  while (ortab == 0)
  {
    ortab = digitalRead(orta);
    myGLCD.enableSleep();
    digitalWrite(5, LOW);
    menu = 0;
  }
  digitalWrite(5, HIGH);
}
void isikcontrol(int pin, String isik)
{
  myGLCD.clrScr();
  myGLCD.print(isik + " Isik", CENTER, 8);
  myGLCD.print("Yaniyor", CENTER, 16);
  digitalWrite(pin, HIGH);
  delay(200);
  ortab = 0;
  while (ortab == 0)
  {
    ortab = digitalRead(orta);
    sagb = digitalRead(sag);
    solb = digitalRead(sol);
    if (sagb == 1)
    {
      myGLCD.clrScr();
      myGLCD.print(isik + " Isik", CENTER, 8);
      myGLCD.print("Yaniyor", CENTER, 16);
      digitalWrite(pin, HIGH);
    }
    if (solb == 1)
    {
      myGLCD.clrScr();
      myGLCD.print(isik + " Isik", CENTER, 8);
      myGLCD.print("Yanmiyor", CENTER, 16);
      digitalWrite(pin, LOW);
    }
    delay(200);
  }
  menu = 0;
}
void kronometre()
{
  int sayac = 0;
  delay(200);
  ortab = 0;
  int dakika = 0;
  while (ortab == 0)
  {
    myGLCD.clrScr();
    ortab = digitalRead(orta);
    if (sayac == 60)
    {
      dakika += 1;
      sayac = 0;
      myGLCD.print((String(dakika) + ":" + String(sayac)), CENTER, 16);
    }
    else
    {
      myGLCD.print((String(dakika) + ":" + String(sayac)), CENTER, 16);
    }
    sayac += 1;
    delay(1000);
  }
  menu = 0;
}
void bildirim()
{
  for (int i = 0; i < 10; i++)
  {
    digitalWrite(5, HIGH);
    if (bildirimonay)
    {
      digitalWrite(A3, HIGH);
    }
    delay(50);
    digitalWrite(5, LOW);
    if (bildirimonay)
    {
      digitalWrite(A3, LOW);
    }
    delay(50);
  }
  digitalWrite(5, HIGH);
}
