#include "nsp_driver.h"
#include "nsp_PlaySample.h"
#include <string.h>  // memcpy��ʹ�ä��뤿��˱��?

//===========================================================
#define MULTI_PLAY_MAX 32 
#define FLASH_PAGE_SIZE	512
#define FLASH_MEMORY_BASE 0x00000000 // �٩`�����ɥ쥹�򁢶�

UINT8 ISP_Buffer[ISP_BUF_SIZE];

UINT32	u32NSP_ID = 0;                                  //�Է֤��O��
UINT32	u32NSP_FW_VERSION = 0;
UINT8	u8NSP_STATUS = 0;
UINT16 	u16PlayListIndex=0;                              //play list index
UINT8 	u8SP_VOL=0;                                      //speech volume:0~0x80
UINT8 	u8IO_FLAG=0; 
UINT8 	u8IO_VALUE=0;
UINT8 	u8LVD_VALUE=0;
UINT16  u16MAX_INDEX = 0;
UINT8 	CHECKSUM_BIT=0;
UINT16 	CHECKSUM_BYTES=0;
UINT16 	ISP_CHECKSUM_BYTES=0;
UINT32 	g_sFlash;
UINT8 	g_au8Buf[FLASH_PAGE_SIZE]= {
	0xFF,0xFE,0xFD,0xFC,0xFB,0xFA,0xF9,0xF8,0xF7,0xF6,0xF5,0xF4,0xF3,0xF2,0xF1,0xF0,
	0xEF,0xEE,0xED,0xEC,0xEB,0xEA,0xE9,0xE8,0xE7,0xE6,0xE5,0xE4,0xE3,0xE2,0xE1,0xE0,
	0xDF,0xDE,0xDD,0xDC,0xDB,0xDA,0xD9,0xD8,0xD7,0xD6,0xD5,0xD4,0xD3,0xD2,0xD1,0xD0,
	0xCF,0xCE,0xCD,0xCC,0xCB,0xCA,0xC9,0xC8,0xC7,0xC6,0xC5,0xC4,0xC3,0xC2,0xC1,0xC0,
	0xBF,0xBE,0xBD,0xBC,0xBB,0xBA,0xB9,0xB8,0xB7,0xB6,0xB5,0xB4,0xB3,0xB2,0xB1,0xB0,
	0xAF,0xAE,0xAD,0xAC,0xAB,0xAA,0xA9,0xA8,0xA7,0xA6,0xA5,0xA4,0xA3,0xA2,0xA1,0xA0,
	0x9F,0x9E,0x9D,0x9C,0x9B,0x9A,0x99,0x98,0x97,0x96,0x95,0x94,0x93,0x92,0x91,0x90,
	0x8F,0x8E,0x8D,0x8C,0x8B,0x8A,0x89,0x88,0x87,0x86,0x85,0x84,0x83,0x82,0x81,0x80,
	0x7F,0x7E,0x7D,0x7C,0x7B,0x7A,0x79,0x78,0x77,0x76,0x75,0x74,0x73,0x72,0x71,0x70,
	0x6F,0x6E,0x6D,0x6C,0x6B,0x6A,0x69,0x68,0x67,0x66,0x65,0x64,0x63,0x62,0x61,0x60,
	0x5F,0x5E,0x5D,0x5C,0x5B,0x5A,0x59,0x58,0x57,0x56,0x55,0x54,0x53,0x52,0x51,0x50,
	0x4F,0x4E,0x4D,0x4C,0x4B,0x4A,0x49,0x48,0x47,0x46,0x45,0x44,0x43,0x42,0x41,0x40,
	0x3F,0x3E,0x3D,0x3C,0x3B,0x3A,0x39,0x38,0x37,0x36,0x35,0x34,0x33,0x32,0x31,0x30,
	0x2F,0x2E,0x2D,0x2C,0x2B,0x2A,0x29,0x28,0x27,0x26,0x25,0x24,0x23,0x22,0x21,0x20,
	0x1F,0x1E,0x1D,0x1C,0x1B,0x1A,0x19,0x18,0x17,0x16,0x15,0x14,0x13,0x12,0x11,0x10,
	0x0F,0x0E,0x0D,0x0C,0x0B,0x0A,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00,
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
	0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
	0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
	0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
	0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
	0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
	0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
	0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
	0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
	0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
	};
UINT8 	g_au8Buf_Read[FLASH_PAGE_SIZE]= {0};
//===========================================================
/*----- �ȴ����Ž��� -----*/ 
/*----- Waiting for playback to finish -----*/
void UART_WaitPlayEND(void)
{
    UINT8 NSP_STATUS = 0;
    while ((N_READ_STATUS(&NSP_STATUS) != 1) || (NSP_STATUS & 0x80))	 // ��ȡ״̬ Read Status
    {                                                  //BIT7(SP_BUSY) 0:NSP���ڲ�����, 1:NSP���ڲ�����
         HOST_Delay500uS();						 
    }
}

/*----- �ȴ�ָ��ִ�н��� -----*/ 
/*----- Waiting for the NSP command execution to finish -----*/
UINT8 UART_WaitExecutionEND(void)
{
    UINT8 NSP_STATUS = 0;
    while (N_READ_STATUS(&NSP_STATUS) != 1)           // ��ȡ״̬�ɹ���ʾָ��ִ�����?
    {                                                 // Read Status success: the NSP command done
          HOST_Delay500uS();						 
    }
    
    if(NSP_STATUS & 0x40)                             // BIT6(CMD_VALID) 0:ָ�����, 1:ָ���ִ��?
          return 1;                                   // �ش�ֵ 1 ���� NSP ָ����ִ�н���
                                                      // The return value 1 means the NSP command has been executed done.
    else								
          return 0; 				      // �ش�ֵ 0 ���� NSPָ���ִ��??
                                                      // The return value of 0 means the NSP command is not executable.
}
                                                         
/*----- ѯ��״̬ -----*/
/*----- Ask for status -----*/
UINT8 UART_AskStatus(void)
{
    UINT8 NSP_STATUS = 0;
    
    if(N_READ_STATUS(&NSP_STATUS) == 1)               // ��ȡ״̬ Read Status
    {
	if(!(NSP_STATUS & 0x80) && (NSP_STATUS & 0x40) )
	{
             return 1;                               // �ش�ֵ 1 ���� NSP ���ڲ����У��� NSP ָ����ִ�н���
	}                                                  // The return value 1 means that the NSP is not playing, and the NSP command has been executed done.
	else
	{
             return 0;                               // �ش�ֵ 0 ���� NSP �ڲ����У����� NSP ָ����У����߲���ִ��??
	}                                                  // The return value of 0 means that the NSP is playing, or the NSP command is being transmitted, or it is not executable.
    }
    else
       return 0;
}

//===========================================================
/*----- Index Play Sample -----*/
void UART_IndexPlaySample(void)
{
    UINT16 temp = 0;                                    // ������ʼ��
    UINT8  u8RESOURCE_COUNT = 0;                        // init parameter            	
    HOST_SYS_Init();                                    // hostϵͳ��ʼ��
                                                        // host system initialization
    N_PLAY(1);                                          // ���� Index 1 ��Ч 
                                                        // Play Index 1 Resource
    UART_WaitPlayEND();                              // �ȴ��������?
                                                        // Waiting for playback to finish
    N_PLAY(2);                                          // ���� Index 2 ��Ч 
                                                        // Play Index 2 Resource
    N_PLAY_REPEAT(0);                                   // ѭ������ Index 2 ��Ч
                                                        // loop Index 2
                                                        
    for (temp= 0; temp < 16000; temp++)                  // ѭ������ 8 ����
    {                                                   // Loop for 8 seconds
        HOST_Delay500uS();     
    }
    
    N_STOP_REPEAT();                                    // ��鲥�Ž����?��ֹͣ����
                                                        // stop playback after this time                                                     
    UART_WaitPlayEND();                                 // �ȴ��������?
                                                        // Waiting for playback to finish
                                                        
                                                        
    N_CHECK_INDEX_RCOUNT(5000);                          // ���? Index 5000�Ƿ���resource
                                                        // check Index 5000 valid
    N_GET_INDEX_RCOUNT(&u8RESOURCE_COUNT);               // �õ� Index 5000��resource����
                                                        // GET resource count of Index 5000
    if (u8RESOURCE_COUNT > 0)
    {
    	N_PLAY(5000);                                    // ���� Index 5000 ��Ч 
                                                         // Play Index 5000 Resource
        UART_WaitPlayEND();                              // �ȴ��������?
                                                         // Waiting for playback to finish                                                 
    }                                                     
    while(1);
}

