#include <iostream>
using namespace std;
 
const int MAX_NODES = 20;   // maximum water source + plots
const int MAX_PIPES = 100;  // maximum possible pipelines
 
// A pipeline between two points with a cost
struct Pipe {
    int from;
    int to;
    int cost;
};
 
Pipe allPipes[MAX_PIPES];   // all possible pipelines entered by user
Pipe chosen[MAX_NODES];     // pipelines selected in the final answer
int totalNodes = 0;         // water source + plots
int pipeCount = 0;          // number of pipelines entered
int parentOf[MAX_NODES];    // used by Union-Find to detect cycles
 
// ---------- Input functions (with validation) ----------
 
// Reads an integer between low and high. Keeps asking until input is valid.
int readInt(const char *message, int low, int high) {
    int value;
    while (true) {
        cout << message;
        if (!(cin >> value)) {            // user typed letters / symbols
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "  Invalid input! Please enter a number.\n";
        } else if (value < low || value > high) {
            cout << "  Value must be between " << low << " and " << high << ".\n";
        } else {
            return value;
        }
    }
}
 
// Checks whether a pipeline between a and b was already entered
bool pipeExists(int a, int b) {
    for (int i = 0; i < pipeCount; i++) {
        if ((allPipes[i].from == a && allPipes[i].to == b) ||
            (allPipes[i].from == b && allPipes[i].to == a)) {
            return true;
        }
    }
    return false;
}
 
// Reads all possible pipelines from the user
void readPipes(int wanted) {
    while (pipeCount < wanted) {
        cout << "\nPipeline " << pipeCount + 1 << ":\n";
        int a = readInt("  Enter first point  (0 = Water Source): ", 0, totalNodes - 1);
        int b = readInt("  Enter second point                   : ", 0, totalNodes - 1);
 
        if (a == b) {
            cout << "  A pipe cannot join a point to itself. Try again.\n";
            continue;
        }
        if (pipeExists(a, b)) {
            cout << "  This pipeline is already entered. Try again.\n";
            continue;
        }
        int c = readInt("  Enter cost of this pipeline (1-1000)  : ", 1, 1000);
 
        allPipes[pipeCount].from = a;
        allPipes[pipeCount].to = b;
        allPipes[pipeCount].cost = c;
        pipeCount++;
    }
}
 
// ---------- Sorting (Selection Sort, written by us) ----------
 
// Sorts all pipelines from smallest cost to largest cost
void sortPipes() {
    for (int i = 0; i < pipeCount - 1; i++) {
        int smallest = i;
        for (int j = i + 1; j < pipeCount; j++) {
            if (allPipes[j].cost < allPipes[smallest].cost) {
                smallest = j;
            }
        }
        // swap
        Pipe temp = allPipes[i];
        allPipes[i] = allPipes[smallest];
        allPipes[smallest] = temp;
    }
}
 
// ---------- Union-Find (to avoid cycles) ----------
 
// At the start every point is in its own group
void makeGroups() {
    for (int i = 0; i < totalNodes; i++) {
        parentOf[i] = i;
    }
}
 
// Finds the leader (representative) of the group of point x
int findLeader(int x) {
    while (parentOf[x] != x) {
        x = parentOf[x];
    }
    return x;
}
 
// Joins the groups of a and b. Returns false if they were already in one group.
bool joinGroups(int a, int b) {
    int leaderA = findLeader(a);
    int leaderB = findLeader(b);
    if (leaderA == leaderB) {
        return false;           // same group -> pipe would make a cycle
    }
    parentOf[leaderA] = leaderB;
    return true;
}
 
// ---------- Kruskal's Algorithm ----------
 
// Returns the number of pipes chosen and stores them in chosen[]
int buildMinimumSpanningTree() {
    sortPipes();
    makeGroups();
    int count = 0;
    for (int i = 0; i < pipeCount && count < totalNodes - 1; i++) {
        if (joinGroups(allPipes[i].from, allPipes[i].to)) {
            chosen[count] = allPipes[i];
            count++;
        }
    }
    return count;
}
 
// ---------- Output functions ----------
 
// Gives a readable name for a point
void printName(int node) {
    if (node == 0) cout << "Water Source";
    else cout << "Plot " << node;
}
 
void showAllPipes() {
    cout << "\nAll possible pipelines (sorted by cost):\n";
    cout << "----------------------------------------\n";
    for (int i = 0; i < pipeCount; i++) {
        cout << i + 1 << ". ";
        printName(allPipes[i].from);
        cout << " <-> ";
        printName(allPipes[i].to);
        cout << "  (cost " << allPipes[i].cost << ")\n";
    }
}
 
void showResult(int count) {
    cout << "\n===== Minimum Cost Irrigation Network =====\n";
    int total = 0;
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". ";
        printName(chosen[i].from);
        cout << " <-> ";
        printName(chosen[i].to);
        cout << "  (cost " << chosen[i].cost << ")\n";
        total += chosen[i].cost;
    }
    cout << "-------------------------------------------\n";
    cout << "Pipelines used : " << count << "\n";
    cout << "Total minimum cost : " << total << "\n";
}
 
// ---------- Main ----------
 
int main() {
    cout << "=== Farm Irrigation System (Minimum Spanning Tree) ===\n\n";
    cout << "Point 0 is the Water Source. Points 1, 2, 3... are farm plots.\n\n";
 
    totalNodes = readInt("Enter total points (Water Source + plots, 2-20): ", 2, MAX_NODES);
 
    int maxPossible = totalNodes * (totalNodes - 1) / 2;
    if (maxPossible > MAX_PIPES) maxPossible = MAX_PIPES;
    int wanted = readInt("Enter number of possible pipelines: ", 1, maxPossible);
 
    readPipes(wanted);
 
    int count = buildMinimumSpanningTree();
    showAllPipes();
 
    if (count == totalNodes - 1) {
        showResult(count);
    } else {
        cout << "\nSome plots cannot be reached with the given pipelines.\n";
        cout << "Please add more possible pipelines and try again.\n";
    }
    return 0;
}
