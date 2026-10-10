/**
 * @file Spi_Runtime.h
 * @brief Kiểu dữ liệu và API quản lý trạng thái runtime của SPI Driver.
 * @details
 * 1. Lưu trạng thái Hardware Unit, Channel, Job và Sequence khi driver chạy.
 * 2. Quản lý vùng IB và các con trỏ buffer runtime cho từng Channel.
 * 3. Đây là header nội bộ của SPI MCAL, không phải API ứng dụng.
 */

#ifndef SPI_RUNTIME_H
#define SPI_RUNTIME_H

#include "Spi_Cfg.h"

/** @brief Giới hạn độ dài dữ liệu khai báo cho một buffer SPI. */
#define MAX_WIDTH_BYTE 32

/** @brief Số phần tử tối đa của một giao dịch dùng EB. */
#define SPI_EB_MAX_LENGTH MAX_WIDTH_BYTE
/** @brief Số phần tử tối đa của một giao dịch dùng IB. */
#define SPI_IB_MAX_LENGTH MAX_WIDTH_BYTE

/** @brief Số slot IB tối đa; một slot ứng với một SPI Channel. */
#define SPI_MAX_IB_CHANNELS SPI_CH_MAX
/** @brief Số slot EB tối đa; một slot ứng với một SPI Channel. */
#define SPI_MAX_EB_CHANNELS SPI_CH_MAX

/* ==========================================================
 * CÁC CẤU TRÚC DỮ LIỆU RUNTIME
 * ========================================================== */

/**
 * @brief Trạng thái runtime của SPI Hardware Unit hoặc Sequence.
 * @details SPI_UNINIT: chưa khởi tạo; SPI_IDLE: sẵn sàng; SPI_BUSY: đang xử lý.
 */
typedef enum
{
    SPI_UNINIT = 0U,
    SPI_IDLE,
    SPI_BUSY,
} Spi_StatusType_e;

/**
 * @brief Trạng thái runtime của một SPI Hardware Unit.
 * @details Status biểu thị trạng thái tổng thể; TxBusy và RxBusy biểu thị
 *          riêng việc sử dụng từng chiều truyền/nhận.
 */
typedef struct
{
    Spi_StatusType_e Status;
    boolean TxBusy;
    boolean RxBusy;
} Spi_RuntimeType_s;

/**
 * @brief Trạng thái buffer và dữ liệu đang dùng của một Channel.
 * @details Channel là đơn vị dữ liệu logic được Job tham chiếu.
 */
typedef struct
{
    Spi_BufferType BufferType; /* Loại buffer được cấu hình: IB hoặc EB. */
    uint16 *ActiveTxPtr;       /* Địa chỉ buffer Tx đang được sử dụng. */
    uint16 *ActiveRxPtr;       /* Địa chỉ buffer Rx đang được sử dụng. */
    uint16 DefaultLength;      /* Độ dài mặc định của Channel, tính theo phần tử. */
} Spi_ChannelRuntimeType_s;

/**
 * @brief Tiến độ runtime của một Job.
 * @details ChannelIndex xác định Channel hiện tại trong danh sách Job;
 *          TotalBytesInCurrentChannel lưu độ dài Channel hiện tại.
 */
typedef struct
{
    Spi_JobType_e CurrentActiveJobId;  /* ID Job hiện được runtime theo dõi. */
    uint8 ChannelIndex;                /* Vị trí Channel hiện tại trong Job. */
    uint16 TotalBytesInCurrentChannel; /* Số phần tử dữ liệu của Channel hiện tại. */
    boolean IsBusy;                    /* TRUE khi Job đang được thực thi. */
} Spi_JobRuntimeType_s;

/**
 * @brief Tiến độ và trạng thái runtime của một Sequence.
 */
typedef struct
{
    Spi_SequenceType_e CurrentSequenceId; /* ID Sequence đang được theo dõi. */
    uint8 CurrentJobIndex;                /* Vị trí Job hiện tại trong Sequence. */
    Spi_TransferModeType Mode;            /* Chế độ polling hoặc interrupt. */
    Spi_StatusType_e Status;              /* Trạng thái runtime của Sequence. */
    Dio_ChannelType ActiveCsPin;          /* CS đang được giữ cho giao dịch. */
} Spi_SequenceRuntimeType_s;

/**
 * @brief Vùng buffer nội bộ Tx/Rx và độ dài của một Channel dùng IB.
 */
