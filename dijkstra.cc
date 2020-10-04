#include<iostream>
#include<climits>
#include<vector>
#include<set>
using namespace std;

#define maxHeapSizeConst 100000
#define parent(i) ((i-1)/2)
#define left_child(i) (2*i+1)
#define right_child(i) (2*i+2)

///// HEAP /////

class MinHeapNode {
public:
	int v;
	int dist;

	MinHeapNode(int v, int dist) {
		this->v = v;
		this->dist = dist;
	}
};

class MinHeap {
public:
	int curHeapSize;
	int maxHeapSize;
	int *pos;
	MinHeapNode * *heapArr;

	// MinHeap() {
	// 	curHeapSize = 0;
	// 	maxHeapSize = maxHeapSizeConst;
	// 	pos = new int[maxHeapSize];
	// 	heapArr = new int[maxHeapSize];
	// }

	MinHeap(int maxSize) {
		curHeapSize = 0;
		maxHeapSize = maxSize;
		pos = new int[maxHeapSize];
		heapArr = new MinHeapNode*[maxHeapSize];
	}

	void insert(int v, int dist) {

		if (curHeapSize == maxHeapSize) return;

		int i = curHeapSize;
		heapArr[curHeapSize] = new MinHeapNode(v, dist);
		curHeapSize++;
		while (i > 0 && heapArr[parent(i)]->dist > heapArr[i]->dist) {
			swap(heapArr[parent(i)], heapArr[i]);
			pos[heapArr[i]->v] = i;
			i = parent(i);
		}

		pos[heapArr[i]->v] = i;
	}

	void heapify(int i) {
		int indexToSwap;
		while (i < curHeapSize) {
			indexToSwap = i;
			if (left_child(i) < curHeapSize && heapArr[i]->dist > heapArr[left_child(i)]->dist) {
				indexToSwap = left_child(i);
			}

			if (right_child(i) < curHeapSize && heapArr[indexToSwap]->dist > heapArr[right_child(i)]->dist) {
				indexToSwap = right_child(i);
			}

			if (i == indexToSwap) break;
			swap(heapArr[i], heapArr[indexToSwap]);
			pos[heapArr[i]->v] = i;
			i = indexToSwap;
		}
		pos[heapArr[i]->v] = i;
	}

	void build_heap(int n, vector<int> dist) {
		curHeapSize = n;

		for (int i = 0; i < n; i++) {
			heapArr[i] = new MinHeapNode(i, dist[i]);
			pos[i] = i;
		}

		for (int i = n - 1; i >= 0; i--) {
			heapify(i);
		}
	}

	bool empty() {
		if (curHeapSize == 0) {
			return true;
		}
		return false;
	}

	MinHeapNode* extract_min() {
		if (curHeapSize == 0) return NULL;

		MinHeapNode* minNode = heapArr[0];
		curHeapSize--;
		swap(heapArr[0], heapArr[curHeapSize]);
		pos[heapArr[0]->v] = 0;
		pos[heapArr[curHeapSize]->v] = curHeapSize;
		int i = 0;

		int indexToSwap;
		while (i < curHeapSize) {
			indexToSwap = i;
			if (left_child(i) < curHeapSize && heapArr[i]->dist > heapArr[left_child(i)]->dist) {
				indexToSwap = left_child(i);
			}

			if (right_child(i) < curHeapSize && heapArr[indexToSwap]->dist > heapArr[right_child(i)]->dist) {
				indexToSwap = right_child(i);
			}

			if (i == indexToSwap) break;
			swap(heapArr[i], heapArr[indexToSwap]);
			pos[heapArr[i]->v] = i;
			i = indexToSwap;
		}

		pos[heapArr[i]->v] = i;
		return minNode;
	}

	MinHeapNode* get_min() {
		if (curHeapSize == 0) return NULL;
		return heapArr[0];
	}

	void decrease_key(int v, int newVal) {
		// newVal should be less than older value

		int indInHeapArr = pos[v];
		heapArr[indInHeapArr]->dist = newVal;
		while (indInHeapArr > 0 && heapArr[parent(indInHeapArr)]->dist > heapArr[indInHeapArr]->dist) {
			swap(heapArr[parent(indInHeapArr)], heapArr[indInHeapArr]);
			pos[heapArr[indInHeapArr]->v] = indInHeapArr;
			indInHeapArr = parent(indInHeapArr);
		}

		pos[heapArr[indInHeapArr]->v] = indInHeapArr;
	}
};

///// GRAPH /////

class GraphAL {
public:
	int v;
	int e;
	bool biDirectional;
	vector<vector<pair<int, int>>> edges;

