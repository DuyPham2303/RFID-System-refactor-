#ifndef APPTYPES_H
#define APPTYPES_H
#include "../Bsw/Cfg/StdTypes.h"

/* error param options */
typedef enum ErrorType_e
{
    NORMAL,
    UID_NOTFOUND, /* Lỗi đọc thẻ rfid (uid ko tồn tại) */
    RFID_SCAN_EMPTY,
} ErrorType_e;

/* device's signal status */
typedef enum
{
    OPENING,
    CLOSING,
    ON,
    OFF
} OutSignalType_e;

/* device's configuration dtype */
typedef struct
{
    OutSignalType_e barrierSignal;
    OutSignalType_e LedSignal;
} DeviceStatusCfg_s;

/* System's properties configuration dtype */
typedef struct ModeStatusCfg_s
{
    AppMode_e apptype;
    ErrorType_e errortype;
    bool IsLcdModeUpdate;
} ModeStatusCfg_s;

typedef struct RfidUserDtypeCfg_s
{
    char *name;          // ten chu xe
    char *license_plate; // bien so xe
} RfidUserDtypeCfg_s;

/* shared global from SdCard_Read.c*/
extern int count;
extern struct RfidUserDtypeCfg_s *list_RfidUserData;

/* shared global from RFID_Scan.c */
extern uint8_t RfidUserData_Index;

/*shared global from control_output.c*/
extern DeviceStatusCfg_s DeviceStatusCfg;

/*shared global from display.c */
extern ModeStatusCfg_s ModeStatusCfg;

#endif