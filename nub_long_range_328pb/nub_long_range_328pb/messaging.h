#ifndef MESSAGING_H
#define MESSAGING_H


uint8_t handleMessages(uint8_t *myDSN);

void buildMsgPath(uint8_t destPhoneAdd[6], uint8_t * myDSN, struct networkStructure network);
void broadcastCon(uint8_t txPWR,uint8_t srcDSN[], uint8_t *networkPtr, uint16_t networkPtrSize);

void writeDestDSN(uint8_t destDSN[]);

void sendAck(uint8_t srcDSN[], uint8_t destDSN[]);

void sendMessageSimple(uint8_t srcDSN[], uint8_t destDSN[], unsigned char msg[],uint8_t msgSize);

#endif