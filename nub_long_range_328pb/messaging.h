#ifndef MESSAGING_H
#define MESSAGING_H


uint8_t handleMessages(uint8_t UARTPort, struct networkStructure *networkPtr, uint16_t sizeOfNetwork, uint8_t *networkPtr2, struct myConData* myCons);

uint8_t* buildMsgPath( uint8_t destDSN[4], struct networkStructure *network, struct myConData* myCons);

void broadcastCon(uint8_t txPWR, uint8_t *networkPtr);
void PFD(uint8_t value, uint8_t port, uint8_t enabled);

uint8_t updateNetworks(struct networkStructure *networkPtr, uint16_t networkSize,uint8_t *networkPtr2,struct myConData* myCons, uint8_t newCon);
void sendNetworkData(uint8_t *networkPtr, uint16_t networkSize, struct myConData* myCons, uint8_t destDSN[]);
void writeDestDSN(uint8_t destDSN[]);
uint8_t* getDestPhoneAdd(uint8_t destPhoneAdd[6],uint8_t myDSN[], struct networkStructure* network);

void sendAck(uint8_t srcDSN[], uint8_t destDSN[]);

void sendMessageSimple(uint8_t msgPathSize, uint8_t deviceNumInPath, uint8_t msgPath[],uint8_t destPhoneAdd[6],uint8_t srcPhoneAdd[6], unsigned char msg[],uint16_t* msgSize);

int findDeviceNum(uint8_t DSN, struct networkStructure network);

#endif