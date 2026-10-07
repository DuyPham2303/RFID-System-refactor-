/**
 * @file Spi_Runtime.c
 * @brief Hiện thực các hàm quản lý Runtime Status của SPI Driver.
 */
#include "Spi_Runtime.h"

////////////*** Đối tượng cục bộ *file hiện tại**////////////
/* Vùng nhớ runtime được giấu kín (static) bên trong file này,
 * bên ngoài không thể truy cập trực tiếp bằng lệnh gán thô nữa.
 */
static Spi_RuntimeType Spi_HWUnitStatus[SPI_MAX_GROUP];

////////////*** Đối tượng toàn cục module Spi ***////////////
/**
 * @brief Bảng cấu hình runtime các SPI Channel.
 * @details Mỗi phần tử liên kết một Channel ID logic với buffer truyền,
 *          buffer nhận và độ dài dữ liệu tương ứng. Các Jobs sẽ sử dụng và ánh xạ tới
 *          channel tương ứng mà nó quản lý để lấy dữ liệu truyền đi hoặc lưu vảo buffer nội bộ
 *          của channel
 */
Spi_ChannelRuntimeType Spi_ChannelsRuntime[SPI_MAX_EB_CHANNELS];
// Mảng quản lý IB đặt tại vùng nhớ RAM của Driver
Spi_IbChannelType Spi_IbPool[SPI_MAX_IB_CHANNELS];

void Spi_Runtime_Init(void)
{
    for (Spi_GroupId_Type i = 0; i < SPI_MAX_GROUP; i++)
    {
        Spi_HWUnitStatus[i].Status = SPI_UNINIT;
        Spi_HWUnitStatus[i].TxBusy = FALSE;
        Spi_HWUnitStatus[i].RxBusy = FALSE;
    }
}

Spi_StatusType Spi_Runtime_GetStatus(Spi_GroupId_Type GroupId)
{
    if (GroupId >= SPI_MAX_GROUP)
    {
        return SPI_UNINIT;
    }

    // Nếu hệ thống đa nhiệm hoặc có ngắt, có thể thêm SchM_Enter_Spi() ở đây nếu cần
    return Spi_HWUnitStatus[GroupId].Status;
}

void Spi_Runtime_SetStatus(Spi_GroupId_Type GroupId, Spi_StatusType Status)
{
    if (GroupId < SPI_MAX_GROUP)
    {
        if (Spi_HWUnitStatus[GroupId].Status != Status)
        {
            Spi_HWUnitStatus[GroupId].Status = Status;
        }
    }
}

void Spi_Runtime_GetBusBusy(Spi_GroupId_Type GroupId, boolean *TxBusy, boolean *RxBusy)
{
    if (GroupId < SPI_MAX_GROUP)
    {
        if (TxBusy != NULL_PTR)
        {
            *TxBusy = Spi_HWUnitStatus[GroupId].TxBusy;
        }
        if (RxBusy != NULL_PTR)
        {
            *RxBusy = Spi_HWUnitStatus[GroupId].RxBusy;
        }
    }
}

void Spi_Runtime_SetBusBusy(Spi_GroupId_Type GroupId, boolean TxBusy, boolean RxBusy)
{
    if (GroupId < SPI_MAX_GROUP)
    {
        // SchM_Enter_Spi();
        Spi_HWUnitStatus[GroupId].TxBusy = TxBusy;
        Spi_HWUnitStatus[GroupId].RxBusy = RxBusy;
        // SchM_Exit_Spi();
    }
}