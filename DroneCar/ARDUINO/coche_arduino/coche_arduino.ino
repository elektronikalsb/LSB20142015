
const int led = 13;                                          
const int motortraccion_1 = 12;                               
const int motortraccion_2 = 11;                              
const int motordirec_1 = 10;                                 
const int motordirec_2 = 9;                                  
const int trigger_1 = 8;                                        
const int echo_1 = 7;                                     
const int trigger_2 = 6;                         //ponemos nombre a cada numero de pin
const int echo_2 = 5;                                     
const int pinBusy = 4;                                       
const int pinData = 3;                                       
const int pinClock = 2;                                      
const int pinReset = 1;                                      

unsigned long pulso1, pulso2;                                
float distancia1, distancia2;                                
int obs_adelante;                                        
int obs_atras;                                           
int encendido = 0;                                             
int start = 0;                                        //declaramos las variables necesarias para el programa
const unsigned int minVol = 0xfff0;                          
const unsigned int maxVol = 0xfff7;                          
const unsigned int PlyPse = 0xfffe;                          
const unsigned int Stop = 0xFFFF;                           
unsigned int volLevel = 0x0005;
int Song = 0;
unsigned int vol;

void setup () 
{
  
   pinMode(led, OUTPUT);
   pinMode(motortraccion_1, OUTPUT);
   pinMode(motortraccion_2, OUTPUT);
   pinMode(motordirec_1, OUTPUT);
   pinMode(motordirec_2, OUTPUT);
   pinMode(trigger_1,OUTPUT); 
   pinMode(echo_1,INPUT);                 //Indicamos cada pin como entrada o salida 
   pinMode(trigger_2,OUTPUT);
   pinMode(echo_2,INPUT);
   pinMode(pinData,OUTPUT); 
   pinMode(pinClock,OUTPUT); 
   pinMode(pinReset,OUTPUT); 
   pinMode(pinBusy,INPUT); 
   
   Serial.begin(9600);             //Inicializamos el puerto serial a 9600 baudios
}
 
