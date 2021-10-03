#include <Arduino.h>
#include "KISS.h"

#define APRS_CONTROL_FIELD 0x03
#define APRS_INFORMATION_FIELD 0xf0

#define HAS_BEEN_DIGIPITED_MASK 0b10000000
#define IS_LAST_ADDRESS_POSITION_MASK 0b1

String encode_kiss(const String& tnc2FormattedFrame, bool &pktFrame);
String decode_kiss(const String &inputKISSTNCFrame, bool &dataFrame, bool &pktFrame);

String encapsulateKISS(const String &ax25Frame, uint8_t TNCCmd);