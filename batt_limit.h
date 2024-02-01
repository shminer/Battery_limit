/* Kernel node path for oneplus Color OS 12 */
#define BATT_CONTROL "/sys/devices/platform/soc/soc:oplus,chg_intf/oplus_chg/battery/mmi_charging_enable"
#define BATT_CAP "/sys/devices/platform/soc/soc:oplus,chg_intf/oplus_chg/battery/capacity"

#define IS_CHARGING_USB "/sys/class/power_supply/usb/online"
#define IS_CHARGING_WIRELESS "/sys/class/power_supply/wireless/online"

#define LIMIT_CAP 85    //default limit charge to 85%
#define BATTSTATS_RESET 1   //clear battery status after USB disconnect when you need
