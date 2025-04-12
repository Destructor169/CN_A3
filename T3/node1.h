#ifndef NODE1_H
#define NODE1_H

#include "common.h"

void rtinit1(void);
void rtupdate1(struct rtpkt *rcvdpkt);
void linkhandler1(int linkid, int newcost);
void printdt1(struct distance_table *dtptr);
void printmincost1(void);

#endif /* NODE1_H */
