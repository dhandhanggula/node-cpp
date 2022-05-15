// To ensure the message is for this node with specific ID
bool messageIsForMe(String message)
{
  if(parsing(message, '|', 2) == nodeID)
  {
    return true;
  }
  else
  {
    return false;
  }
}


// To ensure the message is for node in the network
bool isMyNeighbour(String message)
{
  bool myNeighbour = false;

  for(int i=0; i<network.maxMember; i++)
  {
    if(parsing(message, '|', 3) == network.id)
    {
      myNeighbour = true;
    }
  }

  return myNeighbour;
}
