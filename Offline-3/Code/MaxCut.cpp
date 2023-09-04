#include <iostream>
#include <random>
#include <ctime>
#include <fstream>
#include <set>
#include <map>
using namespace std;
typedef long long ll;

#define SETX 0
#define SETY 1
const int N = 1e6;

ll sigmaX[N], sigmaY[N];
map<pair<ll,ll>, ll> edges;
map< ll, vector< pair<ll,ll> > > adj;
set<ll> V, X, Y; // the two sets in max cut
pair<ll,ll> maxWeightEdge;

double getRealRandom(double limit) {
	mt19937 gen(time(0));

    uniform_real_distribution <> dis(0.0, limit);

    double random_number = dis(gen);

    return random_number;
}

ll getMaxWeight() {
    ll maxwt = LONG_LONG_MIN;
    for (pair p: edges) {
        if (maxwt < p.second)
            maxwt = p.second;
    }
    return maxwt;
}

ll getMinWeight() {
    ll minwt = LONG_LONG_MAX;
    for (pair p: edges) {
        if (minwt > p.second)
            minwt = p.second;
    }
    return minwt;
}

vector<pair<ll,ll>> getRCLe(double miu) {
    vector<pair<ll,ll>> RCL;
    for (pair p: edges) {
        if (p.second >= miu)
            RCL.push_back(p.first);
    }
    return RCL;
}

vector<ll> getRCLv(double miu, set<ll> V_rem) {
    vector<ll> RCL;
    for (ll v: V_rem) {
        if ( max(sigmaX[v], sigmaY[v]) >= miu ) 
            RCL.push_back(v);
    }
    return RCL;
}

pair<ll,ll> getRandomEdge(vector<pair<ll,ll>> RCL) {
    int randIdx = getRealRandom(RCL.size() - 1); // Choosing an edge from the RCL randomly

    return RCL[randIdx];
}

ll getRandomVertex(vector<ll> RCL) {
    int randIdx = getRealRandom(RCL.size() - 1); // Choosing an edge from the RCL randomly

    return RCL[randIdx];
}

void printSets() {
    cout << "Set X: ";
    for (ll a: X) cout << a << ' ';

    cout << endl;

    cout << "Set Y: ";
    for (ll a: Y) cout << a << ' ';

    cout << endl;
}

ll getSumY(int node) {
    ll sum = 0;
    for (ll y: Y) {
        pair<ll,ll> e = {node, y}, es = {y, node}; // seq maybe {a, b} or {b, a} one of them is zero
        sum += edges[e] + edges[es];
    }
    
    return sum;
}

ll getSumX(int node) {
    ll sum = 0;
    for (ll x: X) {
        pair<ll, ll> e = {node, x}, es = {x, node}; // seq maybe {a, b} or {b, a} one of them is zero
        sum += edges[e] + edges[es];
    }
    return sum;
}

void updateAdjacentNodes(ll node, int inSet, string operation) {
    for (pair<ll,ll> adjNode: adj[node]) {
        if (inSet == SETX) {
            if (operation == "add") sigmaY[adjNode.first] += adjNode.second;
            else if (operation == "remove") sigmaY[adjNode.first] -= adjNode.second;
        }
        else {
            if (operation == "add") sigmaX[adjNode.first] += adjNode.second;
            else if (operation == "remove") sigmaX[adjNode.first] -= adjNode.second;
        }
    }
}

void semiGreedyMaxCut() {
    ll val = 0;
    double alpha = getRealRandom(1.0);

    ll maxwt = getMaxWeight();
    ll minwt = getMinWeight();

    X.clear();
    Y.clear();
    
    double miu = minwt + alpha*(maxwt - minwt);
    vector<pair<ll, ll>> RCLe = getRCLe(miu);

    pair<ll,ll> randEdge = getRandomEdge(RCLe);

    X.insert(randEdge.first);
    Y.insert(randEdge.second);

    updateAdjacentNodes(randEdge.first, SETX, "add");
    updateAdjacentNodes(randEdge.second, SETY, "add");

    set<ll> V_rem(V.begin(), V.end());
    // V' = V \ (X union Y)
    V_rem.erase(randEdge.first);
    V_rem.erase(randEdge.second);

    while ((X.size() + Y.size()) != V.size()) {
        ll minsigmaX, maxsigmaX, minsigmaY, maxsigmaY;
        minsigmaX = minsigmaY = LONG_LONG_MAX;
        maxsigmaX = maxsigmaY = LONG_LONG_MIN;

        for (ll v : V_rem) {
            // sigmaX[v] = getSumY(v);
            // sigmaY[v] = getSumX(v);

            maxsigmaX = max(sigmaX[v], maxsigmaX);
            minsigmaX = min(sigmaX[v], minsigmaX);
            maxsigmaY = max(sigmaY[v], maxsigmaY);
            minsigmaY = min(sigmaY[v], minsigmaY);
        }

        maxwt = max(maxsigmaX, maxsigmaY);
        minwt = min(minsigmaX, minsigmaY);

        miu = minwt + alpha*(maxwt - minwt);
        vector<ll> RCLv = getRCLv(miu, V_rem);
        ll v = getRandomVertex(RCLv);

        if (sigmaX[v] > sigmaY[v]) {
            X.insert(v);
            V_rem.erase(v);
            updateAdjacentNodes(v, SETX, "add");
        }
        else {
            Y.insert(v);
            V_rem.erase(v);
            updateAdjacentNodes(v, SETY, "add");
        }
    }
}

