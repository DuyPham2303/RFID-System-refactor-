#include <stdio.h>
#include <stdint.h>

typedef uint8_t PortType;

#define PORT_PIN_A (PortType)(0x00)
#define PORT_PIN_B (PortType)(0x01)
#define PORT_PIN_C (PortType)(0x02)
#define PORT_PIN_D (PortType)(0x03)

/*
 * Định nghĩa cấu trúc để lưu trữ thông tin về một chân GPIO
 */
typedef union
{
    struct
    {
        uint8_t pin : 4;  // lưu tối đa 16 chân (0..15)
        uint8_t port : 2; // lưu tối đa 4 cổng (A..D)
        uint8_t rsv : 2;  // dành cho các bit dự phòng, không sử dụng
    } bits;

    uint8_t raw; // lưu trữ giá trị thô của chân GPIO dưới dạng một byte
} PortPinKey_t;

/*
 * Tạo một khóa PortPinKey_t từ cổng và chân GPIO
 * @param port: Cổng GPIO (PortType)
 * @param pin: Chân GPIO (0..15)
 * @return: Khóa PortPinKey_t tương ứng với cổng và chân GPIO
 */
static inline PortPinKey_t PortPinKey_Create(PortType port, uint8_t pin)
{
    // Khởi tạo một khóa PortPinKey_t với giá trị mặc định là 0
    PortPinKey_t key = {0};

    // Kiểm tra và giới hạn giá trị cổng và chân GPIO
    if (port > PORT_PIN_D)
        port = PORT_PIN_A;
    // Giới hạn giá trị chân GPIO từ 0 đến 15
    if (pin > 15)
        pin = 15;

    key.bits.HwPortId = port;
    key.bits.pin = pin;

    return key;
}

static inline void PortPinKey_ToName(PortPinKey_t key, char *buf, size_t len)
{
    // Mảng ký tự để ánh xạ số cổng sang chữ cái tương ứng
    const char portLetter[] = {'A', 'B', 'C', 'D'};

    if (buf == NULL || len == 0)
        return;

    if (key.bits.HwPortId > 3)
    {
        snprintf(buf, len, "P?%u", key.bits.pin);
        return;
    }
    /* Ánh xạ cổng sang ký tự, cùng với chân GPIO */
    snprintf(buf, len, "P%c%u", portLetter[key.bits.HwPortId], key.bits.pin);
}

int main()
{
    uint8_t pinmap[4][6] = {
        {0, 1, 2, 3, 4, 5},     // Port A
        {6, 7, 8, 9, 10, 11},   // Port B
        {12, 12, 12, 7, 14, 5}, // Port C
        {15, 13, 2, 13, 4, 8}   // Port D
    };
    PortPinKey_t keymap[4][6];
    char name[8];
    // ánh xạ cấu hình lưu trữ bảng đồ chứa pin & port
    for (uint8_t i = 0; i < sizeof(pinmap) / sizeof(pinmap[0]); i++)
    {
        printf("port index : %d\n", i);
        for (uint8_t k = 0; k < sizeof(pinmap[0]) / sizeof(pinmap[0][0]); k++)
        {
            keymap[i][k].bits.HwPortId = i; // ánh xạ port index (mặc định từ 0....4)
            keymap[i][k].bits.pin = pinmap[i][k];
            PortPinKey_ToName(keymap[i][k], name, sizeof(name));
            printf("%s\t", name);
        }
        printf("\n");
    }
    return 0;
}