	GraphAL() {
		v = 0;
		e = 0;
		biDirectional = true;
	}
	GraphAL(int v, int e, int arr[][3], bool biDirectional = true) {
		this->v = v;
		this->e = e;
		this->biDirectional = biDirectional;
		edges = vector<vector<pair<int, int>>>(v);
		if (biDirectional)
			for (int i = 0; i < e; i++) {
				cout << i << endl;
				edges[arr[i][0]].push_back(make_pair(arr[i][1], arr[i][2]));
				edges[arr[i][1]].push_back(make_pair(arr[i][0], arr[i][2]));
			}

		if (!biDirectional)
			for (int i = 0; i < e; i++) {
				edges[arr[i][0]].push_back(make_pair(arr[i][1], arr[i][2]));
			}
	}

	vector<int> dijkstra(int src, int disSrc = 0) {

		if (v == 0) return vector<int>();
		MinHeap* minHeap = new MinHeap(v);
		vector<bool> sptSet(v, false);

		vector<int> distance(v, INT_MAX);
		distance[src] = disSrc;
		minHeap->build_heap(v, distance);

		while (minHeap->empty() == false) {
			MinHeapNode* minHeapNode = minHeap->extract_min();
			int v = minHeapNode->v;
			int dist = minHeapNode->dist;
			sptSet[v] = true;

			distance[v] = dist;

			int len = edges[v].size();
			for (int i = 0; i < len; i++) {
				if (!sptSet[edges[v][i].first] && dist != INT_MAX && edges[v][i].second != INT_MAX
				        && distance[edges[v][i].first] > distance[v] + edges[v][i].second) {
					distance[edges[v][i].first] = distance[v] + edges[v][i].second;
					minHeap->decrease_key(edges[v][i].first, distance[edges[v][i].first]);
				}
			}
		}

		return distance;
	}

	vector<int> bellman_ford(int src, int disSrc = 0) {
		vector<int> dist(v, INT_MAX);
		dist[src] = disSrc;

		for (int i = 0; i < v - 1; i++) {
			for (int j = 0; j < v; j++) {
				int len = edges[j].size();
				for (int k = 0; k < len; k++) {
					if (dist[j] != INT_MAX && dist[edges[j][k].first] > dist[j] + edges[j][k].second) {
						dist[edges[j][k].first] = dist[j] + edges[j][k].second;
					}
				}
			}
		}

		for (int j = 0; j < v; j++) {
			int len = edges[j].size();
			for (int k = 0; k < len; k++) {
				if (dist[j] != INT_MAX && dist[edges[j][k].first] > dist[j] + edges[j][k].second) {
					cout << "Negative Weight Cycle Present" << endl;
					return vector<int>();
				}
			}
		}

		return dist;
	}

	int dijkstraUsingSet(int src, int distSrc = 0) {
		// Create a set to store vertices that are being
		// prerocessed
		set< pair<int, int> > setds;

		// Create a vector for distances and initialize all
		// distances as infinite (INF)
		vector<int> dist(v, INT_MAX);

		// Insert source itself in Set and initialize its
		// distance as 0.
		setds.insert(make_pair(distSrc, src));
		dist[src] = distSrc;

		/* Looping till all shortest distance are finalized
		   then setds will become empty */
		while (!setds.empty())
		{
			// The first vertex in Set is the minimum distance
			// vertex, extract it from set.
			pair<int, int> tmp = *(setds.begin());
			setds.erase(setds.begin());

			// vertex label is stored in second of pair (it
			// has to be done this way to keep the vertices
			// sorted distance (distance must be first item
			// in pair)
			int u = tmp.second;

			// 'i' is used to get all adjacent vertices of a vertex
			int len = edges[u].size();
			for (int i = 0; i < len; i++)
			{
				// Get vertex label and weight of current adjacent
				// of u.
				int v = edges[u][i].first;
				int weight = edges[u][i].second;

				//  If there is shorter path to v through u.
				if (dist[v] > dist[u] + weight)
				{
					/*  If distance of v is not INF then it must be in
					    our set, so removing it and inserting again
					    with updated less distance.
					    Note : We extract only those vertices from Set
					    for which distance is finalized. So for them,
					    we would never reach here.  */
					if (dist[v] != INT_MAX)
						setds.erase(setds.find(make_pair(dist[v], v)));

					// Updating distance of v
					dist[v] = dist[u] + weight;
					setds.insert(make_pair(dist[v], v));
				}
			}
		}

		// Print shortest distances stored in dist[]
		return dist[v - 1];
	}

	vector<vector<pair<int, int>>> floyd_warshall() {

	}
};

int main() {
	// int v;
	// int e;
	// cin >> v >> e;
	// int arr[e][3];
	// for (int i = 0; i < e; i++) {
	// 	cin >> arr[i][0] >> arr[i][1] >> arr[i][2];
	// }
	// GraphAL g(v, e, arr);
	// int src;
	// cin >> src;
	// vector<int> dist = g.bellman_ford(src);

	// int len = dist.size();
	// for (int i = 0; i < len; i++) {
	// 	cout << i << " " << dist[i] << endl;
	// }

	// 6
	// 8
	// 1 2 10
	// 1 3 20
	// 0 4 100
	// 2 5 10
	// 0 3 5
	// 3 5 5
	// 2 4 1
	// 1 4 100
}