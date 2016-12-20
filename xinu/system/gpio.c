/* gpio.c
 * Simple I/O library for  Beaglebone balck
 *
*/

#include <xinu.h>

/* {pinname, {port, pin}, adcin} */
pintoportpin_t pinmap[93] = {
    { P0, {0, 0}, BBBIO_ADC_INVALID } ,
    { P1, {0, 0}, BBBIO_ADC_INVALID } ,
    { P2, {1, 6}, BBBIO_ADC_INVALID } ,
    { P3, {1, 7}, BBBIO_ADC_INVALID } ,
    { P4, {1, 2}, BBBIO_ADC_INVALID } ,
    { P5, {1, 3}, BBBIO_ADC_INVALID } ,
    { P6, {0, 0}, BBBIO_ADC_INVALID } ,
    { P7, {0, 0}, BBBIO_ADC_INVALID } ,
    { P8, {0, 0}, BBBIO_ADC_INVALID } ,
    { P9, {0, 0}, BBBIO_ADC_INVALID } ,
    { P10, {1, 13}, BBBIO_ADC_INVALID } ,
    { P11, {1, 12}, BBBIO_ADC_INVALID } ,
    { P12, {0, 0}, BBBIO_ADC_INVALID } ,
    { P13, {0, 26}, BBBIO_ADC_INVALID } ,
    { P14, {1, 15}, BBBIO_ADC_INVALID } ,
    { P15, {1, 14}, BBBIO_ADC_INVALID } ,
    { P16, {0, 27}, BBBIO_ADC_INVALID } ,
    { P17, {2, 1}, BBBIO_ADC_INVALID } ,
    { P18, {0, 0}, BBBIO_ADC_INVALID } ,
    { P19, {1, 31}, BBBIO_ADC_INVALID } ,
    { P20, {1, 30}, BBBIO_ADC_INVALID } ,
    { P21, {1, 5}, BBBIO_ADC_INVALID } ,
    { P22, {1, 4}, BBBIO_ADC_INVALID } ,
    { P23, {1, 1}, BBBIO_ADC_INVALID } ,
    { P24, {1, 0}, BBBIO_ADC_INVALID } ,
    { P25, {1, 29}, BBBIO_ADC_INVALID } ,
    { P26, {2, 22}, BBBIO_ADC_INVALID } ,
    { P27, {2, 24}, BBBIO_ADC_INVALID } ,
    { P28, {2, 23}, BBBIO_ADC_INVALID } ,
    { P29, {2, 25}, BBBIO_ADC_INVALID } ,
    { P30, {0, 0}, BBBIO_ADC_INVALID } ,
    { P31, {0, 0}, BBBIO_ADC_INVALID } ,
    { P32, {0, 0}, BBBIO_ADC_INVALID } ,
    { P33, {0, 0}, BBBIO_ADC_INVALID } ,
    { P34, {0, 0}, BBBIO_ADC_INVALID } ,
    { P35, {0, 0}, BBBIO_ADC_INVALID } ,
    { P36, {0, 0}, BBBIO_ADC_INVALID } ,
    { P37, {0, 0}, BBBIO_ADC_INVALID } ,
    { P38, {2, 12}, BBBIO_ADC_INVALID } ,
    { P39, {2, 13}, BBBIO_ADC_INVALID } ,
    { P40, {2, 10}, BBBIO_ADC_INVALID } ,
    { P41, {2, 11}, BBBIO_ADC_INVALID } ,
    { P42, {2, 8}, BBBIO_ADC_INVALID } ,
    { P43, {2, 9}, BBBIO_ADC_INVALID } ,
    { P44, {2, 6}, BBBIO_ADC_INVALID } ,
    { P45, {2, 7}, BBBIO_ADC_INVALID } ,
    { P46, {0, 0}, BBBIO_ADC_INVALID } ,
    { P47, {0, 0}, BBBIO_ADC_INVALID } ,
    { P48, {0, 0}, BBBIO_ADC_INVALID } ,
    { P49, {0, 0}, BBBIO_ADC_INVALID } ,
    { P50, {0, 0}, BBBIO_ADC_INVALID } ,
    { P51, {0, 0}, BBBIO_ADC_INVALID } ,
    { P52, {0, 0}, BBBIO_ADC_INVALID } ,
    { P53, {0, 0}, BBBIO_ADC_INVALID } ,
    { P54, {0, 0}, BBBIO_ADC_INVALID } ,
    { P55, {0, 0}, BBBIO_ADC_INVALID } ,
    { P56, {0, 0}, BBBIO_ADC_INVALID } ,
    { P57, {1, 28}, BBBIO_ADC_INVALID } ,
    { P58, {0, 0}, BBBIO_ADC_INVALID } ,
    { P59, {0, 0}, BBBIO_ADC_INVALID } ,
    { P60, {1, 16}, BBBIO_ADC_INVALID } ,
    { P61, {0, 0}, BBBIO_ADC_INVALID } ,
    { P62, {0, 0}, BBBIO_ADC_INVALID } ,
    { P63, {0, 0}, BBBIO_ADC_INVALID } ,
    { P64, {0, 0}, BBBIO_ADC_INVALID } ,
    { P65, {0, 0}, BBBIO_ADC_INVALID } ,
    { P66, {0, 0}, BBBIO_ADC_INVALID } ,
    { P67, {0, 0}, BBBIO_ADC_INVALID } ,
    { P68, {1, 17}, BBBIO_ADC_INVALID } ,
    { P69, {0, 0}, BBBIO_ADC_INVALID } ,
    { P70, {3, 21}, BBBIO_ADC_INVALID } ,
    { P71, {0, 0}, BBBIO_ADC_INVALID } ,
    { P72, {3, 19}, BBBIO_ADC_INVALID } ,
    { P73, {0, 0}, BBBIO_ADC_INVALID } ,
    { P74, {0, 0}, BBBIO_ADC_INVALID } ,
    { P75, {0, 0}, BBBIO_ADC_INVALID } ,
    { P76, {0, 0}, BBBIO_ADC_INVALID } ,
    { P77, {0, 0}, BBBIO_ADC_INVALID } ,
    { P78, {0, 0}, BBBIO_ADC_AIN4 } ,
    { P79, {0, 0}, BBBIO_ADC_INVALID } ,
    { P80, {0, 0}, BBBIO_ADC_AIN6 } ,
    { P81, {0, 0}, BBBIO_ADC_AIN5 } ,
    { P82, {0, 0}, BBBIO_ADC_AIN2 } ,
    { P83, {0, 0}, BBBIO_ADC_AIN3 } ,
    { P84, {0, 0}, BBBIO_ADC_AIN0 } ,
    { P85, {0, 0}, BBBIO_ADC_AIN1 } ,
    { P86B, {3, 20}, BBBIO_ADC_INVALID } ,
    { P87A, {0, 7}, BBBIO_ADC_INVALID } ,
    { P87B, {3, 18}, BBBIO_ADC_INVALID } ,
    { P88, {0, 0}, BBBIO_ADC_INVALID } ,
    { P89, {0, 0}, BBBIO_ADC_INVALID } ,
    { P90, {0, 0}, BBBIO_ADC_INVALID } ,
    { P91, {0, 0}, BBBIO_ADC_INVALID } ,
};


