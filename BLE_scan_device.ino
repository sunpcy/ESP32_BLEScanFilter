/* 
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32
   snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp 
   Ported to Arduino ESP32 by Evandro Copercini 
*/ 

// Include necessary libraries
#include <BLEDevice.h> 
#include <BLEUtils.h> 
#include <BLEScan.h> 
#include <BLEAdvertisedDevice.h> 
#include <movingAvg.h> 
#include <MatrixMath.h> 
#include <Wire.h>  
#include <LiquidCrystal_I2C.h> 

// Initialize LCD object with I2C address and dimensions
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Define moving average objects for each BLE RSSI value
movingAvg RSSI_RAW_A(10); 
movingAvg RSSI_RAW_B(10); 
movingAvg RSSI_RAW_C(10); 
movingAvg RSSI_RAW_D(10); 

// Reference RSSI value and path loss exponent
float RSSI_ref = -58.0; 
float pl_e = 3.48802457307803; 

// Static UUIDs for BLE services
static BLEUUID serviceUUID_A("7cddf5af-453f-40fa-808e-37ae6ad8facd"); 
static BLEUUID serviceUUID_B("69e2c127-7db7-469c-8852-b0a542e15623"); 
static BLEUUID serviceUUID_C("407fc20d-d5f5-4176-bb4d-cc9c1968f9f6"); 
static BLEUUID serviceUUID_D("14fb4533-1997-4bbe-a567-554d2da89738");

// Pointers for BLE devices and flags for device presence
static BLEAdvertisedDevice* Scan_A ; 
static BLEAdvertisedDevice* Scan_B ; 
static BLEAdvertisedDevice* Scan_C ; 
static BLEAdvertisedDevice* Scan_D ; 
static bool Found_A = false; 
static bool Found_B = false; 
static bool Found_C = false; 
static bool Found_D = false; 

// Coordinates and distances for localization
float x_1 = 0.0; 
float y_1 = 0.0; 
float x_2 = 2.0; 
float y_2 = 0.0; 
float x_3 = 2.0; 
float y_3 = 2.0; 
float x_4 = 0.0; 
float y_4 = 2.0; 
float r_1 = 0.0; 
float r_2 = 0.0; 
float r_3 = 0.0; 
float r_4 = 0.0; 

// Matrix variables for least squares method
mtx_type A[3][2]; 
mtx_type B[3][1]; 
mtx_type A_T[2][3]; 
mtx_type A_T_A[2][2]; 
mtx_type A_T_B[2][1]; 
mtx_type X[2][1]; 
mtx_type A_1[2][2]; 

int scanTime = 0; // Scan duration in seconds

// Callback class for BLE advertisement events
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks { 
    void onResult(BLEAdvertisedDevice advertisedDevice) { 
        // Check if advertised device matches service UUID A
        if (advertisedDevice.isAdvertisingService(serviceUUID_A)){ 
            if(Scan_A == NULL){ 
                Found_A = true; 
                Scan_A = new BLEAdvertisedDevice(advertisedDevice); 
            } 
        } 
        
        // Check if advertised device matches service UUID B
        if (advertisedDevice.isAdvertisingService(serviceUUID_B)){ 
            if(Scan_B == NULL){ 
                Found_B = true; 
                Scan_B = new BLEAdvertisedDevice(advertisedDevice); 
            } 
        } 

        // Check if advertised device matches service UUID C
        if (advertisedDevice.isAdvertisingService(serviceUUID_C)){ 
            if(Scan_C == NULL){ 
                Found_C = true; 
                Scan_C = new BLEAdvertisedDevice(advertisedDevice); 
            } 
        } 

        // Check if advertised device matches service UUID D
        if (advertisedDevice.isAdvertisingService(serviceUUID_D)){ 
            if(Scan_D == NULL){ 
                Found_D = true; 
                Scan_D = new BLEAdvertisedDevice(advertisedDevice); 
            } 
        } 
    } 
}; 

// Setup function runs once at Arduino startup
void setup() { 
    // Start serial communication
    Serial.begin(115200); 
    
    // Initialize moving average objects for each RSSI value
    RSSI_RAW_A.begin(); 
    RSSI_RAW_B.begin(); 
    RSSI_RAW_C.begin(); 
    RSSI_RAW_D.begin(); 

    // Initialize LCD display
    lcd.begin(); 
    lcd.backlight(); 

    // Initialize BLE device
    BLEDevice::init(""); 
    BLEScan* pBLEScan = BLEDevice::getScan(); // Create new BLE scan object
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());   
    pBLEScan->setActiveScan(true); // Activate scan mode (uses more power but faster results)
    pBLEScan->start(scanTime); // Start BLE scan with specified duration
}

