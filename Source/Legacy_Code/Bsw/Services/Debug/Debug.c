/**
 * @file Debug.c
 * @brief Triển khai dịch vụ debug và báo cáo trạng thái hệ thống.
 * @details Quản lý callback xuất dữ liệu, báo cáo trạng thái khởi tạo,
 *          báo cáo mã lỗi và điều phối các yêu cầu xuất thông tin cấu hình.
 *          Phần định dạng cấu hình Port, GPT và PWM được triển khai trong
 *          Debug_Info.c.
 * @req AUTOSAR_SWS_DiagnosticCommunicationManager;
 *      triển khai rút gọn phục vụ debug nội bộ của project.
 */

#include "Debug.h"
#include "Debug_Internal.h"

/** @brief Hàm callback để xuất dữ liệu debug. 
 * 
*/
static Debug_WriteType Debug_WriteFunction;

void Debug_WriteText(const char *Text)
{
    if (Debug_WriteFunction != NULL_PTR &&
        Text != NULL_PTR)
    {
        Debug_WriteFunction(Text);
    }
}

void Debug_Init(Debug_WriteType WriteFunction) {
    Debug_WriteFunction = WriteFunction;
}
void Debug_ReportInitStatus(Debug_ModuleType Module,
                            Std_ReturnType Result);
void Debug_ReportError(Debug_ModuleType Module,
                       uint8 ErrorCode);
void Debug_PrintInfo(Debug_InfoType Info){
    switch (Info)
    {
        case DEBUG_INFO_ALL:
            Debug_PrintAllConfig();
            break;
        case DEBUG_INFO_GPT:
            Debug_PrintGptConfig();
            break;
        case DEBUG_INFO_PWM:
            Debug_PrintPwmConfig();
            break;
        case DEBUG_INFO_PORT:
            Debug_PrintPortConfig();
            break;
        default:
            Debug_ReportError();
            break;
    }
}

