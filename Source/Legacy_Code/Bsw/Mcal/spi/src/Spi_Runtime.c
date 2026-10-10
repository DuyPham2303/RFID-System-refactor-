/**
 * @file Spi_Runtime.c
 * @brief Hiện thực các hàm quản lý Runtime Status của SPI Driver.
 */
#include "Spi_Runtime.h"

/* Vùng nhớ runtime được giấu kín (static) bên trong file này,
 * bên ngoài không thể truy cập trực tiếp bằng lệnh gán thô nữa.
 */
static Spi_RuntimeType_s s_GroupHwRuntime[SPI_HW_MAX_UNIT];
static Spi_ChannelRuntimeType_s s_GroupChRuntime[SPI_CH_MAX];
static Spi_JobRuntimeType_s s_GroupJobRuntime[SPI_JOB_MAX];
static Spi_SequenceRuntimeType_s s_GroupSeqRuntime[SPI_SEQ_MAX];
static Spi_IbChannelType_s s_IbPool[SPI_MAX_IB_CHANNELS];
static Spi_HwUnitRuntimeType_s s_ContextHwRuntime;
void Spi_Runtime_Init()
{
    /*Khởi tạo trạng thái của tất cả phần cứng ban đầu*/
    for (uint8 i = 0; i < g_Spi_ConfigSet.DeviceCount; i++)
    {
        s_GroupHwRuntime[i].Status = SPI_UNINIT;
        s_GroupHwRuntime[i].RxBusy = FALSE;
        s_GroupHwRuntime[i].TxBusy = FALSE;
    }

    /*Khởi tạo loại buffer cho các channel*/
    for (uint8 i = 0; i < g_Spi_ConfigSet.ChannelCount; i++)
    {
        if (g_Spi_ConfigSet.ChannelConfigPtr[i].BufferType == SPI_BUFFER_TYPE_EB)
        {
            s_GroupChRuntime[i].BufferType = SPI_BUFFER_TYPE_EB;
            s_GroupChRuntime[i].DefaultLength = g_Spi_ConfigSet.ChannelConfigPtr[i].DefaultLength;
        }
        else
        {
            s_GroupChRuntime[i].BufferType = SPI_BUFFER_TYPE_IB;
            s_IbPool[i].Length = g_Spi_ConfigSet.ChannelConfigPtr[i].DefaultLength;
        }
    }

    /*Khởi tạo trạng thái chưa có nghiệp vụ nào*/
    for (uint8 i = 0; i < g_Spi_ConfigSet.SequenceCount; i++)
    {
        s_GroupSeqRuntime[i].Status = SPI_UNINIT;
    }

    /*Khởi tạo trạng thái chưa có tác vụ nào*/
    for (uint8 i = 0; i < g_Spi_ConfigSet.JobCount; i++)
    {
        s_GroupJobRuntime[i].IsBusy = FALSE;
    }
}

Spi_StatusType_e Spi_Runtime_GetHwGroupStatus(Spi_HwUnitType_e GroupId)
{
    if (GroupId >= SPI_HW_MAX_UNIT)
    {
        return SPI_UNINIT;
    }

    // Nếu hệ thống đa nhiệm hoặc có ngắt, có thể thêm SchM_Enter_Spi() ở đây nếu cần
    return s_GroupHwRuntime[GroupId].Status;
}
void Spi_Runtime_SetHwGroupStatus(Spi_HwUnitType_e GroupId, Spi_StatusType_e Status)
{
    if (GroupId < SPI_HW_MAX_UNIT)
    {
        if (s_GroupHwRuntime[GroupId].Status != Status)
        {
            s_GroupHwRuntime[GroupId].Status = Status;
        }
    }
}
void Spi_Runtime_GetBusBusy(Spi_HwUnitType_e GroupId, boolean *TxBusy, boolean *RxBusy)
{
    if (GroupId < SPI_HW_MAX_UNIT)
    {
        if (TxBusy != NULL_PTR)
        {
            *TxBusy = s_GroupHwRuntime[GroupId].TxBusy;
        }
        if (RxBusy != NULL_PTR)
        {
            *RxBusy = s_GroupHwRuntime[GroupId].RxBusy;
        }
    }
}
void Spi_Runtime_SetBusBusy(Spi_HwUnitType_e GroupId, boolean TxBusy, boolean RxBusy)
{
    if (GroupId < SPI_HW_MAX_UNIT)
    {
        s_GroupHwRuntime[GroupId].TxBusy = TxBusy;
        s_GroupHwRuntime[GroupId].RxBusy = RxBusy;
    }
}
Spi_HwUnitRuntimeType_s *Spi_Runtime_GetHwUnit()
{
    return &s_ContextHwRuntime;
}