/* Memory mapping offset if GPIO , means the memory address of GPIOs */
const unsigned int GPIO_AddressOffset[]={BBBIO_GPIO0_ADDR, BBBIO_GPIO1_ADDR, BBBIO_GPIO2_ADDR, BBBIO_GPIO3_ADDR};

/* GPIO Port number set of Beaglebone Black P8 .
 * -1 as GND or VCC , Number 0/1/2 as GPIO 0/1/2
 */
const signed char p8_PortSet[] = {
                                    -1, -1, 1, 1, 1, 1, 2, 2,
                                    2, 2, 1, 1, 0, 0, 1, 1, 
                                    0, 2, 0, 1, 1, 1, 1, 1, 
                                    1, 1, 2, 2, 2, 2, 0, 0, 
                                    0, 2, 0, 2, 2, 2, 2, 2, 
                                    2, 2, 2, 2, 2, 2
                                };

/* GPIO Port ID set of Beaglebone Black P8  ,
 * 0 as GND , offset X as GPIO ID m this value must combine with GPIO number
 */
const unsigned int p8_PortIDSet[] = {
                                        0,	0,	1<<6,	1<<7,	1<<2,	1<<3,	1<<2,	1<<3,	
                                        1<<5,	1<<4,	1<<13,	1<<12,	1<<23,	1<<26,	1<<15,	
                                        1<<14,	1<<27,	1<<1,	1<<22,	1<<31,	1<<30,	1<<5,	
                                        1<<4,	1<<1,	1<<0,	1<<29,	1<<22,	1<<24,	1<<23,	
                                        1<<25,	1<<10,	1<<11,	1<<9,	1<<17,	1<<8,	1<<16,	
                                        1<<14,	1<<15,	1<<12,	1<<13,	1<<10,	1<<11,	1<<8,	
                                        1<<9,	1<<6,	1<<7
                                    };

