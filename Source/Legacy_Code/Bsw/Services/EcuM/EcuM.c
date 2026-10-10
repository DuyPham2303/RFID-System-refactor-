#include "Gpt.h"
#include "./Bsw/IoHwAb/IoHwAb_Port/IoHwAb_Port.h"
#include "./Bsw/IoHwAb/IoHwAb_Spi/IoHwAb_Spi.h"
#include "System/Board_Clock.h"

// Giả lập hàm EcuM_Init
void EcuM_Init(void)
{
    // --- Phase 0: Board/Clock Setup ---
    Board_PeripheralsClock_Init();
    // --- Phase 1: MCAL Initialization ---
    (void)IoHwAb_PortInit();
    Gpt_Init(&g_Gpt_ConfigGroup);
    (void)IoHwAb_SpiInit();
}
