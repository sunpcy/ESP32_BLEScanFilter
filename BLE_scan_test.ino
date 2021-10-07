/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

static BLEUUID serviceUUID_A("7cddf5af-453f-40fa-808e-37ae6ad8facd");
static BLEUUID serviceUUID_B("69e2c127-7db7-469c-8852-b0a542e15623");
static BLEUUID serviceUUID_C("407fc20d-d5f5-4176-bb4d-cc9c1968f9f6");

static BLEAdvertisedDevice* Scan_A;
static BLEAdvertisedDevice* Scan_B;
static BLEAdvertisedDevice* Scan_C;

static bool Found_A = false;
static bool Found_B = false;
static bool Found_C = false;


int scanTime = 0; //In seconds

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {

      if (advertisedDevice.isAdvertisingService(serviceUUID_A)){
        Scan_A = new BLEAdvertisedDevice(advertisedDevice);
        Found_A = true;
      }
      else{
        Found_A = false;
      }

      if (advertisedDevice.isAdvertisingService(serviceUUID_B)){
        Scan_B = new BLEAdvertisedDevice(advertisedDevice);
        Found_B = true;
      }
      else{
        Found_B = false;
      }

      if (advertisedDevice.isAdvertisingService(serviceUUID_C)){
        Scan_C = new BLEAdvertisedDevice(advertisedDevice);
        Found_C = true;
      }
      else{
        Found_C = false;
      }
    }
};


void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());  
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->start(scanTime);
}

void loop() {
    if(Found_A){
      int rssi_a = Scan_A->getRSSI();
      Serial.printf("RSSI A: %d \n\n", rssi_a);
      delete Scan_A;
      Found_A = false;
      delay(500);
    }
  

    if(Found_B){
      int rssi_b = Scan_B->getRSSI();
      Serial.printf("RSSI B: %d \n\n", rssi_b);
      delete Scan_B;
      Found_B = false;
      delay(500);
    }
    if(Found_C){
      int rssi_c = Scan_C->getRSSI();
      Serial.printf("RSSI C: %d \n\n", rssi_c);
      delete Scan_C;
      Found_C = false;
      delay(500);
    }
  }