/* GPIO Port number set of Beaglebone Black P9  ,
 * -1 as GND or VCC , 0 / 1 / 2 for GPIO port 0/1/2
 */
const signed char p9_PortSet[] = {
                                    -1, -1, -1, -1, -1, -1, -1, -1,
                                    -1, -1,  0,  1,  0,  1,  1,  1,
                                     0,  0,  0,  0,  0,  0,  1,  0, 
                                     3,  0,  3,  3,  3,  3,  3, -1,
                                    -1, -1, -1, -1, -1, -1, -1, -1, 
                                     0,  0, -1, -1, -1, -1
                                };

/*  GPIO Port ID set of Beaglebone Black P9  ,
 * 0 as GND , offset X as GPIO ID m this value must combine with GPIO number
 */
const unsigned int p9_PortIDSet[]={
                                    0,	0,	0,	0,	0,	0,	0,	0,	
                                    0,	0,	1<<30,	1<<28,	1<<31,	1<<18,	1<<16,	1<<19,	
                                    1<<5,	1<<4,	1<<13,	1<<12,	1<<3,	1<<2,	1<<17,	
                                    1<<15,	1<<21,	1<<14,	1<<19,	1<<17,	1<<15,	1<<16,	
                                    1<<14,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1<<20,	
                                    1<<7,	0,	0,	0,	0
                                  };

/* Control Handle */
int ctrlh=0;
volatile unsigned int *gpio_addr[4] = {NULL, NULL, NULL, NULL};
volatile unsigned int *CM_ptr = NULL;
volatile unsigned int *cm_per_addr = NULL;
volatile unsigned int *cm_wkup_addr = NULL ;

/* pointer to const Port set and Port ID set array */
signed char* PortSet_ptr[2];
unsigned int* PortIDSet_ptr[2];

/* BBBIO_GPIO_SAFE_MASK exporess the usable gpio in BBB Expansion Header P8 and P9 .
 * and mask the BBBIO_GPIO_high and BBBIO_GPIO_low to avoid the error access of the 
 * pin which not in P8 and P9 .*/
const unsigned int BBBIO_GPIO_SAFE_MASK [] ={
1<< 2 | 1<<3 | 1<<4 | 1<<5 | 1<<7 | 1<<8 | 1<<9 |
1<<10 | 1<<11 | 1<<12 | 1<<13 | 1<<14 | 1<<15 |
1<<20 | 1<<22 | 1<<23 | 1<<26 | 1<<27 | 1<<30 |
1<<31, // GPIO 0
1<< 0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6 |
1<<7 | 1<<12 | 1<<13 | 1<<14 | 1<<15 | 1<<16 |
1<<17 | 1<<18 | 1<<19 | 1<<28 | 1<<29 | 1<<30 |
1<<31, // GPIO 1
1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6 | 1<<7 |
1<<8 | 1<<9 | 1<<10 | 1<<11 | 1<<12 | 1<<13 |
1<<14 | 1<<15 | 1<<16 | 1<<17 | 1<<22 | 1<<23 |
1<<24 | 1<<25 ,  // GPIO 2
1<<14 | 1<<15 | 1<<16 | 1<<17 | 1<<18 | 1<<20 | 1<<19 | 1<<21 };        // GPIO 3

