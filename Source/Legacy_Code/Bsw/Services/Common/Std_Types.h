/**
 * @file        Std_Types.h
 * @brief       AUTOSAR Standard Type Definitions.
 * @details
 * This file provides common AUTOSAR standard types and return values
 * shared across all MCAL modules.
 *
 * Main contents:
 * - Std_ReturnType
 * - E_OK / E_NOT_OK
 * - NULL_PTR
 * - Standard logic levels
 *
 * Every public MCAL API should return Std_ReturnType whenever an operation
 * can succeed or fail.
 *
 * @version     1.0.0
 * @date        2026
 * @author      Pham Cao Duy
 */
#ifndef STD_TYPES_H
#define STD_TYPES_H

#include "Platform_Types.h"

typedef uint8 Std_ReturnType;

#define E_OK ((Std_ReturnType)0U)
#define E_NOT_OK ((Std_ReturnType)1U)

#ifndef NULL_PTR
#define NULL_PTR ((void *)0)
#endif

typedef enum
{
    STD_LOW = 0,
    STD_HIGH

} Std_LevelType;

/********************************************************
 * @brief dinh nghia con tro null
 * @detail con tro null tro den dia chi 0
 *****************/

#ifndef NULL_PTR
#define NULL_PTR ((void *)0)
#endif

/********************************************************
 * @typedef Std_VersionInfoType
 * @brief kieu du lieu cau truc thong tin phien ban
 * @detail duoc su dung de luu tru thong tin ve phien ban phan mem
 *****************/
typedef struct
{
    uint16 vendorID;        /* ID cua nh� cung cap */
    uint16 moduleID;        /* ID cua module */
    uint8 sw_major_version; /* phien ban chinh cua phan mem */
    uint8 sw_minor_version; /* phien ban chinh cua phan mem */
    uint8 sw_patch_version; /* phien ban chinh cua phan mem */
} Std_VersionInfoType;

/********************************************************
 * @brief dinh nghia trang thai hoat dong/nhan roi
 * @detail duoc su dung trong cac trang thai he thong
 *****************/

#define STD_ACTIVE 0x01U // trang thai nhan toi
#define STD_IDLE 0x00U   // trang thai cho

/********************************************************
 * @brief dinh nghia trang thai bat/tat
 * @detail duoc su dung de dieu khien bat/tat
 *****************/

#define STD_ON 0x01U  // trang thai ON
#define STD_OFF 0X00U // tran thai OFF
#endif