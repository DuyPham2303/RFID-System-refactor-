#include "Dio/inc/Dio.h"
#include "Port/inc/Port.h"
#include "Gpt/inc/Gpt.h"
#include "spi/inc/Spi.h"
#include "Os/Os.h"
#include "System/Board_Clock.h"
#include "Testcase_Types.h"

/**
 * @brief Macro chọn khối lệnh chứa Các API testcase của ngoại vi lựa chọn để chạy
 */
// #define TESTCASE_DIO 0
#define TESTCASE_GPT 1
// #define TESTCASE_PWM 2
// #define TESTCASE_SPI 3
// #define TESTCASE_USART 4
// #define TESTCASE_I2C 5

typedef struct TC_Map
{
    const char *TC_name;
    TCFuncPtr RunFunc;
} TC_MapType;

static TC_IdType_e ty_s;

/*Prototype*/
#ifdef TESTCASE_SPI
static TC_status_e App_Testcase_Initialization(void);
static TC_status_e App_Testcase_SyncTransmit_EB(void);
static TC_status_e App_Testcase_AsyncTransmit_IB(void);

static const TC_MapType TC_SPI_Table[] =
    {
        {"[SPI] TC-01", App_Testcase_Initialization},
        {"[SPI] TC-02", App_Testcase_SyncTransmit_EB},
        {"[SPI] TC-03", App_Testcase_AsyncTransmit_IB},
        {NULL_PTR, NULL_PTR}};
#endif

/*Prototype*/
#ifdef TESTCASE_GPT
static TC_status_e App_Testcase_BlinkLed_IT();
static const TC_MapType TC_GPT_Table[] =
    {
        {"[GPT] TC-01", App_Testcase_BlinkLed_IT},
        {NULL_PTR, NULL_PTR}};

/**
 * @brief bảng đồ ánh xạ key name mô tả testcase
 * @details liệt kê group key name mô tả từng loại test case với Peripheral tương ứng
 * @note Có thể bổ sung linh hoạt nhóm key name tương ứng với Peripheral càn test
 */
static const TC_MapType *TC_MappedTable[TC_CNT_ID] =
    {
        //[TC_SPI_ID] = TC_SPI_Table,
        [TC_GPT_ID] = TC_GPT_Table};
#endif

uint8 TestCaseTotalCount(TC_IdType_e ty)
{
    /* kiểm tra có group testcase nào có sẵn chưa */
    if (TC_MappedTable[ty] == NULL_PTR)
    {
        return 0;
    }
    uint8 cnt = 0;

    /* đếm số lượng key name tương ứng với group testcase của periph cẩn test */

    while (TC_MappedTable[ty][cnt].RunFunc != NULL_PTR)
    {
        cnt++;
    }
    ty_s = ty;
    return cnt;
}

Std_ReturnType TC_TableInitParam(TestCaseMgr_s *TC_Mgr, uint8 size)
{
    if (TC_Mgr == NULL_PTR)
    {
        return E_NOT_OK;
    }
    /*duyệt qua danh sách và gán key name tương ứng và trạng thái xử lý mặc định*/
    const TC_MapType *table = TC_MappedTable[ty_s];

    for (uint8 index = 0; index < size; index++)
    {
        TC_Mgr[index].key = table[index].TC_name;
        TC_Mgr[index].RunFunc = table[index].RunFunc;
        TC_Mgr[index].st = NOT_TEST;
    }
    return E_OK;
}

#ifdef TESTCASE_GPT
/*IoHwAb
 */
static void IoHwAb_Led_SetToggle(void)
{
    Dio_FlipChannel(DIO_CHANNEL_C13);
}
/* Rte */
static void Rte_Call_Led_Flip(void)
{
    IoHwAb_Led_SetToggle();
}
/* Application */
static void BlinkLed_Runnable(void)
{
    Rte_Call_Led_Flip();
}

static TC_status_e App_Testcase_BlinkLed_IT()
{
    Board_PeripheralsClock_Init();
    Port_Init(&Port_Config);
    Gpt_Init(&Gpt_Config);
    /*Vòng lặp chờ đến khi cờ xủ lý ngắt được cập nhật*/

    /*kiểm tra trạng thái cờ và xử lý tác vụ mong muốn*/
    while (1)
    {

        // Dio_FlipChannel(DIO_CHANNEL_C13);
        // Os_DelayMs(500);

        if (g_Flagupdate_Periodic == TRUE)
        {
            BlinkLed_Runnable();
            // Dio_FlipChannel(DIO_CHANNEL_C13);
            /*reset cờ cho lần ngắt kế tiếp*/
            g_Flagupdate_Periodic = FALSE;
        }
    }
    return PASS;
}

#endif

