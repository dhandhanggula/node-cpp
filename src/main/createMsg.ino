String createMsg(String Msg[])
{
  String readyMsg = "";

  for(int i=0; i<sizeof(Msg); i++)
  {
    if(i == sizeof(Msg)-1)
    {
      readyMsg += Msg[i];
    }

    else
    {
      readyMsg += Msg[i] + "|";
    }
  }

  return readyMsg;
}