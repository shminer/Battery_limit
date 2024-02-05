/* Kernel node path for oneplus Color OS 12 */
#define BATT_CONTROL "/sys/devices/platform/soc/soc:oplus,chg_intf/oplus_chg/battery/mmi_charging_enable"
#define BATT_CAP "/sys/devices/platform/soc/soc:oplus,chg_intf/oplus_chg/battery/capacity"

#define LIMIT_CAP 85    //default limit charge to 85%
#define BATTSTATS_RESET 1   //clear battery status after USB disconnect when you need
