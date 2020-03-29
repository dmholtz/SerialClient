#include <SerialClient.h>

/*  Demo for Serial master-client communications
 *   
 * Summary:
 *  * before sending, call protocol setup
 *  * then, retrieve batch size and message length
 *  
 *  * create variable-size arrays with pointers, match sizes according to protocol
 *  * receive commands (operations + params) by calling receiveNByteInts(...)
 * 
 * What input does this demo expect?
 * 1) protocol setup (arbitrary batch size and message length, int_rep must be 2 bytes
 * 2) receive a single batch
 * 3) stress testing 50 batches
 * 
 */

void setup() {
  // put your setup code here, to run once:
  SerialClient sc = {};
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  // setup protocol
  sc.protocolSetup();
  
  int batch = sc.getBatchSize();
  int messageLength = sc.getMessageLength();

  // Create pointers for variable-size arrays
  byte* operations = 0;
  int* data = 0;

  // If pointer is not 0, delete whatever it points to
  // Then, let the pointer point on a new array
  if (operations != 0)
  {
    delete [] operations;
  }
  operations = new byte[batch];
  if (data != 0)
  {
    delete [] data;
  }
  data = new int[batch * messageLength];

  // receiving the first batch
  sc.receive2ByteInts(operations, data);

  for (int i = 0; i < 50; i ++)
  {
    // If pointer is not 0, delete whatever it points to
    // Then, let the pointer point on a new array
    if (operations != 0)
    {
      delete [] operations;
    }
    operations = new byte[batch];
    if (data != 0)
    {
      delete [] data;
    }
    data = new int[batch * messageLength];

    // receiving the first batch
    sc.receive2ByteInts(operations, data);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
