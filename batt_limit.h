#define BATT_CONTROL "/sys/devices/platform/soc/soc:oplus,chg_intf/oplus_chg/battery/mmi_charging_enable"
#define BATT_CAP "/sys/devices/platform/soc/soc:oplus,chg_intf/oplus_chg/battery/capacity"
#define IS_CHARGING_USB "/sys/class/power_supply/usb/online"
#define IS_CHARGING_WIRELESS "/sys/class/power_supply/wireless/online"
#define BATT_CAPFILE "/sdcard/battcap"

#define LIMIT_CAP 85
#define BATTSTATS_RESET 1


#define r 'r'
#define w 'w'
int do_batt_limit();
