/*
Personal LCA implementation

Tree is rooted at 0 by default (for euler tour)

Usage:
    initialize
        param: n, the # of nodes
    add a bidirectional edge
        param: u, node 1
        param: v, node 2 (0-indexed)
    run initializeLCA
        param: none (optionally add root as param1 and param2, default is 0)
        euler tour initializes node order, depths, last occurence
        segment tree initializations of tree (lowest depth in subtree) and minIndex (index of lowest depth)
    query
        param: u, node 1
               v, node 2
        finds indexes of last occurence of u and v in the euler tour (using last)
        segtree query to find the INDEX of the min depth element from last1 to last2
        returns the node of the min depth element (translates index --> noe using nodeOrder)
*/

#include <bits/stdc++.h>
using namespace std;

struct LCA {
    int n, eulerLength;
    vector<vector<int>> adj;
    vector<int> nodeOrder, depths, last;
    vector<int> tree, minIndex;
    bool initialized = false;

    LCA(int n): n(n), adj(n), eulerLength(2*n-1), tree(2*eulerLength), minIndex(2*eulerLength){}

    void addEdge(int u, int v) {
        assert(u < n && v < n);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void eulerTour(int curr, int prev, int depth) {
        nodeOrder.push_back(curr);
        depths.push_back(depth);
        for(int neighbor : adj[curr]) {
            if (neighbor == prev) continue;
            eulerTour(neighbor, curr, depth+1);

            nodeOrder.push_back(curr);
            depths.push_back(depth);
        }
        last[curr] = depths.size()-1;
    }

    void initializeLCA(int root = 0) {
        // run eulerTour for node order, depths, and last
        last.resize(n);
        eulerTour(root, root, 1);

        // initialize segtree
        // fill in last eulerLength values
        for(int i = 0; i < eulerLength; i++) {
            tree[i+eulerLength] = depths[i];
            minIndex[i+eulerLength] = i;
        }

        // propagate values up segTree
        for(int i = eulerLength-1; i >= 0; i--) {
            tree[i] = min(tree[i*2], tree[i*2+1]);
            if (tree[i*2] < tree[i*2+1]) minIndex[i] = minIndex[i*2];
            else minIndex[i] = minIndex[i*2+1];
        }
        
        initialized = true; //mark for error checking
    }

    int query(int u, int v) {
        assert(initialized); // check for initialization

        // find index of last occurence of u and v in euler
        int l = last[u];
        int r = last[v];
        if (l > r) swap(l, r);

        // do segtree query, but get the index of the minDepth node
        int minDepth = INT_MAX;
        int index = -1;

        for(l += eulerLength, r += eulerLength; l<=r; l>>=1, r>>=1) {
            if (l&1) {
                if (tree[l] < minDepth) {
                    minDepth = tree[l];
                    index = minIndex[l];
                }
                l++;
            }

            if (!(r&1)) {
                if (tree[r] < minDepth) {
                    minDepth = tree[r];
                    index = minIndex[r];
                }
                r--;
            }
        }
        
        // translate index of minDepth node to the node #
        return nodeOrder[index];
    }
};

/*
Testing: Company Queries II: https://cses.fi/problemset/task/1688
*/

void setIO(string s) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
	freopen((s + ".in").c_str(), "r", stdin);
	freopen((s + ".out").c_str(), "w", stdout);
}

int main() {
    // setIO("lca");

    int N, Q; cin >> N >> Q;
    LCA lca(N);

    for(int v = 1; v < N; v++) {
        int u; cin >> u; u--;
        lca.addEdge(u, v);
    }

    lca.initializeLCA();

    while(Q--) {
        int u, v; cin >> u >> v; u--, v--;
        cout << lca.query(u, v) + 1 << endl;
    }
}