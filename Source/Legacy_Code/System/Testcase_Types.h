#ifndef TESTCASE_TYPES_H
#define TESTCASE_TYPES_H
#include "./Bsw/Services/Common/Std_Types.h"

/**
 * @brief Kiểu dữ liệu đánh dấu trạng thái xử lý testcase
 *
 */
typedef enum Test_status
{
    PASS,
    FAIL,
    NOT_TEST
} TC_status_e;

/*con trỏ tới testcase API cần chạy*/
typedef TC_status_e (*TCFuncPtr)(void);
/**
 * @brief Kiểu dữ liệu định nghĩa cấu hình quản lý và đánh dấu trạng thái xử lý testcase
 * @memberof [st]  : trạng thái xử lý
 * @memberof [key] : tên mô tả testcase tương ứng
 */
typedef struct TestCaseMgr_s
{
    TC_status_e st;
    TCFuncPtr RunFunc;
    const char *key;
} TestCaseMgr_s;

/**
 * @brief Kiểu dữ liệu ánh xạ đối tượng định danh của periph cần xử lý
 * @details dùng để ánh xạ tới chỉ số của phần tử tương ứng trong bản đồ
 *          lưu trữ nhóm tên mô tả test case cho từng ngoại vi
 */
typedef enum TC_IdType_e
{
    TC_DIO_ID = 0U,
    TC_GPT_ID,
    TC_PWM_ID,
    TC_SPI_ID,
    TC_USART_ID,
    TC_I2C_ID,
    TC_CNT_ID
} TC_IdType_e;

/**
 * @brief đếm số lượng nhóm testcase của Periph cần kiểm thử
 * @param ty loại testcase của ngoại vi tương ứng cần trả về số lượng TC
 * @return uint8 số TC
 */
uint8 TestCaseTotalCount(TC_IdType_e ty);

/**
 * @brief khởi tạo danh sách tên TC cụ thể cho ngoại vi tương ứng cẩn test
 * @param[TC_Mgr] : con trỏ tới mảng danh sách quản lý TC
 * @param[size]   : số lượng TC của danh sách
 */
Std_ReturnType TC_TableInitParam(TestCaseMgr_s *TC_Mgr, uint8 size);

#endif