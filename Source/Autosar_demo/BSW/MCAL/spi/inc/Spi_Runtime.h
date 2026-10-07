/**
 * @file Spi_Runtime.h
 * @brief Quản lý trạng thái thời gian thực (Runtime Status) của SPI Driver.
 * @details Đóng gói mảng trạng thái phần cứng, cung cấp các hàm an toàn
 *          để đọc/ghi trạng thái, tránh xung đột và sửa đổi dữ liệu tùy tiện.
 */

#ifndef SPI_RUNTIME_H
#define SPI_RUNTIME_H

#include "Spi_Types.h"

#define MAX_WIDTH_BYTE 32 // đối với mảng 16-bit thì số byte thực tế là 64

/*Tổng số byte tối đa được phép truyền/nhận trong 1 data transaction*/
#define SPI_EB_MAX_LENGTH MAX_WIDTH_BYTE
#define SPI_IB_MAX_LENGTH MAX_WIDTH_BYTE

/*số kênh Spi tối đa được cấu hình cho 2 cơ chế quản lý dữ liệu EB và IB*/
#define SPI_MAX_IB_CHANNELS SPI_MAX_CHANNEL
#define SPI_MAX_EB_CHANNELS SPI_MAX_CHANNEL

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
} Spi_StatusType;

/**
 * @brief Cấu trúc lưu trữ trạng thái hoạt động của từng SPI Group (HW Unit).
 */
typedef struct
{
    Spi_StatusType Status;
    boolean TxBusy;
    boolean RxBusy;
} Spi_RuntimeType;

typedef enum
{
    SPI_BUFFER_TYPE_IB = 0, // Internal Buffer
    SPI_BUFFER_TYPE_EB      // External Buffer
} Spi_BufferType;

/**
 * @brief Cấu hình dữ liệu của một SPI Channel.
 * @details Mỗi Channel quản lý buffer truyền, buffer nhận và số byte cần
 *          truyền. Channel là đơn vị dữ liệu logic được một Job tham chiếu.
 * @note Chỉ cần biết đang trỏ vào buffer nào và truyền bao nhiêu byte:
 */
typedef struct Spi_ChannelRuntime
{
    Spi_BufferType BufferType;
    uint16 *TxBufferPtr;
    uint16 *RxBufferPtr;
    uint8 DefaultLength;
} Spi_ChannelRuntimeType;

/**
 * @brief Cấu trúc quản lý Internal Buffer (IB) do Driver tự cấp phát vùng nhớ.
 */
typedef struct
{
    uint16 TxBuffer[SPI_IB_MAX_LENGTH];
    uint16 RxBuffer[SPI_IB_MAX_LENGTH];
    uint8 Length;
} Spi_IbChannelType;

/* ==========================================================
 * KHAI BÁO BIẾN TOÀN CỤC RUNTIME (EXTERN)
 * ========================================================== */
extern Spi_ChannelRuntimeType Spi_ChannelsRuntime[SPI_MAX_EB_CHANNELS];
extern Spi_IbChannelType Spi_IbPool[SPI_MAX_IB_CHANNELS];

/**
 * @brief Khởi tạo hoặc reset toàn bộ trạng thái runtime của các SPI Group/HwUnit.
 */
void Spi_Runtime_Init(void);

/**
 * @brief Lấy trạng thái hiện tại của một SPI Group/HwUnit.
 * @param GroupId ID của SPI Group cần kiểm tra.
 * @return Spi_StatusType Trạng thái hiện tại (UNINIT, IDLE, BUSY,...)
 */
Spi_StatusType Spi_Runtime_GetStatus(Spi_GroupId_Type GroupId);

/**
 * @brief Cập nhật trạng thái cho một SPI Group/HwUnit.
 * @param GroupId ID của SPI Group.
 * @param Status Trạng thái mới cần gán.
 */
void Spi_Runtime_SetStatus(Spi_GroupId_Type GroupId, Spi_StatusType Status);

/**
 * @brief Kiểm tra xem chiều truyền (Tx) hoặc nhận (Rx) có đang bận hay không.
 * @param GroupId ID của SPI Group.
 * @param TxBusy Con trỏ nhận trạng thái Tx bận (có thể là NULL nếu không muốn lấy).
 * @param RxBusy Con trỏ nhận trạng thái Rx bận (có thể là NULL nếu không muốn lấy).
 */
void Spi_Runtime_GetBusBusy(Spi_GroupId_Type GroupId, boolean *TxBusy, boolean *RxBusy);

/**
 * @brief Thiết lập trạng thái bận/rảnh cho chiều truyền và nhận.
 * @param GroupId ID của SPI Group.
 * @param TxBusy Trạng thái Tx bận (TRUE/FALSE).
 * @param RxBusy Trạng thái Rx bận (TRUE/FALSE).
 */
void Spi_Runtime_SetBusBusy(Spi_GroupId_Type GroupId, boolean TxBusy, boolean RxBusy);

#endif /* SPI_RUNTIME_H */