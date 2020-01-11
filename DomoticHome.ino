#include <Servo.h>

/*
 *    Sketch de Arduino para controlar la maqueta 
 *    del proyecto de Domótica.
 *    
 *    Desarrollada por Santillan, Lucas.
 *    
 */

//    Configuración del módulo Bluetooth HC-06.
const char Velocidad = '4';
const char NombreBT[13] = "Domotic Home";
const char Pass[5] = "1996";

//		Creamos las constantes para el Switch.
const char Cod_Puerta     =	  'P';
const char Cod_Cortinas   =   'C';
const char Cod_Vent       =	  'V';
const char Cod_Off        = 	'0';
const char Cod_Mas_Vel    =	  '+';
const char Cod_Menos_Vel  =   '-';
const char Cod_Led_Hab2   = 	'2';
const char Cod_Led_Hab1   =   '1';
const char Cod_Led_Bath   = 	'B';
const char Cod_Led_Cocina =   'K';

/*
Códigos:
P = Puerta.
C = Cortinas.
V = Ventilador.
0 = Off.
+ = Aumentar Velocidad del Ventilador.
- = Reducir Velocidad del Ventilador.
2 = Luz Habitación 2.
1 = Luz Habitación 1.
B = Luz Baño.
K = Luz Cocina.
*/

//    Instanciamos los objetos de clase Servo.
Servo servoPuerta;
Servo servoCortinas;


//		Constantes para los pines.
const int Puerta = 10;
const int Cortinas = 9;
const int Ventilador = 7;
const int Pieza_2 = 5;
const int Pieza_1 = 4;
const int Bathroom = 2;
const int Cocina = 3;

int Velocidad_Vent;
bool Door;
bool Windows;
bool Fan;
bool Led1; // Habitación izquierda  (2)
bool Led2; // Habitación derecha	  (1)
bool Led3; // Baño y escaleras      (B)
bool Led4; // Cocina				        (K)

void pip(){
  //	Para apagar y encender el led integrado.
  digitalWrite(13, LOW);
}

void Valores(){
  	Led1 = true;
  	Led2 = true;
  	Led3 = true;
  	Led4 = true;
	  Velocidad_Vent = 155;
    Door = true;
    Windows = true;
    Fan = true; 
}

void setup()
{
  Serial.begin(9600);
  
  pinMode(13, OUTPUT); // Indica que se ejecutó una función.
  
  //	Configuramos los pines de salida.
  pinMode(Ventilador, OUTPUT);
  pinMode(Pieza_2, OUTPUT);
  pinMode(Pieza_1, OUTPUT);
  pinMode(Bathroom, OUTPUT);
  pinMode(Cocina, OUTPUT);

  /*  Configuramos el Módulo Bluetooth HC-06.
   *  Nombre:    Domotic Home
   *  Pin:       1996
   *  Velocidad: 4 (9600 Baudios)
  */
  Serial.print("AT");
  delay(1000);
  Serial.print("AT+NAME");
  Serial.print(NombreBT);
  delay(1000);
  Serial.print("AT+BAUD");
  Serial.print(Velocidad);
  delay(1000);
  Serial.print("AT+PIN");
  Serial.print(Pass);
  delay(1000);  
      
  //	Configuramos los objetos servo.
  servoPuerta.attach(Puerta);
  servoCortinas.attach(Cortinas);
  servoPuerta.write(0);
  servoCortinas.write(0);
  Valores();
}

void loop()
{
  digitalWrite(13, HIGH);
  
  if(Serial.available())
  {

    char Codigo;
    Codigo = Serial.read();
    
    switch(Codigo){
      case Cod_Puerta:
      	if (Door){
          		servoPuerta.write(160);
          		Door = false;
          		pip();
      			break;
        }else{
          		servoPuerta.write(-160);
          		Door = true;
         		pip();
          		break;
        }
      case Cod_Cortinas:
      	if (Windows){
          		servoCortinas.write(180);
          		Windows = false;
          		pip();
      			break;
        }else{
          		servoCortinas.write(-180);
          		Windows = true;
            	pip();
          		break;
        }
      case Cod_Vent:
      	if (!Fan){
          		digitalWrite(Ventilador, LOW);
          		analogWrite(6, 0);
          		Fan = true;
          		pip();
          		break;
        }else{
          		digitalWrite(Ventilador, HIGH);
          		analogWrite(6, Velocidad_Vent);
          		Fan = false;
          		pip();
          		break;
        }
      case Cod_Mas_Vel:
      	if (!Fan){
                if(Velocidad_Vent+20<255){
                  Velocidad_Vent += 20;
                  analogWrite(6, Velocidad_Vent);
                }
          		pip();
        }
          	break;
      case Cod_Menos_Vel:
      	if (!Fan){
                if(Velocidad_Vent-20>20){
                  Velocidad_Vent -= 20;
                  analogWrite(6, Velocidad_Vent);
                }
        		pip();
        }          
          break;
      case Cod_Led_Hab2:
      	if(Led2){
        		digitalWrite(Pieza_2, HIGH);
          		Led2 = false;
          		pip();
        }else{
          		digitalWrite(Pieza_2, LOW);
          		Led2 = true;
          		pip();
        }
      		break;
      case Cod_Led_Hab1:
      	if(Led1){
        		digitalWrite(Pieza_1, HIGH);
          		Led1 = false;
          		pip();
        }else{
          		digitalWrite(Pieza_1, LOW);
          		Led1 = true;
          		pip();
        }
      		break;
      case Cod_Led_Bath:
      	if(Led3){
        		digitalWrite(Bathroom, HIGH);
        		Led3 = false;
          		pip();
      }else{
        		digitalWrite(Bathroom, LOW);
        		Led3 = true;
          		pip();
      }
      		break;
      case Cod_Led_Cocina:
      	if(Led4){
        		digitalWrite(Cocina, HIGH);
          		Led4 = false;
        		pip();
        }else{
          		digitalWrite(Cocina, LOW);
          		Led4 = true;
          		pip();
        }
      		break;
      case Cod_Off:
      			//		Apagar todo
      			digitalWrite(Ventilador, LOW);
      			analogWrite(6, 0);
            servoPuerta.write(0);
      			servoCortinas.write(0);
      			digitalWrite(Cocina, LOW);
      			digitalWrite(Bathroom, LOW);
      			digitalWrite(Pieza_1, LOW);
      			digitalWrite(Pieza_2, LOW);
      			Valores();
      			pip();
      		break;
      default:
      			break;
   }
    
  delay(500);
  }
}
