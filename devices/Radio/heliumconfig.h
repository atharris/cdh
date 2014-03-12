#ifndef HELIUM_CONFIG_
#define HELIUM_CONFIG_
#define BAUD_RATE_9600 0
#define BAUD_RATE_19200 1
#define BAUD_RATE_38400 2
#define BAUD_RATE_76800 3
#define BAUD_RATE_115200 4

#define MODULATION_GFSK 0
#define MODULATION_AFSK 1

#define uint_1 unsigned char
#define uint_4 unsigned int
#define uint_2 unsigned short

typedef struct{
	uint_1 interface_baud_rate;
	uint_1 tx_power_amp_level; //min-max power mapped to 0x00-0xff
	uint_1 rx_rf_baud_rate;
	uint_1 tx_rf_baud_rate;
	uint_1 rx_modulation;
	uint_1 tx_modulation;
	uint_4 rx_freq;
	uint_4 tx_freq;
	unsigned char source[6]; //default NOCALL
	unsigned char destination[6]; //default CQ
	uint_2 tx_preamble;
	uint_2 tx_postamble;
	uint_2 function_config;
#ifdef FIRMWARE_VERSION_3
	uint_2 function_config2;
#endif
}__attribute__((packed))  helium_config_t;

#endif
