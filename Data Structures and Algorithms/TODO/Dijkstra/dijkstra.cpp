#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

const int MAX = 1010;
const int INF = 1000000;

int graph[MAX][MAX];
int dis[MAX];
bool visited[MAX];

void dijkstra(int start, int n) {
	int minimum_distance, closest_vertex;
	dis[start] = 0;
	for(int i=1; i<=n; ++i) {
		minimum_distance = INF;
		closest_vertex = start;

		//find closest vertex
		for(int j=1; j<=n; ++j) {
			if (!visited[j] && dis[j] < minimum_distance) {
				minimum_distance = dis[j];
				closest_vertex = j;
			}
		}

		// printf("closest_vertex : %d minimum_distance : %d\n", closest_vertex, minimum_distance);

		visited[closest_vertex] = true;
		//update distance from this vertex
		for(int j=1; j<=n; ++j) {
			if (!visited[j]) {
				dis[j] = min(dis[j], dis[closest_vertex] + graph[closest_vertex][j]);
			}
		}
	}
}

int main() {
	//Input graph
	int n, m, x, y, z;
	cin>>n>>m;
	memset(graph, INF, sizeof(graph));
	for(int i=1; i<=m; ++i) {
		cin>>x>>y>>z;
		graph[x][y] = z;
		graph[y][x] = z;
	}
	int source = 1;

	//Initially set all distance to infinity
	memset(dis, INF, sizeof(dis));
	//Initially set all vertices as unvisited
	memset(visited, false, sizeof(visited));

	double start = omp_get_wtime();
	//Run dijkstra's
	dijkstra(source, n);
	double end = omp_get_wtime();

	//Print results
	printf("Vertices\tDistances\n");
	for(int i=1; i<=n; ++i) {
		printf("%8d\t%9d\n", i, dis[i]);
	}

	//print time taken
	printf("-------------------------\n");
	printf("Time taken : %lf seconds.\n", end - start);
}