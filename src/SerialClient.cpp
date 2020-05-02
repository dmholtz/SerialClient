/*
	SerialClient.cpp - Library for fast serial communication between Arduino and
    PC. Implements a master - client protocol, which speeds up the transfer of
    numeric data or G-Codes

	(C) 2020 by David Holtz <https://github.com/dmholtz>
*/

#include "Arduino.h"
#include "SerialClient.h"

/**
 * 
 **/
SerialClient::SerialClient()
{
    Serial.begin(115200);
}

/**Awaits four incoming bytes to define the protocol.
 * 1. byte: must be SETUP_INIT_BYTE
 * 2. byte: integer representation: 1, 2 or 4
 * 3. byte: message length: positive integer > 0
 * 4. byte: batch size: positive integer > 0
 * 
 * @returns false if any of these rules is infringed
 * @returns true if all conditions are fulfilled
 * 
 **/
bool SerialClient::protocolSetup()
{
    // Await the setup initialization byte
    int intRepresentation = 0;
    int batchSize = 0;
    int messageLength = 0;
    while (Serial.available() < 1)
    {
    }
    if (Serial.read() != SETUP_INIT_BYTE)
    {
        return false;
    }
    // Await Integer representation byte
    while (Serial.available() < 1)
    {
    }
    intRepresentation = Serial.read();
    if (intRepresentation != 2 && intRepresentation != 1 && intRepresentation != 4)
    {
        return false;
    }
    // Await message length byte
    while (Serial.available() < 1)
    {
    }
    messageLength = Serial.read();
    if (messageLength <= 0)
    {
        return false;
    }
    // Await batch size byte
    while (Serial.available() < 1)
    {
    }
    batchSize = Serial.read();
    if (batchSize <= 0)
    {
        return false;
    }
    Serial.println();

    this->messageLength = messageLength;
    this->intRepresentation = intRepresentation;
    this->batchSize = batchSize;

    return true;
}

bool SerialClient::receive(byte* commands, int16_t* params)
{
    awaitIncomingByte();
    if (Serial.peek() == SETUP_INIT_BYTE)
    {
        protocolSetup();
        return false;
    }
    else
    {
        for (int i = 0; i < batchSize; i++)
        {
            awaitIncomingByte();
            commands[i] = Serial.read();
            for (int j = 0; j < messageLength; j++)
            {
                int param = 0;
                for (int count = 0; count < intRepresentation; count++)
                {
                    awaitIncomingByte();
                    param += (byte)Serial.read();
                    if (count < intRepresentation - 1)
                    {
                        param = param << 8;
                    }
                }
                params[i * messageLength + j] = param;
            }
        }
        Serial.println();
    }
    return true;
}

void SerialClient::awaitIncomingByte()
{
    while (Serial.available() < 1)
    {
    }
}

uint8_t SerialClient::getBatchSize()
{
    return batchSize;
}

uint8_t SerialClient::getMessageLength()
{
    return messageLength;
}

uint8_t SerialClient::getIntRepresentation()
{
    return intRepresentation;
}
