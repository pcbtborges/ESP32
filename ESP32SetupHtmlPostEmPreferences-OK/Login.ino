/*
  Reset/clear Preferences or first time user:
  User: seccam
  Pass: rstcamem

  Default user after clear Preferences:
  User: admin
  Pass: seccam
*/
void configParms(){
  loadHeading();
  loadStyle(); 
  getApName();
  WiFi.softAP(apName, "12345678");
  IPAddress IP = WiFi.softAPIP();
  DBG("AP IP address: ");
  DBGln(IP);

  // Original inspiration from html code at: https://techtutorialsx.com/2018/01/06/esp32-arduino-http-server-serving-html-and-javascript/
  // Assigning URLs to functions
  // When methods are not specified, one function will handle all
  // Data posted from forms are processed specifying HTTP_POST
  server.on("/",formLogin);                                  //enter login info
  server.on("/getLogin",        HTTP_POST, getLogin);        //check login data and navigate to getConfigData
  server.on("/enterConfigData", HTTP_GET,  enterConfigData); //type config data
  server.on("/getConfigData",   HTTP_POST, getConfigData);   //get config and store in preferences
  
  // Init web server
  server.begin();

  while(1){        
    static long restartTimer = millis();          
    server.handleClient();
    if(!userOK&&!passOK){
      if(restartTimer + (1000*60*10)<millis()){    //10min
        ESP.restart();    //if more than 10minutes in setup with no login, reboot system
      }
    }
    if(digitalRead(setupPin)!=LOW){   //when setup jumper removed system reboot after delay()
      delay(1000);
      ESP.restart();  
    }
  }
}

void formLogin(){
  String html = htmlHeading + htmlStyle;
  html += "</head>";
              
  // Web Page Heading
  html += "<body align=\"center\" onload=\"loadLoginParms();\"><br><br><br><h2><center>SecCam Login</center></h2>"; 
  html += "<form action='getLogin' method='post' id='loginForm' onSubmit='submitFormData(this);event.preventDefault();'>";                          
  html += "<table class='roundedCorners' cellpadding=\"10\" align=\"center\">";
  html += "<tr><td colspan=2 align=center><h2>Access Credentials</center></h2></td><tr>";
  html += "<tr><td>Administrador</td><td><input type=\"text\" name=\"adminUser\" id=\"adminUser\" maxlength=\"16\" value=\"\" size='16'></td></tr>";
  html += "<tr><td>Password</td><td><input type=\"password\" name=\"adminPass\" id=\"adminPass\" maxlength=\"16\" value=\"\" size='16'></td></tr>";
  html += "<tr><td colspan=2 align=center><input value=\"Login\" name=\"submit\" type=\"submit\"></td></tr>";
  html += "</table>";
  html += "</form>";
  html += "</body></html>";
  // Sending html code to client
  server.send(200, "text/html", html);
}

void getLogin() {
  userOK = false;
  passOK = false;
  bool resetUser = false;
  bool resetPass = false;
  // HTML form
  String html = htmlHeading + htmlStyle;
  html += "<body align='center'>";
  preferences.begin("AppParms", true);
  int ARGS = server.args()-1;
  for(int i = 0; i<ARGS; i++){
    String fieldName  = server.argName(i);
    String fieldValue = server.arg(i);
    if(fieldName=="adminUser"){
      if(fieldValue==preferences.getString("adminUser")){
        userOK=true;
      } 
      //bypassAdminUser
      if(fieldValue=="seccam"){
        resetUser=true;
      }           
    }
    if(fieldName=="adminPass"){
      if(fieldValue==preferences.getString("adminPass")){
        passOK=true;
      }        
      //byPassAdminPass
      if(fieldValue=="rstcamem"){
        resetPass=true;
      }        
    }
  }

  if(resetUser&&resetPass){
    preferences.end(); 
    preferences.begin("AppParms", false);
    preferences.clear();
    preferences.end(); 

    preferences.begin("AppParms", false);
    preferences.putString("adminUser", "admin");
    preferences.putString("adminPass", "seccam");        
    preferences.end(); 
    // Sending html code to client
    server.send(200, "text/html", "<body><br><br><br><h1><center>Memory Erased. Rebooting...</center></h1></body></html>");     
    ESP.restart();    //reboot system  
  }
  
  html += "<br><br><br><table class='roundedCorners' align='center'>";
  if(userOK==true && passOK==true){
    html += "<tr><th>Autentication OK...</th></tr>";
    html += "<tr><td><a href=enterConfigData>Click here for Configuration.</a></td></tr>";    
  }else{
    html += "<tr><th>Autentication Failed...</th></tr>";
    html += "<tr><td><a href=/>Click here to return</a></td></tr>";       
  } 
  html += "</table>";  
  html += "<br>";  
  html += "</body></html>";
  preferences.end();
  // Sending html code to client
  server.send(200, "text/html", html);
}

