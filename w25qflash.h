#ifndef w25q_h
#define w25q_h

// === W25Q INSTRUCTION SET ===

#define W25Q_WEN      0x06
#define W25Q_WDI      0x04

#define W25Q_READ     0x03

#define W25Q_PROG_PAGE 0x02

#define W25Q_ERASE_SECTOR  0x20  // 4 KByte
#define W25Q_ERASE_BLOCK1  0x52  // 32 KByte
#define W25Q_ERASE_BLOCK2  0xD8  // 64 KByte
#define W25Q_ERASE_CHIP    0xC7  // 1024 KByte

#define W25Q_SR_WEN    0x50
#define W25Q_SR_READ1  0x05
#define W25Q_SR_READ2  0x35
#define W25Q_SR_WRITE  0x01 

// === W25Q MASKS ===

#define W25Q_MASK_BSY (0x01 << 0)
#define W25Q_MASK_WEL (0x01 << 1)


#include "Arduino.h"

class W25QFlash {
	
public :
	
	W25QFlash(byte csPin);
	
	void waitFree();
	void setWriteEnable(bool state = true);
	
	void read(unsigned long addr, byte buffer[], unsigned long n);
	void program(unsigned long addr, byte buffer[], unsigned long n);
	void erase(unsigned long addr, byte command = W25Q_ERASE_SECTOR);
	
protected:
	
	void _select() {digitalWrite(_csPin,LOW);}
	void _deselect() {digitalWrite(_csPin,HIGH);}
	byte _getStatus();
	void _sendCmdAddr(byte cmd, unsigned long addr);
	
private :
	
	byte _csPin;
	
};

#endif