/*----- MultiPlay Sample -----*/
void UART_MultiPlaySample(void)
{
    UINT8 MultiPlayBuffer[10] = {1,1,2,2,3,3,4,4,5,5};  // �趨���ز���˳�����? 
                                                        // Set the multiplay order parameters.
    UINT16 MultiPlay2BBuffer[4] = {257,1,300,2};  	// �趨���ز���˳�����? 
                                                        // Set the multiplay order parameters.
    
    HOST_SYS_Init();                                    // hostϵͳ��ʼ��
                                                        // host system initialization
    
    N_MULTI_PLAY(6,&MultiPlayBuffer[0]);                // �趨6�ض��鲥�ţ�������ʼ�� Buffer[0]��ʼ 
                                                        // Set 6 groups of multi-play, starting playback starts with Buffer[0].
    
    UART_WaitPlayEND();                              // �ȴ�����Index 1 -> Index 1 -> Index2 -> Index 2 -> Index 3 -> Index 3 ����??
                                                        // Waiting for playback Index 1 -> Index 1 -> Index2 -> Index 2 -> Index 3 -> Index 3 is finished
                                                        
    N_MULTI_PLAY_2B(3,&MultiPlay2BBuffer[0]);           // �趨3�ض��鲥�ţ�������ʼ�� 2BBuffer[0]��ʼ 
                                                        // Set 3 groups of multi-play, starting playback starts with 2BBuffer[0].
                                                        
    N_PLAY_REPEAT(2);                                   // �ظ����� Index 257 -> Index 1 -> Index300 ��Ч 2��
                                                        // repeat 2 times
    
    UART_WaitPlayEND();                              // �ȴ�����Index 257 -> Index 1 -> Index300��Ч 2�����???
                                                        // Waiting for playback (Index 257 -> Index 1 -> Index300)* 2times is finished                                                                  
    while(1);
}

/*----- Sleep Wake Up Sample -----*/
void UART_SleepWakeUpSample(void)
{
    UINT16 temp = 0;                                    // ������ʼ��				
                                                        // init parameter 
    HOST_SYS_Init();                                    // hostϵͳ��ʼ��
                                                        // host system initialization
    N_PLAY_SLEEP(1);                                    // ���� Index 1 ��Ч�������������?
                                                        // Plays Index 1 and then goes to sleep.
    for (temp= 0; temp < 14000; temp++)                 // 7 sec �ȴ���Ч���Ž�������
    {                                                   // 7 sec Wait for the sound effect to end and sleep
         HOST_Delay500uS();
    }
    
    
    N_WAKUP();                                          // ����
                                                        // Wake Up   
    N_AUTO_SLEEP_DIS();                                 // GUI�趨sleep time�������£��ر��Զ����߹��ܣ�ֻ��N_PWR_DOWN������˯
                                                        // GUIû���趨sleep time��ָ����Ч    
                                                        // (If user sets the sleep time in the GUI), disable the automatic sleep function, only N_PWR_DOWN can enter sleep
                                                        // The command is invalid if user does not set the sleep time in the GUI
                                                                                                            
    N_PLAY(2);                                          // ���� Index 2 ��Ч 
                                                        // Play Index 2 Resource
    for (temp= 0; temp < 8000; temp++)                  // ���� 4 sec
    {                                                   // play for 4 sec
         HOST_Delay500uS();
    }
    N_STOP();                                           // ǿ�ƽ�������
                                                        // stop speech   
    N_PWR_DOWN();                                       // ǿ��ֱ�ӽ�������
                                                        // Go straight to sleep.                                                 
    for (temp= 0; temp < 8000; temp++)                  // ���� 4 sec
    {                                                   // enter sleep for 4 sec
         HOST_Delay500uS();
    }    
    
                                                    
    
    N_WAKUP();                                          // ����
                                                        // Wake Up 
    N_AUTO_SLEEP_EN();                                  // ���Զ����߹��ܣ�GUI�趨sleep timeʱ����û�в��ţ�û����ָ�û��LVD��checksumִ�оͻ���˯
                                                        // GUIû���趨sleep time��ָ����Ч    
                                                        // Enable the automatic sleep function:if no play, no new CMD, no LVD and checksum execution 
                                                        // during the sleep time(setting in the GUI),then the NSP will enter sleep
    							// The command is invalid if user does not set the sleep time in the GUI
    
    N_PLAY(2);                                          // ���� Index 2 ��Ч 
                                                        // Play Index 2 Resource                                                   
    UART_WaitPlayEND();                                 // �ȴ�������ϣ�GUI�趨sleep timeʱ����û�в��ţ�û����ָ�û��LVD��checksum,NSP�Զ���������
                                                        // Waiting for playback to finish
                                                        // If no play, no new CMD, no LVD and checksum execution 
                                                        // during the sleep time(setting in the GUI),then the NSP will enter sleep
    while(1);
}

/*----- Low Power Detection -----*/
void UART_LowPowerDetectionSample(void)
{
    UINT8 temp = 0;                                     // ������ʼ��
    u8LVD_VALUE=0;                                      // init parameters
	
    HOST_SYS_Init();                                    // hostϵͳ��ʼ��
							// host system initialization
	
    N_DO_LVD();	                                        // ִ�е�ѹ������
                                                        // Start power detection 
    for (temp= 0; temp <16; temp++)                     // �ȴ���ѹ������ִ�� 8 ms
    {                                                   // Waiting for power detection to execute 8 ms
        HOST_Delay500uS();
    }
		
    N_GET_LVD(&u8LVD_VALUE);                            // �õ���ǰ��ѹ u8LVD_VALUE	
                                                        // Get current voltage u8LVD_VALUE		
                                                        // 01H:              	VDD < 2.1V,
                                                        // 02H:  	2.1V �� VDD < 2.4V,
                                                        // 04H: 	2.4V �� VDD < 2.7V,
                                                        // 08H: 	2.7V �� VDD < 3.0V,
                                                        // 10H: 	3.0V �� VDD < 3.3V,
                                                        // 20H: 	3.3V �� VDD.
    while(1);
}