void enterConfigData(){
  String html = htmlHeading + htmlStyle;
  html += "<body align='center'>";

  if(!userOK||!passOK){
    html += "<br><br><br><table class='roundedCorners'>";
    html += "<tr><th>Autentication not valid...</th></tr>";
    html += "<tr><td><a href=/>Click here to return</a></td></tr>";
    html += "</table>";
  }else{
  
    //Javascript to populate html input fields from preferences stored on flash. "AppParms"
    html += "<script language=\"JavaScript\">";
    html += "function loadParms(){";           
    preferences.begin("AppParms", true);
    html += "document.getElementById(\"wifissid\").value        =\"" + preferences.getString("wifissid", "") + "\";";
    html += "document.getElementById(\"wifiPass\").value        =\"" + preferences.getString("wifiPass", "") + "\";";
    html += "document.getElementById(\"delayFirstPhoto\").value =\"" + preferences.getString("delayFirstPhoto", "") + "\";";
    html += "document.getElementById(\"delayBetPhotos\").value  =\"" + preferences.getString("delayBetPhotos", "") + "\";";
    html += "document.getElementById(\"totPhotos\").value       =\"" + preferences.getString("totPhotos", "") + "\";";
    html += "document.getElementById(\"fireFlash\").value       =\"" + preferences.getString("fireFlash", "") + "\";";                  
    html += "document.getElementById(\"timeGMT\").value         =\"" + preferences.getString("timeGMT", "") + "\";";                      
    html += "document.getElementById(\"sendtoemail\").value     =\"" + preferences.getString("sendtoemail", "") + "\";";                     
    html += "document.getElementById(\"smtpServer\").value      =\"" + preferences.getString("smtpServer", "") + "\";";                      
    html += "document.getElementById(\"smtpUser\").value        =\"" + preferences.getString("smtpUser", "") + "\";";                       
    html += "document.getElementById(\"smtpPass\").value        =\"" + preferences.getString("smtpPass", "") + "\";";                        
    html += "document.getElementById(\"emailto\").value         =\"" + preferences.getString("emailto", "") + "\";";                       
    html += "document.getElementById(\"emailTitle\").value      =\"" + preferences.getString("emailTitle", "") + "\";";                      
    html += "document.getElementById(\"emailMessage\").value    =\"" + preferences.getString("emailMessage", "") + "\";";                       
    html += "document.getElementById(\"sendtoftp\").value       =\"" + preferences.getString("sendtoftp", "") + "\";";                      
    html += "document.getElementById(\"ftpServer\").value       =\"" + preferences.getString("ftpServer", "") + "\";";                        
    html += "document.getElementById(\"ftpUser\").value         =\"" + preferences.getString("ftpUser", "") + "\";";                        
    html += "document.getElementById(\"ftpPass\").value         =\"" + preferences.getString("ftpPass", "") + "\";";          
    html += "document.getElementById(\"adminUser\").value       =\"" + preferences.getString("adminUser", "") + "\";";                        
    html += "document.getElementById(\"adminPass\").value       =\"" + preferences.getString("adminPass", "") + "\";";              
    preferences.end();                                           
    html += "}";
              
    html += "function submitFormData(form){";
  
    html += "if(isNaN(form.delayFirstPhoto.value)){";
    html += "alert('Error. This content must be numeric.');";
    html += "form.delayFirstPhoto.focus();";
    html += "return false;}";
 
    html += "if(isNaN(form.delayBetPhotos.value)){";
    html += "alert('Error. This content must be numeric.');";
    html += "form.delayBetPhotos.focus();";
    html += "return false;}";  

    html += "if(isNaN(form.totPhotos.value)){";
    html += "alert('Error. This content must be numeric.');";
    html += "form.totPhotos.focus();";
    html += "return false;}";

    html += "if(form.totPhotos.value>128){";    
    html += "alert('Error. Max number of shots 128.');";
    html += "form.totPhotos.focus();";
    html += "return false;}";       
  
    html += "if(isNaN(form.timeGMT.value)){";
    html += "alert('Error. This content must be numeric.');";
    html += "form.timeGMT.focus();";
    html += "return false;}";                                        
              
    html += "var wifissid         = form.wifissid.value;";
    html += "var wifiPass         = form.wifiPass.value;";             
    html += "var delayFirstPhoto  = form.delayFirstPhoto.value;";
    html += "var delayBetPhotos   = form.delayBetPhotos.value;";
    html += "var timeGMT          = form.timeGMT.value;";
    html += "var totPhotos        = form.totPhotos.value;"; 
    html += "var fireFlash        = form.fireFlash.value;";             
    html += "var sendtoemail      = form.sendtoemail.value;";
    html += "var smtpServer       = form.smtpServer.value;";
    html += "var smtpUser         = form.smtpUser.value;";
    html += "var smtpPass         = form.smtpPass.value;";
    html += "var emailto          = form.emailto.value;";
    html += "var emailTitle       = form.emailTitle.value;";
    html += "var emailMessage     = form.emailMessage.value;";
    html += "var sendtoftp        = form.sendtoftp.value;";
    html += "var ftpServer        = form.ftpServer.value;";
    html += "var ftpUser          = form.ftpUser.value;";
    html += "var ftpPass          = form.ftpPass.value;";
    html += "var adminUser        = form.adminUser.value;";
    html += "var adminPass        = form.adminPass.value;";    

    html += "document.form.submit();";
    html += "}";                         
              
    html += "function loadLoginParms(){";
    html += "document.getElementById(\"adminUser\").focus();";
    html += "}";
 
    html += "function ValidateEmail(mail){";
    html += "if (/^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/.test(mail.value)){";
    html += "return (true);";
    html += "}";
    html += "alert('Invalid Email Address!');";
    html += "return (false);";
    html += "}";
                  
    html += "function submitFormData(form){";     //Login Page
                 
    html += "if(form.adminUser.value===''){";
    html += "alert('Error. Admin username required.');";
    html += "form.adminUser.focus();";
    html += "return false;}";

    html += "if(form.adminUser.value.length<5){";
    html += "alert('Error. Admin username length less than 5 chars.');";
    html += "form.adminUser.focus();";
    html += "return false;}";                

    html += "if(form.adminPass.value===''){";
    html += "alert('Error. Admin password required.');";
    html += "form.adminPass.focus();";
    html += "return false;}";   

    html += "if(form.adminPass.value.length<5){";
    html += "alert('Error. Admin pass length must be > 5.');";
    html += "form.adminPass.focus();";
    html += "return false;}";  
                      
    html += "var adminUser       = form.adminUser.value;";
    html += "var adminPass       = form.adminPass.value;";
    html += "var jsonFormInfo    = JSON.stringify({";
    html += "adminUser             : adminUser,";
    html += "adminPass             : adminPass});";
    html += "document.form.submit();";
    html += "}";  
                                                     
    html += "</script>";
    html += "</head>";
              
    // Web Page Heading
    html += "<body onload=\"loadParms();\">";                            

    //SecCam config:  
    html += "<form action=\"/getConfigData\" method=\"post\" id=\"myForm\" onSubmit=\"submitFormData(this); event.preventDefault();\">";
    html += "<table class='roundedCorners' align=\"center\">";
    html += "<tr><th colspan=2 align=center><h2>Parametrization SecCam</center></h2></th><tr>";
    html += "<tr><td colspan=2 align=center><b>WiFi Connection Credentials</b><br></td><tr>";
    html += "<tr><td>SSID</td><td><input type=\"text\" name=\"wifissid\" id=\"wifissid\" maxlength=\"16\" value=\"\"></td></tr>";
    html += "<tr><td>Password</td><td><input type=\"password\" name=\"wifiPass\" id=\"wifiPass\" maxlength=\"16\" value=\"\"></td></tr>";
 
    html += "<tr><td colspan=2 align=center><b>Photografic Parms: </b>(em segundos)</td></tr>";
    html += "<tr><td>Delay for the first picture</td><td><input type=\"text\" name=\"delayFirstPhoto\" id=\"delayFirstPhoto\" maxlength=\"2\" size='8'></td></tr>";
    html += "<tr><td>Delay between pictures</td><td><input type=\"text\" name=\"delayBetPhotos\" id=\"delayBetPhotos\" maxlength=\"2\" size='8'></td></tr>";
    html += "<tr><td>Total pictures per Event</td><td><input type=\"text\" name=\"totPhotos\" id=\"totPhotos\" maxlength=\"3\" size='8'></td></tr>";
    html += "<tr><td>Fire Flash?</td><td align=center><select name=\"fireFlash\" id=\"fireFlash\"><option value=1>Sim</option><option value=0>Não</option></select></td></tr>";
    html += "<tr><td>Time Zone</td><td align=center><select name=\"timeGMT\" id=\"timeGMT\"><option value=-3>GMT-3 Brasilia</option><option value=2>GMT+2 S.Africa</option><option value=1>GMT+1 França</option></select></td></tr>";

    html += "<tr><td colspan=2 align=center><b>Send Pictures by Email?</b>&nbsp;&nbsp;<select name=\"sendtoemail\" id=\"sendtoemail\"><option value=1>Sim</option><option value=0>Não</option></select></td></tr>";
    html += "<tr><td>URL Email Server</td><td><input type=\"text\" name=\"smtpServer\" id=\"smtpServer\" maxlength=\"32\"></td></tr>";
    html += "<tr><td>Sender: Email account</td><td><input onfocusout=\"ValidateEmail(this)\" type=\"text\" name=\"smtpUser\" id=\"smtpUser\" maxlength=\"64\"></td></tr>";
    html += "<tr><td>Sender: Email Password</td><td><input type=\"password\" name=\"smtpPass\" id=\"smtpPass\" maxlength=\"32\"></td></tr>";
    html += "<tr><td>Destination Email</td><td><input onfocusout=\"ValidateEmail(this)\" type=\"text\" name=\"emailto\" id=\"emailto\" maxlength=\"128\"></td></tr>";
    html += "<tr><td>Email Title</td><td><input type=\"text\" name=\"emailTitle\" id=\"emailTitle\" maxlength=\"128\"></td></tr>";
    html += "<tr><td>Email Message</td><td><input type=\"text\" name=\"emailMessage\" id=\"emailMessage\" maxlength=\"128\"></td></tr>";
 
    html += "<tr><td colspan=2 align=center><b>Send images to FTP Server?</b>&nbsp;&nbsp;<select name=\"sendtoftp\" id=\"sendtoftp\"><option value=1>Sim</option><option value=0>Não</option></select></td></tr>";
    html += "<tr><td>URL FTP Server</td><td><input type=\"text\" name=\"ftpServer\" id=\"ftpServer\" maxlength=\"32\"></td></tr>";
    html += "<tr><td>FTP User</td><td><input type=\"text\" name=\"ftpUser\" id=\"ftpUser\" maxlength=\"32\"></td></tr>";
    html += "<tr><td>FTP Password</td><td><input type=\"password\" name=\"ftpPass\" id=\"ftpPass\" maxlength=\"16\"></td></tr>";              
    html += "<tr><td>System Admin</td><td><input type=\"text\" name=\"adminUser\" id=\"adminUser\" maxlength=\"16\"></td></tr>";
    html += "<tr><td>Admin Password</td><td><input type=\"password\" name=\"adminPass\" id=\"adminPass\" maxlength=\"16\"></td></tr>";              
    html += "<tr><td colspan=2 align=center><input value=\"Save Configuration\" name=\"submit\" type=\"submit\"></td></tr>";
    html += "</table>";
    html += "</form>";
  }
  html += "</body></html>";
  // Sending html code to client
  server.send(200, "text/html", html);
}  