typedef struct
{
    uint16 TxBuffer[SPI_IB_MAX_LENGTH];
    uint16 RxBuffer[SPI_IB_MAX_LENGTH];
    uint8 Length; /* Số phần tử hợp lệ trong cả buffer Tx/Rx. */
} Spi_IbChannelType_s;

/**
 * @brief Ngữ cảnh mà tầng hardware/ISR dùng để tiếp tục một Job bất đồng bộ.
 * @details Ngữ cảnh chứa cấu hình Job, ID Job/Sequence, hướng truyền, kích
 *          thước dữ liệu và chỉ số phần tử hiện tại.
 * @note Triển khai hiện tại lưu một ngữ cảnh dùng chung; không hỗ trợ nhiều
 *       giao dịch interrupt đồng thời trên các Hardware Unit khác nhau.
 */
typedef struct
{
    const Spi_JobConfigType_s *JobCfg; /* Cấu hình Job đang được phục vụ. */
    Spi_SequenceType_e ActiveSeqId;    /* Sequence chứa Job hiện tại. */
    Spi_JobType_e ActiveJobId;         /* ID Job hiện tại. */
    Spi_DirectionType direction;       /* Hướng truyền/nhận của Device. */
    Spi_DataSizeType datasize;         /* Độ rộng dữ liệu 8-bit hoặc 16-bit. */
    uint16 ByteIndex;                  /* Chỉ số phần tử đang xử lý trong Channel. */
} Spi_HwUnitRuntimeType_s;

/* ==========================================================
 * NHÓM API QUẢN LÝ HARDWARE GROUP RUNTIME
 * ========================================================== */
/**
 * @brief Khởi tạo trạng thái runtime của các đối tượng SPI đã cấu hình.
 * @details Đặt trạng thái phần cứng, Sequence và Job về giá trị ban đầu, đồng
 *          thời thiết lập loại buffer và độ dài mặc định của các Channel.
 */
void Spi_Runtime_Init();

/**
 * @brief Đọc trạng thái runtime của một Hardware Unit.
 * @param GroupId ID logic của Hardware Unit.
 * @return Trạng thái hiện tại; SPI_UNINIT nếu ID nằm ngoài giới hạn.
 */
Spi_StatusType_e Spi_Runtime_GetHwGroupStatus(Spi_HwUnitType_e GroupId);

/**
 * @brief Cập nhật trạng thái runtime của một Hardware Unit.
 * @param GroupId ID logic của Hardware Unit.
 * @param Status Trạng thái cần gán.
 * @details Không thay đổi trạng thái nếu GroupId nằm ngoài giới hạn.
 */
void Spi_Runtime_SetHwGroupStatus(Spi_HwUnitType_e GroupId, Spi_StatusType_e Status);

/**
 * @brief Đọc trạng thái bận riêng của bus Tx và Rx.
 * @param GroupId ID logic của Hardware Unit.
 * @param TxBusy Con trỏ nhận trạng thái Tx; có thể NULL_PTR nếu không cần đọc.
 * @param RxBusy Con trỏ nhận trạng thái Rx; có thể NULL_PTR nếu không cần đọc.
 */
void Spi_Runtime_GetBusBusy(Spi_HwUnitType_e GroupId, boolean *TxBusy, boolean *RxBusy);

/**
 * @brief Gán trạng thái bận/rảnh riêng cho bus Tx và Rx.
 * @param GroupId ID logic của Hardware Unit.
 * @param TxBusy Trạng thái bận của Tx.
 * @param RxBusy Trạng thái bận của Rx.
 */
void Spi_Runtime_SetBusBusy(Spi_HwUnitType_e GroupId, boolean TxBusy, boolean RxBusy);

/**
 * @brief Lấy ngữ cảnh runtime dùng bởi tầng xử lý SPI interrupt.
 * @return Con trỏ tới context dùng chung; hiện tại không trả về context riêng
 *         theo Hardware Unit.
 */
Spi_HwUnitRuntimeType_s *Spi_Runtime_GetHwUnit();

/* ==========================================================
 * NHÓM API QUẢN LÝ CHANNEL RUNTIME
 * ========================================================== */

/**
 * @brief Lấy trạng thái runtime của một Channel.
 * @param ChannelId ID logic của Channel.
 * @return Con trỏ runtime của Channel hoặc NULL_PTR nếu ID ngoài giới hạn.
 */
Spi_ChannelRuntimeType_s *Spi_Runtime_GetChannel(Spi_ChannelType_e ChannelId);

