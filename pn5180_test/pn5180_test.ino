#include <PN5180.h>
#include <PN5180ISO15693.h>
//#include <PN5180ISO14443.h>

#define PN5180_NSS  10
#define PN5180_BUSY 9
#define PN5180_RST  7

PN5180ISO15693 nfc(PN5180_NSS, PN5180_BUSY, PN5180_RST);

bool errorFlag = false;
uint32_t loopCnt = 0;

void setup() {
  Serial.begin(115200);
  nfc.begin();
  nfc.reset();


}
void loop() {
  if (errorFlag) {
    uint32_t irqStatus = nfc.getIRQStatus();
    showIRQStatus(irqStatus);

    if (0 == (RX_SOF_DET_IRQ_STAT & irqStatus)) { // no card detected
      //Serial.println(F("*** No card detected!"));
    }
    nfc.reset();
    nfc.setupRF();
    errorFlag = false;
  }
  //Serial.print(F("Loop #"));
  //Serial.println(loopCnt++);
  delay(10);
  uint8_t uid[8];
  //Serial.print(F("Call GetInv"));
  ISO15693ErrorCode rc = nfc.getInventory(uid);
  //ISO14443ErrorCode rc = nfc.getInventory(uid);

  //Serial.print(F("Ret GetInv"));

  if (ISO15693_EC_OK != rc) {
    //Serial.print(F("Error in getInventory: "));
    //Serial.println(nfc.strerror(rc));
    errorFlag = true;
    return;
  }
  Serial.print(F("Inventory successful, UID="));
  for (int i=0; i<8; i++) {
    Serial.print(uid[7-i], HEX); // LSB is first
    if (i < 2) Serial.print(":");
  }
  Serial.println();
}

void showIRQStatus(uint32_t irqStatus) {
  Serial.print(F("IRQ-Status 0x"));
  Serial.print(irqStatus, HEX);
  Serial.print(": [ ");
  if (irqStatus & (1<< 0)) Serial.print(F("RQ "));
  if (irqStatus & (1<< 1)) Serial.print(F("TX "));
  if (irqStatus & (1<< 2)) Serial.print(F("IDLE "));
  if (irqStatus & (1<< 3)) Serial.print(F("MODE_DETECTED "));
  if (irqStatus & (1<< 4)) Serial.print(F("CARD_ACTIVATED "));
  if (irqStatus & (1<< 5)) Serial.print(F("STATE_CHANGE "));
  if (irqStatus & (1<< 6)) Serial.print(F("RFOFF_DET "));
  if (irqStatus & (1<< 7)) Serial.print(F("RFON_DET "));
  if (irqStatus & (1<< 8)) Serial.print(F("TX_RFOFF "));
  if (irqStatus & (1<< 9)) Serial.print(F("TX_RFON "));
  if (irqStatus & (1<<10)) Serial.print(F("RF_ACTIVE_ERROR "));
  if (irqStatus & (1<<11)) Serial.print(F("TIMER0 "));
  if (irqStatus & (1<<12)) Serial.print(F("TIMER1 "));
  if (irqStatus & (1<<13)) Serial.print(F("TIMER2 "));
  if (irqStatus & (1<<14)) Serial.print(F("RX_SOF_DET "));
  if (irqStatus & (1<<15)) Serial.print(F("RX_SC_DET "));
  if (irqStatus & (1<<16)) Serial.print(F("TEMPSENS_ERROR "));
  if (irqStatus & (1<<17)) Serial.print(F("GENERAL_ERROR "));
  if (irqStatus & (1<<18)) Serial.print(F("HV_ERROR "));
  if (irqStatus & (1<<19)) Serial.print(F("LPCD "));
  Serial.println("]");
}