/*-----------------------------------------------------------------------------------------------
 */
int iolib_init(void)
{
	int i;

    if (cm_per_addr != NULL || cm_wkup_addr != NULL) {
        return;
    }

    kprintf("iolib_init>>\n");

	PortSet_ptr[0]=(signed char*)p8_PortSet;
	PortSet_ptr[1]=(signed char*)p9_PortSet;
	PortIDSet_ptr[0]=(unsigned int*)p8_PortIDSet;
	PortIDSet_ptr[1]=(unsigned int*)p9_PortIDSet;

	/* mapping Clock Module Peripheral Registers */
	cm_per_addr = (unsigned int *)BBBIO_CM_PER_ADDR;

	cm_wkup_addr =(void *)cm_per_addr + BBBIO_CM_WKUP_OFFSET_FROM_CM_PER ;

	/* mapping Address of GPIO 0~4 */
	for (i=0; i<4; i++) {
		gpio_addr[i] = (unsigned int *)GPIO_AddressOffset[i];
	}

	/* mapping Control Module Registers
	 * for pin mux control , or display expansion header informaiton
	 *
	 * Useless now , this register must be privigle mode .
	 */
	CM_ptr = (unsigned int *) BBBIO_CONTROL_MODULE;

	BBBIO_ADCTSC_Init();

    kprintf("iolib_init<<\n");

	return 0;
}
/*-----------------------------------------------------------------------------------------------
 */
int iolib_free(void)
{
	return 0;
}
/*-----------------------------------------------------------------------------------------------
 */
int iolib_setdir(char port, char pin, char dir)
{
	int param_error=0;			            // parameter error
	volatile unsigned int* reg;		        // GPIO register

	// sanity checks
	if ((port < 8) || (port > 9))		    // if input is not port8 and port 9 , because BBB support P8/P9 Connector
		param_error=1;
	if ((pin < 1 ) || (pin > 46))		    // if pin over/underflow , range : 1~46
		param_error=1;
	if (PortSet_ptr[port - 8][pin - 1] < 0)	// pass GND OR VCC (PortSet as -1)
		param_error=1;

	if (param_error)
	{
#ifdef BBBIO_LIB_DBG
		kprintf("iolib_setdir: parameter error!\n");
#endif
		return(-1);
	}
#ifdef BBBIO_LIB_DBG
	kprintf("iolib_setdir: PortSet_ptr P%d.%d , %X\n",port ,pin , PortSet_ptr[port-8][pin-1]);
#endif
	reg=(void*)gpio_addr[PortSet_ptr[port-8][pin-1]] + BBBIO_GPIO_OE;

	if (dir == BBBIO_DIR_OUT) {
		*reg &= ~(PortIDSet_ptr[port-8][pin-1]);
	}
	else if (dir == BBBIO_DIR_IN) {
		*reg |= PortIDSet_ptr[port-8][pin-1];
	}

	return(0);
}
/* ----------------------------------------------------------------------------------------------- */
void pin_high(char port, char pin)
{
	*((unsigned int *)((void *)gpio_addr[PortSet_ptr[port-8][pin-1]] + BBBIO_GPIO_SETDATAOUT)) = PortIDSet_ptr[port-8][pin-1];
}
/* ----------------------------------------------------------------------------------------------- */
void pin_low(char port, char pin)
{
	*((unsigned int *)((void *)gpio_addr[PortSet_ptr[port-8][pin-1]] + BBBIO_GPIO_CLEARDATAOUT)) = PortIDSet_ptr[port-8][pin-1];
}
/* ----------------------------------------------------------------------------------------------- */
char is_high(char port, char pin)
{
	return ((*((unsigned int *)((void *)gpio_addr[PortSet_ptr[port-8][pin-1]] + BBBIO_GPIO_DATAIN)) & PortIDSet_ptr[port-8][pin-1])!=0);
}
/* ----------------------------------------------------------------------------------------------- */
char is_low(char port, char pin)
{
	return ((*((unsigned int *)((void *)gpio_addr[PortSet_ptr[port-8][pin-1]] + BBBIO_GPIO_DATAIN)) & PortIDSet_ptr[port-8][pin-1])==0);
}
/* ----------------------------------------------------------------------------------------------- */
int iolib_delay_ms(unsigned int msec)
{
    sleepms(msec);
	return 0;
}

