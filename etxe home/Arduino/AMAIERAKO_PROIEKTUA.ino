//LIBRERIAK
#include <SPI.h>
#include <String.h>
#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x01, 0x94 }; //arduinoren MAc-a
byte ip[] = { 192, 168, 2, 3 }; //gure IPA
EthernetServer server(80); //portua

float tenpera;    //MOBILERA BIDALTZEKO DATUA(BARIABLEA)
float kontsumo;  //MOBILERA BIDALTZEKO DATUA (BARIABLEA)
//SARRERA ETA IRTEEREN PIN KOKALEKUAK:                               
int TENPERATURA = A1;     //SARRERA
int KONTSUMOA = A0;    //SARRERA
int SUTEA = A2;    //SARRERA
int MUGIMENDUA = 2;    //SARRERA
int BENTILADOREA = 3;    //IRTEERA
int ARGIA = 4;    //IRTEERA
int ALARMA = 5;    //IRTEERA
int ENTXUFEA = 6;    //IRTEERA
//PROGRAMAN ZEHAR ERABILITAKO BARIABLEAK:
int bentil;
int sute;
int sensore;
int argi;
int norbait;
int egoera;
long i=0;
int entxufe;
int atzerapena;
int tenala =25; //bentiladorea martxan ipintzeko haserako tenperatura
float sua;
float irakurketa,ff,pW,iA,vV,S_Ratio;
String readString = String(20); 

void setup()
    {
    // ARDUINOKO IRTEERAK:
    pinMode(BENTILADOREA,OUTPUT);
    pinMode(ARGIA,OUTPUT);
    pinMode(ALARMA,OUTPUT);
    pinMode(ENTXUFEA,OUTPUT);
    //ARDUINOKO SARRERAK:
    pinMode(TENPERATURA,INPUT);
    pinMode(MUGIMENDUA,INPUT);
    pinMode(SUTEA,INPUT);
    pinMode(KONTSUMOA,INPUT);
    sensore =0;
    egoera=0;
    entxufe=0;
    atzerapena = 5 ;
    S_Ratio = 36.5;      // Sensor/ratio (mV/mA ) : 36.5
    vV = 230;            
    ff = 5;             // freq. factor / (50Hz -> 5 / 60Hz -> 4.15)
    Ethernet.begin(mac, ip);
    //KALIBRATZEKO DENBORA
    delay (1000);
    //INTERRUPZIOA MUGIMENDUA
    attachInterrupt(0, MUGI , RISING );
    //PUERTO SERIERA KONEXIOA
    Serial.begin(9600);
    Serial.flush();
    }
//MUGIMENDU SENSOREAREN FUNTZIOA (inerrupzioa):    
void MUGI()

    {
        if (sensore==1)//modu automatikoan sentsorea
        {  
        digitalWrite(ARGIA, HIGH);//argia piztu
        norbait=1;//automatikoan eta argia piztuta
        } 
    }
//KONTSUMO IRAKURKETA FUNTZIOA:
 
