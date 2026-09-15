#include "Testcase_Types.h"
/*cấu hình cục bộ lưu trữ để xử lý cho việc gọi các public API*/
static TC_IdType_e ty_s;
/**
 * @brief bảng đồ ánh xạ key name mô tả testcase
 * @details liệt kê group key name mô tả từng loại test case với Peripheral tương ứng
 * @note Có thể bổ sung linh hoạt nhóm key name tương ứng với Peripheral càn test
 */
const char **TC_KeyMappedList[TC_CNT_ID] = {
    [TC_DIO_ID] = (const char *[]){"[DIO] TC-01 : write & read channel",
                                   "[DIO] TC-02 : write & read port",
                                   "[DIO] TC-03 : write & read group",
                                   "[DIO] TC-04 : flip channel",
                                   NULL},
    [TC_PWM_ID] = (const char *[]){"[PWM] TC-01 : F_Pwm = 20Khz - duty = 25%",
                                   "[PWM] TC-02 : F_Pwm = 20Khz - duty = 50%",
                                   "[PWM] TC-03 : F_Pwm = 20Khz - duty = 75%",
                                   "[PWM] TC-04 : F_Pwm = 20Khz - duty = 100%",
                                   NULL}};

uint8 TestCaseTotalCount(TC_IdType_e ty)
{
    /* kiểm tra có group testcase nào có sẵn chưa */
    if (TC_KeyMappedList[ty] == NULL)
    {
        return 0;
    }
    uint8 cnt;

    /* đếm số lượng key name tương ứng với group testcase của periph cẩn test */
    while (TC_KeyMappedList[ty][cnt] != NULL)
    {
        cnt++;
    }

    /*lưu trữ lại cấu hình cục bộ */
    ty_s = ty;

    return cnt;
}

Std_ReturnType TC_TableInitParam(TestCaseMgr_s *TC_Mgr, uint8 size)
{
    if (TC_Mgr == NULL)
    {
        return E_NOT_OK;
    }
    /*duyệt qua danh sách và gán key name tương ứng và trạng thái xử lý mặc định*/
    for (uint8 index = 0; index < size; index++)
    {
        /*copy keyname và gán status xử lý cho từng phần tử con*/
        TC_Mgr[index].key = TC_KeyMappedList[ty_s][index];
        TC_Mgr[index].st = NOT_TEST;
    }
    return E_OK;
}

/**
 * @brief cấu hình Port dùng để demo testcase
 */
#if TESTCASE_TYPE == TESTCASE_DIO
const Port_ConfigType PortParamCfg[] = {
    {.pin = PORT_PIN_A0,
     .ModeType = PORT_MODE_OUTPUT_PUSH_PULL,
     .HwPortId = PORT_A,
     .SpeedType = PORT_SPEED_50MHZ},
    {.pin = PORT_PIN_A1,
     .ModeType = PORT_MODE_INPUT_PULL_UP,
     .HwPortId = PORT_A,
     .SpeedType = PORT_SPEED_50MHZ},
    {.pin = PORT_PIN_A2,
     .ModeType = PORT_MODE_INPUT_PULL_UP,
     .HwPortId = PORT_A,
     .SpeedType = PORT_SPEED_50MHZ},
    {.pin = PORT_PIN_A3,
     .ModeType = PORT_MODE_INPUT_PULL_UP,
     .HwPortId = PORT_A,
     .SpeedType = PORT_SPEED_50MHZ},
    {.pin = PORT_PIN_A4,
     .ModeType = PORT_MODE_INPUT_PULL_UP,
     .HwPortId = PORT_A,
     .SpeedType = PORT_SPEED_50MHZ},
    {.pin = PORT_PIN_A5,
     .ModeType = PORT_MODE_INPUT_PULL_UP,
     .HwPortId = PORT_A,
     .SpeedType = PORT_SPEED_50MHZ},
    {.pin = PORT_PIN_A6,
     .ModeType = PORT_MODE_INPUT_PULL_UP,
     .HwPortId = PORT_A,
     .SpeedType = PORT_SPEED_50MHZ},
    {.pin = PORT_PIN_A7,
     .ModeType = PORT_MODE_INPUT_PULL_UP,
     .HwPortId = PORT_A,
     .SpeedType = PORT_SPEED_50MHZ}};
#elif TESTCASE_TYPE == TESTCASE_PWM
const Port_ConfigType PortParamCfg[] = {
    {.pin = PORT_PIN_B4,
     .ModeType = PORT_MODE_ALTERNATE_PUSH_PULL,
     .HwPortId = PORT_B,
     .SpeedType = PORT_SPEED_50MHZ},
    {.pin = PORT_PIN_B5,
     .ModeType = PORT_MODE_ALTERNATE_PUSH_PULL,
     .HwPortId = PORT_B,
     .SpeedType = PORT_SPEED_50MHZ},
    {.pin = PORT_PIN_B0,
     .ModeType = PORT_MODE_ALTERNATE_PUSH_PULL,
     .HwPortId = PORT_B,
     .SpeedType = PORT_SPEED_50MHZ},
    {.pin = PORT_PIN_B1,
     .ModeType = PORT_MODE_ALTERNATE_PUSH_PULL,
     .HwPortId = PORT_B,
     .SpeedType = PORT_SPEED_50MHZ},
    {.pin = PORT_PIN_C13,
     .ModeType = PORT_MODE_OUTPUT_PUSH_PULL,
     .HwPortId = PORT_C,
     .SpeedType = PORT_SPEED_50MHZ}};

#else
"undefined TC Type"
#endif

/**
 * @brief Cấu hình Port tổng quát cho tất cả ngoại vi
 * @details đổi tên đối tượng cấu hình port tương ứng với ngoại vi cần
 *          sử dụng, để tính ra được số lượng nhóm chân cần sử dụng
 */
Port_ConfigSetType PortCfg = {PortParamCfg, sizeof(PortParamCfg) / sizeof(PortParamCfg[0])};
