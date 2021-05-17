#define INFO

// MODBUS
#include <ModbusMaster.h>
void preTransmission();
void postTransmission();

ModbusMaster node;
#define MAX485_DE 14
#define MAX485_RE_NEG 27
#define SLAVE_ID 1
#define RXD2 16
#define TXD2 17

// MODBUS: PRE AND POS TRANSMISION FUNCTIONS

String readMODBUS(uint16_t id, uint16_t add, uint16_t qty)
{
    pinMode(MAX485_RE_NEG, OUTPUT);
    pinMode(MAX485_DE, OUTPUT);
    // Init in receive mode
    digitalWrite(MAX485_RE_NEG, 0);
    digitalWrite(MAX485_DE, 0);

    // Modbus slave ID 1
    node.begin(id, Serial2);
    // Callbacks allow us to configure the RS485 transceiver correctly
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);

    uint8_t j, result;
    uint16_t data[qty];

    // set word 0 of TX buffer to least-significant word of counter (bits 15..0)
    //  node.setTransmitBuffer(0, lowWord(i));
    // set word 1 of TX buffer to most-significant word of counter (bits 31..16)
    //   node.setTransmitBuffer(1, highWord(i));
    // slave: write TX buffer to (2) 16-bit registers starting at register 0
    // result = node.writeMultipleRegisters(0, 2);

    // slave: read (qty) 16-bit registers starting at register add to RX buffer
    result = node.readHoldingRegisters(add, qty);

    // do something with data if read is successful
    if (result == node.ku8MBSuccess)
    {
        String msg;
        for (j = 0; j < qty; j++)
        {
            data[j] = node.getResponseBuffer(j);

            msg += String(data[j]);
            if (j != (qty - 1))
                msg += ",";
        }

        Serial.println(msg);
        return msg;
    }
    else
    {
        Serial.print("Error: ");
        Serial.println(result);
        return "ERROR";
    }
}

void setupMODBUS(int16_t id)
{
    pinMode(MAX485_RE_NEG, OUTPUT);
    pinMode(MAX485_DE, OUTPUT);
    // Init in receive mode
    digitalWrite(MAX485_RE_NEG, 0);
    digitalWrite(MAX485_DE, 0);

    Serial1.begin(9600, SERIAL_8N1, 0, 4); // not working on default pins
    Serial2.begin(9600, SERIAL_8N1);

    // Modbus slave ID 1
    node.begin(id, Serial2);
    // Callbacks allow us to configure the RS485 transceiver correctly
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);
}

void preTransmission()
{
    delay(10);
    digitalWrite(MAX485_RE_NEG, 1);
    digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
    delay(10);
    digitalWrite(MAX485_RE_NEG, 0);
    digitalWrite(MAX485_DE, 0);
}

// ADC i2c

#include <SPI.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;

void setupADS1115()
{
    Serial.println("Getting single-ended readings from AIN0..3");
    Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");

    // The ADC input range (or gain) can be changed via the following
    // functions, but be careful never to exceed VDD +0.3V max, or to
    // exceed the upper and lower limits if you adjust the input range!
    // Setting these values incorrectly may destroy your ADC!
    //                                                                ADS1015  ADS1115
    //                                                                -------  -------
    // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
    // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
    // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
    // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
    // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
    // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

    ads.begin();
}

void testADS1115()
{
    int16_t adc0, adc1, adc2, adc3;
    float volts0, volts1, volts2, volts3;

    adc0 = ads.readADC_SingleEnded(0);
    adc1 = ads.readADC_SingleEnded(1);
    adc2 = ads.readADC_SingleEnded(2);
    adc3 = ads.readADC_SingleEnded(3);

    volts0 = ads.computeVolts(adc0);
    volts1 = ads.computeVolts(adc1);
    volts2 = ads.computeVolts(adc2);
    volts3 = ads.computeVolts(adc3);

    Serial.println("-----------------------------------------------------------");
    Serial.print("AIN0: ");
    Serial.print(adc0);
    Serial.print("  ");
    Serial.print(volts0);
    Serial.println("V");
    Serial.print("AIN1: ");
    Serial.print(adc1);
    Serial.print("  ");
    Serial.print(volts1);
    Serial.println("V");
    Serial.print("AIN2: ");
    Serial.print(adc2);
    Serial.print("  ");
    Serial.print(volts2);
    Serial.println("V");
    Serial.print("AIN3: ");
    Serial.print(adc3);
    Serial.print("  ");
    Serial.print(volts3);
    Serial.println("V");

    delay(1000);
}

float readASD1115single(int op)
{
    if (op >= 0 && op <= 3)
    {
        int16_t adc = ads.readADC_SingleEnded(op);
        float volts = ads.computeVolts(adc);
        return volts;
    }
    else
    {
        return 0;
    }
}

// SD CARD
#include "FS.h"
#include "SD.h"
#include <SPI.h>
#define SD_CS 5
void setupHW125();
void logSDCard(String dataMessage);
void appendFile(fs::FS &fs, const char *path, const char *message);
void writeFile(fs::FS &fs, const char *path, const char *message);
File myFile;
int counter;

void setupHW125()
{
    //pinMode(23, INPUT_PULLUP);
    // Initialize SD card
    SD.begin(SD_CS);
    if (!SD.begin(SD_CS))
    {
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }
    Serial.println("Initializing SD card...");
    if (!SD.begin(SD_CS))
    {
        Serial.println("ERROR - SD card initialization failed!");
        return; // init failed
    }

    // If the data.txt file doesn't exist
    // Create a file on the SD card and write the data labels
    File file = SD.open("/data.txt");
    if (!file)
    {
        Serial.println("File doens't exist");
        Serial.println("Creating file...");
        writeFile(SD, "/data.txt", "Current Date, ADC1 \r\n");
    }
    else
    {
        Serial.println("File already exists");
    }
    file.close();
}

// Write the sensor readings on the SD card
void logSDCard(String dataMessage)
{
#ifdef INFO
    Serial.print("Save data: ");
    Serial.println(dataMessage);
#endif
    appendFile(SD, "/mayo.txt", dataMessage.c_str());
}

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("File written");
    }
    else
    {
        Serial.println("Write failed");
    }
    file.close();
}

// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.println("Message appended");
    }
    else
    {
        Serial.println("Append failed");
    }
    file.close();
}

String listDirectorySDCard()
{
    using namespace fs;
    File file = SD.open((const char *)"/", FILE_READ);

    if (!file)
    {
        Serial.println("No file");
        return "ERROR";
    }
    else
    {
        String entryName = "";
        while (true)
        {
            File entry = file.openNextFile();

            if (!entry)
            {
                // no more files
                break;
            }
            else
            {
                entryName += entry.name();
                entryName += ",";
            }
        }
        entryName.remove(entryName.length() - 1);
        Serial.println(entryName);
        file.close();

        return entryName;
    }
}