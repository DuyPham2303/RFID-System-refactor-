/**
 * @file        Platform_Types.h
 * @brief       AUTOSAR Platform Type Definitions.
 * @details
 * This file defines platform-independent primitive data types used by
 * all MCAL modules. It provides fixed-width integer types, floating-point
 * types and boolean definitions according to AUTOSAR naming conventions.
 *
 * This file acts as the common foundation for every driver inside the
 * MCAL layer (SPI, I2C, UART, ADC, DIO, PWM, etc.).
 *
 * @version     1.0.0
 * @date        2026
 * @author      Pham Cao Duy
 *
 * @note
 * This implementation is an AUTOSAR-inspired educational implementation
 * based on STM32F10x.
 */
#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

/* #include "stm32f10x.h" */ // Device header
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/********************************************************
 * @brief dinh nghia cac kieu du lieu doc lap voi Autosar platform
 * @detail dinh nghia ve kich thuoc va dau cua moi loai du lieu
 *****************/

typedef unsigned char uint8;       /*kieu so nguyen 8-bit khong dau*/
typedef signed char sint8;         /*kieu so nguyen 8-bit co dau*/
typedef unsigned short uint16;     /*kieu so nguyen 16-bit khong dau*/
typedef signed short sint16;       /*kieu so nguyen 16-bit co dau*/
typedef unsigned long uint32;      /*kieu so nguyen 32-bit khong dau*/
typedef signed long sint32;        /*kieu so nguyen 32-bit co dau*/
typedef unsigned long long uint64; /*kieu so nguyen 64-bit khong dau*/
typedef signed long long sint64;   /*kieu so nguyen 64-bit co dau*/

typedef float float32;  /*kieu so thuc 32-bit*/
typedef double float64; /*kieu so thuc 64-bit*/

/********************************************************
 * @brief dinh nghia cac kieu du lieu volatile doc lap voi Autosar platform
 * @detail duoc su dung cho cac thanh ghi hoac vung nho
 *****************/
typedef volatile uint8 vuit8;    /*kieu so nguyen 8-bit khong dau*/
typedef volatile sint8 vsint8;   /*kieu so nguyen 8-bit co dau*/
typedef volatile uint16 vuint16; /*kieu so nguyen 16-bit khong dau*/
typedef volatile sint16 vsint16; /*kieu so nguyen 16-bit co dau*/
typedef volatile uint32 vuint32; /*kieu so nguyen 32-bit khong dau*/
typedef volatile sint32 vsint32; /*kieu so nguyen 32-bit co dau*/
typedef volatile uint64 vuint64; /*kieu so nguyen 64-bit khong dau*/
typedef volatile sint64 vsint64; /*kieu so nguyen 64-bit co dau*/

/********************************************************
 * @typedef boolean
 * @brief kieu du lieu boolean
 * @detail duoc su dung de danh gia dung hoac sai
 *****************/
typedef bool boolean;
#ifndef TRUE
#define TRUE ((boolean)1U)
#endif

#ifndef FALSE
#define FALSE ((boolean)0U)
#endif
#endif