#define NUM_DEVICES 15
#define NUM_BUSSES 3

#define GYRO 1
#define EPS 2
#define CAMERA 3
#define LENS 4
#define RXN 5
#define TORQ 6
#define MAG 7

int min_temp_table[NUM_DEVICES];
int max_temp_table[NUM_DEVICES];
int amp_table[NUM_BUSSES];

min_temp_table[GYRO] = -40.0;
max_temp_table[GYRO] = 85.0;
min_temp_table[EPS] = 0;
max_temp_table[EPS] = 0;
min_temp_table[CAMERA] =0;
max_temp_table[CAMERA] =0;
min_temp_table[LENS] = 0;
max_temp_table[LENS] = 0;
min_temp_table[RXN] = 0;
max_temp_table[RXN] = 0;
min_temp_table[TORQ] = 0;
max_temp_table[TORQ] = 0;
min_temp_table[MAG] = 0;
max_temp_table[MAG] = 0; 
 





