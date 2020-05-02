/*
	SerialClient.h - Library for fast serial communication between Arduino and
    PC. Implements a master - client protocol, which speeds up the transfer of
    numeric data or G-Codes

	(C) 2020 by David Holtz <https://github.com/dmholtz>
*/

#ifndef SerialClient_h
#define SerialClient_h

#define SETUP_INIT_BYTE 0xFF

#include "Arduino.h"

class SerialClient
{
  public:
	SerialClient();

    /**
     * Expects an incoming protocol setup message. Returns true, if protocol
     * setup is successfull and updates internal parameters for future communication.
     * 
     * Awaits four incoming bytes to define the protocol.
     * 1. byte: must be SETUP_INIT_BYTE
     * 2. byte: integer representation: 1, 2 or 4
     * 3. byte: message length: positive integer > 0
     * 4. byte: batch size: positive integer > 0
     * */
    bool protocolSetup();

    /**
     * Receives a message according to the protocol specified during protocol setup.
     * A message consists of a batch of commands, each of them have messageLength
     * parameters.
     * 
     * @requires: params.size == messageLength * batchSize
     * @requires: commands.size == batchSize
     * @requires: this->intRepresentation <= 2;
     * 
     * @return true if messages have been received sucessfully and false otherwise
     * */
    bool receive(byte* commands, int16_t* params);
    
    void send();

    // Queries
    uint8_t getIntRepresentation();
    uint8_t getBatchSize();
    uint8_t getMessageLength();

    boolean protocolDefined();

  private:
    // Initialize to invalid default values => protocolDefine() == false
    void awaitIncomingByte();

    uint8_t intRepresentation = 0;
    uint8_t messageLength = 0;
    uint8_t batchSize = 0;
	
};

#endif


