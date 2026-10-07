#include "Spi_Cfg.h"
#define MAX_WIDTH_BYTE 32 // đối với mảng 16-bit thì số byte thực tế là 64
/*Callback API được ISR xử lý*/
volatile uint8 g_Ledstatus = FALSE; // cập nhật trạng thái điều khiển led
static void SeqHandler_LedStatus()
{
    g_Ledstatus = TRUE;
}
/////////////////////*** ĐỐI TƯỢNG CẤU HÌNH TĨNH CỤC BỘ***/////////////////////

/**
 * @brief bảng cấu hình thông số NVIC và IRQ priority,channel
 */
static const Mcu_NvicConfigType_s s_NvicCfg = {
    .IrqChannel = MCU_SPI_GROUP1_IRQ, .PreemptionPriority = 0, .SubPriority = 1};
/**
 * @brief Khởi tạo mảng cấu hình tĩnh tương ứng vối từng bộ Spi cứng
 */
static const Spi_ExternalDeviceConfigType_s Spi_DeviceGroupCfg[] =
    {

        {.HwId = SPI_HW_UNIT_1,
         .BaudRatePrescaler = SPI_MR_BAUDRATE_8,
         .DataSize = SPI_MR_DATASIZE_8B,
         .CPOL = SPI_MR_CPOL_LOW,
         .CPHA = SPI_MR_CPHA_1EDGE,
         .FirstBit = SPI_MR_FIRSTBIT_MSB,
         .Dir = SPI_MR_1LINE_TX,
         .Mode = SPI_MR_MODE_MASTER,
         .Nss = SPI_MR_NSS_SOFT,
         .NvicCfgPtr = &s_NvicCfg},
        {.HwId = SPI_HW_UNIT_2,
         .BaudRatePrescaler = SPI_MR_BAUDRATE_8,
         .DataSize = SPI_MR_DATASIZE_8B,
         .CPOL = SPI_MR_CPOL_LOW,
         .CPHA = SPI_MR_CPHA_1EDGE,
         .FirstBit = SPI_MR_FIRSTBIT_MSB,
         .Dir = SPI_MR_1LINE_RX,
         .Nss = SPI_MR_NSS_SOFT,
         .Mode = SPI_MR_MODE_MASTER,
         .NvicCfgPtr = NULL_PTR}};

/**
 * @brief Bảng cấu hình lưu trữ các Spi channel
 * @details Mỗi channel xác định dữ liệu thực tế cần truyền/nhận
 *          sẽ được Job thực thi
 */
static const Spi_ChannelConfigType_s Spi_channels[SPI_CH_MAX] = {
    [SPI_CH_LED_CMD] = {.BufferType = SPI_BUFFER_TYPE_EB, .DefaultLength = MAX_WIDTH_BYTE},
    [SPI_CH_FAN_CMD] = {.BufferType = SPI_BUFFER_TYPE_EB, .DefaultLength = MAX_WIDTH_BYTE},
    [SPI_CH_SENSOR_READ] = {.BufferType = SPI_BUFFER_TYPE_IB, .DefaultLength = MAX_WIDTH_BYTE},
    [SPI_CH_SPEED_CMD] = {.BufferType = SPI_BUFFER_TYPE_EB, .DefaultLength = MAX_WIDTH_BYTE},
    [SPI_CH_WHEEL_CMD] = {.BufferType = SPI_BUFFER_TYPE_IB, .DefaultLength = MAX_WIDTH_BYTE}};
/**
 * @brief Bảng cấu hình các SPI Job.
 * @details Mỗi Job xác định SPI hardware unit và danh sách Channel được
 *          thực thi trong một giao dịch SPI.
 */
static const Spi_JobConfigType_s Spi_Jobs[] = {
    [SPI_JOB_SEND_LED_CMD] =
        {.HwId = SPI_HW_UNIT_1,
         .CsPinId = DIO_CHANNEL_A10,      // id của chân CS ứng với channel
         .ChannelList = {SPI_CH_LED_CMD}, // tối thiểu 1 channel / job
         .ActiveTotalChIncurrentJob = 1},
    [SPI_JOB_1] =
        {.HwId = SPI_HW_UNIT_2,
         .CsPinId = DIO_CHANNEL_B10,                        // id của chân CS ứng với channel
         .ChannelList = {SPI_CH_FAN_CMD, SPI_CH_WHEEL_CMD}, // tối thiểu 1 channel / job
         .ActiveTotalChIncurrentJob = 2}};
/**
 * @brief Bảng cấu hình các SPI Sequence.
 * @details Mỗi Sequence chứa danh sách Job ID theo thứ tự thực thi. Sequence
 *          cho phép driver gom nhiều giao dịch thành một luồng xử lý logic.
 */
static const Spi_SequenceConfigType_s Spi_Sequences[SPI_SEQ_MAX] = {
    [SPI_SEQ_UPDATE_LCD] =
        {.JobList = {SPI_JOB_1}, // Sequence này gọi Job RFID
         .TotalJobIncurrentSequence = 2,
         .SeqNoti = NULL_PTR},
    [SPI_SEQ_UPDATE_LED_STATUS] =
        {.JobList = {SPI_JOB_SEND_LED_CMD},
         .TotalJobIncurrentSequence = 1,
         .SeqNoti = SeqHandler_LedStatus}};
/**
 * @brief cấu hình hoàn chỉnh các kênh SPI
 *
 */
const Spi_ConfigType_s g_Spi_ConfigSet = {
    .DeviceConfigPtr = Spi_DeviceGroupCfg,
    .DeviceCount = sizeof(Spi_DeviceGroupCfg) / sizeof(Spi_DeviceGroupCfg[0]),
    .ChannelConfigPtr = Spi_channels,
    .ChannelCount = sizeof(Spi_channels) / sizeof(Spi_channels[0]),
    .JobConfigPtr = Spi_Jobs,
    .JobCount = sizeof(Spi_Jobs) / sizeof(Spi_Jobs[0]),
    .SequenceConfigPtr = Spi_Sequences,
    .SequenceCount = sizeof(Spi_Sequences) / sizeof(Spi_Sequences[0])};
