/********************************************************
 *******************
 * @file Std_Types.h
 * @brief Khai bao cac kieu du lieu va macro su dung chung cho toan bo he thong
 * @details Khai bao cac kieu tra ve bao gom Std_ReturnType, Std_VersionInfoType
 * @version 1.0
 * @date 2025-19-1
 * @author HALA Academy
 * @website https://hala.edu.vn/
 *********************************************************
 ******************/
 
#ifndef __STD_TYPES_H
#define __STD_TYPES_H
#include "stm32f10x.h"                  // Device header
#include "stdbool.h"
/********************************************************
* @brief dinh nghia cac kieu du lieu doc lap voi Autosar platform
* @detail dinh nghia ve kich thuoc va dau cua moi loai du lieu
*****************/

typedef unsigned char uint8; 				/*kieu so nguyen 8-bit khong dau*/
typedef signed char sint8;					/*kieu so nguyen 8-bit co dau*/
typedef unsigned short uint16; 			/*kieu so nguyen 16-bit khong dau*/
typedef signed short sint16;				/*kieu so nguyen 16-bit co dau*/
typedef unsigned long uint32;				/*kieu so nguyen 32-bit khong dau*/
typedef signed long sint32;					/*kieu so nguyen 32-bit co dau*/
typedef unsigned long long uint64;	/*kieu so nguyen 64-bit khong dau*/
typedef signed long long sint64;		/*kieu so nguyen 64-bit co dau*/

typedef float float32;  /*kieu so thuc 32-bit*/
typedef double float64; /*kieu so thuc 64-bit*/

/********************************************************
* @brief dinh nghia cac kieu du lieu volatile doc lap voi Autosar platform
* @detail duoc su dung cho cac thanh ghi hoac vung nho
*****************/
typedef volatile uint8	vuit8;    /*kieu so nguyen 8-bit khong dau*/
typedef volatile sint8	vsint8;		/*kieu so nguyen 8-bit co dau*/
typedef volatile uint16	vuint16;  /*kieu so nguyen 16-bit khong dau*/
typedef volatile sint16	vsint16;	/*kieu so nguyen 16-bit co dau*/
typedef volatile uint32	vuint32;	/*kieu so nguyen 32-bit khong dau*/
typedef volatile sint32	vsint32;	/*kieu so nguyen 32-bit co dau*/
typedef volatile uint64	vuint64;	/*kieu so nguyen 64-bit khong dau*/
typedef volatile sint64	vsint64;	/*kieu so nguyen 64-bit co dau*/


//kieu du lieu tra ve theo tieu chuan AUTOSAR
typedef uint8 Std_ReturnType;

//cac gia tri tieu chuan tra ve
#define E_OK 		0x00U //Tra ve trang thai thanh cong
#define E_NOT_OK	0x01U //Tra ve trang thai that bai

/********************************************************
* @typedef boolean
* @brief kieu du lieu boolean
* @detail duoc su dung de danh gia dung hoac sai 
*****************/
#ifdef TRUE
	#define TRUE 1U
#endif

#ifndef FALSE
	#define FALSE 0U
#endif

/********************************************************
* @brief dinh nghia con tro null
* @detail con tro null tro den dia chi 0
*****************/

#ifdef NULL_PTR
#define NULL_PTR ((void *)0)
#endif

/********************************************************
* @typedef Std_VersionInfoType
* @brief kieu du lieu cau truc thong tin phien ban
* @detail duoc su dung de luu tru thong tin ve phien ban phan mem
*****************/
typedef struct {
    uint16 vendorID;         /* ID cua nhà cung cap */
    uint16 moduleID;         /* ID cua module */
    uint8 sw_major_version;  /* phien ban chinh cua phan mem */
    uint8 sw_minor_version;  /* phien ban chinh cua phan mem */
    uint8 sw_patch_version;  /* phien ban chinh cua phan mem */
}Std_VersionInfoType;

/********************************************************
* @brief dinh nghia trang thai hoat dong/nhan roi
* @detail duoc su dung trong cac trang thai he thong
*****************/

#define STD_ACTIVE 0x01U  //trang thai nhan toi
#define STD_IDLE	 0x00U  //trang thai cho

/********************************************************
* @brief dinh nghia trang thai bat/tat
* @detail duoc su dung de dieu khien bat/tat
*****************/

#define STD_ON	0x01U //trang thai ON
#define STD_OFF	0X00U //tran thai OFF
#endif