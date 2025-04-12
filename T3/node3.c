#include "common.h"
#include "functions.h"
#include <stdio.h>
#include <string.h>

#define INFINITY 999

struct distance_table dt3;
int connectcosts3[4] = { 7, 999, 2, 0 };
int min_cost_3[4];

int min_3(int a, int b) { return a < b ? a : b; }
int min3(int a[]) {
    return min_3(min_3(min_3(a[0], a[1]), a[2]), a[3]);
}

void calc_min_cost3() {
    //calculate the minimum cost to every other node
    for(int i=0; i<4; i++) {
        min_cost_3[i] = min3(dt3.costs[i]);
    }
}

void printdt3(struct distance_table *dtptr) {
    printf("             via     \n");
    printf("   D3 |    0     2 \n");
    printf("  ----|-----------\n");
    printf("     0|  %3d   %3d\n", dtptr->costs[0][0], dtptr->costs[0][2]);
    printf("dest 1|  %3d   %3d\n", dtptr->costs[1][0], dtptr->costs[1][2]);
    printf("     2|  %3d   %3d\n", dtptr->costs[2][0], dtptr->costs[2][2]);
}

void printmincost3() {
    printf("Minimum cost from %d to other nodes are: %d %d %d %d\n", 3, min_cost_3[0], min_cost_3[1],
           min_cost_3[2], min_cost_3[3]);
}

void sendpkt3() {
    struct rtpkt pkt;
    pkt.sourceid = 3;
    
    // Send to neighbors
    int neighbors[] = {0, 2};
    for(int i=0; i<2; i++) {
        pkt.destid = neighbors[i];
        memcpy(pkt.mincost, min_cost_3, sizeof(min_cost_3));
        tolayer2(pkt);
        printf("At time t=%.3f, node %d sends packet to node %d with: (%d  %d  %d  %d)\n",
            clocktime, 3, neighbors[i], min_cost_3[0], min_cost_3[1], min_cost_3[2], min_cost_3[3]);
    }
}

void rtinit3() {
    printf("rtinit3() called at time t=%.3f\n", clocktime);
    
    // Initialize distance table
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            dt3.costs[i][j] = INFINITY;
        }
    }
    
    // Set direct costs
    dt3.costs[0][0] = 7;  // Cost to node 0
    dt3.costs[2][2] = 2;  // Cost to node 2
    dt3.costs[3][3] = 0;  // Cost to itself
    
    printdt3(&dt3);
    
    // Calculate min costs and send
    calc_min_cost3();
    sendpkt3();
}

void rtupdate3(struct rtpkt *rcvdpkt) {
    int src = rcvdpkt->sourceid;
    printf("rtupdate3() called at time t=%.3f from %d with (%d %d %d %d)\n", 
           clocktime, src, rcvdpkt->mincost[0], rcvdpkt->mincost[1], 
           rcvdpkt->mincost[2], rcvdpkt->mincost[3]);
    
    int changed = 0;
    int old_min_cost[4];
    memcpy(old_min_cost, min_cost_3, sizeof(min_cost_3));
    
    // Update distance table based on received packet
    for(int i=0; i<4; i++) {
        int newcost = dt3.costs[src][src] + rcvdpkt->mincost[i];
        if(newcost < dt3.costs[i][src]) {
            dt3.costs[i][src] = newcost;
            changed = 1;
        }
    }
    
    printdt3(&dt3);
    
    // Recalculate min costs
    calc_min_cost3();
    
    // Check if min costs changed
    for(int i=0; i<4; i++) {
        if(old_min_cost[i] != min_cost_3[i]) {
            changed = 1;
        }
    }
    
    // If costs changed, send updates to neighbors
    if(changed) {
        sendpkt3();
    } else {
        printf("No change in min costs at node 3, no packets sent\n");
    }
}
