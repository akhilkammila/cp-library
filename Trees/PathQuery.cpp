/*
Personal code for queries that go to the root
Idea
    each node stores sum to root
    query: query a single value
    update: update every node in subtree range

    since range updates are annoying, we store difference array and do
    range query point update (query 0-n, update both l and r+1)

Tree is rooted at 0 by default
To change query
    change "path to root" calculation in euler tour, pathSum initial vals
    change update
    change query
    change constructSegtree

Usage:
    initialize
        param: n, the # of nodes
    addEdge
        params: u, v
    initialize
        param: values
        runs euler tour
            finds the start/end range for each element
            finds the sum to root for each el
        initializes segtree
            normally, each node's start index --> value
            we do that, but with dif array
    update
        param: node n
               value v
        find start, end index
        update start to += v
        update end+1 to -= v
    query
        param: node n
        find start index of n
        query from (0, n)
*/

#include <bits/stdc++.h>
#define int long long
using namespace std;

struct PathQ {
    int n;
    int timer = 0; // for euler tour
    vector<int> tree;
    vector<vector<int>> adj;
    vector<pair<int,int>> ranges;
    vector<int> values;
    vector<int> pathSum;
    bool initialized = false;

    PathQ(int n): n(n), adj(n), ranges(n), tree(2*n, 0), pathSum(n) {}

    void addEdge(int u, int v) {
        assert(u < n && v < n);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void eulerTour(int curr = 0, int prev = 0) {
        pathSum[curr] = values[curr] + pathSum[prev]; // update pathSum

        ranges[curr].first = timer;
        for(int neighbor : adj[curr]) {
            if (neighbor == prev) continue;
            timer ++;
            eulerTour(neighbor, curr);
        }
        ranges[curr].second = timer;

        initialized = true; // mark for error checking
    }

    void initialize(vector<int> val, int root = 0) {
        // Run euler tour to get pathSum and ranges
        assert(val.size() == n);
        values = val;
        eulerTour(root, root);

        // Get difference arr for segtree
        // Get normal arr
        vector<int> initialVals(n);
        for(int node = 0; node < n; node++) {
            int start = ranges[node].first;
            initialVals[start] = pathSum[node];
        }
        // Turn to difference arr
        for(int node = n-1; node >= 0; node--) {
            int prev = (node == 0) ? 0 : initialVals[node-1];
            initialVals[node] = initialVals[node] - prev;
        }

        // Initialize segtree
        // Initialize last n values of segtree
        for(int i = 0; i < n; i++) tree[i+n] = initialVals[i];
        // Propogate initialization
        for(int i = n-1; i >= 0; i--) {
            tree[i] = tree[i*2] + tree[i*2+1];
        }
        initialized = true;
    }

    void update(int node, int value) {
        assert(initialized);
        int l = ranges[node].first;
        int r = ranges[node].second + 1;

        // Update l
        for(l += n; l > 0; l >>=1) {
            tree[l] += value; // change for different query
        }

        if (r == n) return;
        // Update r (opposite operation)
        for(r += n; r > 0; r >>=1) {
            tree[r] -= value; // opposite operation
        }
    }

    int query(int node) {
        assert(initialized);
        int l = 0;
        int r = ranges[node].first;

        int answ = 0;
        for(l += n, r += n; l <= r; l>>= 1, r>>=1) {
            if (l&1) answ += tree[l++];
            if (!(r&1)) answ += tree[r--];
        }
        return answ;
    }
};

/*
Test: Path Queries https://cses.fi/problemset/task/1138
*/

void setIO(string s) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
	freopen((s + ".in").c_str(), "r", stdin);
	freopen((s + ".out").c_str(), "w", stdout);
}

int32_t main() {
    // setIO("path");
    int N, Q; cin >> N >> Q;
    vector<int> values(N);
    for(int i = 0; i < N; i++) cin >> values[i];

    PathQ t(N);
    // Add edges
    for(int i = 0; i < N-1; i++) {
        int u, v; cin >> u >> v; u--, v--;
        t.addEdge(u, v);
    }

    // Initialize (add vals + run euler tour)
    t.initialize(values);

    // Queries
    while(Q--) {
        int q; cin >> q;
        if (q == 1) {
            int node, val; cin >> node >> val; node--;

            int addition = val - values[node];
            values[node] = val;

            t.update(node, addition);
        }

        if (q == 2) {
            int node; cin >> node; node--;
            cout << t.query(node) << '\n';
        }
    }
}