struct request_t 
{
    uint8_t ver;
    uint8_t cmd;
    uint8_t rsv;  
    uint8_t atyp;
	uint8_t address[258];
};
struct reply_t 
{
	uint8_t ver;
	uint8_t rep;
	uint8_t rsv;
	uint8_t atyp;
	uint8_t address[258];
};
struct client_hello_t
{
	uint8_t ver;
	uint8_t nmethods;
	uint8_t methods[255];
};
struct server_hello_t
{
	uint8_t ver;
	uint8_t method;
};