//-----------------------------------------------------------------------------------------------
/*********************************
 Check GPIO clock status
 *******************************
 * Display GPIO status , this function just display the register value bellow:
 *	CM_PER_GPIO1_CLKCTRL
 *	CM_PER_GPIO2_CLKCTRL
 *	CM_PER_GPIO3_CLKCTRL
 *
 * Register format in AM335x Technical Reference Manual ,
 *		CM_PER_GPIO1_CLKCTRL , session 8.1.12.1.29 , page:947
 *		CM_PER_GPIO2_CLKCTRL , session 8.1.12.1.30 , page:948
 *		CM_PER_GPIO3_CLKCTRL , session 8.1.12.1.31 , page:949
*/
void BBBIO_sys_GPIO_CLK_status()
{
	volatile unsigned int* reg ;
	unsigned int reg_value =0;
	const char s_GDBLCK[][9]={"FCLK_DIS" ,"FCLK_EN"};
	const char s_IDLEST[][8]={"Func" ,"Trans" ,"Idle" ,"Disable"};
	const char s_MODULEMODE[][8]={"Disable" ,"Reserve" ,"Enable" ,"Reserve"};

	int v_GDBLCK ;
	int v_IDLEST ;
	int v_MODULEMODE ;

	kprintf("\n******************************************************\n");
	kprintf("************ GPIO Clock module Information ***********\n");
	kprintf("******************************************************\n");
	reg =(void*)cm_per_addr + BBBIO_CM_PER_L4LS_CLKSTCTRL;
	reg_value = *reg ;
	kprintf("CM_PER CM_PER_L4LS_CLKSTCTRL : %X\n\n", reg_value );

	reg =(void*)cm_wkup_addr + BBBIO_CM_WKUP_GPIO0_CLKCTRL ;
    reg_value = *reg ;
	v_GDBLCK = (reg_value >> 18) & 0x01 ;
	v_IDLEST = (reg_value >> 16) & 0x02 ;
	v_MODULEMODE =  reg_value & 0x02 ;

    kprintf("CM_WKUP CM_WKUP_GPIO0_CLKCTRL : %X\n", *reg );
    kprintf("\t[18]    OPTFCLKEN_GPIO_0_GDBCLK : %X (%s)\n", v_GDBLCK ,s_GDBLCK[v_GDBLCK]);
    kprintf("\t[17-18] IDLEST                  : %X (%s)\n", v_IDLEST ,s_IDLEST[v_IDLEST]);
    kprintf("\t[0-1]   MODULEMODE              : %X (%s)\n\n", v_MODULEMODE ,s_MODULEMODE[v_MODULEMODE]);

	reg =(void*)cm_per_addr + BBBIO_CM_PER_GPIO1_CLKCTRL;
	reg_value = *reg ;
    v_GDBLCK = (reg_value >> 18) & 0x01 ;
    v_IDLEST = (reg_value >> 16) & 0x02 ;
    v_MODULEMODE =  reg_value & 0x02 ;

    kprintf("CM_PER CM_PER_GPIO1_CLKCTRL : %X\n", *reg );
    kprintf("\t[18]    OPTFCLKEN_GPIO_1_GDBCLK : %X (%s)\n", v_GDBLCK ,s_GDBLCK[v_GDBLCK]);
    kprintf("\t[17-18] IDLEST                  : %X (%s)\n", v_IDLEST ,s_IDLEST[v_IDLEST]);
    kprintf("\t[0-1]   MODULEMODE              : %X (%s)\n\n", v_MODULEMODE ,s_MODULEMODE[v_MODULEMODE]);

	reg =(void*)cm_per_addr + BBBIO_CM_PER_GPIO2_CLKCTRL;
	reg_value = *reg ;
    v_GDBLCK = (reg_value >>18) & 0x01 ;
    v_IDLEST = (reg_value >>16) & 0x02 ;
    v_MODULEMODE =  reg_value & 0x02 ;

    kprintf("CM_PER CM_PER_GPIO2_CLKCTRL : %X\n", *reg );
    kprintf("\t[18]    OPTFCLKEN_GPIO_2_GDBCLK : %X (%s)\n", v_GDBLCK ,s_GDBLCK[v_GDBLCK]);
    kprintf("\t[17-18] IDLEST                  : %X (%s)\n", v_IDLEST ,s_IDLEST[v_IDLEST]);
    kprintf("\t[0-1]   MODULEMODE              : %X (%s)\n\n", v_MODULEMODE ,s_MODULEMODE[v_MODULEMODE]);

	reg =(void*)cm_per_addr + BBBIO_CM_PER_GPIO3_CLKCTRL;
	reg_value = *reg ;
    v_GDBLCK = (reg_value >> 18) & 0x01 ;
    v_IDLEST = (reg_value >> 16) & 0x02 ;
    v_MODULEMODE =  reg_value & 0x02 ;

	kprintf("CM_PER CM_PER_GPIO3_CLKCTRL : %X\n", *reg );
    kprintf("\t[18]    OPTFCLKEN_GPIO_0_GDBCLK : %X (%s)\n", v_GDBLCK ,s_GDBLCK[v_GDBLCK]);
    kprintf("\t[17-18] IDLEST                  : %X (%s)\n", v_IDLEST ,s_IDLEST[v_IDLEST]);
    kprintf("\t[0-1]   MODULEMODE              : %X (%s)\n\n", v_MODULEMODE ,s_MODULEMODE[v_MODULEMODE]);
}

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
/*********************************
 Enable GPIO (enable clock)
 *******************************
 * Enable GPIO module clock
 *	@param gpio	: GPIO number , BBBIO_GPIO0 / BBBIO_GPIO1 / BBBIO_GPIO2 / BBBIO_GPIO3 .
 *
 *	@return		: 1 for success , 0 for failed
 *
 *	@example	: BBBIO_sys_Enable_GPIO(BBBIO_GPIO2);	// enable GPIO2's module clock
 *
 *	Warring		: please confirm Device which loaded on this GPIO is Disable , or it may cause some bad effect of these device
 */
