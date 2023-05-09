The Arduino_UDP_v1 file is the arduino setup with the udp protocol

In the folders are the needed zip libraries for the specific electronic components, which might have to be imported under "Sketch" -> "Include Library" -> "Add .ZIP labrary..."
Other Libraries can be found in the labrary manager:
	SPI
	WiFiNINA
	WiFiUdp
	LiquidCrystal
As well as external libraries, which is in zip files:
	LedControlMS (LedControl.zip) //8x8 led matrix
	dht11 (adidax-dht11-b7fbbcd.zip) //temp & hum sensor

Make sure to have downloaded "Arduino megaAVR Borads", as Arduino Uno Wifi Rev2 is under this pack. (Which is the one used for this project)
Can be found under "Tools" -> "Board:" -> "Boards Manager..."

Make sure to change ssid[] and pass[] to your internets name and password, as well as the serverIPAddress(x,x,x,x) to IP found when running the index.js as a node server in the command prompt - Write with comma (,) instead of dots (.)