/*----- NSP IO Control Sample -----*/
void UART_NSP_IO_CtrlSample(void)
{
    u8IO_FLAG = 0;                                     // ������ʼ��
    u8IO_VALUE = 0x00;                                 // init parameter 
    	
    HOST_SYS_Init();                                   // hostϵͳ��ʼ��
                                                       // host system initialization
    							
    N_IO_CONFIG(0xF3);                                 // ����BP07/BP06Ϊinput pin��BP13/BP02Ϊoutput pin
                                                       // Set BP07/BP06 as input pin and BP13/BP02 as output pin
                                                       // NSP080/170/340 SOP14: bit[7]:BP07,bit[6]:BP06(disable PA),bit[3]:BP13,bit[2]:BP02
                                                       // NSP480/650/960 SOP14: bit[7]:BP17(disable PA),bit[3]:BP13,bit[2]:BP12,bit[0]:BP20   							
    N_IO_TYPE(&u8IO_FLAG);                             // �õ� IO ������ u8IO_FLAG
                                                       // Get the type of IO: u8IO_FLAG

    N_SET_OUT(0x00);                                   // ���� BP13/BP02 output 0
                                                       // Set BP13/BP02 output 0
    N_GET_INOUT(&u8IO_VALUE);                          // �õ� IO ����ֵ u8IO_VALUE
                                                       // Get the value of IO: u8IO_VALUE
                                                       // NSP080/170/340 SOP14: bit[7]:BP07,bit[6]:BP06(disable PA),bit[3]:BP13,bit[2]:BP02
                                                       // NSP480/650/960 SOP14: bit[7]:BP17(disable PA),bit[3]:BP13,bit[2]:BP12,bit[0]:BP20
    while(1);
}

/*----- CheckSum Sample -----*/
void UART_CheckSumSample(void)
{
    UINT16 temp = 0;                                   // ������ʼ��
    CHECKSUM_BIT=0;                                    // init parameter 
    CHECKSUM_BYTES=0;

    HOST_SYS_Init();                                   // hostϵͳ��ʼ��
                                                       // host system initialization

    N_DO_CHECKSUM();                                   // ִ��checksum����,�˹��̴�Լ��Ҫ2~3 sec,Ҳ���滻������ MCU ���س�ʽ
    for (temp= 0; temp <3000; temp++)                  // Execute checksum operation, this process takes about 2~3 sec, and can be replaced with other MCU main program.
    {
         HOST_Delay500uS();
         HOST_Delay500uS();
    }

    N_CHECKSUM_RIGHT(&CHECKSUM_BIT);                    //�õ� checksum��ȷ���Ľ��? CHECKSUM_BIT: 0:right,1:error 
                                                        //Get checksum result correct or not CHECKSUM_BIT: 0:right,1:error
    N_GET_CHECKSUM(&CHECKSUM_BYTES);                    //�õ� checksum 16λ��ֵ���? CHECKSUM_BYTES
                                                        //Get checksum 16-bits result CHECKSUM_BYTES
							
    while(1);
}

/*----- Busy Pin Set Sample -----*/
void UART_BusyPinSetSample(void)
{
    HOST_SYS_Init();                                    // IO �����ʼ��? 
                                                        // host system initialization
    N_BZPIN_EN();                                       // BP02 ��Ϊæ�����ģ�? 1:���Ž���, 0:������
                                                        // Enable the busy-pin function at BP02. 1: playback finish,0:speech is playing.
    N_PLAY(1);                                          // ���� Index 1 ��Ч, ���Ź�����BP02����͵��?
                                                        // Play Index 1 Resource,and BP02 output low during playback
    UART_WaitPlayEND();                              // �ȴ�������ϣ�������Ϻ�BP02����ߵ��?
                                                        // Waiting for playback to finish, BP02 output high level after playback is completed
    //......���� MCU ������ʽ
    //Host MCU Handle 
    N_BZPIN_DIS();                                      // �ر� BP02 æ�����ģ�?, BP02�ָ���ͨinput
                                                        // Disable the busy-pin function at BP02, and BP02 restore normal input
    while(1);
}

/*----- Volume Control Sample -----*/
void UART_VolumeCtrlSample(void)
{
    UINT16 temp = 0;                                    // ������ʼ��
    u8SP_VOL = 0;                                       // init parameter 

    HOST_SYS_Init();                                    // IO �����ʼ��? 
                                                        // host system initialization
    N_PLAY(1);                                          // ���� Index 1 ��Ч
                                                        // Play Index 1 Resource
    N_PLAY_REPEAT(0);                                   // ѭ������ Index 1 ��Ч
                                                        // Looping Index 1
				
    N_GET_VOL(&u8SP_VOL);                               // ��ȡ��ǰ�����趨 
                                                        // Get current volume setting
				
    while ( u8SP_VOL >= 0x20 )	                        // ÿ��3����,�����ݼ�0x10,ֱ�������ݼ���0x20Ϊֹ
    {                                                   // the volume is reduced by 0x16 every 3 seconds until the volume is decremented to 0x20
          u8SP_VOL -= 0x10 ;
          N_SET_VOL(u8SP_VOL);                          // �������� set volume					
          for (temp= 0; temp <3000; temp++)		
          {					
             HOST_Delay500uS();
             HOST_Delay500uS();
          }	
    } 
    
    N_STOP_REPEAT();                                    // ��鲥�Ž����?��ֹͣ����
                                                        // stop playback after this time    

    while(1);       
}

/*----- Read ID & FW Version Sample -----*/
void UART_ReadIdAndFwVerSample(void)
{
    HOST_SYS_Init();                                    // IO �����ʼ��? 
                                                        // host system initialization
    N_GET_FW_VERSION(&u32NSP_FW_VERSION);               // �õ� FW �汾��u32NSP_FW_VERSION		 
                                                        // Get the FW version number u32NSP_FW_VERSION
    N_READ_ID(&u32NSP_ID);                              // �õ� product ID
                                                        // get product ID
    if(u32NSP_ID == 0xD4C3B2A1)                         // ���? product ID��ȷ
    {                                                   // If the product ID is correct
       //.......
    }
	while(1);                                              
}

