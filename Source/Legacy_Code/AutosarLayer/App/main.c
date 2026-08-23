#include "Callback_Mgr.h"

// vai trò module cfg/IoHwAb
/*
- Khi tầng ứng dụng muốn bật LED, nó sẽ gọi hàm trừu tượng
  kiểu: IoHwAb_WriteDigital(IOHWAB_CH_LED_STATUS, STD_HIGH);

- Tầng IoHwAb sẽ tra cứu file cấu hình này để biết
  IOHWAB_CH_LED_STATUS ứng với kênh Dio nào, từ đó gọi tiếp
  xuống MCAL (Dio_WriteChannel).
*/

// vai trò module cfg/mcal và platform/board
/*
- Board_PinMap / Board_Clock: Chỉ làm nhiệm vụ cấu hình phần cứng cơ sở của con chip STM32 (Chân nào làm chức năng gì, bus nào có điện).

- Tầng MCAL (Port, Dio, Spi): Sẽ sử dụng kết quả cấu hình nền tảng đó và ánh xạ chi tiết hơn theo các file cấu hình _Cfg.h để đóng gói thành các API driver chuẩn (ví dụ: cấu hình tốc độ baudrate của SPI1, chế độ truyền).

- Tầng ứng dụng / Trừu tượng hóa (IoHwAb hoặc App): Lúc này mới định nghĩa: "À, module RC522 của tôi sẽ sử dụng driver SPI1 ở trên, chân Chip Select (CS) sẽ gán vào chân X nào đó".

*/
int main()
{
    Serice_Callback_Init();
    return 0;
}