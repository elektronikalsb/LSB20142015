
//Variables para nivel de carga
float lectura;
float nivel_carga_voltios;
float nivel_carga_porcentaje;
float resultado_resta;

//Entradas de los datos del acelerometro
const int xPin = 1;
const int yPin = 2;
const int zPin = 3;

//El valor minimo y maximo del acelerometro probados anteriormente
int minVal = 265;
int maxVal = 402;

//Para guardar los valores en grados
double x;
double y;
double z;

//Se establecen las entradas y salidas 
void setup()
{
  Serial.begin(9600);  
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);  
}

void loop(){
 // Lee el valor analogico del Pin A4 y reliza los calculos del nivel de carga
  lectura = analogRead(A4);
  nivel_carga_voltios = (5 * lectura) / 1024;
  resultado_resta = nivel_carga_voltios - 4.28;
  nivel_carga_porcentaje = resultado_resta * 100 / 0.72;
  
  //Lectura de los valores analogicos del acelerometro
  int xRead = analogRead(xPin);
  int yRead = analogRead(yPin);
  int zRead = analogRead(zPin);

  //Convierte los valores obtenidos en grados entre -90 y 90 los cuales necesitamos para el atan2
  int xAng = map(xRead, minVal, maxVal, -90, 90);
  int yAng = map(yRead, minVal, maxVal, -90, 90);
  int zAng = map(zRead, minVal, maxVal, -90, 90);

  //Convertimos los Grados en Radianes
  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
  
  // Activamos el motor respecto a los grados de inclinación
  
  //Estado1: Estando recto el patinete no ace nada y se para.
 if ((x > 355 )&&(x < 5))
  {
     digitalWrite(5,LOW);
     digitalWrite(6,LOW);
  }
  
  //Estado2: Dependiendo de los grados de inclinación el patinete regulara su velocidad
 else if ((x >= 5 )&&(x < 7))
  {
      analogWrite(5,10);
      digitalWrite(6,HIGH);
  }
  
   else if ((x >= 7 )&&(x < 9))
  {
      analogWrite(5,30);
      digitalWrite(6,HIGH);
  }
  
  else if ((x >= 9 )&&(x < 11))
  {
      analogWrite(5,50);
      digitalWrite(6,HIGH);
  }
  
   else if ((x >= 11 )&&(x < 13))
  {
      analogWrite(5,70);
      digitalWrite(6,HIGH);
  } 
  
  else if ((x >= 13 )&&(x < 15))
  {
      analogWrite(5,90);
      digitalWrite(6,HIGH);
  }
  
   else if ((x >= 15 )&&(x < 17))
  {
      analogWrite(5,110);
      digitalWrite(6,HIGH);
  }
  
  else if ((x >= 17 )&&(x < 19))
  {
      analogWrite(5,130);
      digitalWrite(6,HIGH);
  }
  
   else if ((x >= 19 )&&(x < 21))
  {
      analogWrite(5,150);
      digitalWrite(6,HIGH);
  }  
  
   else if ((x >= 21 )&&(x < 23))
  {
      analogWrite(5,170);
      digitalWrite(6,HIGH);
  }    
  
   else if ((x >= 23 )&&(x < 25))
  {
      analogWrite(5,200);
      digitalWrite(6,HIGH);
  } 
  
 else if ((x <= 355 )&&(x > 353))
  {
      analogWrite(5,10);
      digitalWrite(6,LOW);
  }  

 else if ((x <= 353 )&&(x > 351))
  {
      analogWrite(5,30);
      digitalWrite(6,LOW);
  }
  
 else if ((x <= 351 )&&(x > 349))
  {
      analogWrite(5,50);
      digitalWrite(6,LOW);
  }  

 else if ((x <= 349 )&&(x > 347))
  {
      analogWrite(5,70);
      digitalWrite(6,LOW);
  }  
 
 else if ((x <= 347 )&&(x > 345))
  {
      analogWrite(5,90);
      digitalWrite(6,LOW);
  }  

 else if ((x <= 345 )&&(x > 343))
  {
      analogWrite(5,110);
      digitalWrite(6,LOW);
  }
  
 else if ((x <= 343 )&&(x > 341))
  {
      analogWrite(5,130);
      digitalWrite(6,LOW);
  }  

 else if ((x <= 341 )&&(x > 339))
  {
      analogWrite(5,150);
      digitalWrite(6,LOW);
  }

 else if ((x <= 339 )&&(x > 337))
  {
      analogWrite(5,170);
      digitalWrite(6,LOW);
  } 
  
 else if ((x <= 337 )&&(x > 335))
  {
      analogWrite(5,200);
      digitalWrite(6,LOW);
  } 
  
 //Estado3: Si el patinete se inclina mas de lo devido se detiene.
  else
  {
     digitalWrite(5,LOW);
     digitalWrite(6,LOW);
  }
 
 //Activamos los Leds dependiendo de la carga 
  if (nivel_carga_voltios >= 4.85)
  {
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  }
  
  else if ((nivel_carga_voltios >= 4.53)&&(nivel_carga_voltios < 4.85))
  {
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  }
  
  else 
  {
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  }  
  
  //Muestra el valor de la carga
  Serial.print(nivel_carga_porcentaje);
  Serial.println(" % ");
 
 //Esperamos un tiempo antes de volver a realizar todas las lecturas 
  delay(10);
}  






  
  
  
