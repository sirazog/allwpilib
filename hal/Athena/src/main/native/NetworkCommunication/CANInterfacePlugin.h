// CANInterfacePlugin.h
//
//  Defines the API for building a CAN Interface Plugin to support
//    PWM-cable-free CAN motor control on FRC robots.  This allows you
//    to connect any CAN interface to the secure Jaguar CAN driver.
//

#ifndef __CANInterfacePlugin_h__
#define __CANInterfacePlugin_h__

#include <stdint.h>

#define CAN_IS_FRAME_REMOTE 0x80000000
#define CAN_MESSAGE_ID_MASK 0x1FFFFFFF

class CANInterfacePlugin
{
public:
	CANInterfacePlugin() {}
	virtual ~CANInterfacePlugin() {}

	/**
	 * This entry-point of the CANInterfacePlugin is passed a message that the driver needs to send to
	 * a device on the CAN bus.
	 * 
	 * This function may be called from multiple contexts and must therefore be reentrant.
	 * 
	 * @param messageID The 29-bit CAN message ID in the lsbs.  The msb can indicate a remote frame.
	 * @param data A pointer to a buffer containing between 0 and 8 bytes to send with the message.  May be NULL if dataSize is 0.
	 * @param dataSize The number of bytes to send with the message.
	 * @return Return any error code.  On success return 0.
	 */
	virtual int32_t sendMessage(uint32_t messageID, const uint8_t *data, uint8_t dataSize) = 0;

	/**
	 * This entry-point of the CANInterfacePlugin is passed buffers which should be populated with
	 * any received messages from devices on the CAN bus.
	 * 
	 * This function is always called by a single task in the Jaguar driver, so it need not be reentrant.
	 * 
	 * This function is expected to block for some period of time waiting for a message from the CAN bus.
	 * It may timeout periodically (returning non-zero to indicate no message was populated) to allow for
	 * shutdown and unloading of the plugin.
	 * 
	 * @param messageID A reference to be populated with a received 29-bit CAN message ID in the lsbs.
	 * @param data A pointer to a buffer of 8 bytes to be populated with data received with the message.
	 * @param dataSize A reference to be populated with the size of the data received (0 - 8 bytes).
	 * @return This should return 0 if a message was populated, non-0 if no message was not populated.
	 */
	virtual int32_t receiveMessage(uint32_t &messageID, uint8_t *data, uint8_t &dataSize) = 0;
};

/**
 * This function allows you to register a CANInterfacePlugin to provide access a CAN bus.
 * 
 * @param interface A pointer to an object that inherits from CANInterfacePlugin and implements
 * the pure virtual interface.  If NULL, unregister the current plugin.
 */
void FRC_NetworkCommunication_JaguarCANDriver_registerInterface(CANInterfacePlugin* interface);

#endif // __CANInterfacePlugin_h__
