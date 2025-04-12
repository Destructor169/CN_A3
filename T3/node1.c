#include "common.h"
#include "functions.h"
#include <stdio.h>
#include <string.h>

#define INFINITY 999

struct distance_table dt1;
int connectcosts1[4] = { 1, 0, 1, 999 };
int min_cost_1[4];

int min_1(int a, int b) { return a < b ? a : b; }
int min1(int a[]) {
    return min_1(min_1(min_1(a[0], a[1]), a[2]), a[3]);
}

void calc_min_cost1() {
    // Calculate the minimum cost to every other node
    for(int i=0; i<4; i++) {
        min_cost_1[i] = min1(dt1.costs[i]);
    }
}

void printmincost1() {
    printf("Minimum cost from %d to other nodes are: %d %d %d %d\n", 1, min_cost_1[0], min_cost_1[1],
           min_cost_1[2], min_cost_1[3]);
}

void printdt1(struct distance_table *dtptr) {
    printf("             via   \n");
    printf("   D1 |    0     2 \n");
    printf("  ----|-----------\n");
    printf("     0|  %3d   %3d\n", dtptr->costs[0][0], dtptr->costs[0][2]);
    printf("dest 2|  %3d   %3d\n", dtptr->costs[2][0], dtptr->costs[2][2]);
    printf("     3|  %3d   %3d\n", dtptr->costs[3][0], dtptr->costs[3][2]);
}

void sendpkt1() {
    struct rtpkt pkt;
    pkt.sourceid = 1;
    
    // Send to node 0
    pkt.destid = 0;
    memcpy(pkt.mincost, min_cost_1, sizeof(min_cost_1));
    tolayer2(pkt);
    printf("At time t=%.3f, node %d sends packet to node %d with: (%d  %d  %d  %d)\n",
           clocktime, 1, 0, min_cost_1[0], min_cost_1[1], min_cost_1[2], min_cost_1[3]);
    
    // Send to node 2
    pkt.destid = 2;
    tolayer2(pkt);
    printf("At time t=%.3f, node %d sends packet to node %d with: (%d  %d  %d  %d)\n",
           clocktime, 1, 2, min_cost_1[0], min_cost_1[1], min_cost_1[2], min_cost_1[3]);
}

void rtinit1() {
    printf("rtinit1() called at time t=%.3f\n", clocktime);
    
    // Initialize the distance table
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            dt1.costs[i][j] = INFINITY;
        }
    }
    
    // Set direct connection costs
    dt1.costs[0][0] = 1;  // Cost to node 0
    dt1.costs[1][1] = 0;  // Cost to itself
    dt1.costs[2][2] = 1;  // Cost to node 2
    
    printdt1(&dt1);
    
    // Calculate min costs and send to neighbors
    calc_min_cost1();
    sendpkt1();
}

void rtupdate1(struct rtpkt *rcvdpkt) {
    int src = rcvdpkt->sourceid;
    printf("rtupdate1() called at time t=%.3f from %d with (%d %d %d %d)\n", 
           clocktime, src, rcvdpkt->mincost[0], rcvdpkt->mincost[1], 
           rcvdpkt->mincost[2], rcvdpkt->mincost[3]);
    
    int changed = 0;
    int old_min_cost[4];
    memcpy(old_min_cost, min_cost_1, sizeof(min_cost_1));
    
    // Update distance table based on received packet
    for(int i=0; i<4; i++) {
        int newcost = dt1.costs[src][src] + rcvdpkt->mincost[i];
        if(newcost < dt1.costs[i][src]) {
            dt1.costs[i][src] = newcost;
            changed = 1;
        }
    }
    
    printdt1(&dt1);
    
    // Recalculate min costs
    calc_min_cost1();
    
    // Check if min costs changed
    for(int i=0; i<4; i++) {
        if(old_min_cost[i] != min_cost_1[i]) {
            changed = 1;
        }
    }
    
    // If costs changed, send updates to neighbors
    if(changed) {
        sendpkt1();
    } else {
        printf("No change in min costs, no packets sent\n");
    }
}

void linkhandler1(int linkid, int newcost) {
    printf("linkhandler1() called at time t=%.3f for link to %d with new cost %d\n", 
           clocktime, linkid, newcost);
    
    // Update the direct link cost
    dt1.costs[linkid][linkid] = newcost;
    
    printdt1(&dt1);
    
    // Recalculate and possibly send updates
    int old_min_cost[4];
    memcpy(old_min_cost, min_cost_1, sizeof(min_cost_1));
    calc_min_cost1();
    
    int changed = 0;
    for(int i=0; i<4; i++) {
        if(old_min_cost[i] != min_cost_1[i]) {
            changed = 1;
        }
    }
    
    if(changed) {
        sendpkt1();
    }
}
