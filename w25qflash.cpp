#include "Arduino.h"
#include "w25qflash.h"
#include "SPI.h"



W25QFlash::W25QFlash(byte csPin) :
_csPin(csPin)
{
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setBitOrder(MSBFIRST);
	pinMode(csPin,OUTPUT);
}

void W25QFlash::waitFree()
{
	_select();
	while(_getStatus() & W25Q_MASK_BSY);
	_deselect();
}

void W25QFlash::setWriteEnable(bool state)
{
	_select();
	SPI.transfer(state ? W25Q_WEN : W25Q_WDI);
	_deselect();
	delayMicroseconds(1);
}

void W25QFlash::erase(unsigned long addr, byte command)
{
	waitFree();
	setWriteEnable();
	_select();
	SPI.transfer(W25Q_ERASE_SECTOR);
	SPI.transfer(0x00);SPI.transfer(0x00);SPI.transfer(0x00);
	_deselect();
	
}


void W25QFlash::read(unsigned long addr, byte buffer[], unsigned long n)
{
	int i;
	waitFree();
	_select();
	SPI.transfer(W25Q_READ);
	SPI.transfer(0x00);SPI.transfer(0x00);SPI.transfer(0x00);
	for(i=0;i<256;i++) Serial.println(SPI.transfer(0xFF));
	_deselect();
}

void W25QFlash::write(unsigned long addr, byte buffer[], unsigned long n)
{
	int i;
	waitFree();
	setWriteEnable();
	_select();
	
	SPI.transfer(W25Q_PROG_PAGE);
	SPI.transfer(0x00);SPI.transfer(0x00);SPI.transfer(0x00);
	for(i=0;i<256;i++)
	{
		SPI.transfer((byte)i);
		
	}
	_deselect();
}

byte W25QFlash::_getStatus()
{
	SPI.transfer(W25Q_SR_READ1);
	return SPI.transfer(0xFF);
}