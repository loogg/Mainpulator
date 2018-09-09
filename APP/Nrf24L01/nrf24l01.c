#include "nrf24l01.h"
#include "spi.h"
#include "mcu_gpio.h"
#include "delay.h"

#define NRF_CE(x) GPIO_WriteBit(GPIOA, GPIO_Pin_3, (BitAction)x)
#define NRF_CS(x) GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)x)

#define NRF_IRQ GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)

const u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01};

void Nrf24l01Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    McuGpioInit(GPIOA, GPIO_Pin_3, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
    McuGpioInit(GPIOA, GPIO_Pin_4, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
    McuGpioInit(GPIOB, GPIO_Pin_11, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING);

    Spi1Init();

    NRF_CE(0);
    NRF_CS(1);
}

//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��
u8 NRF24L01_Check(void)
{
    u8 buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
    u8 i;
    NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, buf, 5); //д��5���ֽڵĵ�ַ.
    NRF24L01_Read_Buf(TX_ADDR, buf, 5);                  //����д��ĵ�ַ
    for (i = 0; i < 5; i++)
        if (buf[i] != 0XA5)
            break;
    if (i != 5)
        return 1; //���24L01����
    return 0;     //��⵽24L01
}
//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg, u8 value)
{
    u8 status;
    NRF_CS(0);                                     //ʹ��SPI����
    SpiReadWriteSameData(SPI1, &status, reg, 1);   //���ͼĴ�����
    SpiReadWriteSameData(SPI1, &status, value, 1); //д��Ĵ�����ֵ
    NRF_CS(1);                                     //��ֹSPI����
    return (status);                               //����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
    u8 reg_val;
    NRF_CS(0);                                     //ʹ��SPI����
    SpiReadWriteSameData(SPI1, &reg_val, reg, 1);  //���ͼĴ�����
    SpiReadWriteSameData(SPI1, &reg_val, 0xff, 1); //��ȡ�Ĵ�������
    NRF_CS(1);                                     //��ֹSPI����
    return (reg_val);                              //����״ֵ̬
}
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 len)
{
    u8 status;
    NRF_CS(0);                                   //ʹ��SPI����
    SpiReadWriteSameData(SPI1, &status, reg, 1); //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
    SpiReadWriteSameData(SPI1, pBuf, 0xff, len);
    NRF_CS(1);     //�ر�SPI����
    return status; //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
    u8 status;
    NRF_CS(0);                                   //ʹ��SPI����
    SpiReadWriteSameData(SPI1, &status, reg, 1); //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
    SpiReadWriteBuf(SPI1, Spi1Info.ReceiveBuf, pBuf, len);
    NRF_CS(1);     //�ر�SPI����
    return status; //���ض�����״ֵ̬
}
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
    u8 sta;
    NRF_CE(0);
    NRF24L01_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); //д���ݵ�TX BUF  32���ֽ�
    NRF_CE(1);                                              //��������
    DelayUs(120);                                            //�ȴ��������
    sta = NRF24L01_Read_Reg(STATUS);                 //��ȡ״̬�Ĵ�����ֵ
    NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, sta); //���TX_DS��MAX_RT�жϱ�־
    if (sta & MAX_TX)                                //�ﵽ����ط�����
    {
        NRF24L01_Write_Reg(FLUSH_TX, 0xff); //���TX FIFO�Ĵ���
        return MAX_TX;
    }
    if (sta & TX_OK) //�������
    {
        return TX_OK;
    }
    return 0xff; //����ԭ����ʧ��
}
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
    if(NRF_IRQ) return 1;
    u8 sta;
    sta = NRF24L01_Read_Reg(STATUS);                 //��ȡ״̬�Ĵ�����ֵ
    NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, sta); //���TX_DS��MAX_RT�жϱ�־
    if (sta & RX_OK)                                 //���յ�����
    {
        NRF24L01_Read_Buf(RD_RX_PLOAD, rxbuf, RX_PLOAD_WIDTH); //��ȡ����
        NRF24L01_Write_Reg(FLUSH_RX, 0xff);                    //���RX FIFO�Ĵ���
        return 0;
    }
    return 1; //û�յ��κ�����
}
//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������
void NRF24L01_RX_Mode(void)
{
    NRF_CE(0);
    NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (u8 *)RX_ADDRESS, RX_ADR_WIDTH); //дRX�ڵ��ַ

    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);              //ʹ��ͨ��0���Զ�Ӧ��
    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);          //ʹ��ͨ��0�Ľ��յ�ַ
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH, 40);                //����RFͨ��Ƶ��
    NRF24L01_Write_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //ѡ��ͨ��0����Ч���ݿ��
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);           //����TX�������,0db����,2Mbps,���������濪��
    NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);             //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
    NRF_CE(1);                                                    //CEΪ��,�������ģʽ
}
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������
//CEΪ�ߴ���10us,����������.
void NRF24L01_TX_Mode(void)
{
    NRF_CE(0);
    NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, (u8 *)TX_ADDRESS, TX_ADR_WIDTH);    //дTX�ڵ��ַ
    NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (u8 *)RX_ADDRESS, RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK

    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);      //ʹ��ͨ��0���Զ�Ӧ��
    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);  //ʹ��ͨ��0�Ľ��յ�ַ
    NRF24L01_Write_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1a); //�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH, 40);        //����RFͨ��Ϊ40
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);   //����TX�������,0db����,2Mbps,���������濪��
    NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);     //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
    NRF_CE(1);                                            //CEΪ��,10us����������
}