int BBBIO_sys_Enable_GPIO(unsigned int gpio)		// Enable GPIOx's clock
{
	int param_error=0;				// parameter error 
	volatile unsigned int* reg;		// GPIO register

#ifdef BBBIO_LIB_DBG
    kprintf("BBIO_sys_Enable_GPIO >> %u\n", gpio);
#endif

	// sanity checks
	if (cm_per_addr==0)
		param_error=1;
	if (gpio >3)	//GPIO range
		param_error=1;

	if (param_error) {
#ifdef BBBIO_LIB_DBG
		kprintf("BBBIO_sys_Enable_GPIO: parameter error!\n");
		return 0;
#endif
	}

	switch(gpio) {
	case BBBIO_GPIO0 :
		reg =(void *)cm_wkup_addr + BBBIO_CM_WKUP_GPIO0_CLKCTRL;
		*reg |= 0x2 ;
		break ;
	case BBBIO_GPIO1 :
		reg =(void *)cm_per_addr + BBBIO_CM_PER_GPIO1_CLKCTRL;
		*reg |= 0x2 ;
		break ;
	case BBBIO_GPIO2 :
		reg =(void *)cm_per_addr + BBBIO_CM_PER_GPIO2_CLKCTRL;
		*reg |= 0x2 ;
		break ;
	case BBBIO_GPIO3 :
		reg =(void *)cm_per_addr + BBBIO_CM_PER_GPIO3_CLKCTRL;
		*reg |= 0x2 ;
		break ;
	default :
		return 0 ;
		break ;
	}
	return 1 ;
}
//-----------------------------------------------------------------------------------------------
/*********************************
 Disable GPIO (Disable clock)
 *******************************
 * Disable GPIO module clock
 *	@param gpio	: GPIO number , BBBIO_GPIO0 / BBBIO_GPIO1 / BBBIO_GPIO2 / BBBIO_GPIO3 .
 *
 *	@return 	: 1 for success , 0 for failed
 *
 *	@example 	: BBBIO_sys_Disable_GPIO(BBBIO_GPIO2);	// Disable GPIO2's module clock
 *
 *	Warring 	: please confirm Device which loaded on this GPIO is Disable , or it may cause some bad effect of these device
 */
