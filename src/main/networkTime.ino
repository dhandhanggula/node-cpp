//====================================================================
//====================================================================
// Fuction to set node time
//
// used for timestamp and message code
// RTC used : DS1307
//====================================================================
//====================================================================

void initNetworkTime()
{
  rtc.begin();
  rtc.adjust(DateTime(2022, 1, 1, 0, 0, 0));

}

void adjustNetworkTime(int year, int month, int day, int hour, int minute, int second)
{
  rtc.adjust(DateTime(year, month, day, hour, minute, second));
}