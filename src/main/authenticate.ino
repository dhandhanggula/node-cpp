//====================================================================
//====================================================================
// Fuction for authentication purpose
// List : 
//
// - isForMe(message)                   ==> check that received 
//                                          message is for this node
// - isFromSender(message, msgSender)   ==> check that received 
//                                          message is from the right 
//                                          sender
// - isCodeRight(message, code)         ==> check that message code is
//                                          same as what we want
//====================================================================
//====================================================================

bool isForMe(String message)
{
  String parseDestination = "";
  parseDestination = parsing(message, '|', 3);

  // Check message structure
  if(parseDestination == nodeID || parseDestination == "0")
  {
    return true;
  }

  return false;
}

bool isFromSender(String message, String msgSender)
{
  String parseSender = "";
  parseSender = parsing(message, '|', 2);

  if(parseSender == msgSender){return true;}

  return false;  
}

bool isCodeRight(String message, String wantedCode)
{
  String code = "";
  code = parsing(message, '|', 0);

  if(code == wantedCode){return true;}

  return false;  
}

bool isMsgIDRight(String message, String wantedCode)
{
  String code = "";
  code = parsing(message, '|', 1);

  if(code == wantedCode){return true;}

  return false;  
}

bool isInMsgIDHistory(String msgID)
{
  for(int i=0; i<10; i++)
  {
    if(msgIDHistory[i] == msgID)
    {
      return true;
    }
  }

  return false;
}