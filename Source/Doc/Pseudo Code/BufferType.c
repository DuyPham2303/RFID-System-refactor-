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
 * @brief Cấu trúc quản lý Internal Buffer (IB) do Driver tự cấp phát vùng nhớ.
 */
typedef struct
{
    uint16 TxBuffer[SPI_IB_MAX_LENGTH];
    uint16 RxBuffer[SPI_IB_MAX_LENGTH];
    uint8 Length;
} Spi_IbChannelType_s;
