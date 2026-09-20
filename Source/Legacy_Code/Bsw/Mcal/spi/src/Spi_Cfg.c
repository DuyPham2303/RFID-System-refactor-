#include "Spi_Cfg.h"
#include "Spi_map.h"

/*Khởi tạo mảng cấu hình tĩnh tương ứng vối từng bộ Spi cứng --> public App*/
const Spi_ConfigType Spi_ConfigSet[SPI_CHANNEL_MAX] =
    {
        [SPI_CHANNEL_0] =
            {.HwId = SPI_GROUP_1,
             .BaudRatePrescaler = SPI_MR_BAUDRATE_8,
             .DataSize = SPI_MR_DATASIZE_8B,
             .CPOL = SPI_MR_CPOL_LOW,
             .CPHA = SPI_MR_CPHA_1EDGE,
             .FirstBit = SPI_MR_FIRSTBIT_MSB,
             .Dir = SPI_MR_1LINE_TX,
             .IrqCfgPtr = NULL_PTR},
        [SPI_CHANNEL_1] =
            {.HwId = SPI_GROUP_2,
             .BaudRatePrescaler = SPI_MR_BAUDRATE_8,
             .DataSize = SPI_MR_DATASIZE_8B,
             .CPOL = SPI_MR_CPOL_LOW,
             .CPHA = SPI_MR_CPHA_1EDGE,
             .FirstBit = SPI_MR_FIRSTBIT_MSB,
             .Dir = SPI_MR_1LINE_RX,
             .IrqCfgPtr = NULL_PTR},
        [SPI_CHANNEL_2] =
            {.HwId = SPI_GROUP_2,
             .BaudRatePrescaler = SPI_MR_BAUDRATE_8,
             .DataSize = SPI_MR_DATASIZE_8B,
             .CPOL = SPI_MR_CPOL_LOW,
             .CPHA = SPI_MR_CPHA_1EDGE,
             .FirstBit = SPI_MR_FIRSTBIT_MSB,
             .Dir = SPI_MR_1LINE_TX,
             .IrqCfgPtr = NULL_PTR},
        [SPI_CHANNEL_3] =
            {.HwId = SPI_GROUP_2,
             .BaudRatePrescaler = SPI_MR_BAUDRATE_8,
             .DataSize = SPI_MR_DATASIZE_8B,
             .CPOL = SPI_MR_CPOL_LOW,
             .CPHA = SPI_MR_CPHA_1EDGE,
             .FirstBit = SPI_MR_FIRSTBIT_MSB,
             .Dir = SPI_MR_2LINES_RX_ONLY,
             .IrqCfgPtr = NULL_PTR},
        [SPI_CHANNEL_4] =
            {.HwId = SPI_GROUP_2,
             .BaudRatePrescaler = SPI_MR_BAUDRATE_8,
             .DataSize = SPI_MR_DATASIZE_8B,
             .CPOL = SPI_MR_CPOL_LOW,
             .CPHA = SPI_MR_CPHA_1EDGE,
             .FirstBit = SPI_MR_FIRSTBIT_MSB,
             .Dir = SPI_MR_2LINES_FD,
             .IrqCfgPtr = NULL_PTR}};

/***Các đối tượng sử dụng nội bộ module SPI --> extern Spi_Internal.h***/

/**
 * @brief Bảng đối tượng runtime của SPI driver.
 * @details Lưu trạng thái hoạt động hiện tại của từng SPI instance đã cấu
 *          hình, chẳng hạn trạng thái idle/busy và Channel đang xử lý.
 */
Spi_RuntimeType Spi_Channels_Status[] = {
    [SPI_CHANNEL_0] = {.RxBusy = FALSE, .TxBusy = FALSE, .Status = SPI_UNINIT},
    [SPI_CHANNEL_1] = {.RxBusy = FALSE, .TxBusy = FALSE, .Status = SPI_UNINIT},
    [SPI_CHANNEL_2] = {.RxBusy = FALSE, .TxBusy = FALSE, .Status = SPI_UNINIT},
    [SPI_CHANNEL_3] = {.RxBusy = FALSE, .TxBusy = FALSE, .Status = SPI_UNINIT},
    [SPI_CHANNEL_4] = {.RxBusy = FALSE, .TxBusy = FALSE, .Status = SPI_UNINIT}};

/**
 * @brief Bảng cấu hình các SPI Channel.
 * @details Mỗi phần tử liên kết một Channel ID logic với buffer truyền,
 *          buffer nhận và độ dài dữ liệu tương ứng.
 */
Spi_ChannelConfigType Spi_Channels_data[SPI_CHANNEL_MAX] = {
    [SPI_CHANNEL_0] = {.dataWidthByte = 0, .RxBuffer = NULL_PTR, .TxBuffer = NULL_PTR},
    [SPI_CHANNEL_1] = {.dataWidthByte = 0, .RxBuffer = NULL_PTR, .TxBuffer = NULL_PTR},
    [SPI_CHANNEL_2] = {.dataWidthByte = 0, .RxBuffer = NULL_PTR, .TxBuffer = NULL_PTR},
    [SPI_CHANNEL_3] = {.dataWidthByte = 0, .RxBuffer = NULL_PTR, .TxBuffer = NULL_PTR},
    [SPI_CHANNEL_4] = {.dataWidthByte = 0, .RxBuffer = NULL_PTR, .TxBuffer = NULL_PTR}};

/**
 * @brief Bảng cấu hình các SPI Job.
 * @details Mỗi Job xác định SPI hardware unit và danh sách Channel được
 *          thực thi trong một giao dịch SPI.
 */
const Spi_JobConfigType Spi_Jobs[SPI_JOB_MAX] = {
    [SPI_JOB_0] =
        {
            .CsPinId = 10,                  // id của chân CS ứng với channel
            .ChannelList = {SPI_CHANNEL_0}, // tối thiểu 1 channel / job
            .ActiveChannelCount = 1         // số lượng channel hiện tại mà job quản lý
        },
    [SPI_JOB_1] =
        {
            .CsPinId = 10,                                 // id của chân CS ứng với channel
            .ChannelList = {SPI_CHANNEL_2, SPI_CHANNEL_4}, // tối thiểu 1 channel / job
            .ActiveChannelCount = 2                        // số lượng channel hiện tại mà job quản lý
        },
    [SPI_JOB_2] =
        {
            .CsPinId = 10,                                 // id của chân CS ứng với channel
            .ChannelList = {SPI_CHANNEL_1, SPI_CHANNEL_3}, // tối thiểu 1 channel / job
            .ActiveChannelCount = 2                        // số lượng channel hiện tại mà job quản lý
        }};
/**
 * @brief Bảng cấu hình các SPI Sequence.
 * @details Mỗi Sequence chứa danh sách Job ID theo thứ tự thực thi. Sequence
 *          cho phép driver gom nhiều giao dịch thành một luồng xử lý logic.
 */
const Spi_SequenceConfigType Spi_Sequences[SPI_SEQ_MAX] = {
    [SPI_SEQ_UPDATE_LED_STATUS] =
        {
            .JobList = {SPI_JOB_1, SPI_JOB_2}, // Sequence này gọi Job RFID
            .ActiveJobCount = 2},
    [SPI_SEQUENCE_UPDATE_LCD] = {
        .JobList = {SPI_JOB_0},
        .ActiveJobCount = 1}};
