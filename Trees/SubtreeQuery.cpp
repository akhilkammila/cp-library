/*
Personal code for subtree-sum and other queries

Tree is rooted at 0 by default

To change query type
    change update
    change query
    change tree's initial values (in constructor)

Usage:
    initialize
        param: n, the # of nodes
    addEdge
        params: u, v
    eulerTour
        param: none
        runs euler tour, finds the start/end range for each element
        initializes segtree (now we can update)
    update (DONT FORGET TO INITIALIZE VALS WITH THIS)
        param: node n
               value v
        used for initializing segtree values also (so we only need to change one function)
        find start index of node
        update that index to += v
    query
        param: node n
        find start and end index of n
        query over range (s,e)
*/

#include <bits/stdc++.h>
#define int long long
using namespace std;

struct SubtreeQ {
    int n;
    int timer = 0; // for euler tour
    vector<int> tree;
    vector<vector<int>> adj;
    vector<pair<int,int>> ranges;
    bool initialized = false;

    SubtreeQ(int n): n(n), adj(n), ranges(n), tree(2*n, 0) {}

    void addEdge(int u, int v) {
        assert(u < n && v < n);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void eulerTour(int curr = 0, int prev = 0) {
        ranges[curr].first = timer;
        for(int neighbor : adj[curr]) {
            if (neighbor == prev) continue;
            timer ++;
            eulerTour(neighbor, curr);
        }
        ranges[curr].second = timer;

        initialized = true; // mark for error checking
    }

    void update(int node, int value) {
        assert(initialized);
        int nodeIndex = ranges[node].first;
        for(nodeIndex += n; nodeIndex > 0; nodeIndex >>=1) {
            tree[nodeIndex] += value; // change for different query
        }
    }

    int query(int node) {
        assert(initialized);
        int l = ranges[node].first;
        int r = ranges[node].second;

        int answ = 0;
        for(l += n, r += n; l <= r; l>>= 1, r>>=1) {
            if (l&1) answ += tree[l++];
            if (!(r&1)) answ += tree[r--];
        }
        return answ;
    }
};

/*
Test: Subtree Queries https://cses.fi/problemset/task/1137/
*/

void setIO(string s) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
	freopen((s + ".in").c_str(), "r", stdin);
	freopen((s + ".out").c_str(), "w", stdout);
}

int32_t main() {
    // setIO("subtree");
    int N, Q; cin >> N >> Q;
    SubtreeQ t(N);

    // Read in values
    vector<int> values(N);
    for(int i = 0; i < N; i++) cin >> values[i];

    // Read in and update Edges
    for(int i = 0; i < N-1; i++) {
        int u, v; cin >> u >> v; u--, v--;
        t.addEdge(u, v);
    }
    t.eulerTour(); // Run euler tour

    // Update with initial values
    for(int i = 0; i < N; i++) {
        t.update(i, values[i]);
    }

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