/**
 * @brief Gán buffer Tx/Rx và độ dài runtime cho Channel dùng EB.
 * @param ChannelId ID logic của Channel.
 * @param TxPtr Địa chỉ buffer Tx.
 * @param RxPtr Địa chỉ buffer Rx.
 * @param Length Số phần tử dữ liệu cần truyền/nhận.
 * @return E_OK nếu cập nhật được; E_NOT_OK nếu cặp buffer không hợp lệ hoặc
 *         độ dài vượt giới hạn.
 */
Std_ReturnType Spi_Runtime_SetChannelBuffers(Spi_ChannelType_e ChannelId, uint16 *TxPtr, uint16 *RxPtr, uint16 Length);

/* ==========================================================
 * NHÓM API QUẢN LÝ JOB RUNTIME
 * ========================================================== */

/**
 * @brief Lấy trạng thái runtime của một Job.
 * @param JobId ID logic của Job.
 * @return Con trỏ runtime của Job hoặc NULL_PTR nếu ID ngoài giới hạn.
 */
Spi_JobRuntimeType_s *Spi_Runtime_GetJob(Spi_JobType_e JobId);

/**
 * @brief Kiểm tra Job có đang bận hay không.
 * @param JobId ID logic của Job.
 * @return TRUE nếu Job hợp lệ và đang bận; FALSE nếu rảnh hoặc ID không hợp lệ.
 */
boolean Spi_Runtime_IsJobBusy(Spi_JobType_e JobId);

/**
 * @brief Cập nhật cờ bận runtime của Job.
 * @param JobId ID logic của Job.
 * @param IsBusy Trạng thái bận cần gán.
 */
void Spi_Runtime_SetJobBusy(Spi_JobType_e JobId, boolean IsBusy);

/* ==========================================================
 * NHÓM API QUẢN LÝ SEQUENCE RUNTIME
 * ========================================================== */

/**
 * @brief Lấy trạng thái runtime của một Sequence.
 * @param SeqId ID logic của Sequence.
 * @return Con trỏ runtime của Sequence hoặc NULL_PTR nếu ID ngoài giới hạn.
 */
Spi_SequenceRuntimeType_s *Spi_Runtime_GetSequence(Spi_SequenceType_e SeqId);

/**
 * @brief Đọc trạng thái hiện tại của Sequence.
 * @param SeqId ID logic của Sequence.
 * @return Trạng thái Sequence; SPI_UNINIT nếu ID ngoài giới hạn.
 */
Spi_StatusType_e Spi_Runtime_GetSequenceStatus(Spi_SequenceType_e SeqId);

/**
 * @brief Cập nhật trạng thái runtime của Sequence.
 * @param SeqId ID logic của Sequence.
 * @param Status Trạng thái cần gán.
 */
void Spi_Runtime_SetSequenceStatus(Spi_SequenceType_e SeqId, Spi_StatusType_e Status);

/* ==========================================================
 * NHÓM API QUẢN LÝ INTERNAL BUFFER (IB POOL)
 * ========================================================== */

/**
 * @brief Sao chép dữ liệu nhận từ IB của Channel vào buffer đích.
 * @param Channel ID logic của Channel.
 * @param Rxbuffer Buffer đích phải đủ chỗ chứa toàn bộ dữ liệu của IB.
 * @return E_OK nếu sao chép thành công; E_NOT_OK nếu Rxbuffer là NULL_PTR.
 */
Std_ReturnType Spi_Runtime_ReadBufer_IbChannel(Spi_ChannelType_e Channel, uint16 *Rxbuffer);

/**
 * @brief Sao chép dữ liệu Tx vào IB của Channel và cập nhật độ dài.
 * @param Channel ID logic của Channel.
 * @param txbuffer Buffer dữ liệu nguồn hợp lệ khi length khác 0.
 * @param length Số phần tử cần sao chép.
 * @return E_OK nếu sao chép thành công; E_NOT_OK nếu độ dài vượt giới hạn.
 */
Std_ReturnType Spi_Runtime_Writebuffer_IbChannel(Spi_ChannelType_e Channel, uint16 *txbuffer, uint8 length);

/**
 * @brief Lấy vùng IB gắn với một Channel.
 * @param IbChannel ID logic của Channel; phải nằm trong giới hạn.
 * @return Con trỏ tới vùng IB của Channel.
 */
Spi_IbChannelType_s *Spi_Runtime_GetIbPool(Spi_ChannelType_e IbChannel);

#endif /* SPI_RUNTIME_H */