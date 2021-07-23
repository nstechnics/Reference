//Microwave Sens
#define Microwave0_Trig PORTB.0
#define Microwave0_Echo PINB.1

//Microwave Sens
unsigned char   Microwave_sens_ena_flag = 0;
unsigned char   Microwave_Trig_flag = 0;
int				Microwave0_cnt;
int				Microwave0_Distance;