/*----- Mixed Sample 1 -----*/
/*----- ������ NSP IO BP07 ����͵�ƽ��� -----*/
void UART_MixedSample1(void)
{
    HOST_SYS_Init();                                    // IO �����ʼ��? 
                                                        // host system initialization
    N_IO_CONFIG(0x7F);                                  // �趨 BP07 Ϊ���ģ�?, BIT7(BP07) 1:����ģʽ 0:���ģ�?
                                                        // Setting BP07 is OUTPUT Mode. BIT7(BP07) 1:InPut Mode 0:OutPut Mode
    N_PLAY(1);                                          // ���� Index 1 ��Ч
                                                        // Play Index 1 Resource
    N_SET_OUT(0x7F);                                    // �趨 BP07 Ϊ����͵��?
                                                        // Setting BP07 is Output Low
    UART_WaitPlayEND();                              // �ȴ�������ϣ�������Ϻ�BP02����ߵ��?
                                                        // Waiting for playback to finish, BP02 output high level after playback is completed
    N_SET_OUT(0xFF);                                    // �趨 BP07 Ϊ����ߵ��?
                                                        // Setting BP07 is Output High
    while(1);
}
/*----- Mixed Sample 2 -----*/
/*----- ��ʱ�๤������������ MCU �οɿ���?? -----*/
void UART_MixedSample2(void)
{
    HOST_SYS_Init();                                    // IO �����ʼ��? 
                                                        // host system initialization
    N_PLAY(1);                                          // ���� Index 1 ��Ч
                                                        // Play Index 1 Resource
    while(1)
    {
        //.......
        //���� MCU ������ʽ
        //Host MCU Handle 
        if(UART_AskStatus())                         // ѯ�� NSP ״̬
                                                        // Ask NSP Status
        {
            break;                                      // NSP ������ϼ���ִ��ָ��?���������?��ѭ��?C
                                                        // After the NSP is finished playing and the command is over, leave this loop.
        }
    }
    while(1);
}
/*----- Mixed Sample 3 -----*/
void UART_MixedSample3(void)
{
    PUINT8 PLAY_BUFFER = 0;
    UINT8 g_au8MultiPlayBuf[MULTI_PLAY_MAX]={3,1,2,4,8,6,5,7,10,9,0,0,0,0,0,0,// �趨���ز���˳�����? 
                                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};// Set the multiplay order parameters.
					
    HOST_SYS_Init();                                    // IO �����ʼ��? 
                                                        // host system initialization

    if(N_READ_ID(&u32NSP_ID) == 1)                      // �õ� product ID					
    {                                                   // get product ID	
         if(u32NSP_ID == 0xD4C3B2A1)                    // ���? product ID��ȷ
         {                                              // If the product ID is correct	
               UINT16 temp = 0;                         // ������ʼ��				
                                                        // init parameter 
               N_GET_FW_VERSION(&u32NSP_FW_VERSION);    // �õ� FW �汾��u32NSP_FW_VERSION		 
                                                        // Get the FW version number u32NSP_FW_VERSION
               N_GET_MAX_INDEX(&u16MAX_INDEX);		// �õ����Ĳ������u16MAX_INDEX
                                                        // Get the max PlayList index u16MAX_INDEX
               N_SET_VOL(0x50);                         // �������� set volume	

               PLAY_BUFFER = g_au8MultiPlayBuf;				
               N_MULTI_PLAY(4,PLAY_BUFFER);             // �趨4�ض��鲥�ţ�������ʼ�� g_au8MultiPlayBuf[0]��ʼ 
                                                        // Set 4 groups of multi-play, starting playback starts with g_au8MultiPlayBuf[0]. 			
               N_PLAY_REPEAT(2);                        // �ظ����� Index 3 -> Index 1 -> Index2 -> Index4 ��Ч 2��
                                                        // repeat 2 times:Index 3 -> Index 1 -> Index2 -> Index4 

               UART_WaitPlayEND();                   // �ȴ�����Index 3 -> Index 1 -> Index2 -> Index4 ��Ч 2�����???
                                                        // Waiting for playback (Index 3 -> Index 1 -> Index2 -> Index4)* 2times is finished          
                                                        
               for (u16PlayListIndex= 1; u16PlayListIndex <= u16MAX_INDEX; u16PlayListIndex++)  //������Ч��ͷ��β����һ��
               {                                        // Play all the resource from index 1 to u16MAX_INDEX
                        N_PLAY(u16PlayListIndex);       // ���� ��Ч 
                                                        // Play Resource
                        UART_WaitPlayEND();          // �ȴ��������?
                                                        // Waiting for playback to finish
               }

               for (u16PlayListIndex= 1; u16PlayListIndex <= u16MAX_INDEX; u16PlayListIndex++)//�� index1 ��ʼÿ����5sec ����һ����Ч
               {                                        // Play the next index every 5 seconds from index1
                        N_PLAY(u16PlayListIndex);       // ���� ��Ч 
                                                        // Play Resource
                        for (temp= 0; temp < 10000; temp++)
                        {
                               HOST_Delay500uS();
                        }
                        N_STOP();                       // ֹͣ���ŵ�ǰ��Ч
                                                        // Stop playing the current sound
               }

               N_PWR_DOWN();                            // ǿ��ֱ�ӽ�������	
                                                        // Go straight to sleep			
               for (temp= 0; temp < 2000; temp++)       // ʱ�����? 1 ����
               {                                        // 1 sec
                        HOST_Delay500uS();	
               }


               N_WAKUP();                                // ����
                                                         // Wake Up
               for (u16PlayListIndex= 1; u16PlayListIndex <= u16MAX_INDEX; u16PlayListIndex++) //�� index1 ��ʼÿ���� 7 sec ����һ����Ч����Ч����С��7sec�Ļ�����ݽ�������?
               {									//Play the next index every 7 sec from index1. If the sound length is less than 7sec, it will go to sleep briefly.
                        N_PLAY_SLEEP(u16PlayListIndex); // ���� ��Ч�������������?
                                                        // Plays and then goes to sleep.
                        for (temp= 0; temp < 14000; temp++)
                        {
                               HOST_Delay500uS();
                        }
               N_WAKUP();                                // ����
                                                         // Wake Up
               }
         }
    }
    while(1);
}

//===========================================================
//Read resource bin: starting from u32ByteAddr, the length of u32DataLen data into the buffer pointed to by pau8Data
void Flash_Read(UINT32 *psFlashHandler,UINT32 u32ByteAddr,PUINT8 pau8Data,UINT32 u32DataLen)
{
//����Ϊread resource bin�д�u32ByteAddr��ʼ������Ϊu32DataLen��data����pau8Data��ָ���buffer��
    // ���ɥ쥹���L���ι�������å�?
    // if (u32ByteAddr + u32DataLen > FLASH_PAGE_SIZE) {
    //     // ��������i��ȡ����������Υ���`�����å�
    //     // �g�H�Υ��`�ɤǤϥ���`�ϥ�ɥ�󥰤��Ф��٤�
    //     return;
    // }

    // �ե�å������Όg�H�Υ��ɥ쥹��Ӌ��
    PUINT8 flashMemoryPtr = (PUINT8)(ISP_Buffer + u32ByteAddr);
    Serial.println((uintptr_t)flashMemoryPtr,HEX);
    // �ǩ`����ե�å������?��Хåե��˥��ԩ`
    memcpy(pau8Data, flashMemoryPtr, u32DataLen);
}

//Write resource bin: starting from u32ByteAddr, the length of u32DataLen data into the buffer pointed to by pau8Data
void Flash_Write(UINT32 *psFlashHandler,UINT32 u32WriteAddr,PUINT8 pau8Data,UINT32 u32DataLen)
{

}