/* ==========================================================
 * NHÓM API QUẢN LÝ CHANNEL RUNTIME
 * ========================================================== */

Spi_ChannelRuntimeType_s *Spi_Runtime_GetChannel(Spi_ChannelType_e ChannelId)
{
    if (ChannelId >= SPI_MAX_EB_CHANNELS)
    {
        return NULL_PTR;
    }
    return &s_GroupChRuntime[ChannelId];
}

Std_ReturnType Spi_Runtime_SetChannelBuffers(Spi_ChannelType_e ChannelId, uint16 *TxPtr, uint16 *RxPtr, uint16 Length)
{
    if (TxPtr != NULL_PTR && RxPtr != NULL_PTR && Length > MAX_WIDTH_BYTE)
    {
        return E_NOT_OK;
    }
    s_GroupChRuntime[ChannelId].ActiveTxPtr = TxPtr;
    s_GroupChRuntime[ChannelId].ActiveRxPtr = RxPtr;
    s_GroupChRuntime[ChannelId].DefaultLength = Length;
    return E_OK;
}

/* ==========================================================
 * NHÓM API QUẢN LÝ JOB RUNTIME
 * ========================================================== */

Spi_JobRuntimeType_s *Spi_Runtime_GetJob(Spi_JobType_e JobId)
{
    if (JobId >= SPI_JOB_MAX)
    {
        return NULL_PTR;
    }
    return &s_GroupJobRuntime[JobId];
}
boolean Spi_Runtime_IsJobBusy(Spi_JobType_e JobId)
{
    if (JobId >= SPI_JOB_MAX)
    {
        return FALSE;
    }
    return s_GroupJobRuntime[JobId].IsBusy;
}
void Spi_Runtime_SetJobBusy(Spi_JobType_e JobId, boolean IsBusy)
{
    if (JobId < SPI_JOB_MAX)
    {
        s_GroupJobRuntime[JobId].IsBusy = IsBusy;
    }
}
/* ==========================================================
 * NHÓM API QUẢN LÝ SEQUENCE RUNTIME
 * ========================================================== */

Spi_SequenceRuntimeType_s *Spi_Runtime_GetSequence(Spi_SequenceType_e SeqId)
{
    if (SeqId >= SPI_SEQ_MAX)
    {
        return NULL_PTR;
    }
    return &s_GroupSeqRuntime[SeqId];
}

Spi_StatusType_e Spi_Runtime_GetSequenceStatus(Spi_SequenceType_e SeqId)
{
    if (SeqId >= SPI_SEQ_MAX)
    {
        return SPI_UNINIT;
    }
    return s_GroupSeqRuntime[SeqId].Status;
}

void Spi_Runtime_SetSequenceStatus(Spi_SequenceType_e SeqId, Spi_StatusType_e Status)
{
    if (SeqId < SPI_SEQ_MAX)
    {
        s_GroupSeqRuntime[SeqId].Status = Status;
    }
}

/* ==========================================================
 * NHÓM API QUẢN LÝ INTERNAL BUFFER (IB POOL)
 * ========================================================== */

Std_ReturnType Spi_Runtime_ReadBufer_IbChannel(Spi_ChannelType_e Channel, uint16 *Rxbuffer)
{
    if (Rxbuffer == NULL_PTR)
    {
        return E_NOT_OK;
    }

    for (uint8 index = 0; index < s_IbPool[Channel].Length; index++)
    {
        Rxbuffer[index] = s_IbPool[Channel].RxBuffer[index];
    }
    return E_OK;
}
Std_ReturnType Spi_Runtime_Writebuffer_IbChannel(Spi_ChannelType_e Channel, uint16 *txbuffer, uint8 length)
{
    if (length > SPI_IB_MAX_LENGTH)
    {
        return E_NOT_OK;
    }
    for (uint8 index = 0; index < length; index++)
    {
        s_IbPool[Channel].TxBuffer[index] = txbuffer[index];
    }
    s_IbPool[Channel].Length = length;
    return E_OK;
}
Spi_IbChannelType_s *Spi_Runtime_GetIbPool(Spi_ChannelType_e IbChannel)
{
    return &s_IbPool[IbChannel];
}