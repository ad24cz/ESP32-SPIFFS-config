#include <LoopSpeedLogger.h>
#include <stdlib.h>
#include "FS.h"
#include "SPIFFS.h"

#define FORMAT_SPIFFS_IF_FAILED true

String readFile(const char *path) {
    String buffer = "";

    File file = SPIFFS.open(path);

    if (!file || file.isDirectory())
        return "";

    while (file.available())
        buffer += (char)file.read();

    return buffer;
}

bool writeFile(const char *path, const char *message) {
    File file = SPIFFS.open(path, FILE_WRITE);

    return file.print(message);
}

bool appendFile(const char *path, const char *message) {
    File file = SPIFFS.open(path, FILE_APPEND);

    return file.print(message);
}

void setup() {
    Serial.begin(115200);
    // mount SPIFFS
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    delay(2000);

    // write to a file, the function returns false is failed
    writeFile("/myfile", "foo");

    // read from the file, returns empty string if failed
    Serial.println(readFile("/myfile"));

    // append to the file, returns false is failed
    appendFile("/myfile", "\nbar");

    // read from the file, returns empty string if failed
    Serial.println(readFile("/myfile"));

    // check if the file exists
    Serial.println(SPIFFS.exists("/myfile") ? "the file exists" : "file does not exist");

    // delete the file, returns false is failed
    SPIFFS.remove("/myfile");

    //check if the file exists
    Serial.println(SPIFFS.exists("/myfile") ? "the file exists" : "file does not exist");

    // it can also be done with pure bytes, but the array has to end with 0 because SPIFFS handles everything as a string
    const char a[] = {65, 66, 67, 0};
    writeFile("/byte", a);
    Serial.println(readFile("/byte"));
}

void loop() {
}