/*----- NSP ISP Update all resource Sample -----*/
void UART_ISPUpdateAllResourceSample(void)
{
    UINT32 u32StartAddr = 0;                            // ������ʼ��				
    UINT32 u32ISPSize = 0;                              // init parameter 
    UINT32 u32ISPSizeDone = 0;
    UINT32 u32ISPADDR = 0;
    UINT32 u32FirstSegADDR = 0;
    UINT32 u32ISPReadADDR = 0;
    PUINT8 ISP_BUFFER = 0;
    UINT32 u32CHIP_ID = 0;                  //9023 40BF -> 2,418,229,439 �Է֤��O��
    UINT8 u8NSP_STATUS1 = 0;
    UINT8 u8RightCMD = 0;
    UINT16 u16ISPChecksumFromFile = 0;
	
    HOST_SYS_Init();                                   // IO �����ʼ��? 
                                                       // host system initialization
    N_WAKUP();                                          // ����
                                                        // Wake Up        
    N_PLAY(1);                                         // ���� Index 1 ��Ч
                                                       // Play Index 1 Resource
   // RES_VP_CpuIf_NSPxxx_SOPxx.bin:
   // byte 0x0~0x3 	: Signature (NSPB)
   // byte 0x4~0x5		: Data Format Version 
   // byte 0x6~0xB		: SDS Version
   // byte 0xC~0xD		: Segment Count
   // byte 0xE~0x11	: Offset of segment Information
   // byte 0x12~0x15	: 1st Segment Data Offset
   // byte 0x16~0x19	: 2nd Segment Data Offset
    Flash_Read(&g_sFlash, u32StartAddr, g_au8Buf, 22);
    u32FirstSegADDR = (g_au8Buf[18]<<24) + (g_au8Buf[19]<<16)+ (g_au8Buf[20]<<8) + g_au8Buf[21]; // �õ���һ�������λ��?
                                                      //get 1st Segment Data Offset
    u32StartAddr = u32StartAddr + u32FirstSegADDR;
    Serial.println(u32StartAddr);
    // 1st Segment Data
    // byte 0x0~0x3 	: 1st ISP Address
    // byte 0x4~0x7 	: 1st Data Size
    // byte 0x8~0x9		: 1st checksum
    // byte 0xA~0xD 	: product ID
    // byte 0xE~0x11	: Chip PDID
    Flash_Read(&g_sFlash, u32StartAddr, g_au8Buf, 18);
    u32ISPADDR = (g_au8Buf[0]<<24) + (g_au8Buf[1]<<16) + (g_au8Buf[2]<<8) + g_au8Buf[3];     // �õ���һ�������д���ַ
    u32ISPReadADDR = u32ISPADDR;                      // get 1st Segment ISP write address
    u32ISPSize = (g_au8Buf[4]<<24) + (g_au8Buf[5]<<16) +  (g_au8Buf[6]<<8) + g_au8Buf[7];     // �õ���һ�������д�볤��?
                                                      // get 1st Segment ISP write size
    u16ISPChecksumFromFile = (g_au8Buf[8]<<8) + g_au8Buf[9];				     // �õ���һ�������checksum	
                                                      // get 1st Segment checksum		
    u32NSP_ID = (g_au8Buf[10]<<24)+ (g_au8Buf[11]<<16)+ (g_au8Buf[12]<<8) +  g_au8Buf[13];   // �õ�product ID
                                                      // get product ID
    u32CHIP_ID = (g_au8Buf[14]<<24)+ (g_au8Buf[15]<<16)+ (g_au8Buf[16]<<8) +  g_au8Buf[17];  // �õ�CHIP_ID
                                                      // get CHIP_ID
    u32StartAddr += 10;
    UART_WaitPlayEND();                            // �ȴ��������?
                                                      // Waiting for playback to finish
    //u32CHIP_ID = 0xFFFFFFFF;                          //������õ��f���NSPоƬ��Ո�Ƴ�//
                                                        //If user get the old NSP chips please remove //    
    // N_AUTO_SLEEP_DIS();                                 // GUI�趨sleep time�������£��ر��Զ����߹��ܣ�ֻ��N_PWR_DOWN������˯
    //comment out                                                    // GUIû���趨sleep time��ָ����Ч    
                                                        // (If user sets the sleep time in the GUI), disable the automatic sleep function, only N_PWR_DOWN can enter sleep
                                                        // The command is invalid if user does not set the sleep time in the GUI
    u8RightCMD = N_ISP_WRITE_START(u32NSP_ID,u32CHIP_ID);	// ���product ID�ĸ�2bytes��CHIP_ID�Ƿ�һ�£�һ�µĻ���ISPд��׼��
    						      //����N_ISP_WRITE_START��N_ISP_WRITE_PAGE����60us~300us
                                                      // Check whether the high 2bytes of product ID and CHIP_ID are  are the same. 
                                                      // If they are consistent, do the ISP write preparation.
                                                      //It is recommended that the interval between N_ISP_WRITE_START and N_ISP_WRITE_PAGE be 60us ~ 300us
    Serial.println(u8RightCMD);
    if ( u8RightCMD == 1 )
    {
        Serial.print(u32StartAddr);  
        while(u32ISPSizeDone < u32ISPSize)            // ���ISPд����̣�ÿ��д��?512�ֽ�
                                                       // Complete the ISP write process, writing 512 bytes each time
        {
            Flash_Read(&g_sFlash, u32StartAddr, g_au8Buf, FLASH_PAGE_SIZE);
            u32StartAddr += FLASH_PAGE_SIZE;
            ISP_BUFFER = g_au8Buf;
            N_ISP_WRITE_PAGE(u32ISPADDR,ISP_BUFFER);  // ISPд��512�ֽ�
                                                      // ISP write 512 bytes
            u32ISPADDR += FLASH_PAGE_SIZE;
            u32ISPSizeDone += FLASH_PAGE_SIZE;
        }
        Serial.println("!!!");
        N_ISP_WRITE_END();                            // ISPд�����? 
        Serial.println("end");                                              // ISP write done
        N_ISP_CHECKSUM(&ISP_CHECKSUM_BYTES);          // �õ�ISP checksum
    }
    
    //u32ISPSize = 0x200;
    //ISP_BUFFER = g_au8Buf;
    //N_ISP_READ_PARTIAL(u32ISPReadADDR,u32ISPSize,ISP_BUFFER);
	Serial.println(ISP_CHECKSUM_BYTES);
    if (ISP_CHECKSUM_BYTES == u16ISPChecksumFromFile) // �ж� ISP д�������Ƿ���ȷ
                                                      // if the ISP writes the data correctly
    {	
        N_PLAY(1);                                    // ���� Index 1 ��Ч 
                                                      // Play Index 1 Resource
        UART_WaitPlayEND();                        // �ȴ��������?
                                                      // Waiting for playback to finish
    }   
    // N_AUTO_SLEEP_EN();                                  // ���Զ����߹��ܣ�GUI�趨sleep timeʱ����û�в��ţ�û����ָ�û��LVD��checksumִ�оͻ���˯
    // comment out                                                    // GUIû���趨sleep time��ָ����Ч    
                                                        // Enable the automatic sleep function:if no play, no new CMD, no LVD and checksum execution 
                                                        // during the sleep time(setting in the GUI),then the NSP will enter sleep
    							// The command is invalid if user does not set the sleep time in the GUI
    //while(1);
    // comment out 
}

