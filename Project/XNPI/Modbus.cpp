#include "stdafx.h"
#include "Modbus.h"
#include <iostream>

Modbus* Modbus::m_instance = nullptr;

Modbus::Modbus()
	: m_unitId(0)
	, m_readLengh(1)
	, m_transactionID(0)
	, m_nModbusDataCmdSize(sizeof(MODBUS_DATA_CMD))
{
}

Modbus::~Modbus()
{
}

Modbus* Modbus::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Modbus();
	}

	return m_instance;
}

MODBUS_DATA_CMD Modbus::CreateReadDataCmd(byte unitId, byte FunctionCode, WORD Address, WORD readLength)
{
	MODBUS_DATA_CMD cmdData;

	cmdData.TransactionID = m_transactionID;
	cmdData.StartAddress = Address;
	cmdData.UnitID = unitId;
	cmdData.FunctionCode = FunctionCode;
	cmdData.Data = readLength;

	cmdData.TransactionID = ntohs(cmdData.TransactionID);
	cmdData.ProtocolID = ntohs(cmdData.ProtocolID);
	cmdData.Length = ntohs(cmdData.Length);
	cmdData.StartAddress = ntohs(cmdData.StartAddress);
	cmdData.Data = ntohs(cmdData.Data);

	return cmdData;
}

MODBUS_DATA_CMD Modbus::CreateWriteDataCmd(byte unitId, byte FunctionCode, WORD Address, WORD WriteData)
{
	MODBUS_DATA_CMD cmdData;

	cmdData.TransactionID = m_transactionID;
	cmdData.StartAddress = Address;
	cmdData.UnitID = unitId;
	cmdData.FunctionCode = FunctionCode;
	cmdData.Data = WriteData;

	cmdData.TransactionID = ntohs(cmdData.TransactionID);
	cmdData.ProtocolID = ntohs(cmdData.ProtocolID);
	cmdData.Length = ntohs(cmdData.Length);
	cmdData.StartAddress = ntohs(cmdData.StartAddress);

	if (FunctionCode == FCODE_WRITE_COIL)
		cmdData.Data = (WriteData == TRUE) ? COIL_ON : COIL_OFF;

	cmdData.Data = ntohs(cmdData.Data);

	return cmdData;
}

BOOL Modbus::Create()
{
	return m_socket.Create();
}

BOOL Modbus::Connect(CString ip, int port)
{
	return m_socket.Connect(ip, port);
}

void Modbus::Close()
{
	m_socket.Close();
}

BOOL Modbus::GetDataReceived()
{
	return m_socket.m_bReceived;
}

void Modbus::SetDataReceived(BOOL received)
{
	m_socket.m_bReceived = received;
}

void Modbus::SendData(const void* lpBuf, int nbufLen)
{
	m_transactionID++;
	m_socket.Send(lpBuf, nbufLen);
}

/*!
	@fn  Modbus::SendReadDataCmd(WORD address)
	@brief PLC Input Register의 데이터 읽기를 요청한다.
*/
int Modbus::SendReadDataCmd(WORD address)
{
	int trID = m_transactionID;
	MODBUS_DATA_CMD cmdData = CreateReadDataCmd(m_unitId, FCODE_INPUT_REG, address, m_readLengh);
	SendData((char*)&cmdData, m_nModbusDataCmdSize);
	//std::wcout << "Read Cmd : " << address << std::endl;
	m_socket.m_nTestCnt++;

	return trID;
}

/*!
	@fn Modbus::SendWriteDataCmd(WORD address, WORD writeData)
	@brief PLC Holding Register의 데이터 쓰기를 요청한다.
*/
int Modbus::SendWriteDataCmd(WORD address, WORD writeData)
{
	int trID = m_transactionID;
	MODBUS_DATA_CMD cmdData = CreateWriteDataCmd(m_unitId, FCODE_HOLDING_REG, address, writeData);
	SendData((char*)&cmdData, m_nModbusDataCmdSize);
	//std::wcout << "Write Cmd : " << address << "," << writeData << std::endl;
	m_socket.m_nTestCnt++;

	return trID;
}

void Modbus::SetOnCloseCallback(tfSocketClosed funcOnClose)
{
	m_socket.SetOnCloseCallback(funcOnClose);
}

void Modbus::SetOnReceiveCallback(tfOnReceive funcOnReceive)
{
	m_socket.SetOnReceiveCallback(funcOnReceive);
}
