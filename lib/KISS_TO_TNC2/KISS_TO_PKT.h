#include <Arduino.h>
#include "KISS.h"

//Data control frames, ported from Direwolf
#define T_PKT             0xaa
#define R_PKT             0xa6
//END
//PACKET MODE
String encode_kiss_pkt(const String& tnc2FormattedFrame, bool &pktFrame);
String decode_kiss_pkt(const String &inputKISSTNCFrame, bool &dataFrame, bool &pktFrame);

String encapsulateKISS_pkt(const String &ax25Frame, uint8_t TNCCmd);