void greedyMaxCut() {
    pair<ll,ll> maxEdge = maxWeightEdge;

    X.clear();
    Y.clear();

    X.insert(maxEdge.first);
    Y.insert(maxEdge.second);

    set<ll> V_rem(V.begin(), V.end());

    V_rem.erase(maxEdge.first);
    V_rem.erase(maxEdge.second);

    while (!V_rem.empty()) {
        ll selectedNode, greedy = LONG_LONG_MIN;

        for (ll v: V_rem) {
            sigmaX[v] = getSumY(v);
            sigmaY[v] = getSumX(v);

            ll maxval = max(sigmaX[v], sigmaY[v]);
            if ( greedy < maxval ) {
                greedy = maxval;
                selectedNode = v;
            }
        }
        if (sigmaX[selectedNode] > sigmaY[selectedNode]) 
            X.insert(selectedNode), V_rem.erase(selectedNode);
        else
            Y.insert(selectedNode), V_rem.erase(selectedNode);
    }
}

void randomizedMaxCut() {
    X.clear();
    Y.clear();

    for (ll v: V) {
        int randVal = getRealRandom(rand() % 20);
        if (randVal & 1) // odd
            X.insert(v);
        else
            Y.insert(v);
    }
}

void localSearch() {
    bool change = true;

    while(change) {
        change = false;

        for (ll v: V) {
            // sigmaX[v] = getSumY(v);
            // sigmaY[v] = getSumX(v);

            if (X.find(v) != X.end() and (sigmaY[v] > sigmaX[v])) {
                X.erase(v);
                Y.insert(v);
                updateAdjacentNodes(v, SETX, "remove");
                updateAdjacentNodes(v, SETY, "add");
                change = true;
            }
            if (Y.find(v) != Y.end() and (sigmaX[v] > sigmaY[v])) {
                Y.erase(v);
                X.insert(v);
                updateAdjacentNodes(v, SETY, "remove");
                updateAdjacentNodes(v, SETX, "add");
                change = true;
            }
        }
    }
}

ll getValue() {
    ll val = 0;
    for (ll elem: Y)
        val += getSumX(elem);
    return val;
}

ll GRASP(ll maxItrs) {
    ll maxVal = LONG_LONG_MIN;

    for (int i=1; i<=maxItrs; i++) {
        semiGreedyMaxCut();
        localSearch();
        maxVal = max(maxVal, getValue());
    }

    return maxVal;
}



int main() {
    fstream cin("input.txt");
	ll e, n, a, b, w;
	cin >> n >> e;

    ll maxwt = LONG_LONG_MIN;
	for (int i=0; i<e; i++) {
		cin >> a >> b >> w;
        edges[{a,b}] = w;
        V.insert(a);
        V.insert(b);

        adj[a].push_back({b, w});
        adj[b].push_back({a, w}); // undirected graph

        if (w > maxwt) {
            maxwt = w;
            maxWeightEdge = {a, b};
        }
	}

    semiGreedyMaxCut();
    // printSets();
    cout << "Value: " << getValue() << endl;

    // cout << "\n\n\nAfter local search" << endl;
    // localSearch();
    // printSets();
    // cout << "Value: " << getValue() << endl;

    // greedyMaxCut();
    // printSets();
    // cout << "Value : " << getValue() << endl;

    // randomizedMaxCut();
    // printSets();
    // cout << "Value: " << getValue() << endl;

    cout << "After GRASP" << endl;
    ll maxVal = GRASP(10);
    cout << "MaxCut: " << maxVal << endl;
    // printSets();

    return 0;
}

