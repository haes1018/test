/////////////////////////////
//	COMM_CONF.C
/////////////////////////////
#define COMM_CONF_GLOBALS
#include "includes.h"
/////////////////////////////

// comm_conf2.c 1st changed....
// comm_conf2.c 1st changed....2nd..... for void commit message

/////////////////////////////
// Local Variables
/////////////////////////////
static INT8U DUMMY_SYNC_DATA[14] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x16, 0x16, 0x16, 0x16 };

/////////////////////////////
// Local Functions
/////////////////////////////

/////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
//
// Functions Definitions
//
///////////////////////////////////////////////////////////////////////////////////////


// ST2550���� �ܺ� ���ͷ�Ʈ�� ȣ�� �Ǵ� �Լ�
void EXTI0_INT( void )
{
	ST2550_Int( COMM_2G_LTE, SCC_UART1_CS1_ADDR ); // ST2550 ���ͷ�Ʈ ó�� ��ƾ
}
void EXTI1_INT( void )
{
	ST2550_Int( COMM_LTEA01, SCC_UART1_CS2_ADDR ); // ST2550 ���ͷ�Ʈ ó�� ��ƾ
}



void UDP_Reset( void )
{	
	TCP_Reset_Low();	// TCP_Reset
	OSTimeDly( OS_DELAY_1msec );
	TCP_Reset_High();	// TCP_Reset
	
	Debug_PutString( "UDP Reset", 1 );	
}

void SaveSvrInfo( INT8U* svr_ip_addr, INT16U svr_port )
{
/*	
	memcpy( m_Eep.svr_ip, svr_ip_addr, 4 );
	m_Eep.svr_port = svr_port;
	m_Env.is_eep_write = EEP_WRITE_CODE;
	Debug_PutString( "IP Cange", 1 );
*/	
}

void PreTxData( INT8U port, INT8U *data )
{
	switch( port )
	{			
		case COMM_TERM:		
			COM_LED_1_ON();			
			break;

		case COMM_DIGI:
			COM_LED_2_ON();
			break;	

//		case COMM_DIGI_CM:
//			COM_LED_3_ON();
//			break;

		case COMM_UDP_NODE:
			//==========
			#if (defined __GSI_Q_RO__ )||(defined __CS_New_Q_RO__ ) // #ifdef __GSI_Q_RO__ 			
			LED_Ctrl(_LINK_LED_, _LED_NORMAL_); 		
			#else // SOL, CS_New
			Comm_LED_Ctrl( TX_LED, ON );
			#endif
			//==========
			break;


		default: return;
	}	
	OSTimeDly( OS_DELAY_10msec );			
}

void EndTxData( INT8U port )
{
	switch( port )
	{			
		case COMM_TERM:		
			COM_LED_1_OFF();
			break;

		case COMM_DIGI:	
			COM_LED_2_OFF();
			break;

//		case COMM_DIGI_CM:	
//			COM_LED_3_OFF();			
//			break;	

		case COMM_UDP_NODE:
			//==========
			#if (defined __GSI_Q_RO__ )||(defined __CS_New_Q_RO__ ) // #ifdef __GSI_Q_RO__ 			
			LED_Ctrl(_LINK_LED_, _LED_ALARM_); 		
			#else // SOL, CS_New
			Comm_LED_Ctrl( TX_LED, OFF );
			#endif
			//==========
		break;
		
	}
}

// ���� Unit���� ��� ������ ���� ��.
void Recv_DN_Data( INT8U port, INT8U *ack )
{	
	switch( port )
	{
/*		case :
			COM_LED_3_ON();
			OSTimeDly( OS_DELAY_10msec );		
			COM_LED_3_OFF();
			break;*/	
	}
}


