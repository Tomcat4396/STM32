#include "ble_app.h"
#include "ble_uart.h"
#include "ble_cfg.h"
#include "usart3.h"
#include "string.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "key.h"

//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ATK-BLE02ģ�鹦������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2020/1/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2020-2030
//All rights reserved
//********************************************************************************
//��


//BLEģ��IO��ʼ��
void Ble_IoInit(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF, ENABLE); //ʹ��GPIOC,GPIOFʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //BLE_WKUP
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC

    BLE_WKUP = 1;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //BLE_STA
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
    GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
    
    usart3_init(115200);

}

 u8 sendbuf[50];

//BLEģ���������
void Ble_SetInit(void)
{
    u8 t = 0;
    u8 flag=1;
    u8 data=0;
    u32 ble_len,uart_len,i= 0;
    
    LED0 = 0; /*LED0��*/

    /*****��������ģʽ*****/
    while (ble_send_cmd("+++a", "a+ok", 5))
    {
        if (!ble_send_cmd("+++a", "ERROR", 10)) break; /**�п���ģ����������ģʽ,��ʱ����+++a,�᷵��error*/

        LED1 = ~LED1;
        delay_ms(100);
    }

    /*****AT����*****/
    if (!ble_send_cmd("AT", "OK", 5))
    {
        printf("AT OK\r\n");
    }

    /*****������������*****/
    sprintf((char *)sendbuf, "AT+NAME=%s", BLE_NAME);

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }else
    {
        printf("%s  ERROR\r\n", sendbuf);
    }

    /*****������ӭ������*****/
    sprintf((char *)sendbuf, "AT+HELLO=%s", BLE_HELLO);

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }else
    {
        printf("%s  ERROR\r\n", sendbuf);
    }

    /*****���书������*****/
    sprintf((char *)sendbuf, "AT+TPL=%d", BLE_TP_P0dBm);

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }
    else{
        printf("%s  ERROR\r\n", sendbuf);
    }

    /*****��������*****/
    sprintf((char *)sendbuf, "AT+UART=%d,%d,%d,%d", BLE_UARTBPS, BLE_UARTDATE, BLE_UARTPR, BLE_UARTSTOP);

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }else
    {
        printf("%s  ERROR\r\n", sendbuf);
    }

    /*****�㲥�ٶ�����*****/
    sprintf((char *)sendbuf, "AT+ADPTIM=%d", BLE_ADPTIM );

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }else
    {
        printf("%s  ERROR\r\n", sendbuf);
    }

    /*****��·ƥ����������*****/
#if BLE_LINKPASSEN

    if (!ble_send_cmd("AT+LINKPASSEN=ON", "OK", 10)) /*ʹ��*/
    {
        printf("AT+LINKPASSEN=ON  OK\r\n");
    }else
    {
        printf("AT+LINKPASSEN=ON  ERROR\r\n");
    }

#else
    
    if (!ble_send_cmd("AT+LINKPASSEN=OFF", "OK", 10)) /*�ر�*/
    {
        printf("AT+LINKPASSEN=OFF  OK\r\n");
    }else
    {
        printf("AT+LINKPASSEN=OFF  ERROR\r\n");
    }
    
#endif        
    
    /*****����LED����*****/
#if BLE_LEDEN

    if (!ble_send_cmd("AT+LEDEN=ON", "OK", 10)) /*ʹ��*/
    {
        printf("AT+LEDEN=ON  OK\r\n");
    }else
    {
        printf("AT+LEDEN=ON  ERROR\r\n");
    }

#else

    if (!ble_send_cmd("AT+LEDEN=OFF", "OK", 50)) /*�ر�*/
    {
        printf("AT+LEDEN=OFF  OK\r\n");
    }else
    {
        printf("AT+LEDEN=OFF  ERROR\r\n");
    }

#endif

    /*****���豸����˯������*****/
