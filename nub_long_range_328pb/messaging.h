#ifndef MESSAGING_H
#define MESSAGING_H


uint8_t handleMessages(uint8_t UARTPort, struct networkStructure *networkPtr, uint16_t sizeOfNetwork, uint8_t *networkPtr2, struct myConData* myCons);

void buildMsgPath(uint8_t destPhoneAdd[6], uint8_t * myDSN, struct networkStructure network);

void broadcastCon(uint8_t txPWR, uint8_t *networkPtr);


void updateNetworks(struct networkStructure *networkPtr, uint16_t networkSize,uint8_t *networkPtr2,struct myConData* myCons, uint8_t newCon);
void sendNetworkData(uint8_t *networkPtr, uint16_t networkSize, struct myConData* myCons, uint8_t destDSN[]);
void writeDestDSN(uint8_t destDSN[]);
uint8_t* getDestPhoneAdd(uint8_t destPhoneAdd[6],uint8_t myDSN[], struct networkStructure* network);

void sendAck(uint8_t srcDSN[], uint8_t destDSN[]);

void sendMessageSimple(uint8_t srcDSN[], uint8_t destDSN[], unsigned char msg[],uint8_t msgSize);

int findDeviceNum(uint8_t DSN, struct networkStructure network);

#endif