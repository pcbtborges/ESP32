/*String arg(String name); -- recupera o valor do campo pelo nome do parâmetro
  String arg(int i); -- recupera o valor do campo pelo índice do parâmetro
  String argName(int i); -- recupera o nome do campo pelo índice do parâmetro
  int args(); -- recupera a quantidade de parâmetros
  bool hasArg(String name); -- verifica se o parâmetro existe
*/

#define DEBUG_ESP              //comment out to activate debug console

#ifdef DEBUG_ESP
  #define DBG(x)   Serial.print(x)
  #define DBGln(x) Serial.println(x)
#else 
  #define DBG(...)
  #define DBGln(...)
#endif

#include <WiFiClientSecure.h>
#include <WebServer.h>
WebServer server(80);

#include <Preferences.h>
Preferences preferences;

char apName[] = "xxxxxxxxxxxxxxxxxxx";  //19
String htmlHeading;
String htmlStyle;
bool userOK=false,passOK=false;                 //prevents browsing without authentication
byte triggerPin = 13;
byte setupPin = 15;

//Function Prototype
void configParms();
void formLogin();
void getLogin();
void enterConfigData();
void getConfigData();
void getApName();

void setup() {
  // Iniciando Serial
  Serial.begin(115200);
  pinMode(setupPin, INPUT);
  if(digitalRead(setupPin)==LOW){
    DBGln("Starting Config...");
    configParms();
  }
  showParms();
}

void showParms(){
  preferences.begin("AppParms", true);
  DBGln("");
  DBG("wifissid: ");         
  DBGln(preferences.getString("wifissid", ""));
  DBG("wifiPass: ");         
  DBGln(preferences.getString("wifiPass", ""));
  DBG("delayFirstPhoto: ");    
  DBGln(preferences.getString("delayFirstPhoto", ""));
  DBG("delayBetPhotos: ");  
  DBGln(preferences.getString("delayBetPhotos", ""));
  DBG("totPhotos: ");         
  DBGln(preferences.getString("totPhotos", ""));
  DBG("fireFlash: ");        
  DBGln(preferences.getString("fireFlash", ""));                   
  DBG("timeGMT: ");          
  DBGln(preferences.getString("timeGMT", ""));                      
  DBG("sendtoemail: ");      
  DBGln(preferences.getString("sendtoemail", ""));                     
  DBG("smtpServer: ");       
  DBGln(preferences.getString("smtpServer", ""));                      
  DBG("smtpUser: ");         
  DBGln(preferences.getString("smtpUser", ""));                       
  DBG("smtpPass: ");         
  DBGln(preferences.getString("smtpPass", ""));                        
  DBG("emailto: ");          
  DBGln(preferences.getString("emailto", ""));                       
  DBG("emailTitle: ");       
  DBGln(preferences.getString("emailTitle", ""));                      
  DBG("emailMessage: ");     
  DBGln(preferences.getString("emailMessage", ""));                       
  DBG("sendtoftp: ");        
  DBGln(preferences.getString("sendtoftp", ""));                      
  DBG("ftpServer: ");        
  DBGln(preferences.getString("ftpServer", ""));                        
  DBG("ftpUser: ");          
  DBGln(preferences.getString("ftpUser", ""));                        
  DBG("ftpPass: ");          
  DBGln(preferences.getString("ftpPass", ""));          
  DBG("adminUser: ");        
  DBGln(preferences.getString("adminUser", ""));                        
  DBG("adminPass: ");        
  DBGln(preferences.getString("adminPass", ""));
  preferences.end();
  DBGln("-----------------------------------"); 
}

void loop() {
  //Your code here. Inserir seu código aqui.
}
