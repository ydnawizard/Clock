#include <rtc_driver.h>

void RTC_Init(void)
{
    // Enable PWR clock
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // Enable access to backup domain
    PWR->CR |= PWR_CR_DBP;

    // Enable LSI
    RCC->CSR |= RCC_CSR_LSION;

    while (!(RCC->CSR & RCC_CSR_LSIRDY));

    // Reset backup domain
    RCC->BDCR |= RCC_BDCR_BDRST;
    RCC->BDCR &= ~RCC_BDCR_BDRST;

    // Select LSI as RTC clock source
    RCC->BDCR &= ~RCC_BDCR_RTCSEL;
    RCC->BDCR |= RCC_BDCR_RTCSEL_1;

    // Enable RTC
    RCC->BDCR |= RCC_BDCR_RTCEN;

    // Disable RTC write protection
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    // Enter initialization mode
    RTC->ISR |= RTC_ISR_INIT;

    while (!(RTC->ISR & RTC_ISR_INITF));

    // LSI is nominally ~32 kHz
    // 32,000 / 128 / 250 = 1 Hz
    RTC->PRER =
        (127 << RTC_PRER_PREDIV_A_Pos) |
        (249 << RTC_PRER_PREDIV_S_Pos);

    // Initialize time to 00:00:00
    RTC->TR = 0;

    // Initialize date to 00/01/01
    RTC->DR =
        (DecToBCD(1) << RTC_DR_DU_Pos) |
        (DecToBCD(1) << RTC_DR_MU_Pos);

    // Leave initialization mode
    RTC->ISR &= ~RTC_ISR_INIT;

    // Re-enable write protection
    RTC->WPR = 0xFF;
}

// Helper function to convert standard decimal to BCD
static uint8_t DecToBCD(uint8_t val) {
    return ((val / 10) << 4) | (val % 10);
}

void RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    // Unlock write protection
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    // Enter Initialization Mode
    RTC->ISR |= RTC_ISR_INIT;
    while (!(RTC->ISR & RTC_ISR_INITF));

    // Write to Time Register (TR)
    RTC->TR = (DecToBCD(hours)   << RTC_TR_HU_Pos) |
              (DecToBCD(minutes) << RTC_TR_MNU_Pos) |
              (DecToBCD(seconds) << RTC_TR_SU_Pos);

    // Exit Initialization Mode and Lock Protection
    RTC->ISR &= ~RTC_ISR_INIT;
    RTC->WPR = 0xFF;
}

void RTC_SetDate(uint16_t year, uint8_t month, uint8_t day, uint8_t weekday)
{
    // weekday:
    // 1 = Monday
    // 2 = Tuesday
    // 3 = Wednesday
    // 4 = Thursday
    // 5 = Friday
    // 6 = Saturday
    // 7 = Sunday

    // Unlock write protection
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    // Enter Initialization Mode
    RTC->ISR |= RTC_ISR_INIT;
    while (!(RTC->ISR & RTC_ISR_INITF));

    // Split values into tens and units
    uint8_t year_tens   = year / 10;
    uint8_t year_units  = year % 10;

    uint8_t month_tens  = month / 10;
    uint8_t month_units = month % 10;

    uint8_t day_tens    = day / 10;
    uint8_t day_units   = day % 10;

    // Write Date Register
    RTC->DR =
        (year_tens   << RTC_DR_YT_Pos)  |
        (year_units  << RTC_DR_YU_Pos)  |
        (weekday     << RTC_DR_WDU_Pos) |
        (month_tens  << RTC_DR_MT_Pos)  |
        (month_units << RTC_DR_MU_Pos)  |
        (day_tens    << RTC_DR_DT_Pos)  |
        (day_units   << RTC_DR_DU_Pos);

    // Exit Initialization Mode
    RTC->ISR &= ~RTC_ISR_INIT;

    // Re-enable write protection
    RTC->WPR = 0xFF;
}

// Helper function to convert BCD back to standard decimal
static uint8_t BCDToDec(uint8_t val) {
    return ((val >> 4) * 10) + (val & 0x0F);
}

void RTC_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds)
{
    uint32_t tr = RTC->TR;
    uint32_t dr = RTC->DR;

    (void)dr;

    uint8_t ht  = (tr >> RTC_TR_HT_Pos)  & 0x03;
    uint8_t hu  = (tr >> RTC_TR_HU_Pos)  & 0x0F;

    uint8_t mnt = (tr >> RTC_TR_MNT_Pos) & 0x07;
    uint8_t mnu = (tr >> RTC_TR_MNU_Pos) & 0x0F;

    uint8_t st  = (tr >> RTC_TR_ST_Pos)  & 0x07;
    uint8_t su  = (tr >> RTC_TR_SU_Pos)  & 0x0F;

    *hours   = ht * 10 + hu;
    *minutes = mnt * 10 + mnu;
    *seconds = st * 10 + su;
}

void RTC_GetDate(uint8_t *year, uint8_t *month, uint8_t *day)
{
    uint32_t tr = RTC->TR;
    uint32_t dr = RTC->DR;

    (void)tr;

    uint8_t yt = (dr >> RTC_DR_YT_Pos) & 0x0F;
    uint8_t yu = (dr >> RTC_DR_YU_Pos) & 0x0F;

    uint8_t mt = (dr >> RTC_DR_MT_Pos) & 0x01;
    uint8_t mu = (dr >> RTC_DR_MU_Pos) & 0x0F;

    uint8_t dt = (dr >> RTC_DR_DT_Pos) & 0x03;
    uint8_t du = (dr >> RTC_DR_DU_Pos) & 0x0F;

    *year  = yt * 10 + yu;
    *month = mt * 10 + mu;
    *day   = dt * 10 + du;
}

void RTC_GetStructTM(struct tm *out_time)
{
    uint32_t tr = RTC->TR;
    uint32_t dr = RTC->DR;

    uint8_t seconds = BCDToDec((tr >> RTC_TR_SU_Pos) & 0x0F);
    uint8_t minutes = BCDToDec((tr >> RTC_TR_MNU_Pos) & 0x0F);
    uint8_t hours   = BCDToDec((tr >> RTC_TR_HU_Pos) & 0x0F);

    uint8_t day     = BCDToDec((dr >> RTC_DR_DU_Pos) & 0x0F);
    uint8_t month   = BCDToDec((dr >> RTC_DR_MU_Pos) & 0x0F);
    uint8_t year    = BCDToDec((dr >> RTC_DR_YU_Pos) & 0x0F);

    // STM32 RTC weekday: 1 = Monday ... 7 = Sunday
    // struct tm:         0 = Sunday ... 6 = Saturday
    uint8_t rtc_weekday = (dr >> RTC_DR_WDU_Pos) & 0x07;

    uint8_t tm_weekday;

    if (rtc_weekday == 7)
        tm_weekday = 0;              // Sunday
    else
        tm_weekday = rtc_weekday;    // Monday=1 ... Saturday=6

    out_time->tm_sec  = seconds;
    out_time->tm_min  = minutes;
    out_time->tm_hour = hours;

    out_time->tm_mday = day;
    out_time->tm_mon  = month - 1;
    out_time->tm_year = year + 100;

    out_time->tm_wday = tm_weekday;
}