/*----- NSP ISP Update one resource Sample -----*/
void UART_ISPUpdateOneResourceSample(void)
{
    UINT32 u32StartAddr = 0;                            // ������ʼ��
    UINT32 u32ChunkCount = 0;
    UINT32 u32ChunkSize = 0;
    UINT32 u32ChunkType = 0;
    UINT16 u16ISPChecksumFromFile = 0;
    UINT32 u32ISPSize = 0;
    UINT32 i = 0;
    
    UINT32 u32SpaceSize = 0;
    UINT32 u32SpaceSizeUnWrite = 0;
    UINT16 u16FisrtPageDataByte = 0;
    UINT16 u16PageCount = 0;
    UINT16 u16LastPageDataByte = 0;
     
    UINT32 u32ISPADDR = 0;
    PUINT8 ISP_BUFFER = 0;
    UINT8 u8RightCMD = 0;

    UINT8 u8NSP_STATUS1 = 0;
    UINT8 u8IsResourceChunk = 0;	
	
    HOST_SYS_Init();                                   // IO �����ʼ��? 
                                                       // host system initialization
    N_WAKUP();                                          // ����
                                                        // Wake Up                                                     
    N_PLAY(3);                                         // ���� Index 3 ��Ч(����resource index=2)
                                                       // Play Index 3 Resource, including resource index=2
   // xx.bin:
   // byte 0x0~0x3      : Signature (NSPB)
   // byte 0x4~0x7      : Chunk Data Size (4 bytes) 
   // byte 0x8~0xB      : Total Chunk Count(4 bytes)
   // byte 0xC~0xF      : Total Chunk Size(4 bytes)
   // byte 0x10~0x13	: Chunk type(4 bytes) = 0x2
   // byte 0x14~0x17	: Chunk Data Size (4 bytes)
   // byte 0x18~0x19	: Resource Checksum (2 bytes)
    Flash_Read(&g_sFlash, u32StartAddr, g_au8Buf, 16);
    u32ChunkCount = (g_au8Buf[8]<<24) + (g_au8Buf[9]<<16)+ (g_au8Buf[10]<<8) + g_au8Buf[11]; // �õ�Chunk Count
                                                      //get Chunk Count  
    u32StartAddr += 16;                                                   
    while ((u32ChunkCount >0 ) && (i < u32ChunkCount) && ( u8IsResourceChunk ==0 ))
    {
         Flash_Read(&g_sFlash, u32StartAddr, g_au8Buf, 10);
         u32ChunkType = (g_au8Buf[0]<<24) + (g_au8Buf[1]<<16)+ (g_au8Buf[2]<<8) + g_au8Buf[3]; // �õ�Chunk Type
                                                      //get Chunk Type  
         u32ChunkSize = (g_au8Buf[4]<<24) + (g_au8Buf[5]<<16)+ (g_au8Buf[6]<<8) + g_au8Buf[7]; // �õ�Chunk Size
                                                      //get Chunk Size 
         if (u32ChunkType == 0x2 )
         {
                 u32ISPSize =  u32ChunkSize - 2;  
                 u16ISPChecksumFromFile = (g_au8Buf[8]<<8) + g_au8Buf[9];		       // �õ�resource��checksum	
	                                                                                       // get resource checksum  
	         u32StartAddr += 10;
	         u8IsResourceChunk = 1;  
	         i ++;                                          
	 }
	 else
	 {
	 	u32StartAddr = u32StartAddr + 8 + u32ChunkSize ;
	 	i ++;
	 }                                                                                                 
    }                                                  
                                                      
    UART_WaitPlayEND();                            // �ȴ��������?
                                                      // Waiting for playback to finish
     
    N_AUTO_SLEEP_DIS();                                 // GUI�趨sleep time�������£��ر��Զ����߹��ܣ�ֻ��N_PWR_DOWN������˯
                                                        // GUIû���趨sleep time��ָ����Ч    
                                                        // (If user sets the sleep time in the GUI), disable the automatic sleep function, only N_PWR_DOWN can enter sleep
                                                        // The command is invalid if user does not set the sleep time in the GUI	
    u8RightCMD = N_ISP_READ_RES_INDEX(2);	                      // ��ȡresouce index =2 resource������?
                                                      // Read index = 2 resource related information 
   
    if ( u8RightCMD == 1 )
    {                                                   
	    N_ISP_GET_RES_INFO(&u32SpaceSize, &u32ISPADDR, &u16FisrtPageDataByte, &u16PageCount, &u16LastPageDataByte);
	    // �õ�resouce index =2 resource��ʵ�ʿռ�size,��ַ,��һ��ҳ���data��Ŀ,������page��Ŀ,���һ��ҳ���data��Ŀ
	    // Get index = 2 resource related information�� real space size, address, firt page bytes, page count,last page bytes 
	    
	    if ( u32ISPSize <= u32SpaceSize )
	    {
	    	N_ISP_WRITE_PARTIAL_START();                  //ΪISP���ָ�����׼��
	                                                      //����N_ISP_WRITE_PARTIAL_START��N_ISP_WRITE_PARTIAL_BAK����60us~300us
	                                                      //do the ISP write partial preparation.
	                                                      //It is recommended that the interval between N_ISP_WRITE_PARTIAL_START and N_ISP_WRITE_PARTIAL_BAK be 60us ~ 300us
	        u32SpaceSizeUnWrite = u32ISPSize;
	        
	    	if ( u16FisrtPageDataByte > 0 )
	    	{
	    	    if (u16FisrtPageDataByte <= u32ISPSize)
	    	    {
		    	    Flash_Read(&g_sFlash, u32StartAddr, g_au8Buf, u16FisrtPageDataByte);
		            u32StartAddr += u16FisrtPageDataByte;
		            ISP_BUFFER = g_au8Buf;
		            N_ISP_WRITE_PARTIAL_BAK(u32ISPADDR);      //��һ��ҳ�汸�ݣ���ֹӰ��ǰ���resource
		            					      //First page backup to prevent affecting the previous resource
		            N_ISP_WRITE_PARTIAL(u32ISPADDR,u16FisrtPageDataByte,ISP_BUFFER);  // ISPд���һ��ҳ��?
		                                                      // ISP write the first page
		            u32ISPADDR += u16FisrtPageDataByte;
		            u32SpaceSizeUnWrite = u32ISPSize - u16FisrtPageDataByte;
		     }
		     else
		     {
		     	    Flash_Read(&g_sFlash, u32StartAddr, g_au8Buf, u32ISPSize);
		            u32StartAddr += u32ISPSize;
		            ISP_BUFFER = g_au8Buf;
		            N_ISP_WRITE_PARTIAL_BAK(u32ISPADDR);      //��һ��ҳ�汸�ݣ���ֹӰ��ǰ���resource
		            					      //First page backup to prevent affecting the previous resource
		            N_ISP_WRITE_PARTIAL(u32ISPADDR,u32ISPSize,ISP_BUFFER);  // ISPд���һ��ҳ��?
		                                                      // ISP write the first page
		            u32ISPADDR += u32ISPSize;
		            u32SpaceSizeUnWrite = u32ISPSize - u32ISPSize;
		     }
		        
	    	}
	        while( u32SpaceSizeUnWrite >= 0x200 )         // д��������page��ÿ��д��512�ֽ�
	                                                      // Write complete page, write 512 bytes at a time
	        {
	            Flash_Read(&g_sFlash, u32StartAddr, g_au8Buf, FLASH_PAGE_SIZE);
	            u32StartAddr += FLASH_PAGE_SIZE;
	            ISP_BUFFER = g_au8Buf;
	            N_ISP_WRITE_PAGE(u32ISPADDR,ISP_BUFFER);  // ISPд��512�ֽ�
	                                                      // ISP write 512 bytes
	            u32ISPADDR += FLASH_PAGE_SIZE;
	            u32SpaceSizeUnWrite = u32SpaceSizeUnWrite - FLASH_PAGE_SIZE;
	        }
	        if ( u32SpaceSizeUnWrite > 0 )
	    	{
	    	    Flash_Read(&g_sFlash, u32StartAddr, g_au8Buf, u32SpaceSizeUnWrite);
	            u32StartAddr += u32SpaceSizeUnWrite;
	            ISP_BUFFER = g_au8Buf;
	            N_ISP_WRITE_PARTIAL_BAK(u32ISPADDR);      //���һ��ҳ�汸�ݣ���ֹӰ������resource
	            					      //Last page backup to prevent affecting the next resource
	            N_ISP_WRITE_PARTIAL(u32ISPADDR,u32SpaceSizeUnWrite,ISP_BUFFER);  // ISPд���һ��ҳ��?
	                                                      // ISP write the first page
	            u32ISPADDR += u32SpaceSizeUnWrite;
	            u32SpaceSizeUnWrite = u32SpaceSizeUnWrite - u32SpaceSizeUnWrite;
	    	}
	        
	        N_ISP_WRITE_END();                            // ISPд�����? 
	                                                      // ISP write done
	        N_ISP_CHECKSUM(&ISP_CHECKSUM_BYTES);          // �õ�ISP checksum
	        
	        if (ISP_CHECKSUM_BYTES == u16ISPChecksumFromFile) // �ж� ISP д�������Ƿ���ȷ
	                                                         // if the ISP writes the data correctly
	        {	
	            N_PLAY(3);                                    // ���� Index 3 ��Ч(����resource index=2)
	                                                          // Play Index 3 Resource, including resource index=2
	            UART_WaitPlayEND();                        // �ȴ��������?
	                                                         // Waiting for playback to finish
	        }   
	    }
	}
	
    N_AUTO_SLEEP_EN();                                  // ���Զ����߹��ܣ�GUI�趨sleep timeʱ����û�в��ţ�û����ָ�û��LVD��checksumִ�оͻ���˯
                                                        // GUIû���趨sleep time��ָ����Ч    
                                                        // Enable the automatic sleep function:if no play, no new CMD, no LVD and checksum execution 
                                                        // during the sleep time(setting in the GUI),then the NSP will enter sleep
    							// The command is invalid if user does not set the sleep time in the GUI
    while(1);
}

