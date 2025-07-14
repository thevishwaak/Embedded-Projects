u8 dec_to_bcd(u8 val) {
    return ((val / 10) << 4) | (val % 10);
}

u8 bcd_to_dec(u8 val) {
    return ((val >> 4) * 10) + (val & 0x0F);
}

void rtc_write(u8 sec,u8 min, u8 hours,u8 day,u8 date, u8 month, u8 year)
{
	i2c_start();
	i2c_write(0x68<<1);
	i2c_write(0x00);
	i2c_write(dec_to_bcd(sec));
	i2c_write(dec_to_bcd(min));
	i2c_write(dec_to_bcd(hours));
	i2c_write(dec_to_bcd(day));
	i2c_write(dec_to_bcd(date));
	i2c_write(dec_to_bcd(month));
	i2c_write(dec_to_bcd(year));
	i2c_stop();
}


void rtc_read(u8 *sec,u8 *min, u8 *hours,u8 *day,u8 *date, u8 *month, u8 *year)
{
	u8 rawhours;
	i2c_start();
	i2c_write(0x68<<1);
	i2c_write(0x00);
	i2c_restart();
	i2c_write(0x68<<1|1);
	*sec = bcd_to_dec(i2c_masterack());
	*min = bcd_to_dec(i2c_masterack());
	rawhours = i2c_masterack();
	*hours = bcd_to_dec(rawhours & 0x3f);
	*day = bcd_to_dec(i2c_masterack());
	*date = bcd_to_dec(i2c_masterack());
	*month = bcd_to_dec(i2c_masterack());
	*year = bcd_to_dec(i2c_nack());
	i2c_stop();
}
