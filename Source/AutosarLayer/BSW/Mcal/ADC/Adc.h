/**
 * @file Adc.h
 * @brief MCAL Analog-to-Digital Converter driver interface.
 * @details Declares ADC initialization, conversion, result-buffer, trigger,
 *          notification, and status services.
 * @req AUTOSAR MCAL ADC module; implementation is MCU dependent.
 */
#ifndef ADC_H
#define ADC_H
#include "Adc_Config.h"
#include "Adc_Types.h"

/**
 * @brief   khoi tao cac thong so ADC
 * @details cai dat cac thong so duoc cung cap trong `configPtr`.
 * @param   configPtr con tro den cau hinh adc
 * @note    - ham nay phai duoc goi dau tien truoc khi su dung ADC
 *          - chi goi 1 lan duy nhat, neu goi lai co the gay loi hoac ghi de cau hinh ban dau
 */
void Adc_Init(const Adc_ConfigType *configPtr);

/**
 * @brief   reset adc ve trang thai ban dau
 * @details dung tat ca hoat dong cua adc
 * @note    sau khi goi ham nay, adc se khong hoat dong lai cho den khi goi Adc_Init()
 */
void Adc_DeInit(void);

/**
 * @brief   thiet lap buffer luu ket qua chuyen doi adc cho 1 nhom adc
 * @param   Group : Id cua nhom adc can thiet lap trong buffer
 * @param   DataBufferPtr: con tro den buffer chua ket qua chuyen doi adc
 * @return  `E_OK` n?u buffer du?c thi?t l?p th�nh c�ng, `E_NOT_OK` n?u c� l?i.
 * @note    - Buffer ph?i c� k�ch thu?c d? d? ch?a t?t c? d? li?u ADC c?a nh�m.
 *          - Ph?i g?i h�m n�y tru?c khi b?t d?u chuy?n d?i nh�m (`Adc_StartGroupConversion`).
 */
Std_ReturnType Adc_SetupResultBuffer(
    Adc_GroupType Group,
    Adc_ValueGroupType *DataBufferPtr);

/**
 * @brief   bat dau qua trinh chuyen doi cac kenh adc
 * @details ham nay se reset buffer luu tru ket qua,va tra
 *					ve ket qua chuyen doi luu trong dia chi cua buffer
 *					duoc cau hinh trong Adc_SetupResultBuffer
 * @param   Group: ID cua kenh ADC muon chuyen doi gia tri
 * @note    ham nay phai duoc goi truoc khi muon doc ve ket qua chuyen doi adc
 **/

void Adc_StartGroupConversion(Adc_GroupType Group);

void Adc_StopGroupConversion(Adc_GroupType Group);

/**
 * @brief   doc ve ket qua chuyen doi ADC
 * @details ham nay se reset buffer luu tru ket qua,va tra
 *					ve ket qua chuyen doi luu trong dia chi cua buffer
 *					duoc cau hinh trong Adc_SetupResultBuffer
 * @param[IN]   Group: ID cua kenh adc muon doc ve ket qua chuyen doi
 * @param[OUT]  DataBufferPtr: luu tru ket qua chuyen doi ADC
 * @Returnvalue Std_ReturnType:
 *								+ E_OK: ket qua chuyen doi da duoc luu trong data buffer
 *								+ E_NOT_OK: ket qua chuyen xay ra loi
 * @note
 **/
Std_ReturnType Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType *DataBufferPtr);

/**
 * @brief   cho phep kenh ADC hoat dong
 * @details kich hoat bo ADC, va cho phep qua trinh chuyen doi adc
 *					duoc cau hinh trong Adc_SetupResultBuffer
 * @param[IN]   Group: ID cua kenh adc muon doc ve ket qua chuyen doi
 * @note   ham nay chi duoc su ung khi lua chon internal trigger source
                duoc dieu khien boi ADC harware
**/
void Adc_EnableHardwareTrigger(Adc_GroupType Group);

void Adc_DisableHardwareTrigger(Adc_GroupType Group);

void Adc_EnableGroupNotification(Adc_GroupType Group);

void Adc_DisableGroupNotification(Adc_GroupType Group);

Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group);

Adc_StreamNumSampleType Adc_GetStreamLastPointer(Adc_GroupType Group, Adc_ValueGroupType **PtrToSamplePtr);

#endif