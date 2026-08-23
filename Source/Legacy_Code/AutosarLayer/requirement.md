
Module cfg/IoHwAb
------------------------
Vị trí trong AUTOSAR: Thuộc ECU Abstraction Layer.

## Chức năng: 
Cung cấp lớp trừu tượng hóa phần cứng, 
giúp tầng ứng dụng hoặc các dịch vụ không cần quan 
tâm đến việc chân vi điều khiển cụ thể nào đang được nối.

IoHwAb_Digital_Cfg.h: Cấu hình trừu tượng cho các chân I/O số (rất liên quan đến việc bật/tắt LED của bạn).

IoHwAb_Adc_Cfg.h: Cấu hình trừu tượng cho bộ chuyển đổi tương tự-số (ADC).

Liên quan tới project của bạn: Rất liên quan. Khi chuẩn hóa sang AUTOSAR, các hàm như Led_Init() hoặc lệnh điều khiển LED sẽ không gọi thẳng phần cứng mà đi qua lớp IoHwAb (IoHwAb_Digital) này.
---------------------------

Module cfg/mcal/ 
---------------------------
Vị trí trong AUTOSAR: Thuộc tầng thấp nhất của BSW (MCAL), nằm sát phần cứng nhất.

Chức năng: Cấu hình trực tiếp các ngoại vi bên trong chip vi điều khiển (MCU):

Dio_Cfg.h: Cấu hình các chân Digital Input/Output (dùng để điều khiển trạng thái chân cắm LED).

Port_Cfg.h: Cấu hình hướng chân (Input/Output), mức điện áp kéo (pull-up/pull-down), tốc độ chân (slew rate).

Liên quan tới project của bạn: Rất liên quan. Hàm Servo_Init() (thường dùng PWM/Dio) và Led_Init() của bạn khi cấu hình chân phần cứng sẽ phải dựa trên các file cấu hình Port_Cfg.h và Dio_Cfg.h này.
---------------------------

cfg/platform/board/ (Board Support / Initialization Configuration)
Vị trí ngoài/bổ trợ cho AUTOSAR: Thường là phần mở rộng cấp board mạch (Board Support Package - BSP) dùng chung trước hoặc song song với quá trình khởi động AUTOSAR.


cfg/platform/board/ (Board Support / Initialization Configuration)
--------------------
Chức năng:

Board_Clock.h: Cấu hình xung nhịp hệ thống (PLL, Flash latency, tần số bus).

Board_PinMap.h: Định nghĩa ánh xạ chân phần cứng vật lý trên bo mạch thực tế.


Liên quan tới project của bạn: Có liên quan ở bước khởi động. Trước khi Servo_Init() hay Led_Init() chạy được, hệ thống bắt buộc phải cấu hình clock và pin map thông qua các file thuộc thư mục board này để cấp nguồn và định tuyến tín hiệu ra chân chip.
---------------------