// Loop function continuously executes after setup
void loop() { 
    // Process when device A is found
    if(Found_A){ 
        float Rssi_A = Scan_A->getRSSI(); 
        float MA10_RSSI_A = RSSI_RAW_A.reading(Rssi_A); 
        //Serial.printf("RSSI A: %d \t", MA10_RSSI_A); 
        float r_1_pow = (RSSI_ref - MA10_RSSI_A); 
        float r_1 = 0; 
        //Serial.println(r_1); 
        delete Scan_A; 
        Scan_A = NULL; 
        Found_A = false; 
        delay(200); 
    } 

    // Process when device B is found
    if(Found_B){ 
        float Rssi_B = Scan_B->getRSSI(); 
        float MA10_RSSI_B = RSSI_RAW_B.reading(Rssi_B); 
        //Serial.printf("RSSI B: %d \t", MA10_RSSI_B); 
        float r_2 = pow(10,((RSSI_ref - MA10_RSSI_B)/(10 * pl_e))); 
        //Serial.println(r_2);       
        delete Scan_B; 
        Scan_B = NULL; 
        Found_B = false; 
        delay(200); 
    } 

    // Process when device C is found
    if(Found_C){ 
        float Rssi_C = Scan_C->getRSSI(); 
        float MA10_RSSI_C = RSSI_RAW_C.reading(Rssi_C); 
        //Serial.printf("RSSI C: %d \t", MA10_RSSI_C); 
        float r_3 = pow(10,((RSSI_ref - MA10_RSSI_C)/(10 * pl_e))); 
        //Serial.println(r_3); 
        delete Scan_C; 
        Scan_C = NULL; 
        Found_C = false; 
        delay(200); 
    } 

    // Process when device D is found
    if(Found_D){ 
        float Rssi_D = Scan_D->getRSSI(); 
        float MA10_RSSI_D = RSSI_RAW_D.reading(Rssi_D); 
        //Serial.printf("RSSI D: %d \t", MA10_RSSI_D); 
        float r_4 = pow(10,((RSSI_ref - MA10_RSSI_D)/(10 * pl_e))); 
        //Serial.println(r_4); 
        delete Scan_D; 
        Scan_D = NULL; 
        Found_D = false; 
        delay(200); 
    } 

    // Process when all devices A, B, C, and D are found
    if(Found_A && Found_B && Found_C && Found_D){ 
        // Collect RSSI values
        float Rssi_A = Scan_A->getRSSI(); 
        float Rssi_B = Scan_B->getRSSI(); 
        float Rssi_C = Scan_C->getRSSI(); 
        float Rssi_D = Scan_D->getRSSI(); 
        
        // Filter RSSI values with moving average
        float MA10_RSSI_A = RSSI_RAW_A.reading(Rssi_A); 
        float MA10_RSSI_B = RSSI_RAW_B.reading(Rssi_B); 
        float MA10_RSSI_C = RSSI_RAW_C.reading(Rssi_C); 
        float MA10_RSSI_D = RSSI_RAW_D.reading(Rssi_D); 

        // Convert RSSI to estimated distances
        float r_1 = pow(10,((RSSI_ref - MA10_RSSI_A)/(10 * pl_e))); 
        float r_2 = pow(10,((RSSI_ref - MA10_RSSI_B)/(10 * pl_e))); 
        float r_3 = pow(10,((RSSI_ref - MA10_RSSI_C)/(10 * pl_e))); 
        float r_4 = pow(10,((RSSI_ref - MA10_RSSI_D)/(10 * pl_e))); 

        // Use least squares method for position estimation
        float d_21 = sqrt(pow(x_2-x_1,2)+pow(y_2-y_1,2)); 
        float d_31 = sqrt(pow(x_3-x_1,2)+pow(y_3-y_1,2)); 
        float d_41 = sqrt(pow(x_4-x_1,2)+pow(y_4-y_1,2)); 
        float b_21 = 0.5*(pow(r_1,2)+pow(d_21,2)-pow(r_2,2)); 
        float b_31 = 0.5*(pow(r_1,2)+pow(d_31,2)-pow(r_3,2)); 
        float b_41 = 0.5*(pow(r_1,2)+pow(d_41,2)-pow(r_4,2)); 

        mtx_type B[3][1] = {b_21,b_31,b_41}; 
        mtx_type A[3][2] = {{x_2,y_2},{x_3,y_3},{x_4,y_4}};  
        
        // Perform matrix operations for least squares solution
        Matrix.Transpose((mtx_type*)A,3,2,(mtx_type*)A_T);  // Transpose of A (2x3 matrix)
        Matrix.Multiply((mtx_type*)A_T,(mtx_type*)B,2,3,1,(mtx_type*)A_T_B); // A_T * B
        Matrix.Multiply((mtx_type*)A_T,(mtx_type*)A,2,3,2,(mtx_type*)A_T_A); // A_T * A
        Matrix.Invert((mtx_type*)A_T_A,2); // Invert A_T_A
        Matrix.Multiply((mtx_type*)A_T_A,(mtx_type*)A_T_B,2,2,1,(mtx_type*)X); // X = (A_T_A)^(-1) * A_T_B
    }
}
