#ifndef ModbusLib_h
#define ModbusLib_h

#ifndef MODBUS_LISTENER_SIZE
#define MODBUS_LISTENER_SIZE 20
#endif
#define MODBUS_BUFFER_SIZE 128

#include "./src/Util.h"
#include "./src/ModbusClient.h"
#include "./src/ModbusClient.cpp"
#include "./src/ModbusListener.h"
#include "./src/ModbusListener.cpp"

#endif