void loop () 
{
  
   digitalWrite(trigger_1, HIGH);
   delayMicroseconds(20);             //activamos el trigger_1 del sensor 20us para mandar un pulso
   digitalWrite(trigger_1, LOW);
 
   pulso1 = pulseIn(echo_1, HIGH);    //medimos el tiempo que tarda el pulso1 en rebotar

   digitalWrite(trigger_2, HIGH);
   delayMicroseconds(20);             //activamos el trigger_2 del sensor 20us para mandar un pulso
   digitalWrite(trigger_2, LOW);  

   pulso2 = pulseIn(echo_2, HIGH);    //medimos el tiempo que tarda el pulso2 en rebotar
  
   distancia1 = ((float(pulso1/1000.0))*34.32)/2;        //hacemos el calculo de la distancia en cm
   distancia2 = ((float(pulso2/1000.0))*34.32)/2;
   
   obs_adelante = 0;      //inicializamos a 0 las variables
   obs_atras = 0;
   
   if (distancia1 < 40)
   {
     digitalWrite(motortraccion_1,LOW);      //en caso de haber menos distancia de la indicada parar el motor, parar el motor
     obs_adelante = 1;
   }
   
   if (distancia2 < 40)
   {
     digitalWrite(motortraccion_2,LOW);     //en caso de haber menos distancia de la indicada parar el motor, parar el motor
     obs_atras = 1;
   }
  
   if (Serial.available())    //Si está disponible la conexion serial
   { 
      char c = Serial.read();     //Guardamos la lectura en una variable char
      
      if (c == 'H')
      { 
         if (encendido == 0)             //Si nos mandan una 'H' desde la conexion serial encender o apagar la luz
         {
           digitalWrite(led, HIGH);
           encendido = 1;
         }
         else
         {
           digitalWrite(led, LOW);
           encendido = 0;
         }
      }
 
      if (c == 'W')                  //Si se manda una 'W' y no hay obstaculo ir hacia delante
      {
          if (obs_adelante == 0)
          {
          digitalWrite(motortraccion_1,HIGH);   
          digitalWrite(motortraccion_2,LOW);
          }

      }
      
      if (c == 'S')                 //Si se manda una 'S' y no hay obstaculo ir hacia atras
      {
          if (obs_atras == 0)
          {
          digitalWrite(motortraccion_1,LOW);
          digitalWrite(motortraccion_2,HIGH);
          }
 
      }
      
      if (c == 'E')                //Si se manda una 'E' y no hay obstaculo ir hacia delante+derecha
      {
        if (obs_adelante == 0)
        {
        digitalWrite(motordirec_1,HIGH);
        digitalWrite(motordirec_2,LOW);
        digitalWrite(motortraccion_1,HIGH);
        digitalWrite(motortraccion_2,LOW);
        }
      }
      
      if (c == 'Q')               //Si se manda una 'Q' y no hay obstaculo ir hacia delante+izquierda
      {
        if (obs_adelante == 0)
        {
        digitalWrite(motordirec_1,LOW);
        digitalWrite(motordirec_2,HIGH);
        digitalWrite(motortraccion_1,HIGH);
        digitalWrite(motortraccion_2,LOW);
        }
      }
      
      if (c == 'D')              //Si se manda una 'D' y no hay obstaculo ir hacia atras+derecha
      {
        if (obs_atras == 0)
        {
        digitalWrite(motordirec_1,HIGH);
        digitalWrite(motordirec_2,LOW);
        digitalWrite(motortraccion_1,LOW);
        digitalWrite(motortraccion_2,HIGH);
        }
      } 
      
      if (c == 'A')             //Si se manda una 'A' y no hay obstaculo ir hacia atras+izquierda
      {
        if (obs_atras == 0)
        {
        digitalWrite(motordirec_1,LOW);
        digitalWrite(motordirec_2,HIGH);
        digitalWrite(motortraccion_1,LOW);
        digitalWrite(motortraccion_2,HIGH);
        }
      }  
      
      if (c == 'P')            //Si se manda una 'P' parar todos los motores
      {
        digitalWrite(motortraccion_1,LOW);
        digitalWrite(motortraccion_2,LOW);  
        digitalWrite(motordirec_1,LOW);
        digitalWrite(motordirec_2,LOW);
      } 
   
      if (c == 'C'){nextPlay();}          //Si se manda una 'C' reproducir siguiente cancion
      if (c == 'X'){pausePlay();}         //Si se manda una 'X' empezar a reproducir
      if (c == 'Z'){rewindPlay();}        //Si se manda una 'Z' reproducir cancion anterior
      if (c == 'M'){incVol();}            //Si se manda una 'M' subir volumen
      if (c == 'N'){decVol();}            //Si se manda una 'N' bajar volumen
      if (c == 'B'){Reset();}             //Si se manda una 'B' resetear
  
   }
}

      void sendData(int ThisSong)
      {
        int TheSong = ThisSong;
        int ClockCounter=0; 
        int ClockCycle=15;
        
        delay(2);

        digitalWrite(pinClock,HIGH);      //mantener 300ms para preparar inicio de data
        delay(300); 
        digitalWrite(pinClock,LOW);       //mantener 2ms para indicar a data que empiece
        delay(2); 

        while(ClockCounter <= ClockCycle)
         { 
           digitalWrite(pinClock,LOW); 
           if (TheSong & 0x8000)
           {
             digitalWrite(pinData,HIGH);
           }
           else
           {
             digitalWrite(pinData,LOW);
           }
            
           TheSong = TheSong << 1;
           delayMicroseconds(200);       //Periodo del ciclo del reloj: 200us LOW
           digitalWrite(pinClock,HIGH);
           ClockCounter++;
           delayMicroseconds(200);      //Periodo del ciclo del reloj: 200us HIGH
         }

        digitalWrite(pinData,LOW);
        digitalWrite(pinClock,HIGH); // Place clock high to signal end of data
      }
      

  
        void nextPlay()            //funcion para pasar de cancion en un rango de 0-511
        {
          if (Song >= 0 && Song < 512){ Song++;} 
          if (Song >= 512 || Song <0){ Song=0;}
          sendData(Song);
          start = 1;
          
        }
        
        void rewindPlay()         //funcion para pasar a la cancion anterior en un rango de 1-512
        {
          if (Song > 0 && Song <= 512){ Song--;} 
          if (Song > 512 || Song <=0){ Song=512;}
          sendData(Song);
        }
  
  
  
        void pausePlay()         //funcion para empezar a reproducir o pause si esta reproduciendo 
        {
          if (start == 0)
          {
            sendData(Song);
            start = 1;
          }
          else
          {
            sendData(PlyPse);
          }
        }


      void incVol()           //funcion para incrementar volumen
      { 
        if (vol >= minVol && vol < maxVol){ vol++;}
        if (vol >= maxVol | vol <minVol){ vol=maxVol;}
        int tVol=vol;
        volLevel = tVol - 0xfff0;
        sendData(tVol);

      }

      void decVol()          //funcion para decrementar volumen
      {
        if (vol > minVol && vol <= maxVol){ vol--;}
        if (vol >= maxVol | vol <minVol){vol=maxVol;}
        int tVol=vol;
        volLevel = tVol-0xfff0;
        sendData(tVol);
      }


      void Reset()          //funcion para parar la reproduccion y reiniciarlo
      {
        digitalWrite(pinReset,LOW);
        delay(50);
        digitalWrite(pinReset,HIGH);
        sendData(Stop);
        Song = 0;
        start = 0;
      }

