#include "Spi_Cfg.h"

/*Callback API được ISR xử lý*/
volatile uint8 g_CmdstatusCnt = FALSE; // đếm số lần chuỗi data cmd được gửi
volatile uint8 g_Ledstatus = FALSE;    // cập nhật trạng thái điều khiển led

static void JobHandler_CmdStatus()
{
    g_CmdstatusCnt++;
}

static void SeqHandler_LedStatus()
{
    g_Ledstatus = TRUE;
}
/////////////////////*** ĐỐI TƯỢNG CẤU HÌNH TĨNH CỤC BỘ***/////////////////////

/**
 * @brief Bảng cấu hình thông tin quản lý danh sách Id và tác vụ callback ứng với các nghiệp vụ
 * @details được module Spi_Internal sử dụng để ánh xạ tới loại callback cụ thể và nguồn ngắt
 *          xử lý phù hợp
 */
static const Spi_AsyncSeqCfgType s_AsyncNotiPtr[SPI_SEQ_MAX] = {
    [SPI_SEQ_UPDATE_LED_STATUS] = {
        .JobAsyncArray = {
            [SPI_JOB_SEND_LED_CMD] = {.flag = Spi_IRQ_SOURCE_TXE, .JobNoti = JobHandler_CmdStatus}},
        .SeqNoti = SeqHandler_LedStatus}};
/**
 * @brief bảng cấu hình thông số NVIC và IRQ priority,channel
 */
static const Mcu_NvicConfigType s_NvicCfg[SPI_MAX_GROUP] = {
    [SPI_GROUP_1] = {.IrqChannel = MCU_SPI_CH1_IRQ, .PreemptionPriority = 0, .SubPriority = 1}};
/**
 * @brief Khởi tạo mảng cấu hình tĩnh tương ứng vối từng bộ Spi cứng --> public App
 */
const Spi_ParamConfigType Spi_ChannelConfigArray[SPI_MAX_CHANNEL] =
    {
        [SPI_CHANNEL_LED_CMD] =
            {.HwId = SPI_GROUP_1,
             .BaudRatePrescaler = SPI_MR_BAUDRATE_8,
             .DataSize = SPI_MR_DATASIZE_8B,
             .CPOL = SPI_MR_CPOL_LOW,
             .CPHA = SPI_MR_CPHA_1EDGE,
             .FirstBit = SPI_MR_FIRSTBIT_MSB,
             .Dir = SPI_MR_1LINE_TX,
             .NvicCfgPtr = &s_NvicCfg[SPI_GROUP_1],
             .AsyncNotiPtr = &s_AsyncNotiPtr[SPI_SEQ_UPDATE_LED_STATUS]},
        [SPI_CHANNEL_SENSOR_READ] =
            {.HwId = SPI_GROUP_3,
             .BaudRatePrescaler = SPI_MR_BAUDRATE_8,
             .DataSize = SPI_MR_DATASIZE_8B,
             .CPOL = SPI_MR_CPOL_LOW,
             .CPHA = SPI_MR_CPHA_1EDGE,
             .FirstBit = SPI_MR_FIRSTBIT_MSB,
             .Dir = SPI_MR_1LINE_RX,
             .NvicCfgPtr = NULL_PTR},
        [SPI_CHANNEL_FAN_CMD] =
            {.HwId = SPI_GROUP_2,
             .BaudRatePrescaler = SPI_MR_BAUDRATE_8,
             .DataSize = SPI_MR_DATASIZE_8B,
             .CPOL = SPI_MR_CPOL_LOW,
             .CPHA = SPI_MR_CPHA_1EDGE,
             .FirstBit = SPI_MR_FIRSTBIT_MSB,
             .Dir = SPI_MR_1LINE_TX,
             .NvicCfgPtr = NULL_PTR},
        [SPI_CHANNEL_SPEED_CMD] =
            {.HwId = SPI_GROUP_3,
             .BaudRatePrescaler = SPI_MR_BAUDRATE_8,
             .DataSize = SPI_MR_DATASIZE_8B,
             .CPOL = SPI_MR_CPOL_LOW,
             .CPHA = SPI_MR_CPHA_1EDGE,
             .FirstBit = SPI_MR_FIRSTBIT_MSB,
             .Dir = SPI_MR_2LINES_RX_ONLY,
             .NvicCfgPtr = NULL_PTR},
        [SPI_CHANNEL_WHEEL_CMD] =
            {.HwId = SPI_GROUP_2,
             .BaudRatePrescaler = SPI_MR_BAUDRATE_8,
             .DataSize = SPI_MR_DATASIZE_8B,
             .CPOL = SPI_MR_CPOL_LOW,
             .CPHA = SPI_MR_CPHA_1EDGE,
             .FirstBit = SPI_MR_FIRSTBIT_MSB,
             .Dir = SPI_MR_2LINES_FD,
             .NvicCfgPtr = NULL_PTR}};
/**
 * @brief cấu hình hoàn chỉnh các kênh SPI
 *
 */
const Spi_ConfigTypes_s Spi_ConfigSet = {
    .ChannelParamCfgPtr = Spi_ChannelConfigArray,
    .SpiCfgCount = sizeof(Spi_ChannelConfigArray) / sizeof(Spi_ChannelConfigArray[0])};
/**
 * @brief Bảng cấu hình các SPI Job.
 * @details Mỗi Job xác định SPI hardware unit và danh sách Channel được
 *          thực thi trong một giao dịch SPI.
 */
const Spi_JobConfigType Spi_Jobs[SPI_JOB_MAX] = {
    [SPI_JOB_SEND_LED_CMD] =
        {
            .HwId = SPI_GROUP_1,
            .CsPinId = 10,                        // id của chân CS ứng với channel
            .ChannelList = {SPI_CHANNEL_LED_CMD}, // tối thiểu 1 channel / job
            .ActiveChannelCount = 1               // số lượng channel hiện tại mà job quản lý
        },
    [SPI_JOB_1] =
        {
            .HwId = SPI_GROUP_2,
            .CsPinId = 10,                                               // id của chân CS ứng với channel
            .ChannelList = {SPI_CHANNEL_FAN_CMD, SPI_CHANNEL_WHEEL_CMD}, // tối thiểu 1 channel / job
            .ActiveChannelCount = 2                                      // số lượng channel hiện tại mà job quản lý
        },
    [SPI_JOB_2] =
        {
            .HwId = SPI_GROUP_3,
            .CsPinId = 10,                                                   // id của chân CS ứng với channel
            .ChannelList = {SPI_CHANNEL_SENSOR_READ, SPI_CHANNEL_SPEED_CMD}, // tối thiểu 1 channel / job
            .ActiveChannelCount = 2                                          // số lượng channel hiện tại mà job quản lý
        }};
/**
 * @brief Bảng cấu hình các SPI Sequence.
 * @details Mỗi Sequence chứa danh sách Job ID theo thứ tự thực thi. Sequence
 *          cho phép driver gom nhiều giao dịch thành một luồng xử lý logic.
 */
const Spi_SequenceConfigType Spi_Sequences[SPI_SEQ_MAX] = {
    [SPI_SEQ_UPDATE_LCD] =
        {
            .JobList = {SPI_JOB_1, SPI_JOB_2}, // Sequence này gọi Job RFID
            .ActiveJobCount = 2},
    [SPI_SEQ_UPDATE_LED_STATUS] = {
        .JobList = {SPI_JOB_SEND_LED_CMD},
        .ActiveJobCount = 1}};
