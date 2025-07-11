#include <bluefruit.h>
#include <Wire.h>



uint8_t mode = 0;

String msg;
bool msgflag = 0;
// BLE UART service
BLEUart bleuart;
BLEDis bledis;
BLEDfu bledfu; // DFU Service instance

// === BLE connection callbacks ===
void onConnect(uint16_t connHandle) {
  Serial.println("BLE connected");
}

void onDisconnect(uint16_t connHandle, uint8_t reason) {
  Serial.println("BLE disconnected");
  if (mode == 1){

     Serial.println("Waiting for BLE...");
  }
  // Restart advertising
  Bluefruit.Advertising.start(0);
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
  delay(2000);
  Serial.println("Boot");
  
  // Init BLE
  Bluefruit.begin();
  Bluefruit.setName("VisRing");
  bledis.setModel("VisRing");
  // Set BLE callbacks
  Bluefruit.Periph.setConnectCallback(onConnect);
  Bluefruit.Periph.setDisconnectCallback(onDisconnect);

  // Init BLE UART
  bleuart.begin();
  bledfu.begin();
  bledis.begin();

  // Advertise
  Bluefruit.Advertising.addService(bleuart);
  Bluefruit.Advertising.addService(bledfu);  // DFU advertised
  Bluefruit.ScanResponse.addName();
  Bluefruit.Advertising.start(0);  // 0 = advertise forever
  Serial.println("BLE Started!");
}

void loop() {
  if (bleuart.available()){
      msg = bleuart.readString();
      msg.trim();  // Remove \n or \r if present
      msgflag = 1;
      Serial.print("Received: ");
      Serial.println(msg);
  }
  if (mode == 1){
    Serial.println("mode 1");
  }

}



