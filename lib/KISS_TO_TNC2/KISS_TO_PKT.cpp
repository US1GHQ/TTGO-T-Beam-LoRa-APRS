#include "KISS_TO_PKT.h"

bool validateTNC2Frame_pkt(const String &tnc2FormattedFrame);
bool validateKISSFrame_pkt(const String &kissFormattedFrame);
String decapsulateKISS_pkt(const String &frame);

/*
 * https://ham.zmailer.org/oh2mqk/aprx/PROTOCOLS

	After successfull login, communication carries "TNC2" format
	APRS messages.  Namely text encoding of AX.25 UI frames in
	what became known as "TNC2 monitor style":

	    SOURCE>DESTIN:payload
	    SOURCE>DESTIN,VIA,VIA:payload

	The SOURCE, DESTIN, and VIA fields are AX.25 address fields,
        and have "-SSID" value annexed if the SSID is not zero.
	Also in VIA-fields, if the "HAS BEEN DIGIPEATED" bit is set
	(AX.25 v2 protocol feature) a star ('*') character is appended.
        VIA-fields are separated by comma (',') from DESTIN, and each
        other.

	A double-colon (':') separates address data from payload.
	The payload is passed _AS_IS_ without altering any message
	content bytes, however ending at first CR or LF character
	encountered in the packet.

 */


String encode_kiss_pkt(const String &tnc2FormattedFrame, bool &pktFrame) 
{
  String TNC2Frame = "";
  if (!pktFrame) {
  String ax25Frame = encapsulateKISS_pkt(tnc2FormattedFrame, CMD_DATA);
  TNC2Frame += ax25Frame;
  }
  return TNC2Frame;
}

String encapsulateKISS_pkt(const String &ax25Frame, uint8_t TNCCmd) 
{
  String kissFrame = "";
  kissFrame += (char) FEND; // start of frame
  kissFrame += (char) (0x0f & TNCCmd); // TNC0, cmd
  for (int i = 0; i < ax25Frame.length() ; ++i) 
  {
    char currentChar = ax25Frame.charAt(i);
    if (currentChar == (char) FEND) 
    {
      kissFrame += (char) FESC;
      kissFrame += (char) TFEND;
    }
     else if (currentChar == (char) FESC) 
    {
      kissFrame += (char) FESC;
      kissFrame += (char) TFESC;
    } 
     else 
    {
      kissFrame += currentChar;
    }
  }
  kissFrame += (char) FEND; // end of frame
  return kissFrame;
}


String decapsulateKISS_pkt(const String &frame, uint8_t TNCCmd) 
{
  String ax25Frame = "";
  for (int i = 2; i < frame.length() - 1; ++i) 
  {
    char currentChar = frame.charAt(i);
        if (currentChar == (char)FESC)
        {
            char nextChar = frame.charAt(i + 1);
            if (nextChar == (char)TFEND)
            {
                ax25Frame += (char)FEND;
            } 
            else if (nextChar == (char)TFESC)
            {
                ax25Frame += (char)FESC;
            }
            ++i;
        } 
     else 
    {
      ax25Frame += currentChar;
    }
  }
  return ax25Frame;
}

String decode_kiss_pkt(const String &inputKISSTNCFrame, bool &dataFrame, bool &pktFrame) {
  String TNC2Frame = "";
  if (validateKISSFrame_pkt(inputKISSTNCFrame)) {
  dataFrame = inputKISSTNCFrame.charAt(1) == CMD_DATA;
  String ax25Frame = decapsulateKISS_pkt(inputKISSTNCFrame, CMD_DATA);
  //END
  if (dataFrame && !pktFrame){
      String ax25Frame = decapsulateKISS_pkt(inputKISSTNCFrame, CMD_DATA);
      delay(250);
      TNC2Frame += ax25Frame;
    }
  }
  return TNC2Frame;
}

bool validateTNC2Frame_pkt(const String &tnc2FormattedFrame) {
  return (tnc2FormattedFrame.indexOf(':') != -1) && 
         (tnc2FormattedFrame.indexOf('>') != -1);
}

bool validateKISSFrame_pkt(const String &kissFormattedFrame) {
  return kissFormattedFrame.charAt(0) == (char) FEND &&
         kissFormattedFrame.charAt(kissFormattedFrame.length() - 1) == (char) FEND;
}
