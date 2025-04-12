
# Distance Vector Routing Simulation - Comprehensive Report

## 1. Overview and Instructions

This report documents the implementation, execution, and analysis of a distributed asynchronous distance vector routing simulation. This report is based on the provided code and aims to fulfill the requirements of the assignment, including analyzing the network behavior as depicted in the included image.

### Key Files to Include in Your Submission

1. Source code files:
   - prog3.c - Main simulation file
   - node0.c, node1.c, node2.c, node3.c - Node implementations
   - common.h - Common structure definitions
   - functions.h - Function declarations

2. Result files:
   - trace_level_0.txt, trace_level_1.txt, trace_level_2.txt - Simulation outputs
   - README.md - Your analysis and explanation

## 2. Running the Simulation

### Step 1: Compilation

```bash
# Compile the program
gcc -o prog3 prog3.c node0.c node1.c node2.c node3.c
```

### Step 2: Run Simulations with Different Trace Levels

```bash
# Run with trace level 0 (minimal output)
echo "0" | ./prog3 > trace_level_0.txt

# Run with trace level 1 (moderate detail)
echo "1" | ./prog3 > trace_level_1.txt

# Run with trace level 2 (high detail - required for report)
echo "2" | ./prog3 > trace_level_2.txt
```

### Step 3: Extract Key Results for Analysis

```bash
# Create results directory
mkdir -p results

# Extract final minimum costs
grep "Minimum cost from" trace_level_2.txt | tail -4 > results/final_costs.txt

# Extract initial distance tables
grep -A 15 "rtinit" trace_level_2.txt | grep -A 10 "Distance table" | head -40 > results/initial_tables.txt

# Extract link changes and effects
grep -A 20 "linkhandler" trace_level_2.txt > results/link_changes.txt

# Calculate message overhead
grep -c "sends packet to node" trace_level_2.txt > results/message_count.txt
```

## 3. Information to Include in Your Report

### Network Topology Analysis

Based on the provided image, the network topology is as follows:

```
0 --1-- 1
| \     |
7   3   1
|     \ |
3 --2-- 2
```

- **Nodes:** 4 nodes labeled 0, 1, 2, and 3.
- **Links and Costs:**
    - Between Node 0 and Node 1: Cost 1
    - Between Node 0 and Node 2: Cost 3
    - Between Node 1 and Node 2: Cost 1
    - Between Node 2 and Node 3: Cost 2
    - Between Node 0 and Node 3: Cost 7 (as indicated in the initial code and likely the initial configuration)
    - **Important Note:** The image shows a direct link between Node 0 and Node 2 with a cost of 3, and a direct link between Node 1 and Node 2 with a cost of 1. It also shows a direct link between Node 2 and Node 3 with a cost of 2. The initial code also indicates a direct link between Node 0 and Node 3 with a cost of 7, and a direct link between Node 0 and Node 1 with a cost of 1.

- **Link Changes:** The simulation introduces link cost changes for the link between Node 0 and Node 1:
    - Cost changes to 20 at time t=10000.
    - Cost changes back to 1 at time t=20000.

### Algorithm Implementation

Describe the key components of your code:

1. **Initialization Phase (`rtinit#()`):**
   - Each node initializes its local distance table. The cost to directly connected neighbors is set to the link cost, the cost to itself is 0, and the cost to non-neighbors is set to infinity (999).
   - After initialization, each node sends its initial distance vector to all its directly connected neighbors using the `tolayer2()` function.

2. **Update Phase (`rtupdate#()`):**
   - When a node receives a routing packet from a neighbor, it updates its distance table. For each destination, the node considers the cost to reach that destination through the neighbor plus the cost from the neighbor to the destination (as advertised in the received packet).
   - After updating its distance table, the node recalculates its minimum cost to each destination. If any of these minimum costs have decreased, it means a shorter path has been found.
   - If a minimum cost to any destination has changed, the node creates a new routing packet containing its updated distance vector and sends it to all its directly connected neighbors.

3. **Link Change Handling (`linkhandler0()`, `linkhandler1()`):**
   - At specific simulation times (t=10000 and t=20000), the cost of the link between Node 0 and Node 1 is modified.
   - The `linkhandler` functions are called to update the local cost information at the affected nodes (Node 0 and Node 1).
   - These changes in link cost trigger subsequent updates in the distance vectors as the nodes react to the altered network topology.

### Results Analysis

Analyze these key aspects from your trace files:

1. **Initial State**:
   From the trace (e.g., `trace_level_2.txt`), analyze the initial distance tables of each node after the `rtinit()` functions are executed. For Node 0, it should reflect the direct costs to its neighbors (1 to Node 1, 3 to Node 2, 7 to Node 3).

2. **Convergence Process**:
   Examine the sequence of routing packets exchanged between neighbors and how the distance tables evolve over time. Describe how nodes learn about paths to other nodes through intermediate hops. Note the number of iterations or time units it takes for the routing tables to stabilize.

3. **Link Cost Changes**:
   - **At t=10000 (Link 0-1 cost changes to 20):** Observe how the routing tables at Node 0 and Node 1 are immediately updated. Track how this change propagates through the network as nodes exchange updated distance vectors. Analyze how the routes to destinations that previously used the 0-1 link are affected and if alternative paths are discovered.
   - **At t=20000 (Link 0-1 cost changes back to 1):** Similarly, observe the immediate impact on Node 0 and Node 1 and the subsequent propagation of this change. Analyze how the routes readjust to potentially utilize the now lower-cost 0-1 link again.

4. **Final Routes**:
   After the network has converged following the second link change (at t > 20000), report the final minimum cost from each node to all other nodes. This information can be extracted from the "Minimum cost from" lines in your trace file.

### Relevant Screenshots

Include screenshots in your report to visually demonstrate:

- Successful compilation of the code.
- The initial state of the distance tables for each node (shortly after the simulation starts).
- Intermediate routing updates (if the trace level allows for clear visualization of packet exchanges and table updates).
- The state of the distance tables and minimum costs after the first link change (at t=10000) and after the network has reconverged.
- The final converged state of the distance tables and minimum costs after the second link change (at t=20000).

## 4. Problems Encountered and Solutions

Document any difficulties you faced during the implementation or analysis and the steps you took to resolve them. This could include:

- Compilation errors and their fixes.
- Issues in understanding the algorithm's behavior in the simulation.
- Challenges in extracting and interpreting the information from the trace files.
- Any discrepancies between the expected behavior based on the algorithm and the observed behavior in the simulation.

## 5. Script for Generating Full Report

```bash
#!/bin/bash

# Create report directory
mkdir -p report

# Run simulation with different trace levels
echo "0" | ./prog3 > report/trace_level_0.txt
echo "1" | ./prog3 > report/trace_level_1.txt
echo "2" | ./prog3 > report/trace_level_2.txt

# Extract key results
echo "Final Routing Tables:" > report/analysis.txt
grep "Minimum cost from" report/trace_level_2.txt | tail -4 >> report/analysis.txt

echo -e "\nInitial Distance Tables:" >> report/analysis.txt
grep -A 15 "rtinit" report/trace_level_2.txt | grep -A 10 "Distance table" | head -40 >> report/analysis.txt

echo -e "\nLink Cost Changes:" >> report/analysis.txt
grep -A 20 "linkhandler" report/trace_level_2.txt >> report/analysis.txt

echo -e "\nMessage Overhead:" >> report/analysis.txt
echo "Total packets exchanged: $(grep -c "sends packet to node" report/trace_level_2.txt)" >> report/analysis.txt

# Create message timing analysis (optional)
grep "sends packet to node" report/trace_level_2.txt | awk '{print $2}' | sed 's/t=//' | sort -n > report/message_timing.txt

# Generate PDF report if pandoc is available (optional)
if command -v pandoc &> /dev/null; then
    pandoc README.md -o report/distance_vector_routing_report.pdf
fi
```

Save this script as `generate_report.sh` and make it executable with `chmod +x generate_report.sh`.

## 6. Conclusion

Summarize your findings from the simulation and analysis. Discuss the behavior of the distance vector routing algorithm in this specific network topology, its convergence properties, and its response to link cost changes. You can also discuss any limitations or potential issues observed, such as the possibility of count-to-infinity (though it might not be explicitly demonstrated in this small, stable topology).

For your submitted report, ensure you include:

- A `README.md` file containing your detailed analysis and explanations.
- The script(s) used to create and configure the network (in this case, the C code itself configures the network).
- All the test results requested for each of the questions (analyzed output from the trace files).
- Explanation of any problems encountered and the approach taken to resolve them.
- Relevant screenshots demonstrating successful connection, performance metrics (e.g., convergence time, message overhead), and routing tables at different stages of the simulation.

Remember to refer to the network topology depicted in the provided image when describing the network and analyzing the routing paths.
```
