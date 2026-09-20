#include "Spi_map.h"
uint16 Spi_Map_GetMode(Spi_ModeType Mode)
{
    switch (Mode)
    {
    case SPI_MR_MODE_MASTER:
        return SPI_Mode_Master;
        break;
    case SPI_MR_MODE_SLAVE:
        return SPI_Mode_Slave;
        break;
    default:
        return SPI_Mode_Master;
        break;
    }
}

uint16 Spi_Map_GetNss(Spi_NssType Nss)
{
    switch (Nss)
    {
    case SPI_MR_NSS_HARD:
        return SPI_NSS_Hard;
        break;
    case SPI_MR_NSS_SOFT:
        return SPI_NSS_Soft;
        break;
    default:
        return SPI_NSS_Soft;
        break;
    }
}

uint16 Spi_Map_GetDir(Spi_DirectionType Dir)
{
    switch (Dir)
    {
    case SPI_MR_2LINES_FD:
        return SPI_Direction_2Lines_FullDuplex;
        break;
    case SPI_MR_2LINES_RX_ONLY:
        return SPI_Direction_2Lines_RxOnly;
        break;
    case SPI_MR_1LINE_RX:
        return SPI_Direction_1Line_Rx;
        break;
    case SPI_MR_1LINE_TX:
        return SPI_Direction_1Line_Tx;
        break;
    default:
        return SPI_Direction_2Lines_FullDuplex;
        break;
    }
}

uint16 Spi_Map_GetBaudRate(Spi_BaudRateType BaudRate)
{
    switch (BaudRate)
    {
    case SPI_MR_BAUDRATE_2:
        return SPI_BaudRatePrescaler_2;
    case SPI_MR_BAUDRATE_4:
        return SPI_BaudRatePrescaler_4;
    case SPI_MR_BAUDRATE_8:
        return SPI_BaudRatePrescaler_8;
    case SPI_MR_BAUDRATE_16:
        return SPI_BaudRatePrescaler_16;
    case SPI_MR_BAUDRATE_32:
        return SPI_BaudRatePrescaler_32;
    case SPI_MR_BAUDRATE_64:
        return SPI_BaudRatePrescaler_64;
    case SPI_MR_BAUDRATE_128:
        return SPI_BaudRatePrescaler_128;
    case SPI_MR_BAUDRATE_256:
        return SPI_BaudRatePrescaler_256;
    default:
        return SPI_BaudRatePrescaler_2; // Mặc định an toàn
    }
}

uint16 Spi_Map_GetDataSize(Spi_DataSizeType DataSize)
{
    switch (DataSize)
    {
    case SPI_MR_DATASIZE_8B:
        return SPI_DataSize_8b;
    case SPI_MR_DATASIZE_16B:
        return SPI_DataSize_16b;
    default:
        return SPI_DataSize_8b;
    }
}

uint16 Spi_Map_GetCpol(Spi_CpolType Cpol)
{
    switch (Cpol)
    {
    case SPI_MR_CPOL_LOW:
        return SPI_CPOL_Low;
    case SPI_MR_CPOL_HIGH:
        return SPI_CPOL_High;
    default:
        return SPI_CPOL_Low;
    }
}

uint16 Spi_Map_GetCpha(Spi_CphaType Cpha)
{
    switch (Cpha)
    {
    case SPI_MR_CPHA_1EDGE:
        return SPI_CPHA_1Edge;
    case SPI_MR_CPHA_2EDGE:
        return SPI_CPHA_2Edge;
    default:
        return SPI_CPHA_1Edge;
    }
}

uint16 Spi_Map_GetFirstBit(Spi_FirstBitType FirstBit)
{
    switch (FirstBit)
    {
    case SPI_MR_FIRSTBIT_MSB:
        return SPI_FirstBit_MSB;
    case SPI_MR_FIRSTBIT_LSB:
        return SPI_FirstBit_LSB;
    default:
        return SPI_FirstBit_MSB;
    }
}

SPI_TypeDef *Spi_Map_GetHwInstance(Spi_GroupId_Type HwUnitId)
{
    // Ánh xạ ID phần cứng logic sang địa chỉ base của các peripheral SPI trên STM32F103
    switch (HwUnitId)
    {
    case SPI_GROUP_1:
        return SPI1;
    case SPI_GROUP_2:
        return SPI2;
    case SPI_GROUP_3:
        return SPI3;
    default:
        return SPI1;
    }
}