#ifndef NODE3_H
#define NODE3_H

#include "common.h"

void rtinit3(void);
void rtupdate3(struct rtpkt *rcvdpkt);
void printdt3(struct distance_table *dtptr);
void printmincost3(void);

#endif /* NODE3_H */
