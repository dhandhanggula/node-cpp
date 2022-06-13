// read how many character in given string

int charMode(String givenString, char Targeted)
{
  int stringLength = givenString.length();
  int charCount = 0;

  for(int i=0; i<stringLength; i++)
  {
    if(givenString.charAt(i) == Targeted)
    {
      charCount += 1;
    }
  }

  return charCount;
}