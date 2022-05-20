//ATTENTION a bien télécharger RBDDIMMER-master qui a été modifié pour des raisons d'entremêlement entre plusieurs librairies utiliser pour ce projet. 

#include <Servo.h>
#include <RTClib.h>
#include <Wire.h>
#include <DHT.h>
#include <RBDdimmer.h>
#define DHTTYPE DHT22


int B = 0;           // variable du bouton. 
int F = 0;           // variable d'un tour de moteur.  
const int bouton = 13;        //Pin 13
int etatBouton;
const int TEMP_PIN = A0;      //Pin A0
const int zeroCrossPin  = 2;  //Pin 2
const int acdPin  = 6;        //Pin 6

DHT dht(TEMP_PIN, DHTTYPE);
dimmerLamp acd(acdPin);

Servo monServomoteur;
RTC_DS3231 RTC;




void setup() 
{
  acd.begin(NORMAL_MODE, ON1);  
  dht.begin();
  monServomoteur.attach(6);
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  while (!Serial);
  while (!RTC.begin());
  RTC.adjust(DateTime(F(__DATE__) , F(__TIME__)));
  Serial.println("Horloge du module RTC mise a jour");
  pinMode(bouton, INPUT);
  etatBouton =LOW;
 
  
}

void loop() {

  
  DateTime now = RTC.now();
  char heure[10];
  sprintf(heure, "il est %02d:%02d:%02d", now.hour(), now.minute(), now.second());    //ligne à écrire dans le moniteur. 
  Serial.println(heure);
  etatBouton = digitalRead(bouton);
  float t = dht.readTemperature();
  Serial.println("Température : " + String(t) + "°C");                               //ligne à écrire dans le moniteur. 
   
   Serial.print("resistance -> ");
      Serial.print(acd.getPower());
      Serial.println("%");
  
  
  
  
   if ( (now.hour() == 10) && (now.minute() == 00) && (F == 0) && (B == 0) or (now.hour() == 16) && (now.minute() == 00) && (F == 0) && (B == 0) or (now.hour() == 21) && (now.minute() == 30) && (F == 0) && (B == 0) )
  {
     FORWARD();
     F = 1;
  }
  //Le moteur se déclenche aux heures 10,00H, 16,00H et 21,30H et passe la variable F à 1.
  else 
  {
    monServomoteur.detach();  //Moteur detach pour ne plus qu'il fonctionne.
  }

   if ( (now.hour() == 10) && (now.minute() == 10)  or (now.hour() == 16) && (now.minute() == 10)  or (now.hour() == 21) && (now.minute() == 45)  )
   {
     F = 0;
     B = 0;
     
   }
   //Cette ligne permet de réinitialiser les variables pour le prochain cycle de l'automatisation. 


if (etatBouton == LOW)  //Low pour causse bouton inversait. 
    {  
       FORWARD();      //Quand le moteur est déclenché en marche forcer par le bouton une variable B prend la valeur pour pas que le programme redonne de la nourriture en automatique.        
       B = 1;
    }
 else 
  {
    monServomoteur.detach(); //Moteur detach pour ne plus qu'il fonctionne. 
  }
 

  



  
   // si la température est inférieure à 20 (20°)et supérieur à 18 (18°) on active le module à 50%
  if((t < 20) && (t > 18)){
     acd.setPower(50); //50%
  } 
   // si la température est inférieure à 18 (18°) on active le module à 75% 
  if(t < 18){
     acd.setPower(75); //75%
  }
   // si la température est inférieure à 21 (21°)et supérieur à 20 (20°) on active le module à 10% 
  if((t < 21 ) && (t > 20)){
     acd.setPower(10); //10%
  }else {
    // sinon on l'éteint
     acd.setPower(0); //0%
  }


   
 }





void FORWARD(){
  
      monServomoteur.attach(6);
      // Fait bouger le bras de 0° à 180°
      for (int position = 0; position <= 180; position++) {
      monServomoteur.write(position);
      delay(50); }

      // Fait bouger le bras de 180° à 0°
     for (int position = 180; position >= 0; position--) {
     monServomoteur.write(position);
     delay(50);
  
  }
  }