/*----- NSP Updata touch config -----*/
UINT8 UART_TouchUpdataSample(UINT16 u16ConfigSize)
{
    HOST_SYS_Init();                                    // IO ????? 
                                                        // IO Interface Initialization
	Flash_Read(&g_sFlash, 0, g_au8Buf, u16ConfigSize);  // Read Config from SPIFlash.
    N_WAKUP();                                          // ??
                                                        // Wake Up        
    N_AUTO_SLEEP_DIS();                                 // GUI??sleep time????????????????N_PWR_DOWN????
                                                        // GUI????sleep time?????    
                                                        // (If user sets the sleep time in the GUI), disable the automatic sleep function, only N_PWR_DOWN can enter sleep
                                                        // The command is invalid if user does not set the sleep time in the GUI													
	 // Send config to NSP. after send, NSP will update this config.
	return N_TOUCH_UPDATA(0, u16ConfigSize, g_au8Buf);   
}

/*----- NSP Load touch config -----*/
UINT8 UART_TouchLoadSample(UINT16 u16ConfigSize)
{
    UINT8 u8RightCMD = 0;
	
    HOST_SYS_Init();                                    // IO ????? 
                                                        // IO Interface Initialization
    N_WAKUP();                                          // ??
                                                        // Wake Up        
    N_AUTO_SLEEP_DIS();                                 // GUI??sleep time????????????????N_PWR_DOWN????
                                                        // GUI????sleep time?????    
                                                        // (If user sets the sleep time in the GUI), disable the automatic sleep function, only N_PWR_DOWN can enter sleep
                                                        // The command is invalid if user does not set the sleep time in the GUI													
	// Send config to NSP. after send, NSP will update this config.
	u8RightCMD = N_TOUCH_LOAD(0, u16ConfigSize, g_au8Buf);   
	// Write into SPIFlash.
	if( u8RightCMD )
	{
		Flash_Write(&g_sFlash, 0, g_au8Buf, u16ConfigSize);
	}
	return u8RightCMD;
}

//===========================================================
/*----- Continue Play Sample -----*/
void UART_ContinuePlaySample(void)
{
    UINT16 temp = 0;                                    // ������ʼ��
                                                        // init parameter  
    UINT8 MultiPlayBuffer[5] = {7,5,3,4,1};             // �趨���ز���˳�����? 
                                                        // Set the multiplay order parameters.
    UINT16 MultiPlay2BBuffer[3] = {6,300,2};  	        // �趨���ز���˳�����? 
                                                        // Set the multiplay order parameters.                                                                             	
    HOST_SYS_Init();                                    // IO�����ʼ��?
                                                        // IO Interface Initialization
    N_AUTO_SLEEP_DIS();                                 // GUI�趨sleep time�������£��ر��Զ����߹��ܣ�ֻ��N_PWR_DOWN������˯
                                                        // GUIû���趨sleep time��ָ����Ч    
                                                        // (If user sets the sleep time in the GUI), disable the automatic sleep function, only N_PWR_DOWN can enter sleep
                                                        // The command is invalid if user does not set the sleep time in the GUI
    N_PLAY(1);                                          // ���ű����� Index 1 ��Ч 
                                                        // Play Index 1 Resource
    N_PLAY_REPEAT(2);                                   // ѭ������ Index 1 ��Ч2��
                                                        // repeat 2 times                                                    
    for (temp= 0; temp < 16000; temp++)                 // ���� 8 ����
    {                                                   // play for 8 seconds
        HOST_Delay500uS();     
    }    
    N_PAUSE(); 						// ��ͣ����Index 1 ��Ч
                                                        // pause play Index 1
                                                        
    N_MULTI_PLAY(5,&MultiPlayBuffer[0]);                // �趨5�ض��鲥�ţ�������ʼ�� Buffer[0]��ʼ 
                                                        // Set 5 groups of multi-play, starting playback starts with Buffer[0].
    for (temp= 0; temp < 8000; temp++)                  // ���� 4 ����
    {                                                   // play for 4 seconds
        HOST_Delay500uS();     
    }   
    
    N_MULTI_PLAY_2B(3,&MultiPlay2BBuffer[0]);           // �趨3�ض��鲥�ţ�������ʼ�� 2BBuffer[0]��ʼ 
                                                        // Set 3 groups of multi-play, starting playback starts with 2BBuffer[0].
    UART_WaitPlayEND();                                 // �ȴ��������?
                                                        // Waiting for playback to finish
    
    N_RESUME(); 					// ��������Index 1 ��Ч
                                                        // resume play Index 1                                                   
    UART_WaitPlayEND();                                 // �ȴ��������?
                                                        // Waiting for playback to finish
    N_AUTO_SLEEP_EN();                                  // ���Զ����߹��ܣ�GUI�趨sleep timeʱ����û�в��ţ�û����ָ�û��LVD��checksumִ�оͻ���˯
                                                        // GUIû���趨sleep time��ָ����Ч    
                                                        // Enable the automatic sleep function:if no play, no new CMD, no LVD and checksum execution 
                                                        // during the sleep time(setting in the GUI),then the NSP will enter sleep
    							// The command is invalid if user does not set the sleep time in the GUI
    while(1);
}

//===========================================================
/*----- NSP ISP Update user data Sample -----*/
UINT8 UART_UserDataWrite(UINT32 UserWrite_ADDR,PUINT8 ISP_BUFFER,UINT16 WriteSize)
{
    UINT8 RTN = 0;
    UINT32 u32ISPWriteADDR = 0;
    UINT32 u32SpaceSizeUnWrite = 0;
    UINT32 u32ISPADDR = 0;
    UINT32 u32SpaceSize = 0;				//���ݿռ���? 
    UINT16 u16FisrtPageDataByte = 0;
    UINT16 u16PageCount = 0;
    UINT16 u16LastPageDataByte = 0;
    	
    HOST_SYS_Init();                                   // IO �����ʼ��? HOST_BUS_Init();?
                                                       // IO Interface Initialization
    
    N_ISP_GET_USER_SPACE_INFO(&u32SpaceSize, &u32ISPADDR, &u16FisrtPageDataByte, &u16PageCount, &u16LastPageDataByte);
	    // �õ��û����ݿռ���?,��ַ,��һ��ҳ���data��Ŀ,������page��Ŀ,���һ��ҳ���data��Ŀ
	    // Get user space related information�� real space size, address, firt page bytes, page count,last page bytes 
	    
    u32ISPWriteADDR = u32ISPADDR + UserWrite_ADDR;
    u32SpaceSizeUnWrite = WriteSize;
    
    if ((WriteSize == 0) || (u32SpaceSize == 0))
    	return 0;
    if ((WriteSize > u32SpaceSize) || ((u32ISPWriteADDR + WriteSize) > (u32ISPADDR + u32SpaceSize)))
    	return 0;
    	
    RTN = N_ISP_WRITE_PARTIAL_START();                  //ΪISP���ָ�����׼��
                                                      //����N_ISP_WRITE_PARTIAL_START��N_ISP_WRITE_PARTIAL_BAK����60us~300us
                                                      //do the ISP write partial preparation.
                                                      //It is recommended that the interval between N_ISP_WRITE_PARTIAL_START and N_ISP_WRITE_PARTIAL_BAK be 60us ~ 300us
    if ( RTN  == 1)
    {    
    	u16FisrtPageDataByte = 0x200 - (u32ISPWriteADDR & 0x1FF);
    	if (( u16FisrtPageDataByte > 0 ) && ( u16FisrtPageDataByte < 0x200 ))
    	{
    	    RTN = N_ISP_WRITE_PARTIAL_BAK(u32ISPWriteADDR);      //��һ��ҳ�汸�ݣ���ֹӰ��ǰ���resource
	            					      //First page backup to prevent affecting the previous resource
	    if ( RTN == 0 )
	            return 0;        					      
    	    if (u16FisrtPageDataByte <= u32SpaceSizeUnWrite)
    	    {	            
	            RTN = N_ISP_WRITE_PARTIAL(u32ISPWriteADDR,u16FisrtPageDataByte,ISP_BUFFER);  // ISPд���һ��ҳ��?
	                                                      // ISP write the first page
	            if ( RTN == 0 )
	            return 0;  
	            u32ISPWriteADDR += u16FisrtPageDataByte;
	            u32SpaceSizeUnWrite -= u16FisrtPageDataByte;
	     }
	     else
	     {
	            RTN = N_ISP_WRITE_PARTIAL(u32ISPWriteADDR,u32SpaceSizeUnWrite,ISP_BUFFER);  // ISPд���һ��ҳ��?
	                                                      // ISP write the first page
	            if ( RTN == 0 )
	            return 0;  
	            u32ISPWriteADDR += u32SpaceSizeUnWrite;
	            u32SpaceSizeUnWrite -= u32SpaceSizeUnWrite;
	     }
	        
    	}
        while( u32SpaceSizeUnWrite >= 0x200 )         // д��������page��ÿ��д��512�ֽ�
                                                      // Write complete page, write 512 bytes at a time
        {
            RTN = N_ISP_WRITE_PAGE(u32ISPWriteADDR,ISP_BUFFER);  // ISPд��512�ֽ�
                                                      // ISP write 512 bytes
            if ( RTN == 0 )
	            return 0;  
            u32ISPWriteADDR += FLASH_PAGE_SIZE;
            u32SpaceSizeUnWrite -= FLASH_PAGE_SIZE;
        }
        if ( u32SpaceSizeUnWrite > 0 )
    	{
            RTN = N_ISP_WRITE_PARTIAL_BAK(u32ISPWriteADDR);      //���һ��ҳ�汸�ݣ���ֹӰ������resource
            					      //Last page backup to prevent affecting the next resource
            if ( RTN == 0 )
	            return 0;  
            RTN = N_ISP_WRITE_PARTIAL(u32ISPWriteADDR,u32SpaceSizeUnWrite,ISP_BUFFER);  // ISPд���һ��ҳ��?
                                                      // ISP write the first page
            if ( RTN == 0 )
	            return 0;  
            u32ISPWriteADDR += u32SpaceSizeUnWrite;
            u32SpaceSizeUnWrite -= u32SpaceSizeUnWrite;
    	}
        
    	RTN = N_ISP_WRITE_END();                            // ISPд�����? 
                                                      	    // ISP write done
        if ( RTN == 0 )
	      return 0;   
	                                                    	  
    	N_ISP_CHECKSUM(&ISP_CHECKSUM_BYTES);          // �õ�ISP checksum
    }
    return RTN;	
}