#ifdef TESTCASE_SPI
static TC_status_e App_Testcase_Initialization()
{
    /**
     *@brief Testcase 1: Khởi tạo Driver và Kiểm tra Trạng thái (Spi_Init, Spi_GetStatus)
     *@details Mục đích: Đảm bảo phần cứng và driver đã sẵn sàng trước khi thực hiện bất kỳ
     *         giao dịch SPI nào.
     *         Kịch bản thực tế: Khi vi điều khiển vừa bật nguồn (Power-on),
     *         tầng Application yêu cầu khởi tạo toàn bộ cấu hình SPI tĩnh và kiểm tra trạng thái driver.
     */
    Std_ReturnType initStatus;
    Spi_StatusType driverStatus;

    // 1. Khởi tạo driver bằng cách truyền con trỏ bảng cấu hình tĩnh (thường được định nghĩa ở Spi_Cfg.c)
    // Giả sử ta có biến cấu hình toàn cục Spi_ConfigSet
    initStatus = Spi_Init(Spi_ConfigSet);

    if (initStatus == E_OK)
    {
        // 2. Kiểm tra xem driver đã thực sự ở trạng thái sẵn sàng (IDLE) chưa
        driverStatus = Spi_GetStatus();

        if (driverStatus == SPI_IDLE)
        {
            // Driver đã sẵn sàng nhận các Job/Sequence tiếp theo
        }
    }
    else
    {
        // Xử lý lỗi khởi tạo phần cứng
        return FAIL;
    }
    return PASS;
}
static TC_status_e App_Testcase_SyncTransmit_EB(void)
{
    /**
     * @brief Truyền dữ liệu Đồng bộ dùng External Buffer (Spi_SetupEB, Spi_SyncTransmit)
     * @details Mục đích: Minh họa cách truyền dữ liệu bằng cơ chế External Buffer (EB).
     *          Kịch bản thực tế: Ứng dụng muốn đọc dữ liệu từ một cảm biến nhiệt độ qua
     *          SPI (ví dụ gửi một lệnh đọc và nhận về 2 byte dữ liệu). Ứng dụng tự cấp
     *          phát mảng bộ nhớ (RAM buffer) và trỏ vào driver.
     */
    Std_ReturnType setupResult;
    Std_ReturnType txResult;
// Khai báo sẵn các buffer truyền/nhận ở tầng Application
#define SENSOR_DATA_LEN 3U
    static uint8 TxData[SENSOR_DATA_LEN] = {0x01, 0x00, 0x00}; // Mã lệnh đọc cảm biến
    static uint8 RxData[SENSOR_DATA_LEN] = {0x00, 0x00, 0x00}; // Mảng chứa dữ liệu trả về

    // Bước 1: Gắn (Setup) buffer của Application vào Channel tương ứng (Ví dụ: SPI_CHANNEL_SENSOR)
    // Driver sẽ lưu thông tin con trỏ này để dùng khi thực hiện Sequence
    setupResult = Spi_SetupEB(SPI_CHANNEL_1, (uint16 *)TxData, (uint16 *)RxData, SENSOR_DATA_LEN);

    if (setupResult == E_OK)
    {
        // Bước 2: Thực hiện truyền đồng bộ một Sequence chứa Channel/Job đó.
        // Hàm này sẽ block (chờ) cho đến khi truyền xong toàn bộ byte thì mới trả về.
        txResult = Spi_SyncTransmit(SPI_SEQ_UPDATE_LED_STATUS);

        if (txResult == E_OK)
        {
            // Truyền thành công! Dữ liệu trả về từ cảm biến đã nằm sẵn trong mảng RxData
            // Application có thể đọc RxData[1] và RxData[2] để xử lý.
        }
        else
        {
            // Giao dịch SPI bị lỗi phần cứng hoặc timeout
            return FAIL;
        }
    }
    return PASS;
}
static TC_status_e App_Testcase_AsyncTransmit_IB()
{
    /**
     * @brief Testcase 3: Truyền dữ liệu dùng Internal Buffer (Spi_WriteIB, Spi_ReadIB, Spi_AsyncTransmit)
     * @details Mục đích: Minh họa cơ chế Internal Buffer (IB) và truyền bất đồng bộ (AsyncTransmit).
     *          Kịch bản thực tế: Ứng dụng muốn gửi một chuỗi lệnh điều khiển hiển thị ra màn hình
     *          LCD qua SPI. Vì Application không muốn quản lý vòng đời của buffer, nó dùng hàm WriteIB
     *          để copy dữ liệu vào vùng nhớ nội bộ của driver, sau đó gọi truyền bất đồng bộ để rảnh tay
     *          làm việc khác.
     */
    uint8 lcdCommand[4] = {0xFE, 0x51, 0x0A, 0xFF}; // Dữ liệu cần gửi ra LCD
    Std_ReturnType writeResult;
    Std_ReturnType asyncResult;

    // Bước 1: Ghi dữ liệu từ App vào buffer nội bộ do Driver quản lý
    writeResult = Spi_WriteIB(SPI_CHANNEL_0, lcdCommand);

    if (writeResult == E_OK)
    {
        // Bước 2: Kích hoạt truyền bất đồng bộ (Driver sẽ tự lo việc phát tín hiệu SPI,
        // có thể dùng ngắt hoặc DMA ngầm, và trả về ngay lập tức không chờ lâu)
        asyncResult = Spi_AsyncTransmit(SPI_SEQUENCE_UPDATE_LCD);

        if (asyncResult == E_OK)
        {
            // Giao dịch đang được xử lý ở nền (Background).
            // Application có thể tiếp tục làm các tác vụ khác.
            // Khi hoàn tất, Driver sẽ tự gọi hàm Callback thông báo cho App.
            return PASS;
        }
    }

    return FAIL;
}
#endif
