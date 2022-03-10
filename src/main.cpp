#define LEFT_PIN_A 1        // Обычный пин для мотора слева
#define LEFT_PIN_B 1        // ШИМ пин для мотора слева
#define RIGHT_PIN_A 1       // Обычный пин для мотора справа
#define RIGHT_PIN_B 1       // ШИМ пин для мотора справа
#define MAX_SPEED 1023      // Максимальная скорость от 0 до 1024
#define MIN_DUTY 0          // Минимальная скорость от 0 до 1024

#include <Arduino.h>        // Main file for Arduino framework
#include "GParser.h"        // Librarie for parse string
#include "GyverMotor.cpp"   // Librarie for control motor
#include "wifi.h"
GMotor LMotor(DRIVER2WIRE, LEFT_PIN_A, LEFT_PIN_B, HIGH);
GMotor RMotor(DRIVER2WIRE, RIGHT_PIN_A, RIGHT_PIN_B, HIGH);

WiFiServer server(PORT);

void handle_root(WiFiClient &client, char* req);
void send_file(WiFiClient &client, char* path);
void update_motors();

int spd = MAX_SPEED / 2;
GM_workMode mode[] = {STOP, STOP};

void setup() {
    // Serial and Wi-Fi
    Serial.begin(115200);
    WiFiTools::connectWifi();
    server.begin();

    // SPIFFS
    SPIFFS.begin();

    // Motors
    LMotor.setResolution(10);
    RMotor.setResolution(10);

    LMotor.setMinDuty(MIN_DUTY);
    RMotor.setMinDuty(MIN_DUTY);

    LMotor.setMode(STOP);
    RMotor.setMode(STOP);
}


void loop() {
    WiFiClient client = server.available();
    if (client){
        char buf[100];
        uint8_t ind = 0;
        while (client.connected()){
            if (client.available()){
                char ch = client.read();
                if (ch == '\r') {
                    buf[ind] = '\0';
                    handle_root(client, buf);
                    return;
                }
                buf[ind++] = ch;
            }
        }
        client.stop();
    }
}

void handle_root(WiFiClient& client, char* req){
    Serial.printf("[%d:%d] --- %s\n", (millis()/60000) % 60, (millis()/1000) % 60, req);
    GParser parser(req, ' ');
    parser.split();
    String req_str = parser[1];
    if (req_str == "/"){
        send_file(client, "/index.html");
    }
    else if (req_str.startsWith("/upd/spd/")){
        spd = map(req_str.substring(9).toInt(), 0, 100, 0, MAX_SPEED);
        Serial.printf("[%d:%d] --- Speed: %d\n", (millis()/60000) % 60, (millis()/1000) % 60, spd);
        LMotor.setSpeed(spd);
        RMotor.setSpeed(spd);
    }
    else if (req_str.startsWith("/upd/dir/")){
        String dir = req_str.substring(9);
        Serial.printf("[%d:%d] --- Direction: %s\n", (millis()/60000) % 60, (millis()/1000) % 60, dir.c_str());
        if (dir == "forward")
            mode[0] = mode[1] = FORWARD;
        else if (dir == "backward")
            mode[0] = mode[1] = BACKWARD;
        else if (dir == "stop")
            mode[0] = mode[1] = STOP;
        else if (dir == "right"){
            mode[0] = FORWARD;
            mode[1] = BACKWARD;
        }
        else if (dir == "left"){
            mode[0] = BACKWARD;
            mode[1] = FORWARD;
        }
        update_motors();
    }
    else
        send_file(client, (char*)req_str.c_str());
}

void send_file(WiFiClient &client, char* path){
    File fl = SPIFFS.open(path, "r");
    client.print(HEADERS);
    if (fl && fl.isFile()){
        String str = fl.readStringUntil(EOF);
        client.print(str);
    }
    else if (fl && fl.isDirectory()){
        client.print(PAGE_FILE_NOT_FOUND);
    }
    else if (!fl){
        client.print("Ups... Error reading file");
    }
}

void update_motors(){
    RMotor.setMode(mode[0]);
    LMotor.setMode(mode[1]);
}
