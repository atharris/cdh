#define MAX_PAYLOAD_LENGTH 255

//Configuration Commands
#define NO_OP_COMMAND           0x01
#define RESET_SYSTEM            0x02
#define TRANSMIT_DATA           0x03
#define RECEIVE_DATA            0x04
#define GET_TRANSCEIVER_CONFIG  0x05
#define SET_TRANSCEIVER_CONFIG  0x06
#define TELEMETRY_QUERY         0x07
#define WRITE_FLASH             0x08

//Sync bytes are 'H' and 'e'
#define SYNC_1  0x48
#define SYNC_2  0x65

//input and output message ID bytes
#define I_MESSAGE_TYPE 0x10
#define O_MESSAGE_TYPE 0x20


class Packet{

 public:
  Packet(unsigned char payload_bytes[],unsigned char packet_type,unsigned char payload_bytes_size);
  Packet(unsigned char packet_bytes[]);
  unsigned char* get_bytes();
  int get_size();

 private:
  //TODO change header to a struct, you noob
  unsigned char header[8];
  unsigned char *payload;
  unsigned char payload_size;
  unsigned char type;
  void calculate_header_checksum(unsigned char* header);
  void calculate_payload_checksum(unsigned char* payload,int size);
  bool test_header_checksum(unsigned char* packet);
  bool test_payload_checksum(unsigned char* payload, int size);
  bool check_packet_format(unsigned char* packet);
  bool is_ack();
  bool is_nack();

};