int BBBIO_sys_Disable_GPIO(unsigned int gpio)		// Disable GPIOx's clock
{
	int param_error=0;				// parameter error
	volatile unsigned int* reg;		// GPIO register

#ifdef BBBIO_LIB_DBG
        kprintf("BBIO_sys_Disable_GPIO >>\n");
#endif

	// sanity checks
	if (cm_per_addr==0)
		param_error=1;
	if (gpio >3)	//GPIO range
		param_error=1;

	if (param_error) {
#ifdef BBBIO_LIB_DBG
		kprintf("BBBIO_sys_Disable_GPIO: parameter error!\n");
#endif
		return 0;
	}

	switch(gpio) {
	case BBBIO_GPIO0 :
		reg =(void *)cm_wkup_addr + BBBIO_CM_WKUP_GPIO0_CLKCTRL;
		*reg &= 0x2 ;
		break ;
	case BBBIO_GPIO1 :
		reg =(void *)cm_per_addr + BBBIO_CM_PER_GPIO1_CLKCTRL;
		*reg &= ~0x2 ;
		break ;
	case BBBIO_GPIO2 :
		reg =(void *)cm_per_addr + BBBIO_CM_PER_GPIO2_CLKCTRL;
		*reg &= ~0x2 ;
		break ;
	case BBBIO_GPIO3 :
		reg =(void *)cm_per_addr + BBBIO_CM_PER_GPIO3_CLKCTRL;
		*reg &= ~0x2 ;
		break ;
	default :
		return 0 ;
		break ;
	}
	return 1 ;
}
//-------------------------------------------------------------------------------------------
/*********************************
  millisecond  sleep
 *******************************
 *      @param msec	: sleep time ,millisecond unit .
 *
 *      @return 	: 1 for success , 0 for failed
 *
 *      @example 	: BBBIO_sys_delay_ms(2); // Sleep 2ms
 *
 *      Warring 	: this function is not an accurate function , it's effected by os ,
 *		  	  so , if you want to take an accurate action  , like motor controller , this function may take some bad effect .
 */

inline int BBBIO_sys_delay_ms(unsigned long msec)
{
    sleepms(msec);
	return 1;
}

//-------------------------------------------------------------------------------------------
/*********************************
 micro second sleep
 *******************************
 *      @param msec 	: sleep time ,microsecond unit .
 *
 *      @return 	: 1 for success , 0 for failed
 *
 *      @example 	: BBBIO_sys_delay_us(2000); // Sleep 2000us (2ms)
 *
 *      Warring 	: this function is not an accurate function , it's effected by os ,
 *                	  so , if you want to take an accurate action  , like motor controller , this function may take some bad effect .
 */

inline int BBBIO_sys_delay_us(unsigned long usec)
{
        sleepms(usec/1000);
        return 1;
}

/*=============================================================================================
 * BBBIO I/O control function (whole GPIO)
 * All function below is unstable
 *
 * pin_high and pin_low is very useful , but in some case ,
 * ex : 7-Segment display , it must pin high/low 8 times , it's very waste time .
 *
 * if you need to control many I/O in same time , please try to using BBBIO_GPIO_ prefix funcion ,
 * these function allow you control whole GPIO at once function call .
 *
 */

