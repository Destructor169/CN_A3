#include "common.h"
#include "functions.h"
#include <stdio.h>
#include <string.h>

#define INFINITY 999

struct distance_table dt2;
int connectcosts2[4] = { 3, 1, 0, 2 };
int min_cost_2[4];

int min_2(int a, int b) { return a < b ? a : b; }
int min2(int a[]) {
    return min_2(min_2(min_2(a[0], a[1]), a[2]), a[3]);
}

void calc_min_cost2() {
    //calculate the minimum cost to every other node
    for(int i=0; i<4; i++) {
        min_cost_2[i] = min2(dt2.costs[i]);
    }
}

void printdt2(struct distance_table *dtptr) {
    printf("                via     \n");
    printf("   D2 |    0     1    3 \n");
    printf("  ----|-----------------\n");
    printf("     0|  %3d   %3d   %3d\n", dtptr->costs[0][0], dtptr->costs[0][1], dtptr->costs[0][3]);
    printf("dest 1|  %3d   %3d   %3d\n", dtptr->costs[1][0], dtptr->costs[1][1], dtptr->costs[1][3]);
    printf("     3|  %3d   %3d   %3d\n", dtptr->costs[3][0], dtptr->costs[3][1], dtptr->costs[3][3]);
}

void printmincost2() {
    printf("Minimum cost from %d to other nodes are: %d %d %d %d\n", 2, min_cost_2[0], min_cost_2[1],
           min_cost_2[2], min_cost_2[3]);
}

void sendpkt2() {
    struct rtpkt pkt;
    pkt.sourceid = 2;
    
    // Send to neighbors
    int neighbors[] = {0, 1, 3};
    for(int i=0; i<3; i++) {
        pkt.destid = neighbors[i];
        memcpy(pkt.mincost, min_cost_2, sizeof(min_cost_2));
        tolayer2(pkt);
        printf("At time t=%.3f, node %d sends packet to node %d with: (%d  %d  %d  %d)\n",
            clocktime, 2, neighbors[i], min_cost_2[0], min_cost_2[1], min_cost_2[2], min_cost_2[3]);
    }
}

void rtinit2() {
    printf("rtinit2() called at time t=%.3f\n", clocktime);
    
    // Initialize distance table
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            dt2.costs[i][j] = INFINITY;
        }
    }
    
    // Set direct costs
    dt2.costs[0][0] = 3;  // Cost to node 0
    dt2.costs[1][1] = 1;  // Cost to node 1
    dt2.costs[2][2] = 0;  // Cost to itself
    dt2.costs[3][3] = 2;  // Cost to node 3
    
    printdt2(&dt2);
    
    // Calculate min costs and send
    calc_min_cost2();
    sendpkt2();
}

void rtupdate2(struct rtpkt *rcvdpkt) {
    int src = rcvdpkt->sourceid;
    printf("rtupdate2() called at time t=%.3f from %d with (%d %d %d %d)\n", 
           clocktime, src, rcvdpkt->mincost[0], rcvdpkt->mincost[1], 
           rcvdpkt->mincost[2], rcvdpkt->mincost[3]);
    
    int changed = 0;
    int old_min_cost[4];
    memcpy(old_min_cost, min_cost_2, sizeof(min_cost_2));
    
    // Update distance table based on received packet
    for(int i=0; i<4; i++) {
        int newcost = dt2.costs[src][src] + rcvdpkt->mincost[i];
        if(newcost < dt2.costs[i][src]) {
            dt2.costs[i][src] = newcost;
            changed = 1;
        }
    }
    
    printdt2(&dt2);
    
    // Recalculate min costs
    calc_min_cost2();
    
    // Check if min costs changed
    for(int i=0; i<4; i++) {
        if(old_min_cost[i] != min_cost_2[i]) {
            changed = 1;
        }
    }
    
    // If costs changed, send updates to neighbors
    if(changed) {
        sendpkt2();
    } else {
        printf("No change in min costs at node 2, no packets sent\n");
    }
}
