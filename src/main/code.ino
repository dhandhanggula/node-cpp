//====================================================================
//====================================================================
// Message code
// List :
//
// General Purpose
// - 00   ==> ping
// - 01   ==> answer Ping
//
// Messaging
// - 10   ==> send msg
// - 11   ==> ACK
//
// DSR Routing
// - 20   ==> Route request (RREQ)
// - 21   ==> Route reply (RREP)
//
// Network Maintenance
// - 30   ==> Route maintenance(RERR)
//====================================================================
//====================================================================

String code(String codeName)
{
  if(codeName == "ping"){return "00";}
  if(codeName == "ansPing"){return "01";}
  if(codeName == "msg"){return "10";}
  if(codeName == "ack"){return "11";}
  if(codeName == "rreq"){return "20";}
  if(codeName == "rrep"){return " 21";}
  if(codeName == "rerr"){return "30";}
}