//-----------------------------------------------------------------------------------------------
/*********************************
 GPIO Set direction
 *******************************
 * Set whole GPIO transmission direction
 *      @param gpio     : GPIO number , BBBIO_GPIO0 / BBBIO_GPIO1 / BBBIO_GPIO2 / BBBIO_GPIO3 .
 *      @param inset    : Inpute pin set , using BBBIO_GPIO_PIN_? and | operator to express it.
 *                                        The pin which not selected will keeping origin status .
 *      @param outset   : Output pin set , format as same as inset .
 *
 *      @return         : 1 for success , 0 for failed
 *
 *      @example        : BBBIO_GPIO_set_dir(BBBIO_GPIO1 ,                  // handle GPIO1
 *                                   BBBIO_GPIO_PIN_11 | BBBIO_GPIO_PIN_12 , // pin 11 and 12 as input
 *                                   BBBIO_GPIO_PIN_15 | BBBIO_GPIO_PIN_16); // pin 15 and 16 as output
 *
 *      Warring         : if you nedd this function , please check value you input , or it may effect other chip or device
 */

int BBBIO_GPIO_set_dir(unsigned int  gpio, unsigned int inset , unsigned int outset)
{
        int param_error=0;
	volatile unsigned int* reg;

#ifdef BBBIO_LIB_DBG
     kprintf("BBBIO_GPIO_set_dir >> %u %x %x\n", gpio, inset, outset);
#endif

    /* sanity checks */

    if (gpio > 3) 	/* GPIO range */
            param_error = 1;

    if (inset == 0 && outset == 0)	/* pin identify error */
            param_error = 1;

    if (param_error) {
#ifdef BBBIO_LIB_DBG
	    kprintf("BBBIO_GPIO_set_dir: parameter error!\n");
#endif
        return -1;
    }

    reg=(void *)gpio_addr[gpio] + BBBIO_GPIO_OE;
    *reg &= ~outset ;
    *reg |= inset ;

    return 0;
}

//-----------------------------------------------------------------------------------------------
/*********************************
 GPIO High / Low
 *******************************
 * Set whole GPIO Output logic
 *      @param gpio     : GPIO number , BBBIO_GPIO0 / BBBIO_GPIO1 / BBBIO_GPIO2 / BBBIO_GPIO3 .
 *      @param pinset    : control pin set , using BBBIO_GPIO_PIN_? and | operator to express it.
 *                         The pin which not selected will keeping origin status .
 *
 *      @return : void
 *
 *      @example : BBBIO_GPIO_high(BBBIO_GPIO1 ,  // control GPIO 1
 *      BBBIO_GPIO_PIN_11 | BBBIO_GPIO_PIN_12 | BBBIO_GPIO_PIN_15);	// PIN 11 12 15 pin high simultaneously
 *
 *      Warning : if you nedd this function , please check value you input , or it may effect other chip or device .
 */

void BBBIO_GPIO_high(unsigned int gpio ,unsigned int pinset)
{
#ifdef BBBIO_LIB_DBG
    kprintf("BBBIO_GPIO_high >> %u %x\n", gpio, pinset);
#endif
	*((unsigned int *)((void *)gpio_addr[gpio]+BBBIO_GPIO_SETDATAOUT))
                                    = pinset & BBBIO_GPIO_SAFE_MASK[gpio] ;
}

void BBBIO_GPIO_low(unsigned int gpio ,unsigned int pinset)
{
#ifdef BBBIO_LIB_DBG
        kprintf("BBBIO_GPIO_low >> %u %x\n", gpio, pinset);
#endif

	*((unsigned int *)((void *)gpio_addr[gpio] + BBBIO_GPIO_CLEARDATAOUT)) 
                                    = pinset &  BBBIO_GPIO_SAFE_MASK[gpio];
}

int BBBIO_GPIO_get(char gpio, unsigned int pinset)
{
#ifdef BBBIO_LIB_DBG
            kprintf("BBBIO_GPIO_get >> %u %x\n", gpio, pinset);
#endif

	 return *((unsigned int *)((void *)gpio_addr[gpio] + BBBIO_GPIO_DATAIN)) & pinset;
}
