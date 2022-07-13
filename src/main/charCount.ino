// read how many character in given string

int charCount(String givenString, char Targeted)
{
  int stringLength = givenString.length();
  uint8_t charCount = 0;

  for(int i=0; i<stringLength; i++)
  {
    if(givenString.charAt(i) == Targeted)
    {
      charCount += 1;
    }
  }

  return charCount;
}