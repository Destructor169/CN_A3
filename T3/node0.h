#ifndef NODE0_H
#define NODE0_H

#include "common.h"

void rtinit0(void);
void rtupdate0(struct rtpkt *rcvdpkt);
void linkhandler0(int linkid, int newcost);
void printdt0(struct distance_table *dtptr);
void printmincost0(void);

#endif /* NODE0_H */
