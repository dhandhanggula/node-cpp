//====================================================================
//====================================================================
// To check header message corrupted or not
//
// Check every sub message is not null
//====================================================================
//====================================================================

bool precheckmsg(String message)
{
  if(charMode(message, '|') != 5){return false;}

  else
  {
    return true;
  }
}