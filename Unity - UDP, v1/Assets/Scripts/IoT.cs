using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using KyleDulce.SocketIo;

public class IoT : MonoBehaviour
{
    private bool runLocal = true;
    Socket socket;

    //private string currentLEDValue = "0";
   /* private string currentPotValue = "0";
    private float potRotation = 0.0f;
    */
    private string currentTemperature = "0";
    private string currentHumidity = "0";
    public Text tempText;
    public Text humText;


    [SerializeField]
    List<string> displayList = new List<string>();

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
        //socket.on("CurrentLEDValue", SetCurrentLEDValue);
        /*
        //On "CurrentPotentiometerValue"
        socket.on("CurrentPotentiometerValue", SetCurrentPotentiometerValue);
        */
        //On "CurrentTemperature"
        socket.on("CurrentTemperature", SetCurrentTemperature);

        //On "CurrentHumidity"
        socket.on("CurrentHumidity", SetCurrentHumidity);
    }
    /*
    void SetCurrentLEDValue(string data)
    {
        currentLEDValue = data;
        Debug.Log("CurrentLEDValue Received: " + currentLEDValue);
    }*/
    /*
    void SetCurrentPotentiometerValue(string data)
    {
        currentPotValue = data;
        Debug.Log("CurrentPotValue Received: " + currentPotValue);
    }
    */
    void SetCurrentTemperature(string data) {
        currentTemperature = data;
        Debug.Log("Current Temp Received: " + currentTemperature);
        tempText.text = currentTemperature;
    }

    void SetCurrentHumidity(string data) {
        currentHumidity = data;
        Debug.Log("Current Humidity Received: " + currentHumidity);
        humText.text = currentHumidity;
    }

    void Update()
    {
        /*
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
        }*/
        /*
        //Rotate 3D Object
        potRotation = 50+float.Parse(currentPotValue)/4;
        this.transform.rotation = Quaternion.Euler(0.0f, potRotation, 0.0f);
        */
    }

    public void SendText(Text textField) {
        string submitText = textField.text.ToString();
        //Sends new value to server with the function name "UpdateTextField".
        socket.emit("UpdateTextField", submitText);
        Debug.Log("New text emitted to server: " + submitText);
    }

    public void SetToggles(Text label) {
        string labelText = label.text.ToString();
        
        if(!displayList.Contains(labelText)) {
            displayList.Add(labelText);
        } else if (displayList.Contains(labelText)) {
            displayList.Remove(labelText);
        }
        foreach(var x in displayList) {
            Debug.Log("Array contains: " + x.ToString());
        }
        Debug.Log("End of List!");
    }

    public void SendList() {
        if (displayList.Count > 0) {
            string jsonText = Newtonsoft.Json.JsonConvert.SerializeObject(displayList);
            //Sends new value to server with the function name "UpdateToggleValues".
            socket.emit("UpdateToggleValues", jsonText);
            Debug.Log("List emitted to server: " + jsonText);
        } else {
            string noListMsg = "There are no elements in the list!";
            //Sends new value to server with the function name "UpdateToggleValues".
            socket.emit("UpdateToggleValues", noListMsg);
            Debug.Log(noListMsg);
        }
    }
}

