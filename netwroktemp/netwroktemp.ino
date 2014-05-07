#include <Arduino.h>
#include <lib/UIPEthernet.h>

/*
  Turns on an LED on for one second, then off for one second, repeatedly.
*/
EthernetServer server = EthernetServer(1000);

void setup()
{
  Serial.begin(9600);

  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
  IPAddress myIP(192,168,1,6);

  Ethernet.begin(mac,myIP);

  server.begin();
}

void loop()
{
  size_t size;

  if (EthernetClient client = server.available())
    {
      while((size = client.available()) > 0)
        {
          uint8_t* msg = (uint8_t*)malloc(size);
          size = client.read(msg,size);
          Serial.write(msg,size);
          free(msg);
        }
      client.println("DATA from Server!");
      client.stop();
    }
}
