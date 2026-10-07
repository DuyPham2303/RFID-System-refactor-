/**
 * @file Spi_Runtime.h
 * @brief Quản lý trạng thái thời gian thực (Runtime Status) của SPI Driver.
 * @details Đóng gói mảng trạng thái phần cứng, cung cấp các hàm an toàn
 *          để đọc/ghi trạng thái, tránh xung đột và sửa đổi dữ liệu tùy tiện.
 */

#ifndef SPI_RUNTIME_H
#define SPI_RUNTIME_H

#include "Spi_Cfg.h"

#define MAX_WIDTH_BYTE 32 // đối với mảng 16-bit thì số byte thực tế là 64

/*Tổng số byte tối đa được phép truyền/nhận trong 1 data transaction*/
#define SPI_EB_MAX_LENGTH MAX_WIDTH_BYTE
#define SPI_IB_MAX_LENGTH MAX_WIDTH_BYTE

/*số kênh Spi tối đa được cấu hình cho 2 cơ chế quản lý dữ liệu EB và IB*/
#define SPI_MAX_IB_CHANNELS SPI_CH_MAX
#define SPI_MAX_EB_CHANNELS SPI_CH_MAX

/* ==========================================================
 * CÁC CẤU TRÚC DỮ LIỆU RUNTIME
 * ========================================================== */

/**
 * @brief Trạng thái hiện tại của SPI driver.
 */
typedef enum
{
    SPI_UNINIT = 0U,
    SPI_IDLE,
    SPI_BUSY,
} Spi_StatusType_e;

/**
 * @brief Cấu trúc lưu trữ trạng thái hoạt động của từng SPI Group (HW Unit).
 */
typedef struct
{
    Spi_StatusType_e Status;
    boolean TxBusy;
    boolean RxBusy;
} Spi_RuntimeType_s;

/**
 * @brief Cấu hình dữ liệu của một SPI Channel.
 * @details Mỗi Channel quản lý buffer truyền, buffer nhận và số byte cần
 *          truyền. Channel là đơn vị dữ liệu logic được một Job tham chiếu.
 * @note Chỉ cần biết đang trỏ vào buffer nào và truyền bao nhiêu byte:
 */
typedef struct
{
    Spi_BufferType BufferType; /* Phân biệt loại buffer IB hay EB */
    uint16 *ActiveTxPtr;       /* Con trỏ thực tế chứa dữ liệu truyền đi */
    uint16 *ActiveRxPtr;       /* Con trỏ thực tế chứa dữ liệu nhận về */
    uint16 DefaultLength;      /* Số lượng byte cần truyền/nhận trong channel này */
} Spi_ChannelRuntimeType_s;

/**
 * @brief là "bộ đếm tiến trình" trong quá trình truyền nhận một nhóm các channel. Nó
 *        quản lý xem Job đang đứng ở channel nào và đã truyền được bao nhiêu
 *        byte:
 */
typedef struct
{
    Spi_JobType_e CurrentActiveJobId;  /* ID của Job đang thực thi */
    uint8 ChannelIndex;                /* Đang đứng ở Channel thứ mấy trong danh sách của Job */
    uint16 TotalBytesInCurrentChannel; /* Tổng số byte của Channel hiện tại */
    boolean IsBusy;                    /* Cờ đánh dấu Job này có đang bận không */
} Spi_JobRuntimeType_s;

/**
 * @brief Quản lý tiến trình chạy của chuỗi nghiệp vụ (Sequence):
 */
typedef struct
{
    Spi_SequenceType_e CurrentSequenceId; /* ID của Sequence đang chạy */
    uint8 CurrentJobIndex;                /* Đang thực thi Job thứ mấy trong Sequence */
    Spi_TransferModeType Mode;            /* mode polling / Interrupt*/
    Spi_StatusType_e Status;              /* Trạng thái tổng quan (IDLE, BUSY, COMPLETE) */
    Dio_ChannelType ActiveCsPin;          /* chân Cs của job đang xủ lý*/
} Spi_SequenceRuntimeType_s;

/**
 * @brief Cấu trúc quản lý Internal Buffer (IB) do Driver tự cấp phát vùng nhớ.
 */
