#include <SoftwareSerial.h>                                     // imoporta una libreria
                                                                
#define rxPin 10                                                
#define txPin 8
// Set up a new SoftwareSerial object
// attributi tx e rx 
//metodi begin() avaiable() read() print() println()
//begin serve a configurare la velocita della porta seriale
//avaiable usato per verificare presenza dati trasmessi da modulo wifi per arduino
//print serve ad inviare i dati ad arduino al modulo wifi
//read serve a leggere i dati dal moduleìo wifi
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);        // creazione oggetto

String check4answer(){                                          //verifica la presenza di una risposta dati
    String str = "";    
    while (mySerial.available() > 0) {                          //verifica disponibilità dati
        char c = mySerial.read();                               //lettura dati
        Serial.print(c);                                        //verifico risposta modulo wifi
        str += String(c);                                       //compone la stringa di risposta del modulo wifi
    }
    return str;
}

//invia comandi al modulo wifi ovvero sp01
String esp01cmd(String cmd) {
  Serial.println("sending: " + cmd);    //stampo su monitor seriale l'invio del comando (debug)
  mySerial.println(cmd);                //invio dati 
  delay(10);
  String risposta = check4answer();     //verifica risposta modulo wifi
  return risposta;                      //restituisce risposta modulo wifi
    }

void setup()  {
    // Define pin modes for TX and RX
    // pinMode(rxPin, INPUT);
    // pinMode(txPin, OUTPUT);
    
    // Set the baud rate for the SoftwareSerial object
    mySerial.begin(115200);
    Serial.begin(9600);
    delay(1000);
    esp01cmd("AT");
    delay(1000);
    esp01cmd("AT+CWMODE=2");    //seleziona la modalita del access point 
    delay(1000);
    esp01cmd("AT+CWSAP=\"robottino\",\"robottino\",1,4");   //impostiamo ssid e password del access point 
    delay(1000);
    esp01cmd("AT+CIFSR"); //mostra indirizzo ip dell'access point
    esp01cmd("AT+CIPMUX=1"); //consente massimo ad una connessione per volta
    
    
    Serial.println("ESP-01 Configuration Completed");
}

void loop() {
    Serial.println("loop...");
    while(esp01cmd("AT+CWLIF").substring(11,18) != "192.168") {     //at+cwlif serve a restituire l'indirizzo ip del client. verifica la presenza di un client collegato
      Serial.println("no connections so far... still waiting");
      delay(1000);
    }
    //dopo che viene stabilita una connessione 
    Serial.println("Connection from remote device was Established!!!");
    //Socket ID: 3
    //accept packets from any IP address/devices
    //Listen to local port 4567
    //outgoing packets could go to any remote host without restrictions...
    esp01cmd("AT+CIPSTART=3,\"UDP\",\"0.0.0.0\",0,4567,2"); //starting UDP Socket Server (programma che ti mette in ascolto e in attesa di ricevere un pacchetto UDP)
    
//    String str = ;    
//    Serial.println("received: "+esp01cmd("AT+CWLIF").substring(11,18));
    delay(3000);
    while(true) {
      String str = mySerial.readString();
      if(str != "") {
        int startOfSTR = str.indexOf(":",10)+1;
        Serial.println("Received: "+str.substring(startOfSTR));
        //Serial.println("Received: "+str);
        //Serial.println(startOfSTR);
      }
    }

}
