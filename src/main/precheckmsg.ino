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
    for(int i=0; i<5; i++)
    {      
      if(parsing(message, '|', i) == NULL || parsing(message, '|', i) == "" )
      {
        return false;
      }
    }

    return true;
  }
}