#if BLE_SLAVESLEEPEN

    if (!ble_send_cmd("AT+SLAVESLEEPEN=ON", "OK", 10)) /*ʹ��*/
    {
        printf("AT+SLAVESLEEPEN=ON  OK\r\n");
    }else
    {
        printf("AT+SLAVESLEEPEN=ON  ERROR\r\n");
    }

#else

    if (!ble_send_cmd("AT+SLAVESLEEPEN=OFF", "OK", 10)) /*�ر�*/
    {
        printf("AT+SLAVESLEEPEN=OFF  OK\r\n");
    }else
    {
        printf("AT+SLAVESLEEPEN=OFF  ERROR\r\n");
    }

#endif

    /*****��������������*****/
#if BLE_MAXPUT

    if (!ble_send_cmd("AT+MAXPUT=ON", "OK", 10)) /*ʹ��*/
    {
        printf("AT+MAXPUT=ON  OK\r\n");
    }else
    {
        printf("AT+MAXPUT=ON  ERROR\r\n");
    }

#else

    if (!ble_send_cmd("AT+MAXPUT=OFF", "OK", 10)) /*�ر�*/
    {
        printf("AT+MAXPUT=OFF  OK\r\n");
    }else
    {
        printf("AT+MAXPUT=OFF  ERROR\r\n");
    }

#endif

    /*****����ģʽ����(ģʽ���ú�,ģ��������λ����͸��ģʽ)*****/
    sprintf((char *)sendbuf, "AT+MODE=%c", BLE_MODE_M );

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }else
    {
        printf("%s  ERROR\r\n", sendbuf);
    }
    
     /*****�ٴν�������ģʽ*****/
    while (ble_send_cmd("+++a", "a+ok", 5))
    {
        if (!ble_send_cmd("+++a", "ERROR", 10)) break; /**�п���ģ����������ģʽ,��ʱ����+++a,�᷵��error*/

        LED1 = ~LED1;
        delay_ms(100);
    }
    
    /*****������Χ�Ĵӻ�*****/
    printf("AT+SCAN\r\n");
    if (!ble_send_cmd("AT+SCAN", "OK", 10)) /*ʹ��*/
    {
        while(flag)
        {
            if (USART3_RX_STA & 0x8000)/*������������ģ�鷵�ص�����*/
            {
                ble_len = USART3_RX_STA & 0X7FFF;
                USART3_RX_BUF[ble_len] = 0; //��ӽ�����
                USART3_RX_STA = 0;

                for (i = 0; i < ble_len; i++)
                {
                    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); //ѭ������,ֱ���������
                    USART_SendData(USART1, USART3_RX_BUF[i]);
                }
            }
            
            if (USART_RX_STA & 0x8000)/*���������û����ڵ�����*/
            {
                 uart_len = USART_RX_STA & 0x3fff; //�õ��˴ν��յ������ݳ���
                 if(uart_len <=2)
                 {
                    USART_RX_BUF[uart_len] = 0; //��ӽ�����
                     if(uart_len==1)
                     {
                         data = USART_RX_BUF[0]-'0';
                         
                     }else if(uart_len==2)
                     {
                         data =  (USART_RX_BUF[1]-'0')*10 + (USART_RX_BUF[0]-'0');
                     }
                   
                 }
                 USART_RX_STA = 0;
                 
                 printf("\r\n");
                 sprintf((char *)sendbuf, "AT+CONN=%d", data );

                if (!ble_send_cmd(sendbuf, "OK", 10))
                {
                    printf("%s  OK\r\n", sendbuf);
                    delay_ms(300);
                    while(BLE_STA);
                    if(!BLE_STA)   /*����������*/
                    {
                        flag=0;
                        
                        /*****����͸��ģʽ*****/
                        sprintf((char *)sendbuf, "AT+ENTM" );

                        if (!ble_send_cmd("AT+ENTM", "OK", 10))
                        {
                            printf("AT+ENTM  OK\r\n");
                        }else
                        {
                            printf("AT+ENTM  ERROR\r\n");
                        }
                    }
                    
                }else
                {
                    printf("%s  ERROR\r\n", sendbuf);
                }     
                data = 0;
            }
            
            t++;
            if (t == 200)
            {
                t = 0;
                LED0= ~LED0;
            }
            delay_ms(1);
        }
    }else
    {
        printf("AT+SCAN  ERROR\r\n");
    }

    LED0 = 1;/*LED0��*/
}

