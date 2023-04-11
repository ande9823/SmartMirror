using UnityEngine;
using KyleDulce.SocketIo;

public class IoT : MonoBehaviour
{
    private bool runLocal = true;
    Socket socket;

    private string currentLEDValue = "0";
    private string currentPotValue = "0";
    private float potRotation = 0.0f;

    // Start is called before the first frame update
    void Start()
    {
        /*--------------               Connect to Server                                    ----------------------*/
        if (runLocal)
        {
            Debug.Log("Connect to Local Server");
            socket = SocketIo.establishSocketConnection("ws://localhost:3000");
        }
        else
        {
            Debug.Log("Connect to Online Server");
            socket = SocketIo.establishSocketConnection("ws://sdu-e22-iot-v1.eu-4.evennode.com");
        }

        //Connect to server
        socket.connect();
        Debug.Log("Socket IO - Connected");



        /*--------------               Receive Updates                                    ----------------------*/
        //On "CurrentLEDValue"
        socket.on("CurrentLEDValue", SetCurrentLEDValue);

        //On "CurrentPotentiometerValue"
        socket.on("CurrentPotentiometerValue", SetCurrentPotentiometerValue);
    }

    void SetCurrentLEDValue(string data)
    {
        currentLEDValue = data;
        Debug.Log("CurrentLEDValue Received: " + currentLEDValue);
    }

    void SetCurrentPotentiometerValue(string data)
    {
        currentPotValue = data;
        Debug.Log("CurrentPotValue Received: " + currentPotValue);
    }

    void Update()
    {
        //On space - Update LED Value
        if (Input.GetKeyDown("space"))
        {
            if (currentLEDValue == "0")
            {
                currentLEDValue = "1";
            }
            else
            {
                currentLEDValue = "0";
            }

            //Emit new value to server
            socket.emit("UpdateCurrentLEDValue", currentLEDValue);
            Debug.Log("New LED value emitted to server: " + currentLEDValue);
        }

        //Rotate 3D Object
        potRotation = 50+float.Parse(currentPotValue)/4;
        this.transform.rotation = Quaternion.Euler(0.0f, potRotation, 0.0f);
    }
}

