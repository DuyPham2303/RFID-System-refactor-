void SPI1_IRQHandler(void)
{
    Spi_HwUnitRuntimeType_s *currentHwunitPtr = Spi_Runtime_GetHwUnit();
    if (currentHwunitPtr == NULL_PTR)
    {
        return;
    }

    Spi_JobType_e currentJobId = currentHwunitPtr->ActiveJobId;
    Spi_JobRuntimeType_s *JobRuntime = Spi_Runtime_GetJob(currentJobId);
    Spi_JobConfigType_s *JobConfig = currentHwunitPtr->JobCfg;

    if (JobRuntime == NULL_PTR || JobConfig == NULL_PTR)
    {
        return;
    }

    Spi_SequenceType_e currentSeqId = currentHwunitPtr->ActiveSeqId;
    Spi_DirectionType direction = currentHwunitPtr->direction;
    Spi_DataSizeType datasize = currentHwunitPtr->datasize;
    uint8 length = JobRuntime->TotalBytesInCurrentChannel;
    uint8 TotalCh = JobConfig->ActiveTotalChIncurrentJob;
    uint8 ActiveChIndex = JobRuntime->ChannelIndex;

    if (ActiveChIndex < TotalCh)
    {
        Spi_ChannelType_e ChId = JobConfig->ChannelList[ActiveChIndex];
        Spi_ChannelRuntimeType_s *ChPtr = Spi_Runtime_GetChannel(ChId);
        uint8 byteIndex = currentHwunitPtr->ByteIndex;

        // 1. Xử lý ngắt nhận (RXNE) trước hoặc sau đều được, miễn là đọc dữ liệu khỏi DR để xóa cờ
        if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) != RESET)
        {
            uint16 rxData = SPI_I2S_ReceiveData(SPI1);
            if (ChPtr->ActiveRxPtr != NULL_PTR)
            {
                if (datasize == SPI_MR_DATASIZE_16B)
                {
                    ((uint16_t *)ChPtr->ActiveRxPtr)[byteIndex] = rxData;
                }
                else
                {
                    ((uint8_t *)ChPtr->ActiveRxPtr)[byteIndex] = (uint8_t)rxData;
                }
            }
        }

        // 2. Xử lý ngắt truyền (TXE)
        if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) != RESET)
        {
            if (byteIndex < length)
            {
                uint16 txVal = 0xFFFFU;
                if (direction != SPI_MR_2LINES_RX_ONLY && ChPtr->ActiveTxPtr != NULL_PTR)
                {
                    if (datasize == SPI_MR_DATASIZE_16B)
                    {
                        txVal = ((uint16_t *)ChPtr->ActiveTxPtr)[byteIndex];
                    }
                    else
                    {
                        txVal = ((uint8_t *)ChPtr->ActiveTxPtr)[byteIndex];
                    }
                }

                // Gửi dữ liệu qua SPL trực tiếp tương ứng DataSize
                if (datasize == SPI_MR_DATASIZE_16B)
                {
                    SPI_I2S_SendData16(SPI1, txVal);
                }
                else
                {
                    SPI_I2S_SendData(SPI1, (uint16_t)txVal);
                }

                // Tăng chỉ số byte sau khi đã gửi thành công
                currentHwunitPtr->ByteIndex++;

                // Nếu đây là byte cuối của channel này, chuẩn bị cho channel kế tiếp
                if (currentHwunitPtr->ByteIndex >= length)
                {
                    currentHwunitPtr->ByteIndex = 0;
                    JobRuntime->ChannelIndex++;
                }
            }
            else
            {
                // Nếu đã hết dữ liệu của Job/Channel, tạm thời khóa ngắt TXE để tránh ngắt vô tận
                SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, DISABLE);
            }
        }
    }
    else
    {
        // Hoàn tất toàn bộ Job
        SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, DISABLE);
        SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, DISABLE);
        Spi_InternalFinishJob(currentJobId, currentSeqId);
    }
}