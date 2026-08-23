// LÝ THUYẾT - GỢI Ý CODE (app/main.c hoặc bsw/services/EcuM.c)
#include "Board_PinMap.h"
#include "Adc.h"
#include "Adc_Cfg.h" // Chứa biến AdcConfig_Default

// Giả lập hàm EcuM_Init
void EcuM_Init(void)
{
    // --- Phase 0: Board/Clock Setup ---
    Board_Init_ADC_Pins_Clock();

    // --- Phase 1: MCAL Initialization ---
    // Truyền biến cấu hình tĩnh (nằm ở tầng cfg) vào hàm khởi tạo (nằm ở tầng bsw)
    Adc_Init(&AdcConfig_Default);

    // Tiếp tục khởi tạo các module khác (Port, Dio, Spi...)
}

int main(void)
{
    // 1. Gọi hệ thống khởi tạo theo lớp
    EcuM_Init();

    // 2. Chạy vòng lặp điều khiển chính (Super-loop)
    while (1)
    {
        // App Logic: Đọc ADC, xử lý cảm biến...
    }
    return 0;
}