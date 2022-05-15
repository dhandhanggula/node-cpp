void initNetworkTime()
{
  rtc.begin();
  rtc.adjust(DateTime(2022, 1, 1, 0, 0, 0));

}

void adjustNetworkTime(int year, int month, int day, int hour, int minute, int second)
{
  rtc.adjust(DateTime(year, month, day, hour, minute, second));
}

unsigned long int timeNow()
{
  DateTime now = rtc.now();
  networkTime = now.unixtime();

  return networkTime;
}
