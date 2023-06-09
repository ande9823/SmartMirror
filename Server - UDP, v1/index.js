// ------------------------------ setup the server ------------------------------
var express = require('express');
var app = express();
var server = require('http').createServer(app);
var path = require('path');
app.use(express.static(path.join(__dirname, '')));

//server variables
/*
var currentPotValue = 0;
*/
var currentLEDValue = 0;

//start server
server.listen(3000, function () {
    console.log("Server listening on port: 3000\n");
});

//on HTTP GET "/" (e.g.: http://localhost:3000/)
app.get('/', function (req, res) {
    //send index.html to client browser
    res.sendFile(__dirname + '/index.html');
});





// ------------------------------ Find and print the server IP-address ------------------------------ 
var networkInterfaces = require('os').networkInterfaces();
var address;

Object.keys(networkInterfaces).forEach(function(interface) {
    networkInterfaces[interface].filter(function(details) {
        if (details.family === 'IPv4' && details.internal === false) {
            address = details.address;
        }
    });
});

console.log("Server IP-address: " + address + "\n");




// ------------------------------ ARDUINO -------------------------------
// ------------------------------ setup UDP (user datagram protocol) ------------------------------ 
var UPD = require("dgram");
var UDPsocket = UPD.createSocket('udp4');

//set the port for which to listen for UDP messages on
UDPsocket.bind(3001);

//store arduino IP info
var arduinoIPAddress;
var arduinoPort;

//on error
UDPsocket.on('error', function (error) {
    console.error("UDP error: " + error.stack + "\n");
});

//on begin listening
UDPsocket.on('listening', function () {
    var listenAtPort = UDPsocket.address().port;
    console.log('Server listening for UDP packets at port: ' + listenAtPort + "\n");
});

//on received message - Recieve data from Arduino
UDPsocket.on('message', (msg, senderInfo) => {
    console.log("Received UDP message: " + msg);
    console.log("From addr: " + senderInfo.address + ", at port: " + senderInfo.port + "\n");

    //recieve "temp,hum" 
    currentTempHumValues = msg.toString();
    /*
    //currentPotValue = msg.toString();
*/
    arduinoIPAddress = senderInfo.address;
    arduinoPort = senderInfo.port;
    
    //Calls function which sends the temp and humidity values to unity.
    EmitTempHumValues();
    /*
    //EmitPotValue();
*/
    //send acknowledgement message
    //sendUDPMessage(arduinoIPAddress, arduinoPort, "SERVER: The message was received");
});

//send UDP message - Sends data to Arduino from server
function sendUDPMessage(receiverIPAddress, receiverPort, message) {
    var UDPMessage = Buffer.from(message);
    UDPsocket.send(UDPMessage, receiverPort, receiverIPAddress, function (error) {
        if (error) {
            client.close();
        } else {
            console.log('UDP message sent: ' + message + "\n");
        }
    });
}



// ------------------------------ Unity -------------------------------
// ------------------------------ setup Socket.IO ------------------------------ 
var io = require('socket.io')(server);

//on new client connection
io.on('connection', function(IOsocket) {
    console.log("Client has connected" + "\n");

    //on client disconnection
    IOsocket.on('disconnect', function () {
        console.log("Client has disconnected" + "\n");
    });

    //on "UpdateCurrentLEDValue"
    IOsocket.on('UpdateCurrentLEDValue', function (data) {
        console.log("Current LED Value received from client: " + data + "\n");
        currentLEDValue = data;

        io.emit('CurrentLEDValue', currentLEDValue);

        // <<<<< Sends data as last parameter to arduino >>>>>
        //If arduino, send LED value with UDP
        if (arduinoIPAddress != null && arduinoPort != null) {
            sendUDPMessage(arduinoIPAddress, arduinoPort, currentLEDValue)
        }
    });
    
    //on "UpdateTextField"
    IOsocket.on('UpdateTextField', function (data) {
        console.log("Current textfield Value received from client: " + data + "\n");
        currentTextFieldValue = data;

        io.emit('CurrentTextValue', currentTextFieldValue);

        // <<<<< Sends data as last parameter to arduino >>>>>
        //If arduino, send LED value with UDP
        if (arduinoIPAddress != null && arduinoPort != null) {
            sendUDPMessage(arduinoIPAddress, arduinoPort, currentTextFieldValue)
        }
    });
    
    //on "UpdateToggleValues"
    //Recieves data from unity through the function 'UpdateToggleValues'.
    IOsocket.on('UpdateToggleValues', function (data) {
        console.log("Current toggle Values received from client: " + data + "\n");
        //Sets the value variable.
        currentToggleValues = data;
        //Sends the value variable to the website with the function 'CurrentToggleValue'.
        io.emit('CurrentToggleValue', currentToggleValues);

        
        //Can send data to arduino with UDP Here like above.
    });
});
/*
//emit "CurrentPotentiometerValue"
function EmitPotValue() {
    io.emit('CurrentPotentiometerValue', currentPotValue);
}
*/
function EmitTempHumValues() {
    //splits the string sent from arduino into temp and humidity values and sends to unity.
    const splitArray = currentTempHumValues.split(",");
    var temp = splitArray[1];
    var hum = splitArray[0];
    io.emit('CurrentTemperature', temp);
    io.emit('CurrentHumidity', hum);
}
