//Tim Imports
#include <SLIPEncodedSerial.h>
#include <OSCData.h>
#include <OSCBundle.h>
#include <OSCBoards.h>
#include <OSCTiming.h>
#include <OSCMessage.h>
#include <OSCMatch.h>
#include <SLIPEncodedUSBSerial.h>

#include "Arduino.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <OSCMessage.h>

// -----------------------
// Internals TIM
// -----------------------

WiFiUDP udp;             // Wifi UDP instance
IPAddress ip;           // The ESP's IP
IPAddress outIp(xxx, xx, xx, x); //IP Address to send to (DASHBOARD IP)
OSCMessage msg("/Client-XX"); // OSC Address, replace XX by your Client ID

// -----------------------
// Network Stuff
// -----------------------

char ssid[] = "xxxx";                       // your network SSID (name)
char pass[] = "xxxx";                       // your network password
const unsigned int receivePort = 8888;  // Local port to listen (Same as port in dashboard)
const unsigned int outPort = 57111;      // Port to send to DO NOT CHANGE

// -----------------------
// General Program Logic TIM
// -----------------------

/**
 * Send OSC Message with certain state
 */

void stuurMessage(int state) {
  // Add state to message
  msg.add(state);

  // Send a message
  sendMessage(outIp, msg);
}

/**
 * Sends an OSC message to a specific address
 */
void sendMessage(IPAddress to, OSCMessage& msg) {
  // Sending over udp, begin the packet header
  udp.beginPacket(to, outPort);

  // Send the message, the bytes to the SLIP stream
  msg.send(udp);

  // Mark the end of the OSC packet
  udp.endPacket();

  // Free space occupied by message
  msg.empty();
}

/**
 * Receiving the messages from a specific address
 */
void receiveMessage() {
  // Creates the internal
  OSCMessage inmsg;

  // Parse the UDP package
  int size = udp.parsePacket();

  // Did we receive something?
  if (size > 0) {
    while (size--) {
      inmsg.fill(udp.read());
    }
    if (!inmsg.hasError()) {
      inmsg.dispatch("/servermessage", handleReceive);
      // @TODO dispatch other functions
    }
  }
}

// ------------------------------
// Dispatched Logic from receiver
// ------------------------------

/**
 * A demo function to know if the receiver is working
 */
void handleReceive(OSCMessage &msg) {
//  handleState(msg.getInt(0));
//@TODO Zelf schrijven hoe je de ontvangen state handlet
}

// ------------------------------
// General loop
// ------------------------------

void loop() {
  //@TODO Jouw code

  //@TODO gebruik de stuurMessage() functie met een state

  // Checks if we received a message
  receiveMessage();

}