float smoothread(float fc){   // fc (factor corrector)
      int ni = 15;          // n. de iteraciones => smooth,(ni) => rango 10 a 50 mejor promedio [smoothing]                     
      float retorno = 0.0;
      for (int x = 0; x< ni; x++)
      {
      do {                         // espero paso por cero  
          delayMicroseconds(20); 
          } while (analogRead(KONTSUMOA) != 0) ;
          delay (ff);            // espera centro de ciclo
          delay (10);            // estabilizacion CAD
          retorno= retorno +(analogRead(KONTSUMOA)*fc); 
      }
      return retorno / ni; 
}
void loop()
    {
      
             
            //TENPERATURA SEINALEA IRAKURTZEKO FORMULAK
            
            tenpera = analogRead(TENPERATURA); //sentsorearen irakurketa analogikoa
    
            tenpera = ((5.0 * tenpera * 100.0)/1024.0); //tenperatura definitu
        
            
                  //BENTILADOREA NOIZ IPINI MARTXAN
                        
                          if(tenpera>tenala) //tenperatura handiagoa tenala baino?
                            {
                              digitalWrite(BENTILADOREA, HIGH);
                              bentil=1;   //bentiladorea martxan dagoela dio
                            }
                          else
                            {
                              digitalWrite(BENTILADOREA, LOW); 
                              bentil=0;   //bentiladorea itzalia dagoela dio
                            }
                            
            //ETXEKO KONTSUMOA BIDALTZEKO ERAGIKETAK:
                irakurketa = smoothread (1) / 1.41;   // irakurketa (rms) berez,smoothread (1) / 1.41  
                iA = (irakurketa * S_Ratio)/1000;     // Intentsitatea (A)
                pW = (vV * iA);                       // Potentzia (kW)
            
            //KEA EDO SUTEA DETEKTATZEKO ETA  ALARMA NOIZ IPINI MARTXAN:
              sua =0;
              sua = analogRead(SUTEA); //kearen detektorearen irakurketa analogikoa          
         
                  if (sua >=100) //sentsorearen egoera/irakurketa 100 baino handiagoa bada
                  {
                    digitalWrite(ALARMA, HIGH); 
                    sute=1; //kea dagoenean
                  }
                  else
                  {
                    digitalWrite(ALARMA, LOW);
                    sute=0; //kerik ez dagoenean  
                  }
            //MUGIMENDU SENSOREA, ARGIA ITZALI 5 SEGUNDURA KONTADOREA ERABILIZ:
                  if (norbait==1) //argia piztua eta automatikoan badago
                   {
                     noInterrupts(); //interrupzioa desaktibatu
                     if (i==50) //40 denean argia itzali
                     {
                      digitalWrite (ARGIA, LOW);
                     
                     }
                       if (i>=100) //100 denean interrupzioa aktibatu
                     {
                       interrupts();
                       digitalWrite (ARGIA, LOW);
                       norbait=0; //argia itzalita
                       i=0;//kontadorea zerora ipini
                     }
                     i++; //kontadoreri bat gehitu
                    
                   } 
               
            //KLIENTEA  KONEKTATUA BADAGO:
                EthernetClient client = server.available();
                  if (client)
                  {
                    while (client.connected())
                    {
                        if (client.available())
                          {
                          char c = client.read(); //klienteak irakurtzen badu
                          
                              if (readString.length() < 30)//strina 30 baino txikiagoa bada
                              {
                          
                                readString += (c);
                              }
                                  if (c == '\n')
                                  
                                  {
                                    if(readString.indexOf("piztu")>3) //strinean piztu ageri bada:
                                      {
                                        digitalWrite(ARGIA, HIGH);
                                        noInterrupts();
                                        sensore=0; //manualean ipini
                                        egoera=1; //itzalita
                                       
                                      }
                                    if(readString.indexOf("itzali")>0) //strinean itzali ageri bada:
                                      {
                                        digitalWrite(ARGIA, LOW);
                                        noInterrupts(); //interrupzioa desaktibatu
                                        sensore=0; //manualean ipini
                                        egoera=0; //itzalita
                                       
                                      }
                                    if(readString.indexOf("auto")>3) //strinean auto ageri bada:
                                      {
                                        digitalWrite(ARGIA, LOW);
                                        interrupts(); 
                                        sensore=1;
                                        egoera=0;
                                        
                                      }
                                      if(readString.indexOf("manual")>3) //strinean manual ageri bada:
                                      {
                                        digitalWrite(ARGIA, LOW);
                                        noInterrupts(); 
                                        sensore=0;
                                        egoera=0;
                                        
                                      }
                                   if(readString.indexOf("entxon")>3) //strinean entxon ageri bada:
                                      {
                                        digitalWrite(ENTXUFEA, HIGH);
                                        entxufe=1;
                                      }  
                                   if(readString.indexOf("entxof")>3) //strinean entxof ageri bada:
                                      {
                                        digitalWrite(ENTXUFEA, LOW);
                                        entxufe=0;
                                      }  
                                   if(readString.indexOf("tenala")>3) //strinean tenala ageri bada:
                                      {
                                        //tenalaren ondoren dagoena tenala moduan gorde
                                        tenala=readString.substring(readString.indexOf("tenala")+6,readString.indexOf("tenala")+8).toInt();
                                        delay(10);
                                      }  
                                       
                                            //HTML HASIERA:
                              
                                            client.println("HTTP/1.1 200 OK");
                                            client.println("Content-Type: text/html");
                                            client.println();
                             
                                            client.print("<font size='20'>");
                                            client.print("<P>");
                                            
                                            //SERBIDORERA BIDALIKO DUGUNA(TXURRO GUZTIA):
                                            client.println("graduak");
                                            client.println(tenpera); //dagoen tenperatura
                                            client.println("c°");
                                            client.println("kontsumo");
                                            client.println(pW); //etxeko kontsumoa
                                            client.println("W       ");
                                            client.println("tenala");
                                            client.println(tenala); //mobiletik bidalitako datua
                                            
                                           //BENTILADORE EGOERA BIDALI:
                                         
                                           if (bentil) 
                                              {
                                               client.print("bentilon");
                                              }
                                           else 
                                              {
                                              client.print("bentilof");
                                              }
                                                                          
                                           //ALARMA EGOERA BIDALI:
                                         
                                           if (sute) 
                                              {
                                               client.print("suteon");
                                              }
                                           else 
                                              {
                                              client.print("suteof");
                                              }
                                                                                                                 
                                           //ARGIA EDO MUGIMENDU SENSORE EGOERA BIDALI:
                                         
                                           if (sensore==0) 
                                              {
                                               client.print("manual");
                                              }
                                           else 
                                              {
                                              client.print("auto");
                                              }
                                           if ((norbait==0) && (egoera==0))
                                              {
                                               client.print("itzalita");
                                              }
                                           else
                                              {
                                               if ((norbait==1) ||(egoera==1))
                                                {
                                               client.print("piztuta");
                                                }
                                              }
                                           //ENTXUFEAREN EGOERA:
                                            if (entxufe==1) 
                                              {
                                               client.print("entxuon");
                                              }
                                           else 
                                              {
                                              client.print("entxuof");
                                              }
                                              
                                          readString="";
                          
                                          delay(300); //denbora tarte bat datuak bidaltzeko
                                          client.stop(); //klientearekin konexioa amaitu
                                    }
                            }    
                        }
                      }
                   
    } 
