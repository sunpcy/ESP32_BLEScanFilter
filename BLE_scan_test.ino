/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

//UUIDs of what server we want to filtering.
static BLEUUID serviceUUID_A("7cddf5af-453f-40fa-808e-37ae6ad8facd");
static BLEUUID serviceUUID_B("69e2c127-7db7-469c-8852-b0a542e15623");
static BLEUUID serviceUUID_C("407fc20d-d5f5-4176-bb4d-cc9c1968f9f6");
static BLEUUID serviceUUID_D("14fb4533-1997-4bbe-a567-554d2da89738");

//Set "Scan_x" for condition statement.
static BLEAdvertisedDevice* Scan_A ;
static BLEAdvertisedDevice* Scan_B ;
static BLEAdvertisedDevice* Scan_C ;
static BLEAdvertisedDevice* Scan_D ;

//Set "Found_x" for condition statement.
static bool Found_A = false;
static bool Found_B = false;
static bool Found_C = false;
static bool Found_D = false;


//Set scanTime to forever "scanTime = 0".
int scanTime = 0; //In seconds

//Function when found device and can use in void loop(); for do other things.
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      if (advertisedDevice.isAdvertisingService(serviceUUID_A)){
        
         if(Scan_A == NULL){
            Found_A = true;
            Scan_A = new BLEAdvertisedDevice(advertisedDevice);
          }
      }

      if (advertisedDevice.isAdvertisingService(serviceUUID_B)){
         if(Scan_B == NULL){
            Found_B = true;
            Scan_B = new BLEAdvertisedDevice(advertisedDevice);
          }
      }
      
      if (advertisedDevice.isAdvertisingService(serviceUUID_C)){
         if(Scan_C == NULL){
            Found_C = true;
            Scan_C = new BLEAdvertisedDevice(advertisedDevice);

          }
      }
      if (advertisedDevice.isAdvertisingService(serviceUUID_D)){
         if(Scan_D == NULL){
            Found_D = true;
            Scan_D = new BLEAdvertisedDevice(advertisedDevice);

          }
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
      int Rssi_A = Scan_A->getRSSI();
      Serial.printf("RSSI A: %d \t", Rssi_A);
      delete Scan_A;
      Scan_A = NULL;
      Found_A = false;
      delay(100);
    }

    if(Found_B){
      int Rssi_B = Scan_B->getRSSI();
      Serial.printf("RSSI B: %d \t", Rssi_B);
      delete Scan_B;
      Scan_B = NULL;
      Found_B = false;
      delay(100);
    }


    if(Found_C){
      int Rssi_C = Scan_C->getRSSI();
      Serial.printf("RSSI C: %d \t", Rssi_C);
      delete Scan_C;
      Scan_C = NULL;
      Found_C = false;
      delay(100);
    }

    if(Found_D){
      int Rssi_D = Scan_D->getRSSI();
      Serial.printf("RSSI D: %d \t", Rssi_D);
      delete Scan_D;
      Scan_D = NULL;
      Found_D = false;
      delay(100);
    }
    Serial.print("\n");
}
