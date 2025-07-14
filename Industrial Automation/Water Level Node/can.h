typedef struct can_message 
{
	int id;
	char rtr;
	char dlc;
	int byteA;

}CAN2_MSG;

CAN2_MSG m1_tx;
CAN2_MSG m1_rx;


void can2_init(void){

	PINSEL1|=0x00014000;//P0.23-->RD2 & P0.24-->TD2
	VPBDIV=1; //PCLK=60MHz
	C2MOD=0x1; //CAN2 into Reset  Mode 
	AFMR=0x2; //accept all receiving messages(data/remote)
	C2BTR=0x001C001D; //B125Kbps @ PLCK=60MHz(BRP=Pclk/bit rate*16)
	C2MOD=0x0; //CAN1 into Normal Mode

}


void can2_rx(CAN2_MSG *m1_rx){

	while((C2GSR&0x1)==0);
	m1_rx->id=C2RID;
	m1_rx->dlc=(C2RFS>>16)&0xF;
	m1_rx->rtr=(C2RFS>>30)&0x1;
	if(m1_rx->rtr==0){ //if data frame

		m1_rx->byteA=C2RDA;
	}
	C2CMR=(1<<2);//free receiver buffer(imp)

}

void can2_tx(CAN2_MSG m1_tx)
{

  	C2TID1=m1_tx.id;
	C2TFI1=(m1_tx.dlc<<16);	
	if(m1_tx.rtr==0) //if data frame
	{
		C2TFI1&=~(1<<30);//RTR=0
		C2TDA1=m1_tx.byteA; //lower 4bytes of data
	}
	else
	{
		C2TFI1|=(1<<30); //RTR=1
	}
	C2CMR=(1<<0)|(1<<5);//Start Xmission & select Tx Buf1
	//C2CMR=(3<<0)|(1<<5);//Start Xmission & select Tx Buf1
	while((C2GSR&(1<<3))==0);//wait for data Xmission.

}
