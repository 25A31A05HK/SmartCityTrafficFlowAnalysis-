
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <string>
#include <limits>
#include <iomanip>

using namespace std;

const int INF = 1000000000;

// Junction structure
struct Junction {
    int id;
    string name;
};

// Traffic sensor structure
struct Traffic {
    int vehicles;
    double speed;
};

class CityTraffic {

private:

    int n;

    vector<Junction> junctions;

    vector<vector<int>> graph;

    unordered_map<int, Traffic> trafficMap;

    list<string> trafficHistory;

public:

    // Constructor
    CityTraffic(int size) {

        n = size;

        graph.assign(n, vector<int>(n, INF));

        for (int i = 0; i < n; i++) {
            graph[i][i] = 0;
        }
    }

    // Add junction
    void addJunction(int id, string name) {

        junctions.push_back({id, name});
    }

    // Add road
    void addRoad(int u, int v, int distance) {

        if (u < 0 || u >= n || v < 0 || v >= n ||
            distance < 0) {
            cout << "Invalid road!\n";
            return;
        }

        graph[u][v] = distance;
        graph[v][u] = distance;
    }

    // Display city network
    void displayCity() {

        cout << "\n--- CITY JUNCTIONS ---\n";

        for (auto j : junctions) {

            cout << j.id << " - " << j.name << endl;
        }

        cout << "\n--- ROAD NETWORK ---\n";

        for (int i = 0; i < n; i++) {

            for (int j = i + 1; j < n; j++) {

                if (graph[i][j] != INF) {

                    cout << junctions[i].name
                         << " <--> "
                         << junctions[j].name
                         << " : "
                         << graph[i][j] << " km\n";
                }
            }
        }
    }

    // Floyd-Warshall Algorithm
    void floydWarshall() {

        vector<vector<int>> dist = graph;

        vector<vector<int>> nextNode(
            n, vector<int>(n, -1)
        );

        for (int i = 0; i < n; i++) {

            for (int j = 0; j < n; j++) {

                if (i != j && graph[i][j] != INF) {

                    nextNode[i][j] = j;
                }
            }
        }

        // Dynamic Programming
        for (int k = 0; k < n; k++) {

            for (int i = 0; i < n; i++) {

                for (int j = 0; j < n; j++) {

                    if (dist[i][k] != INF &&
                        dist[k][j] != INF &&
                        dist[i][k] + dist[k][j] < dist[i][j]) {

                        dist[i][j] =
                            dist[i][k] + dist[k][j];

                        nextNode[i][j] = nextNode[i][k];
                    }
                }
            }
        }

        cout << "\n--- ALL PAIRS SHORTEST PATHS ---\n";

        cout << setw(15) << "Junction";

        for (int i = 0; i < n; i++) {
            cout << setw(12) << junctions[i].name;
        }

        cout << endl;

        for (int i = 0; i < n; i++) {

            cout << setw(15) << junctions[i].name;

            for (int j = 0; j < n; j++) {

                if (dist[i][j] == INF) {
                    cout << setw(12) << "INF";
                }
                else {
                    cout << setw(12) << dist[i][j];
                }
            }

            cout << endl;
        }

        cout << "\n--- SHORTEST ROUTE DETAILS ---\n";

        for (int i = 0; i < n; i++) {

            for (int j = 0; j < n; j++) {

                if (i == j || nextNode[i][j] == -1) {
                    continue;
                }

                cout << junctions[i].name << " to "
                     << junctions[j].name << " = ";

                int current = i;

                cout << junctions[current].name;

                while (current != j) {

                    current = nextNode[current][j];

                    cout << " -> "
                         << junctions[current].name;
                }

                cout << " ("
                     << dist[i][j] << " km)\n";
            }
        }
    }

    // Dijkstra Algorithm
    void dijkstra(int source) {

        if (source < 0 || source >= n) {

            cout << "Invalid junction ID!\n";
            return;
        }

        vector<int> dist(n, INF);

        vector<int> parent(n, -1);

        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > pq;

        dist[source] = 0;

        pq.push({0, source});

        while (!pq.empty()) {

            int d = pq.top().first;

            int u = pq.top().second;

            pq.pop();

            if (d != dist[u]) {
                continue;
            }

            for (int v = 0; v < n; v++) {

                // Correct shortest path calculation
                if (graph[u][v] != INF &&
                    d + graph[u][v] < dist[v]) {

                    dist[v] = d + graph[u][v];

                    parent[v] = u;

                    pq.push({dist[v], v});
                }
            }
        }

        cout << "\n--- DIJKSTRA SHORTEST ROUTES ---\n";

        for (int i = 0; i < n; i++) {

            cout << junctions[i].name << " : ";

            if (dist[i] == INF) {

                cout << "Unreachable\n";
                continue;
            }

            cout << dist[i] << " km | Path: ";

            stack<int> path;

            int current = i;

            while (current != -1) {

                path.push(current);

                current = parent[current];
            }

            while (!path.empty()) {

                cout << junctions[path.top()].name;

                path.pop();

                if (!path.empty()) {
                    cout << " -> ";
                }
            }

            cout << endl;
        }
    }

