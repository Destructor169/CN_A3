#ifndef NODE2_H
#define NODE2_H

#include "common.h"

void rtinit2(void);
void rtupdate2(struct rtpkt *rcvdpkt);
void printdt2(struct distance_table *dtptr);
void printmincost2(void);

#endif /* NODE2_H */
