#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>

// WiFi
#define ERROR_STR "Some error in funtction \"send_request(const char*)\"...."
#define WIFI_SSID "ArseniiNet"
#define WIFI_PASS "mio9led1"
#define HEADERS "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n"
#define PAGE_FILE_NOT_FOUND "<!DOCTYPE html>\n\r<html lang=\"en\">\n\r\t<head>\n\r\t\t<meta charset=\"UTF-8\">\n\r\t\t<title>404 ERROR</title>\n\r\t</head>\n\r\t<body>\n\r\t\t<center>\n\r\t\t\t<h1>404 Page not found Error</h1>\n\r\t\t\t<h2>Go to <a href=\"/\">main page</a></h2>\n\r\t\t</center>\n\r\t</body>\n\r</html>"
#define PORT 80

namespace WiFiTools{
    void connectWifi();
    String send_request(const char*);
}