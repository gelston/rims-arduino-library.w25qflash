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
	_sendCmdAddr(command,addr);
	_deselect();
	
}


void W25QFlash::read(unsigned long addr, byte buffer[], unsigned long n)
{
	unsigned long i;
	waitFree();
	_select();
	_sendCmdAddr(W25Q_READ,addr);
	for(i=0;i<n;i++) buffer[i] = SPI.transfer(0xFF);
	_deselect();
}

void W25QFlash::program(unsigned long addr, byte buffer[], unsigned long n)
{
	unsigned long i;
	waitFree();
	setWriteEnable();
	_select();
	_sendCmdAddr(W25Q_PROG_PAGE,addr);
	for(i=0;i<n;i++)
	{
		SPI.transfer(buffer[i]);
		if(not((i+1) & 0xFF))
		{
		_deselect();
		waitFree();
		setWriteEnable();
		_select();
		_sendCmdAddr(W25Q_PROG_PAGE,addr+i+1);
		}
	}
	_deselect();
}

byte W25QFlash::_getStatus()
{
	SPI.transfer(W25Q_SR_READ1);
	return SPI.transfer(0xFF);
}

void W25QFlash::_sendCmdAddr(byte cmd, unsigned long addr)
{
	SPI.transfer(cmd);
	SPI.transfer((addr>>16) & 0xFF);
	SPI.transfer((addr>> 8) & 0xFF);
	SPI.transfer((addr)     & 0xFF);
}