void getConfigData(){
  String html = htmlHeading + htmlStyle;
  html += "<body align='center'>";
  preferences.begin("AppParms", false);
  int ARGS = server.args()-1;
  for(int i = 0; i<ARGS; i++){
    preferences.putString(server.argName(i).c_str(), server.arg(i).c_str());
  }
  preferences.end();
  html += "<br><br><br><table class='roundedCorners' align='center'>";
  html += "<tr><th>Data Sucessfuly Saved...</th></tr>";
  html += "<tr><td><a href=enterConfigData>Click here to Return.</a></td></tr>";
  html += "</table>";  
  html += "</body></html>";
  // Sending html code to client
  server.send(200, "text/html", html);  
}

void getApName(){
  uint8_t baseMac[6];
  // Get MAC address for WiFi station
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  // Write unique name into apName
  sprintf(apName, "SecCAM-%02X%02X%02X%02X%02X%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
  return;
}

void loadHeading(){
  // Display the HTML login web page
  htmlHeading  = "<!DOCTYPE html><html lang=pt-BR>";
  htmlHeading += "<head>";
  htmlHeading += "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>";
  htmlHeading += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  htmlHeading += "<link rel=\"icon\" href=\"data:,\">";
}

void loadStyle(){
  // CSS to style the on/off buttons 
  // Feel free to change the background-color and font-size attributes to fit your preferences
  htmlStyle   += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: left;}";
  htmlStyle   += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;";
  htmlStyle   += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  htmlStyle   += ".button2 {background-color: #555555;}";       
  htmlStyle   += "table.roundedCorners {"; 
  htmlStyle   += "border: 1px solid DarkOrange;";
  htmlStyle   += "border-radius: 13px; ";
  htmlStyle   += "border-spacing: 0;";
  htmlStyle   += "}";
  htmlStyle   += "table.roundedCorners td,"; 
  htmlStyle   += "table.roundedCorners th {"; 
  htmlStyle   += "border-bottom: 1px solid DarkOrange;";
  htmlStyle   += "padding: 2px;"; 
  htmlStyle   += "}";
  htmlStyle   += "table.roundedCorners tr:last-child > td {";
  htmlStyle   += "border-bottom: none;";
  htmlStyle   += "}";
  htmlStyle   += "</style>";  
 }
