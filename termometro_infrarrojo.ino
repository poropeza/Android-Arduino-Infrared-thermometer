#include <Wire.h>
#include "Adafruit_MLX90614.h"
#include <NewPing.h>
#include <SoftwareSerial.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200

int pinLaser=6;
float distancia;

//bluetooth hc-05
int ledPin = 13; // usamos un pin de salida al LED
int state = 0; // Variable lectrura serial

SoftwareSerial BT(2, 3); // RX | TX

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() 
{
    // Iniciamos la comunicación serie
    Serial.begin(9600);

    //iniciamos el termómetro infrarrojo
    mlx.begin();

    pinMode(pinLaser,OUTPUT);
    digitalWrite(pinLaser,HIGH);

     BT.begin(9600);
}


void loop() 
{
  if (BT.available())
  {
    
    state=BT.read();
    //Serial.println(state);
  }

   if(state=='0')//ambiental
   {
    
    String mystring;
  
    mystring = /*"0" +*/ String(mlx.readAmbientTempC()-2.09);//"Ambiente: " + String(mlx.readAmbientTempC())+" *C\nAmbiente: " + String(mlx.readAmbientTempC()*9/5 + 32) +" *F";
          
    BT.print(mystring);
    Serial.println(mystring);
   }
   else if(state=='1')//se toma la medida del termómetro hasta llegar a los 10cm de distancia (frente)
   {
    //obtengo la distancia    
    do
    {
      // Obtener medicion de tiempo de viaje del sonido y guardar en variable uS
      int uS = sonar.ping_median();
      distancia = uS / US_ROUNDTRIP_CM; //cm
     
        if(distancia==10.0)//obtengo la muestra de temperatura (frente)
        {
          
          //Serial.print("Ambiente = ");
          //Serial.print(mlx.readAmbientTempC()); 
          //Serial.print("ºC\tObjeto = "); 
        
           float  average = 0;
           /*for (int i=0; i < 50; i++) 
           {
            average = average + mlx.readObjectTempC();
           }*/
          // average = average/50 + 5.85;
          average = mlx.readObjectTempC() + 7.05;
  
          String mystring;
  
          mystring = /*"1" +*/ String(average);//"Frente: " + String(average)+" *C\nFrente" + String(average*9/5 + 32) +" *F";  //String(average);//"Ambiente: " + String(mlx.readAmbientTempC()) + "°C\nObj: " + String(average) + " °C";
          
          BT.print(mystring);
          //Serial.print(average); 
          //Serial.println("ºC");
          
        }
      
    }while(distancia!=10);
   }
   else if (state == '2') //se toma la medida del termómetro hasta llegar a los 5cm de distancia (oreja)
   {
    //Serial.println("entra"); 

    //obtengo la distancia    
    do
    {
      // Obtener medicion de tiempo de viaje del sonido y guardar en variable uS
      int uS = sonar.ping_median();
      distancia = uS / US_ROUNDTRIP_CM; //cm
      
        if(distancia==5.0)//obtengo la muestra de temperatura (oreja)
        {
          
          //Serial.print("Ambiente = ");
          //Serial.print(mlx.readAmbientTempC()); 
          //Serial.print("ºC\tObjeto = "); 
        
           float  average = 0;
           /*for (int i=0; i < 50; i++) 
           {
            average = average + mlx.readObjectTempC();
           }*/
          // average = average/50 + 6.24;
          average = mlx.readObjectTempC()+5.67;
  
          String mystring;
  
          mystring = /*"2" + */String(average);//"Oreja: " + String(average)+" *C\nOreja: " + String(average*9/5 + 32) +" *F";
          
          BT.print(mystring);
          //Serial.print(average); 
          //Serial.println("ºC");
          
        } 
      
      
    }while(distancia!=5.0);
    
    
   } 
   
  delay(400); 
}