    // Update sensor data
    void updateTraffic(int id, int vehicles, double speed) {

        if (id < 0 || id >= n ||
            vehicles < 0 || speed < 0 || speed > 200) {

            cout << "Invalid sensor data!\n";
            return;
        }

        trafficMap[id] = {vehicles, speed};

        string record =
            junctions[id].name + " : " +
            to_string(vehicles) + " vehicles";

        trafficHistory.push_back(record);

        cout << "Traffic sensor updated successfully!\n";
    }

    // Display sensor data
    void displayTraffic() {

        cout << "\n--- TRAFFIC SENSOR DATA ---\n";

        cout << left
             << setw(18) << "Junction"
             << setw(15) << "Vehicles"
             << setw(15) << "Speed"
             << "Status\n";

        for (auto &item : trafficMap) {

            int id = item.first;

            Traffic t = item.second;

            string status;

            if (t.vehicles >= 100) {
                status = "Heavy";
            }
            else if (t.vehicles >= 50) {
                status = "Moderate";
            }
            else {
                status = "Low";
            }

            cout << left
                 << setw(18) << junctions[id].name
                 << setw(15) << t.vehicles
                 << setw(15) << t.speed
                 << status << endl;
        }
    }

    // Priority Queue - Top K congestion
    void topKCongested(int k) {

        if (k <= 0) {

            cout << "Enter a positive K value!\n";
            return;
        }

        priority_queue<pair<int, int>> pq;

        for (auto &item : trafficMap) {

            pq.push({
                item.second.vehicles,
                item.first
            });
        }

        cout << "\n--- TOP K CONGESTED JUNCTIONS ---\n";

        int count = 0;

        while (!pq.empty() && count < k) {

            int vehicles = pq.top().first;

            int id = pq.top().second;

            pq.pop();

            cout << count + 1 << ". "
                 << junctions[id].name
                 << " - " << vehicles
                 << " vehicles\n";

            count++;
        }

        if (count == 0) {
            cout << "No traffic data available.\n";
        }
    }

    // BFS using Queue
    void BFS(int start) {

        if (start < 0 || start >= n) {

            cout << "Invalid junction ID!\n";
            return;
        }

        vector<bool> visited(n, false);

        queue<int> q;

        visited[start] = true;

        q.push(start);

        cout << "\nBFS Traversal: ";

        while (!q.empty()) {

            int u = q.front();

            q.pop();

            cout << junctions[u].name << " ";

            for (int v = 0; v < n; v++) {

                if (graph[u][v] != INF &&
                    u != v && !visited[v]) {

                    visited[v] = true;

                    q.push(v);
                }
            }
        }

        cout << endl;
    }

    // DFS recursive function
    void DFSUtil(int u, vector<bool> &visited) {

        visited[u] = true;

        cout << junctions[u].name << " ";

        for (int v = 0; v < n; v++) {

            if (graph[u][v] != INF &&
                u != v && !visited[v]) {

                DFSUtil(v, visited);
            }
        }
    }

    // DFS traversal
    void DFS(int start) {

        if (start < 0 || start >= n) {

            cout << "Invalid junction ID!\n";
            return;
        }

        vector<bool> visited(n, false);

        cout << "\nDFS Traversal: ";

        DFSUtil(start, visited);

        cout << endl;
    }

    // Search junction
    void searchJunction(string name) {

        for (auto j : junctions) {

            if (j.name == name) {

                cout << "Junction Found!\n";

                cout << "ID: " << j.id << endl;

                return;
            }
        }

        cout << "Junction not found!\n";
    }

    // Traffic prediction using historical average
    void predictTraffic(int id) {

        if (id < 0 || id >= n) {

            cout << "Invalid junction ID!\n";
            return;
        }

        double total = 0;

        int count = 0;

        string key = junctions[id].name + " : ";

        for (auto &record : trafficHistory) {

            if (record.find(key) == 0) {

                size_t pos = record.find("vehicles");

                if (pos != string::npos) {

                    int vehicles = stoi(
                        record.substr(
                            key.length(),
                            pos - key.length()
                        )
                    );

                    total += vehicles;

                    count++;
                }
            }
        }

        if (count == 0) {

            cout << "No historical traffic data available.\n";
            return;
        }

        double prediction = total / count;

        cout << "\n--- TRAFFIC PREDICTION ---\n";

        cout << "Junction: "
             << junctions[id].name << endl;

        cout << "Average vehicles: "
             << fixed << setprecision(2)
             << prediction << endl;

        if (prediction >= 100) {
            cout << "Predicted status: Heavy Traffic\n";
        }
        else if (prediction >= 50) {
            cout << "Predicted status: Moderate Traffic\n";
        }
        else {
            cout << "Predicted status: Low Traffic\n";
        }

        cout << "Based on historical average estimates.\n";
    }

