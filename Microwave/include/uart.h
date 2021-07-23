#define UART0_RCV_BUFF_SIZE  50
//UART_0
unsigned char   UART0_R_Data[UART0_RCV_BUFF_SIZE];
unsigned char   UART0_R_Count = 0;
unsigned char   UART0_heder_flag = 0;

char getchar_0();
void putchar_0(int ich);