typedef struct
{
    uint16 TxBuffer[SPI_IB_MAX_LENGTH];
    uint16 RxBuffer[SPI_IB_MAX_LENGTH];
    uint8 Length;
} Spi_IbChannelType_s;

/**
 * @brief quản lý trạng thái theo từng Khối phần cứng (Hardware Unit Context)
 * @details tại một thời điểm, một bộ điều khiển SPI phần cứng (ví dụ: SPI0 hoặc SPI1)
 *          chỉ có thể phục vụ đúng một Job/Channel độc lập, nên ta sẽ lưu
 *         "dấu vết" này ở một biến toàn cục trong tầng Runtime hoặc Hardware.
 */
typedef struct
{
    const Spi_JobConfigType_s *JobCfg;
    Spi_SequenceType_e ActiveSeqId; // sequence đang chạy trên phần cứng này
    Spi_JobType_e ActiveJobId;      // Job đang chạy trên phần cứng này
    Spi_DirectionType direction;
    Spi_DataSizeType datasize;
    uint16 ByteIndex; /* Đang truyền đến byte thứ mấy của Channel hiện tại */
} Spi_HwUnitRuntimeType_s;

/* ==========================================================
 * NHÓM API QUẢN LÝ HARDWARE GROUP RUNTIME
 * ========================================================== */
/**
 * @brief Khởi tạo hoặc reset toàn bộ trạng thái runtime của các SPI Group/HwUnit.
 */
void Spi_Runtime_Init();

/**
 * @brief Lấy trạng thái hiện tại của một SPI Group/HwUnit.
 * @param GroupId ID của SPI Group cần kiểm tra.
 * @return Spi_StatusType_e Trạng thái hiện tại (UNINIT, IDLE, BUSY,...)
 */
Spi_StatusType_e Spi_Runtime_GetHwGroupStatus(Spi_HwUnitType_e GroupId);

/**
 * @brief Cập nhật trạng thái cho một SPI Group/HwUnit.
 * @param GroupId ID của SPI Group.
 * @param Status Trạng thái mới cần gán.
 */
void Spi_Runtime_SetHwGroupStatus(Spi_HwUnitType_e GroupId, Spi_StatusType_e Status);

/**
 * @brief Kiểm tra xem chiều truyền (Tx) hoặc nhận (Rx) có đang bận hay không.
 * @param GroupId ID của SPI Group.
 * @param TxBusy Con trỏ nhận trạng thái Tx bận (có thể là NULL nếu không muốn lấy).
 * @param RxBusy Con trỏ nhận trạng thái Rx bận (có thể là NULL nếu không muốn lấy).
 */
void Spi_Runtime_GetBusBusy(Spi_HwUnitType_e GroupId, boolean *TxBusy, boolean *RxBusy);

/**
 * @brief Thiết lập trạng thái bận/rảnh cho chiều truyền và nhận.
 * @param GroupId ID của SPI Group.
 * @param TxBusy Trạng thái Tx bận (TRUE/FALSE).
 * @param RxBusy Trạng thái Rx bận (TRUE/FALSE).
 */
void Spi_Runtime_SetBusBusy(Spi_HwUnitType_e GroupId, boolean TxBusy, boolean RxBusy);

/**
 * @brief Lấy con trỏ quản lý runtime của một phần cứng Spi cụ thể
 * @return Spi_HwUnitRuntimeType_s* : địa chỉ trỏ tới 1 hardware duy nhất tại 1 thời điểm
 */
Spi_HwUnitRuntimeType_s *Spi_Runtime_GetHwUnit();

/* ==========================================================
 * NHÓM API QUẢN LÝ CHANNEL RUNTIME
 * ========================================================== */

/**
 * @brief Lấy con trỏ quản lý runtime của một Channel cụ thể.
 * @param ChannelId ID của Channel cần lấy thông tin.
 * @return Spi_ChannelRuntimeType_s* Con trỏ tới cấu trúc runtime của Channel.
 */
Spi_ChannelRuntimeType_s *Spi_Runtime_GetChannel(Spi_ChannelType_e ChannelId);