/*----- NSP ISP read user data Sample -----*/
UINT8 UART_UserDataRead(UINT32 UserRead_ADDR,PUINT8 ISP_BUFFER,UINT16 ReadSize)
{
    UINT8 RTN = 0;
    UINT32 u32ISPReadADDR = 0;
    UINT32 u32SpaceSizeUnRead = 0;
    UINT32 u32ISPADDR = 0;
    UINT32 u32SpaceSize = 0;				//���ݿռ���?
    UINT16 u16FisrtPageDataByte = 0;
    UINT16 u16PageCount = 0;
    UINT16 u16LastPageDataByte = 0;
	
    HOST_SYS_Init();                                   // IO �����ʼ��? HOST_BUS_Init();?
                                                       // IO Interface Initialization
    
    N_ISP_GET_USER_SPACE_INFO(&u32SpaceSize, &u32ISPADDR, &u16FisrtPageDataByte, &u16PageCount, &u16LastPageDataByte);
	    // �õ��û����ݿռ���?,��ַ,��һ��ҳ���data��Ŀ,������page��Ŀ,���һ��ҳ���data��Ŀ
	    // Get user space related information�� real space size, address, firt page bytes, page count,last page bytes 

    u32ISPReadADDR = u32ISPADDR + UserRead_ADDR;
    u32SpaceSizeUnRead = ReadSize;
    
    if ((ReadSize == 0) || (u32SpaceSize == 0))
    	return 0;
    if ((ReadSize > u32SpaceSize) || ((u32ISPReadADDR + ReadSize) > (u32ISPADDR + u32SpaceSize)))
    	return 0;
    
    u16FisrtPageDataByte = 0x200 - (u32ISPReadADDR & 0x1FF);	
    if (( u16FisrtPageDataByte > 0 ) && ( u16FisrtPageDataByte < 0x200 ))
    	{
    	    if (u16FisrtPageDataByte <= u32SpaceSizeUnRead)
    	    {
    	    	    RTN = N_ISP_READ_PARTIAL(u32ISPReadADDR,u16FisrtPageDataByte,ISP_BUFFER);  //��ȡָ����ַ��ʼ��ָ���ֽ�����(�ֽ���С��512),���뻺��
                                                     //Read the specified byte data at the beginning of the specified address (the number of bytes is less than 512), and store it in the buffer
	            if ( RTN == 0 )
	            	return 0;
	            u32ISPReadADDR += u16FisrtPageDataByte;
	            u32SpaceSizeUnRead -=  u16FisrtPageDataByte;
	     }
	     else
	     {
	     	    RTN = N_ISP_READ_PARTIAL(u32ISPReadADDR,ReadSize,ISP_BUFFER);  //��ȡָ����ַ��ʼ��ָ���ֽ�����(�ֽ���С��512),���뻺��
                                                     //Read the specified byte data at the beginning of the specified address (the number of bytes is less than 512), and store it in the buffer
	            if ( RTN == 0 )
	            	return 0;
	            u32ISPReadADDR += ReadSize;
	            u32SpaceSizeUnRead -= ReadSize;
	     }
	        
    	}
        while( u32SpaceSizeUnRead >= 0x200 )         // д��������page��ÿ��д��512�ֽ�
                                                      // Write complete page, write 512 bytes at a time
        {
            RTN = N_ISP_READ_PAGE(u32ISPReadADDR,ISP_BUFFER); //��ȡָ����ַ��ʼ��һ��ҳ��(512�ֽ�)������,���뻺��
                                                     //Read the data of a page (512 bytes) starting at the specified address and store it in the buffer
            if ( RTN == 0 )
	        return 0;
            u32ISPReadADDR += FLASH_PAGE_SIZE;
            u32SpaceSizeUnRead -= FLASH_PAGE_SIZE;
        }
        if ( u32SpaceSizeUnRead > 0 )
    	{
    	    
    	    RTN = N_ISP_READ_PARTIAL(u32ISPReadADDR,u32SpaceSizeUnRead,ISP_BUFFER);  //��ȡָ����ַ��ʼ��ָ���ֽ�����(�ֽ���С��512),���뻺��
                                                     //Read the specified byte data at the beginning of the specified address (the number of bytes is less than 512), and store it in the buffer
	    if ( RTN == 0 )
	        return 0;
	    u32ISPReadADDR += u32SpaceSizeUnRead;
	    u32SpaceSizeUnRead -= u32SpaceSizeUnRead;
    	}
    return RTN;	
}

/*----- NSP Provide space to store MCU user data -----*/
void UART_ISPUserSpaceWriteAndRead(void)
{							// ������ʼ��
    UINT8 RTN = 0;					// ������ʼ��

    UINT8 g_au8Buf_Write[10]= {0x5A,0xA5,0x55,0xAA,0x9C,0x87,0x2B,0x64,0xF7,0x96};

    RTN = UART_UserDataWrite(2,&g_au8Buf_Write[0],1);  
    RTN = UART_UserDataRead(2,&g_au8Buf_Read[5],1);
    
    RTN = UART_UserDataWrite(0,&g_au8Buf[0],512);  
    RTN = UART_UserDataRead(0,&g_au8Buf_Read[0],512);								
    
    while(1);
}