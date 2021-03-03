#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int ss1,mm1,hh1,AP1,ss2,mm2,hh2,AP2 ;//ss1,mm1,hh1 variabile pentru ceas
// AP1 variabila pentru meridianul ceasului
// variabile pentru ss2,mm2,hh2 pentru alarma
// AP2 variabila pentru meridianul alarmei

int k=0;   //contor pentru alarma
int ButtonState1 = 0;   //snooze button
int ButtonState2 = 0;   //dismiss button
const int buzzer = 9; 

byte clk_arms[8] = {
  0b00000, 0b00100, 0b01110, 0b01110,
  0b10001, 0b10101, 0b10001, 0b01110
};

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.createChar(0, clk_arms);
  pinMode(7,INPUT);   //snooze
  pinMode(8,INPUT);   //dismiss
  pinMode(buzzer, OUTPUT); 
}

void line()
{
  Serial.println("");
}

void clockDetails()
{ //prima afisare a orei 
  Serial.println("Enter current hour:");
  while(Serial.available() == 0);
  hh1=Serial.parseInt();
  Serial.println(hh1);
  
  
  line();
  //afisarea minutelor
  Serial.println("Enter current minute:");
  while(Serial.available()==0);
  mm1=Serial.parseInt();
  Serial.println(mm1);
  
  line();
  //afisarea secundelor
  Serial.println("Enter current second:");
  while(Serial.available()==0);
  ss1=Serial.parseInt();
  Serial.println(ss1);
  
  line();
  //alegerea meridianului
  Serial.println("Enter AM(0) or PM(1)");
  while(Serial.available()==0);
  AP1=Serial.parseInt();
  Serial.println(AP1);
  
  line();
  k=k+1;
  //cresterea contorului pt o ora specificata
  
}

void alarmDetails()
{//ora alarmei 
  Serial.println("Enter hour:");
  while(Serial.available() == 0);
  hh2=Serial.parseInt();
  Serial.println(hh2);
  
  line(); 
  //minutul alarmei 
  Serial.println("Enter minute:");
  while(Serial.available()==0);
  mm2=Serial.parseInt();
  Serial.println(mm2);
  
  line();
  //meridianul alarmei
  Serial.println("Enter AM(0) or PM(1)");
  while(Serial.available()==0);
  AP2=Serial.parseInt();
  Serial.println(AP2);
  
  line();
  k=k+1;
  //contorul pentru alarme
}

void printTime(int x)
{
  if(x<=9)
  {
    lcd.print("0");
    lcd.print(x);
  }
  else
  {
    lcd.print(x);
  }
}
void printclockTime()
{
  lcd.setCursor(0,0);
  //printTime - pune "0" in fata fiecarei cifre
  printTime(hh1); lcd.print(":");
  printTime(mm1); lcd.print(":");
  printTime(ss1); lcd.print(" ");
  
  if(AP1%2==0)   
  {	
    lcd.print("AM");
  }
  if(AP1%2==1)   
  {
    lcd.print("PM");}
    lcd.setCursor(0,1);
    delay(1000);
} 
void printalarmTime()
{
    lcd.setCursor(1,1);
  	lcd.write((byte)0);
  	lcd.setCursor(3,1);
  
  	printTime(hh2); 
    lcd.print(":");
  	printTime(mm2); 
    lcd.print(" ");
  
  	if(AP2%2==0)   
     {lcd.print("AM");}
  	if(AP2%2==1)   
  	 {lcd.print("PM");}
}

void loop()
{
  if(k<=0) 
  {
    Serial.println("Set clock time");
    clockDetails();   //clockDetails() - Asks for clock time
  }
  ss1=ss1+1;//incrementarea secundelor
  if(ss1==60)
  {
    ss1=0;
    mm1=mm1+1;//incrementarea minutelor
    if(mm1==60)
    {
      mm1=0;
      hh1=hh1+1; 
      if(hh1==12)
      {
        AP1=AP1+1;
        printclockTime();
      }
      if(hh1==13)
      {
        hh1=1;
      }
    }
  }
  
  if(k<=1)
  {
    Serial.println("Set alarm time");
    alarmDetails();  
  }
  printclockTime();   
  printalarmTime();  

  
  while(hh1==hh2 && mm1==mm2)//daca alarma este atinsa
  {
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("Ringing");
    printclockTime();
    ss1=ss1+1;
    if(ss1==60)
  	{
      ss1=0;
      mm1=mm1+1;
      if(mm1==60)
      {
      	mm1=0;
      	hh1=hh1+1;
      }
    }
    tone(buzzer, 1000);  
    ButtonState1 = digitalRead(7);
    ButtonState2 = digitalRead(8);
    if(ButtonState1 == HIGH)
      Serial.println("Snoozed");
    else if(ButtonState2 == HIGH)
      Serial.println("Dismiss");
    
    if(ButtonState1 == HIGH)
    {
      while (ButtonState1 == HIGH)
      {
      	noTone(buzzer);//oprirea soneriei pt snooze 
      	ss1=ss1+1;
      	if(ss1==60)
  	  	{
      	  ss1=0;
          mm1=mm1+1;
          if(mm1==60)
          {
      	    mm1=0;
      	    hh1=hh1+1;
      	  }
         }
        printclockTime();
        lcd.setCursor(2,1);
        lcd.print("Snoozed");
        while(hh1==hh2 && mm1==mm2+5)//snooze pt 5 min 
        {
          tone(buzzer, 1000);
          lcd.clear();
    	  lcd.setCursor(2,1);
    	  lcd.print("Ringing");
    	  printclockTime();
    	  ss1=ss1+1;
    	  if(ss1==60)
  		  {
      	  	ss1=0;
      	    mm1=mm1+1;
      	  	if(mm1==60)
      	  	{
      	      mm1=0;
      	      hh1=hh1+1;
      	    }
          }
          if (ButtonState2 == HIGH)//oprirea soneriei complet
          {
            noTone(buzzer);
        	hh2=0;
      		mm2=0;
      		lcd.clear();
            printclockTime();
        	lcd.setCursor(2,1);
        	lcd.print("Alarm off");
          }
        }
      }
    }
    else if (ButtonState2 == HIGH)
    {
      while (ButtonState2 == HIGH)
      {
      	noTone(buzzer);
        hh2=0;
      	mm2=0;
      	lcd.clear();
      	ss1=ss1+1;
      	if(ss1==60)
  	    {
          ss1=0;
      	  mm1=mm1+1;
      	  if(mm1==60)
      	  {
          	mm1=0;
      	  	hh1=hh1+1;
      	  }
        }
        printclockTime();
        lcd.setCursor(2,1);
        lcd.print("Alarm off");
      }
    }
  }
}


