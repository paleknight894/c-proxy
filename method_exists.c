#include<stdint.h>
#include"sockstypes.h"
int method_exists(struct client_hello_t * client_hello,uint8_t method)
{
    int i;
    for (i = 0; i < client_hello->nmethods; i++) {
        if (client_hello->methods[i] == method) {
            return 1;
        }
    }
    return 0;
}
