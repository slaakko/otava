export module std.time;

import std.type.fundamental;
import std.rt;
import std.trace;

export namespace std {

struct tm
{
    int tm_sec;   // seconds after the minute - [0, 60] including leap second
    int tm_min;   // minutes after the hour - [0, 59]
    int tm_hour;  // hours since midnight - [0, 23]
    int tm_mday;  // day of the month - [1, 31]
    int tm_mon;   // months since January - [0, 11]
    int tm_year;  // years since 1900
    int tm_wday;  // days since Sunday - [0, 6]
    int tm_yday;  // days since January 1 - [0, 365]
    int tm_isdst; // daylight savings time flag
};

inline time_t time(time_t* arg) 
{
    return ort_time(arg);
}

inline tm* localtime(const time_t* time)
{
    return static_cast<tm*>(ort_localtime(time));
}

inline time_t mktime(void* time)
{
    return ort_mktime(time);
}

inline void strftime(char* buffer, int bufSize, const char* format, void* time)
{
    ort_strftime(buffer, bufSize, format, time);
}

} // namespace std
