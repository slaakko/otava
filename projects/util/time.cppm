export module util.time;

import std;

export namespace util {

enum class Month : std::int8_t
{
    january = 1, february, march, april, may, june, july, august, september, october, november, december
};

int GetMonthDays(Month month, int year) noexcept;

class Date
{
public:
    inline Date() noexcept : year(0), month(Month::january), day(1)
    {
    }
    inline Date(short year_, Month month_, std::int8_t day_) noexcept : year(year_), month(month_), day(day_)
    {
    }
    inline std::int16_t Year() const noexcept
    {
        return year;
    }
    inline Month GetMonth() const noexcept
    {
        return month;
    }
    inline std::int8_t Day() const noexcept
    {
        return day;
    }
    Date AddDays(int n) noexcept;
    Date AddMonths(int n) noexcept;
    Date AddYears(int n) noexcept;
    std::string ToString() const;
    std::string ToString(bool omitDashes) const;
private:
    std::int16_t year;
    Month month;
    std::int8_t day;
};

Date GetCurrentDate() noexcept;

bool operator==(const Date& left, const Date& right) noexcept;

inline bool operator!=(const Date& left, const Date& right) noexcept
{
    return !(left == right);
}

bool operator<(const Date& left, const Date& right) noexcept;

inline bool operator>(const Date& left, const Date& right) noexcept
{
    return right < left;
}

inline bool operator<=(const Date& left, const Date& right) noexcept
{
    return !(right > left);
}

inline bool operator>=(const Date& left, const Date& right) noexcept
{
    return !(left < right);
}

Date ParseDate(const std::string& dateStr);

class DateTime
{
public:
    inline DateTime() noexcept : date(), secs(0)
    {
    }
    inline DateTime(const Date& date_) noexcept : date(date_), secs(0)
    {
    }
    inline DateTime(const Date& date_, std::int32_t secs_) noexcept : date(date_), secs(secs_)
    {
    }
    inline Date GetDate() const noexcept
    {
        return date;
    }
    inline std::int32_t Hours() const noexcept
    {
        return secs / 3600;
    }
    inline std::int32_t Minutes() const noexcept
    {
        return secs / 60;
    }
    inline std::int32_t Seconds() const noexcept
    {
        return secs;
    }
    std::string ToString() const;
    std::string ToString(bool omitDashes, bool omitColons, bool omitMins, bool omitSecs) const;
private:
    Date date;
    std::int32_t secs;
};

DateTime GetCurrentDateTime();

bool operator==(const DateTime& left, const DateTime& right) noexcept;

inline bool operator!=(const DateTime& left, const DateTime& right) noexcept
{
    return !(left == right);
}

bool operator<(const DateTime& left, const DateTime& right) noexcept;

inline bool operator>(const DateTime& left, const DateTime& right) noexcept
{
    return right < left;
}

inline bool operator<=(const DateTime& left, const DateTime& right) noexcept
{
    return !(right > left);
}

inline bool operator>=(const DateTime& left, const DateTime& right) noexcept
{
    return !(left < right);
}

DateTime ParseDateTime(const std::string& dateTimeStr);

const int secsInDay = 24 * 3600;

std::string FormatTimeMs(std::int32_t milliseconds);

std::int64_t CurrentMs() noexcept;

std::int64_t GetCurrentTime() noexcept;

std::string DurationStr(std::int64_t nanosecs);

std::time_t Time() noexcept;

std::time_t MkTime(const DateTime& dt);

DateTime ToDateTime(std::time_t time);

std::string TimeToString(std::time_t time);

} // namespace soulng::util
