static void Spi_Hw_TransmitReceive(Spi_ChannelType_e channel, const void *pTxData, void *pRxData, uint8 size)
{
    Spi_ExternalDeviceConfigType_s *channelcfg = &Spi_ConfigPtr_s->DeviceCfgPtr[channel];
    SPI_TypeDef *Spix = Spi_Map_GetHwInstance(channelcfg->HwId);

    uint8 byte = 0;

    if (channelcfg->DataSize == SPI_DataSize_16b)
    {
        const uint16_t *pTx16 = (const uint16_t *)pTxData;
        uint16_t *pRx16 = (uint16_t *)pRxData;

        while (byte < size)
        {
            // 1. Chờ bộ đệm truyền trống
            Spi_WaitTxEmpty(Spix);
            // Nếu pTxData là NULL (trường hợp chỉ muốn đọc), gửi dữ liệu giả (Dummy: 0xFFFF)
            uint16_t txVal = (pTx16 != NULL_PTR) ? pTx16[byte] : 0xFFFF;
            SPI_I2S_SendData(Spix, txVal);

            // 2. Chờ nhận dữ liệu về đồng thời
            Spi_WaitRxReady(Spix);
            uint16_t rxVal = SPI_I2S_ReceiveData(Spix);
            if (pRx16 != NULL_PTR)
            {
                pRx16[byte] = rxVal;
            }
            byte++;
        }
    }
    else
    {
        const uint8_t *pTx8 = (const uint8_t *)pTxData;
        uint8_t *pRx8 = (uint8_t *)pRxData;

        while (byte < size)
        {
            // 1. Chờ truyền trống và gửi 1 byte (hoặc dummy 0xFF nếu chỉ đọc)
            Spi_WaitTxEmpty(Spix);
            uint8_t txVal = (pTx8 != NULL_PTR) ? pTx8[byte] : 0xFF;
            SPI_I2S_SendData(Spix, txVal);

            // 2. Chờ nhận dữ liệu về đồng thời
            Spi_WaitRxReady(Spix);
            uint8_t rxVal = (uint8_t)SPI_I2S_ReceiveData(Spix);
            if (pRx8 != NULL_PTR)
            {
                pRx8[byte] = rxVal;
            }
            byte++;
        }
    }

    // Đợi cho đến khi hoàn tất hẳn chu kỳ giao dịch trên bus
    Spi_WaitBusyClear(Spix);
}