    // Display linked list history
    void displayHistory() {

        cout << "\n--- TRAFFIC UPDATE HISTORY ---\n";

        if (trafficHistory.empty()) {

            cout << "No history available.\n";
            return;
        }

        int count = 1;

        for (auto record : trafficHistory) {

            cout << count++ << ". "
                 << record << endl;
        }
    }

    // Sorting congestion
    void sortCongestion() {

        vector<pair<int, int>> data;

        for (auto &item : trafficMap) {

            data.push_back({
                item.second.vehicles,
                item.first
            });
        }

        sort(
            data.begin(),
            data.end(),
            greater<pair<int, int>>()
        );

        cout << "\n--- CONGESTION RANKING ---\n";

        for (auto item : data) {

            cout << junctions[item.second].name
                 << " : " << item.first
                 << " vehicles\n";
        }

        if (data.empty()) {
            cout << "No sensor data available.\n";
        }
    }
};

// Main function
int main() {

    CityTraffic city(6);

    // Create junctions
    city.addJunction(0, "Main");

    city.addJunction(1, "Railway");

    city.addJunction(2, "BusStand");

    city.addJunction(3, "College");

    city.addJunction(4, "Hospital");

    city.addJunction(5, "Market");

    // Create roads
    city.addRoad(0, 1, 4);

    city.addRoad(0, 2, 3);

    city.addRoad(1, 2, 2);

    city.addRoad(1, 3, 5);

    city.addRoad(2, 3, 3);

    city.addRoad(2, 4, 6);

    city.addRoad(3, 4, 2);

    city.addRoad(3, 5, 4);

    city.addRoad(4, 5, 3);

    int choice;

    do {

        cout << "\n";
        cout << "========================================\n";

        cout << "    SMART CITY TRAFFIC FLOW ANALYSIS\n";

        cout << "========================================\n";

        cout << "1. Display City Road Network\n";

        cout << "2. Floyd Warshall Shortest Paths\n";

        cout << "3. Dijkstra Shortest Route\n";

        cout << "4. Update Traffic Sensor Data\n";

        cout << "5. Display Traffic Data\n";

        cout << "6. Top K Congested Intersections\n";

        cout << "7. Predict Future Traffic\n";

        cout << "8. BFS Traversal\n";

        cout << "9. DFS Traversal\n";

        cout << "10. Search Junction\n";

        cout << "11. Display Traffic History\n";

        cout << "12. Sort Congestion Ranking\n";

        cout << "13. Exit\n";

        cout << "\nEnter your choice: ";

        if (!(cin >> choice)) {

            cin.clear();

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cout << "Invalid input!\n";

            continue;
        }

        int id, vehicles, k, start;

        double speed;

        string name;

        switch (choice) {

            case 1:

                city.displayCity();

                break;

            case 2:

                city.floydWarshall();

                break;

            case 3:

                cout << "Enter starting junction ID (0-5): ";

                cin >> start;

                city.dijkstra(start);

                break;

            case 4:

                cout << "Enter junction ID (0-5): ";

                cin >> id;

                cout << "Enter number of vehicles: ";

                cin >> vehicles;

                cout << "Enter average speed: ";

                cin >> speed;

                city.updateTraffic(id, vehicles, speed);

                break;

            case 5:

                city.displayTraffic();

                break;

            case 6:

                cout << "Enter K value: ";

                cin >> k;

                city.topKCongested(k);

                break;

            case 7:

                cout << "Enter junction ID (0-5): ";

                cin >> id;

                city.predictTraffic(id);

                break;

            case 8:

                cout << "Enter starting junction ID (0-5): ";

                cin >> start;

                city.BFS(start);

                break;

            case 9:

                cout << "Enter starting junction ID (0-5): ";

                cin >> start;

                city.DFS(start);

                break;

            case 10:

                cout << "Enter junction name: ";

                cin >> name;

                city.searchJunction(name);

                break;

            case 11:

                city.displayHistory();

                break;

            case 12:

                city.sortCongestion();

                break;

            case 13:

                cout << "\nThank you for using Smart City Traffic Analysis!\n";

                break;

            default:

                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 13);

    return 0;
}
