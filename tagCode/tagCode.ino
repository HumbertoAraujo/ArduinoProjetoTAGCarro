#include <SoftwareSerial.h>

#define USER1 "3F0054E76DE1" //ID TAG1 CADASTRADA  
#define USER2 "3F0082CD4030" //ID TAG2 CADASTRADA   
#define USER3 "3F005639D282" //ID TAG3 CADASTRADA  

SoftwareSerial rfid = SoftwareSerial(2, 3); // DEFININDO PORTAS PARA O MÓDULO RFID RDM6300

//DECLARAÇÃO DAS VARIAVÉIS GLOBAIS

String apoio;
int trava = 4; //PINO TRAVA
int destrava = 5; //PINO DESTRAVA
int bomba = 6; //PINO BOMBA
int sirene = 7; //PINO SIRENE
int statusbomba = 0; //LER STATUS DA BOMBA

void setup()  
{
  //DEFINIÇÃO DO ESTADO INCIAL DO PROGRAMA E MÓDULOS
  
  Serial.begin(9600);
  rfid.begin(9600);
  pinMode(trava, OUTPUT);
  pinMode(destrava, OUTPUT);
  pinMode(bomba, OUTPUT);
  pinMode(sirene, OUTPUT);  
  digitalWrite(trava,HIGH);
  digitalWrite(destrava,HIGH);
  digitalWrite(bomba,HIGH);  
  digitalWrite(sirene,HIGH);  
  Serial.println("APROXIME A TAG");
 
  
}

//DECLARAÇÃO DE VARIÁVEL LOCAL PARA ARMAZENAR LEITURA MOMENTÂNEA

char leitura;

void loop(){
  
  while(rfid.available()>0){ // AGUARDA O LEITOR RECONHECER ALGUM SINAL RFID PARA RODAR PROGRAMAR
    leitura=rfid.read(); //ARMAZENA A LEITURA
    apoio += leitura;    //VARIÁVEL GLOBAL DE APOIO RECEBE A LEITURA PARA PODER SER USADA NA VALIDAÇÃO DO CADASTRO
    Serial.println(apoio);  
    Serial.println(apoio.length());
  }
  apoio=apoio.substring(1,13); //FATIA A STRING PARA OS 12 CARACTERES DA TAG QUE FOI APROXIMADA
               
  //APLICANDO A CONDIÇÃO DE COMPARAÇÃO DAS VARIÁVEIS LIDA COM AS TAGS CADASTRADAS, CASO VALIDADA, EXECUTANDO A FUNÇÃO pulso DE AÇÃO PARA TRAVAR OU DESTRAVAR O VEÍCULO.      
 
  if(apoio.indexOf(USER1)>=0) pulso(); 
  else if(apoio.indexOf(USER2)>=0) pulso(); 
  else if(apoio.indexOf(USER3)>=0) pulso();
  else if(apoio != 0){
    Serial.println("TAG INVÁLIDA - APROXIME TAG VÁLIDA"); 
    
  }
  
   
  //LIMPANDO A VARIÁVEL DE APOIO
  
  apoio="";
  
}

//FUNÇÃO DE TRAVA, REALIZANDO O TRAVAMENTO DO CARRO, BLOQUEIO DA BOMBA/IGNIÇÃO, E ACIOANDO UM "BEEP" DA SIRENE/BUZINA.
 
void tagtrava()
{
      digitalWrite(trava,LOW);
      digitalWrite(sirene,LOW);
      digitalWrite(bomba,LOW); 
      delay(500); 
      digitalWrite(trava,HIGH);
      digitalWrite(sirene,HIGH);
      Serial.println("INICIANDO TRAVAMENTO");
      Serial.println("PULSO TRAVA MEIO SEGUNDO ENVIADO");      
      Serial.println("BOMBA COMBUSTÍVEL DESLIGADA");
      Serial.println("SIRENE TRAVAMENTO DISPARADA");
      Serial.println("TRAVAMENTO OK");
      delay(2000);
      Serial.println("ENCOSTE A TAG");  
  }

//FUNÇÃO DE DESTRAVA, REALIZANDO O DESTRAVAMENTO DO CARRO, DESBLOQUEIO DA BOMBA/IGNIÇÃO, E ACIOANDO DOIS "BEEPS" DA SIRENE/BUZINA.
  
void tagdestrava()

{
      digitalWrite(destrava,LOW);
      digitalWrite(sirene,LOW); 
      digitalWrite(bomba,HIGH); 
      delay(500);
      digitalWrite(destrava,HIGH);
      digitalWrite(sirene,HIGH); 
      delay(500); 
      digitalWrite(sirene,LOW); 
      delay(500); 
      digitalWrite(sirene,HIGH); 
      Serial.println("PULSO DESTRAVA 1 SEGUNDO ENVIADO");
      Serial.println("BOMBA COMBUSTÍVEL LIGADA");  
      Serial.println("SIRENE DESTRAVAMENTO DISPARADA");
      Serial.println("DESTRAVAMENTO OK");
      delay(2000);
      Serial.println("ENCOSTE A TAG");
  }

//FUNÇÃO CRIADA PARA VER O STATUS DA PORTA BOMBA, PARA PODER SABER QUAL AÇÃO DEVE SER TOMADA NO MOMENTO DO PULSO, SE SERÁ TRAVAMENTO, OU DESTRAVAMENTO


void lerbomba()
  {
      statusbomba=digitalRead(bomba);
            
  }

//FUNÇÃO CRIADA PARA APÓS LER O STATUS DA BOMBA, EXECUTAR A AÇÃO DE TRAVAMENTO, OU DESTRAVAMENTO


void pulso()
{

lerbomba();
 
 if (statusbomba == LOW)
  {
  tagdestrava();
}

else {
  tagtrava();
  }
      
}
