#pragma once
#include "ConnectSocket.h"
#include <iostream>

#define PORT_NUM 502

//FunctionCode
#define FCODE_READ_COIL		0x01
#define FCODE_WRITE_COIL	0x05
#define FCODE_INPUT_REG		0x04	//input Register
#define FCODE_HOLDING_REG	0x06	//holding Register

#define COIL_ON		0xff00
#define COIL_OFF	0x0000

#pragma pack(push, 1)
/*
struct ModbusReadDataCmd
{
WORD TransactionID = 0x00;
WORD ProtocolID = 0x00;
WORD Length = 0x06;
byte UnitID;
byte FunctionCode;
WORD StartAddress;
WORD ReadLength;
}typedef MODBUS_READ_DATA_CMD;

struct ModbusWriteDataCmd
{
WORD TransactionID = 0x00;
WORD ProtocolID = 0x00;
WORD Length = 0x06;
byte UnitID;
byte FunctionCode;
WORD StartAddress;
WORD WriteData;
}typedef MODBUS_WRITE_DATA_CMD;
*/

struct ModbusDataCmd
{
	WORD TransactionID = 0x00;
	WORD ProtocolID = 0x00;
	WORD Length = 0x06;
	byte UnitID;
	byte FunctionCode;
	WORD StartAddress;
	WORD Data;
}typedef MODBUS_DATA_CMD;

#pragma pack(pop)

class Modbus
{
private:
	Modbus();
	void SendData(const void* lpBuf, int nbufLen);
	CConnectSocket m_socket;
	static Modbus* m_instance;
	WORD m_transactionID;
	int m_nModbusDataCmdSize;

public:
	~Modbus();

	byte m_unitId;
	WORD m_readLengh;

	static Modbus* GetInstance();
	MODBUS_DATA_CMD CreateReadDataCmd(byte unitId, byte FunctionCode, WORD Address, WORD readLength = 0x01);
	MODBUS_DATA_CMD CreateWriteDataCmd(byte unitId, byte FunctionCode, WORD Address, WORD WriteData);

	//socket 관련 메서드들
	BOOL Create();
	BOOL Connect(CString ip, int port = 502);
	void Close();

	BOOL GetDataReceived();
	void SetDataReceived(BOOL received);

	int SendReadDataCmd(WORD address);
	int SendWriteDataCmd(WORD address, WORD writeData);

	void SetOnCloseCallback(tfSocketClosed funcOnClose);
	void SetOnReceiveCallback(tfOnReceive funcOnReceive);
};