/*BLEģ�����ݷ��ʹ���*/
void Ble_SendData(void)
{
    static u8 num = 0;

    u3_printf("ATK-BLE02 TEST %d\r\n", num);
    printf("S: ATK-BLE02 TEST %d\r\n", num);

    num++;
    if (num == 255) num = 0;
}

/*BLEģ��������ݴ���*/
void Ble_ReceData(void)
{
    u16 i = 0;
    u16 len = 0;

    //����������
    if (USART3_RX_STA & 0x8000)
    {
        len = USART3_RX_STA & 0X7FFF;
        USART3_RX_BUF[len] = 0; //��ӽ�����
        USART3_RX_STA = 0;

        printf("R:");

        for (i = 0; i < len; i++)
        {
            while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); //ѭ������,ֱ���������
            USART_SendData(USART1, USART3_RX_BUF[i]);
        }

        if (strstr((const char *)USART3_RX_BUF, LED0_ON) && (len == (sizeof(LED0_ON) - 1)))        LED0 = 0; /*LED0����*/
        else if (strstr((const char *)USART3_RX_BUF, LED0_OFF) && (len == (sizeof(LED0_OFF) - 1))) LED0 = 1; /*LED0�ر�*/

        memset((char *)USART3_RX_BUF, 0x00, len); //���ڽ��ջ�������0
        printf("\r\n");
    }

}

/*BLE���̴���*/
void Ble_Process(void)
{
    u16 len;
    u8 key = 0;
    u8 t = 0;
    u8 BLE_LINK = 0; //1:�������� 0:δ����
    static u8 flag1, flag2 = 0;

    printf("\r\nKEY1:��������\r\n");

    while (1)
    {
        
        if (USART_RX_STA & 0x8000)
        {
            len = USART_RX_STA & 0x3fff; //�õ��˴ν��յ������ݳ���
            
            printf("S: ");

            for (t = 0; t < len; t++)
            {
                while (USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET); //�ȴ����ͽ���
                USART_SendData(USART3, USART_RX_BUF[t]);//�򴮿�3��������
                
                while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET); //�ȴ����ͽ���
                USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
                
            }

            printf("\r\n");//���뻻��
            USART_RX_STA = 0;
        }
        
        key = KEY_Scan(0);

        if(key == KEY1_PRES) //��������
        {
            if (BLE_LINK) /*����������*/
            {
                Ble_SendData();/*���ݷ��ʹ���*/
            }
        }

        /*���ݽ��մ���*/
        Ble_ReceData();

        /*BLE���Ӵ���*/
        if (!BLE_STA)
        {
            BLE_LINK = 1; //��������

            if (!flag1)
            {
                flag1 = 1;
                flag2 = 0;
                printf("\r\nble connect!\r\n");
            }
        }
        else
        {
            BLE_LINK = 0; //�����Ͽ�

            if (!flag2)
            {

                flag2 = 1;
                flag1 = 0;
                printf("\r\nble disconnect!\r\n");
            }
        }

        t++;
        if (t == 20)
        {
            t = 0;
            LED1 = ~LED1;
        }
        delay_ms(10);

    }

}

//�����Ժ���
void Ble_Test(void)
{

    printf("\r\nATK-BLE02 ģ��͸�����Գ���\r\n\r\n");

    Ble_IoInit(); //BLE���ų�ʼ��
    Ble_SetInit();//BLE��������
    Ble_Process();//���ݲ���

}





