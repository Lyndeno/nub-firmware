#ifndef ROUTING_H
#define ROUTING_H

#include "buffer.h"

#define MaxNetworkSize	5
#define MAX_NUB_CON		3
#define MAX_PHONE_CON	3

void read_frame (circular_buf * );

void route_message(circular_buf * );

#endif