/**
 * @brief Thiết lập con trỏ truyền/nhận và chiều dài cho Channel ở chế độ Runtime.
 * @param ChannelId ID của Channel.
 * @param TxPtr Con trỏ dữ liệu truyền đi.
 * @param RxPtr Con trỏ dữ liệu nhận về.
 * @param Length Số lượng byte cần truyền/nhận.
 */
Std_ReturnType Spi_Runtime_SetChannelBuffers(Spi_ChannelType_e ChannelId, uint16 *TxPtr, uint16 *RxPtr, uint16 Length);

/* ==========================================================
 * NHÓM API QUẢN LÝ JOB RUNTIME
 * ========================================================== */

/**
 * @brief Lấy con trỏ quản lý runtime của một Job cụ thể.
 * @param JobId ID của Job cần truy xuất.
 * @return Spi_JobRuntimeType_s* Con trỏ tới cấu trúc runtime của Job.
 */
Spi_JobRuntimeType_s *Spi_Runtime_GetJob(Spi_JobType_e JobId);

/**
 * @brief Kiểm tra xem một Job có đang trong trạng thái bận hay không.
 * @param JobId ID của Job.
 * @return boolean TRUE nếu đang bận, FALSE nếu rảnh.
 */
boolean Spi_Runtime_IsJobBusy(Spi_JobType_e JobId);

/**
 * @brief Cập nhật trạng thái bận cho một Job.
 * @param JobId ID của Job.
 * @param IsBusy Trạng thái bận cần gán (TRUE/FALSE).
 */
void Spi_Runtime_SetJobBusy(Spi_JobType_e JobId, boolean IsBusy);

/* ==========================================================
 * NHÓM API QUẢN LÝ SEQUENCE RUNTIME
 * ========================================================== */

/**
 * @brief Lấy con trỏ quản lý runtime của một Sequence cụ thể.
 * @param SeqId ID của Sequence cần truy xuất.
 * @return Spi_SequenceRuntimeType_s* Con trỏ tới cấu trúc runtime của Sequence.
 */
Spi_SequenceRuntimeType_s *Spi_Runtime_GetSequence(Spi_SequenceType_e SeqId);

/**
 * @brief Lấy trạng thái hiện tại của một Sequence.
 * @param SeqId ID của Sequence.
 * @return Spi_StatusType_e Trạng thái hiện tại của Sequence.
 */
Spi_StatusType_e Spi_Runtime_GetSequenceStatus(Spi_SequenceType_e SeqId);

/**
 * @brief Cập nhật trạng thái cho một Sequence.
 * @param SeqId ID của Sequence.
 * @param Status Trạng thái mới cần gán.
 */
void Spi_Runtime_SetSequenceStatus(Spi_SequenceType_e SeqId, Spi_StatusType_e Status);

/* ==========================================================
 * NHÓM API QUẢN LÝ INTERNAL BUFFER (IB POOL)
 * ========================================================== */

/**
 * @brief Lấy con trỏ tới vùng nhớ Internal Buffer của Channel tương ứng.
 * @param IbIndex Chỉ số định danh của Internal Buffer Channel.
 * @return Spi_IbChannelType_s* Con trỏ tới vùng nhớ buffer nội bộ.
 */
Std_ReturnType Spi_Runtime_ReadBufer_IbChannel(Spi_ChannelType_e Channel, uint16 *Rxbuffer);

/**
 * @brief copy từng phần tử của App buffer vào vùng nhớ do driver quản lý
 * @param txdata
 */
Std_ReturnType Spi_Runtime_Writebuffer_IbChannel(Spi_ChannelType_e Channel, uint16 *txbuffer, uint8 length);

/**
 * @brief Lấy con trỏ quản lý runtime của một IbPool cụ thể.
 * @param SeqId ID của IbPool cần truy xuất.
 * @return Spi_IbChannelType_s* Con trỏ tới cấu trúc IbPool của Channel.
 */
Spi_IbChannelType_s *Spi_Runtime_GetIbPool(Spi_ChannelType_e IbChannel);

#endif /* SPI_RUNTIME_H */