// Terminal�κ����� ��Ŷ ó��.
INT16U Recv_UP_Data( INT8U port, INT8U *txf, INT8U* rxf, INT8U* pRx_IP )
{
	switch( port )
	{
		case COMM_DIGI:
			COM_LED_2_ON();	
			break;		
			
		case COMM_TERM:
			COM_LED_1_ON();
			break;

		case COMM_UDP_NODE:
			//==========
			#if (defined __GSI_Q_RO__ )||(defined __CS_New_Q_RO__ ) // #ifdef __GSI_Q_RO__
			LED_Ctrl(_LINK_LED_, _LED_NORMAL_); 		
			#else // SOL, CS_New
			Comm_LED_Ctrl( RX_LED, ON );
			#endif
			//==========
			break;
	}

	OSTimeDly( OS_DELAY_10msec );

	switch( port )
	{
		case COMM_DIGI:
			COM_LED_2_OFF();
			break;		
			
		case COMM_TERM:
			COM_LED_1_OFF();
			break;

		case COMM_UDP_NODE:
			//==========
			#if (defined __GSI_Q_RO__ )||(defined __CS_New_Q_RO__ ) // #ifdef __GSI_Q_RO__ 			
			LED_Ctrl(_LINK_LED_, _LED_ALARM_); 		
			#else // SOL, CS_New
			Comm_LED_Ctrl( RX_LED, OFF );
			#endif
			//==========
			break;
	}

	switch( port )
	{
		case COMM_TERM:
		case COMM_UDP_NODE:		
			return UP_Data_Analysis( (PROTOCOL_FRAME*)rxf, (PROTOCOL_FRAME*)txf, port, pRx_IP );
	}
	return 0;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// �� �ø��� ��Ʈ�� ��Ʈ ����, �ּҼ���, �ӵ� ����, ó���Լ� ����...
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void Init_Comm( INT8U port )
{
	switch( port )
	{
		case COMM_TERM:
			Init_STM_Comm( port, (INT32U)USART1, COMM_TYPE_STM_UART, 38400, (PTR_GetFrame*)GetFrame, &DUMMY_SYNC_DATA[10], 4 );
			break;

		case COMM_DIGI:
			Init_STM_Comm( port, (INT32U)USART2, COMM_TYPE_STM_UART, 115200, (PTR_GetFrame*)GetFrame, &DUMMY_SYNC_DATA[10], 4 );			
			break;

//		case COMM_DIGI_CM:
//			Init_STM_Comm( port, (INT32U)USART3, COMM_TYPE_STM_UART, 38400, (PTR_GetFrame*)GetFrame, &DUMMY_SYNC_DATA[10], 4 );
//			break;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifdef __CS_New_Q_RO__
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		case COMM_3G_WiB:
			Init_STM_Comm( port, (INT32U)UART4, COMM_TYPE_STM_UART, 38400, (PTR_GetFrame*)GetFrame_PAU, &DUMMY_SYNC_DATA[10], 4 );
			break;	

		case COMM_2G_LTE:
			Init2550_Int( port, GPIO_PortSourceGPIOC, 0, OS_INT_ID_EXTI0, EXTI0_INT );
			Init_STM_Comm( port, SCC_UART1_CS1_ADDR, COMM_TYPE_2550_UART, 38400, (PTR_GetFrame*)GetFrame_PAU, &DUMMY_SYNC_DATA[10], 4 );
			break;	

		case COMM_LTEA01:
			Init2550_Int( port, GPIO_PortSourceGPIOC, 1, OS_INT_ID_EXTI1, EXTI1_INT );
			Init_STM_Comm( port, SCC_UART1_CS2_ADDR, COMM_TYPE_2550_UART, 38400, (PTR_GetFrame*)GetFrame_PAU, &DUMMY_SYNC_DATA[10], 4 );
			break;	
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#else
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		case COMM_3G_WiB:
			Init_STM_Comm( port, (INT32U)UART4, COMM_TYPE_STM_UART, 115200, (PTR_GetFrame*)GetFrame_PAU, &DUMMY_SYNC_DATA[10], 4 );
			break;

		case COMM_2G_LTE:
			Init2550_Int( port, GPIO_PortSourceGPIOC, 0, OS_INT_ID_EXTI0, EXTI0_INT );
			Init_STM_Comm( port, SCC_UART1_CS1_ADDR, COMM_TYPE_2550_UART, 115200, (PTR_GetFrame*)GetFrame_PAU, &DUMMY_SYNC_DATA[10], 4 );
			break;	

		case COMM_LTEA01:
			Init2550_Int( port, GPIO_PortSourceGPIOC, 1, OS_INT_ID_EXTI1, EXTI1_INT );
			Init_STM_Comm( port, SCC_UART1_CS2_ADDR, COMM_TYPE_2550_UART, 115200, (PTR_GetFrame*)GetFrame_PAU, &DUMMY_SYNC_DATA[10], 4 );
			break;	
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#endif
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		case COMM_UDP_NODE:
			UDP_IP_Setting( port, m_Env.rep_ip, m_Env.gw, m_Env.sm, m_Env.mac, m_Env.svr_ip, m_Env.svr_port );
			Init_STM_Comm( port, 0, COMM_TYPE_UDP, 0, (PTR_GetFrame*)GetFrame, &DUMMY_SYNC_DATA[10], 4 );
			break;				
	}
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

