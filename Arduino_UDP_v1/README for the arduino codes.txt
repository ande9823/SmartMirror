The Arduino_UDP_v1 file was only a test early on (will be changed when the other two files are done)

Arduino_UDP_8x8 file in folder by same name is the arduino setup with the udp protocol.

Arduino_8x8-Setup file in folder by same name is a setup and test of the 8x8 led matrices, where user can write in the console what is to be displayed.

In the folders are the needed zip libraries for the specific electronic components, which might have to be imported under "Sketch" -> "Include Library" -> "Add .ZIP labrary..."
Other Libraries can be found in the labrary manager:
	SPI
	WiFiNINA
	WiFiUdp
	LiquidCrystal

Make sure to have downloaded "Arduino megaAVR Borads", as Arduino Uno Wifi Rev2 is under this pack. (Which is the one used for this project)
Can be found under "Tools" -> "Board:" -> "Boards Manager..."

Make sure to change ssid[] and pass[] to your internets name and password, as well as the serverIPAddress(x,x,x,x) to IP found when running the index.js as a node server in the command prompt - Write with comma (,) instead of dots (.)
