# ESP32
ESP32 based code

ESP32 parameters setup via web browser.

This is part of a ESP32-CAM project to setup app parameters using a web browser.
For this project one imput pin is selected (15) as the setup GPIO pin.
When grownded the sketch will branch out of the main app flow to execute setup.
The Sketch then initializes WiFi in AP mode so it is easy to locate it (192.168.4.1)
Browse to the above address to enter setup.
Username and Password to first time user (User: seccam, Pass: rstcamem)
This procedure will initialize the Preferences storage area.
Next login as defaul Admin as follows (User: admin, Pass: seccam)
Once login is sucessfull fill up the parameters page and save.
Remove jumper from pin 15 and the ESP32 will automaticaly reboot and show current content of preferences.
Notice that web setup is only available while the jumper sets pin 15 to ground.
Any available GPIO pin will work for this purpose and can have a different purpose during app run.

Login.ino must be copied into a folder of the main sketch

This setup routine can be used in any app of your choice with few modifications to match your needs for different parameters.
This is part of an ESP32-CAM app that can send pictures via email and to a FTP server and will soon be publicly available.
