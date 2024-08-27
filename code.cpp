
#include <iostream>
#include <unordered_map>
#include <vector>
#include <utility>
#include <queue>
#include <climits>
#include <stack>
#include <bits/stdc++.h>

using namespace std;

struct Path {
    double cost;
    double time;
    vector<string> route;
};

class Graph {
private:
    unordered_map<string, vector<pair<string, pair<double, double>>>> adjacencyList;

public:
    void addCity(const string& cityName) {
        adjacencyList[cityName] = vector<pair<string, pair<double, double>>>();
    }

    void addEdge(const string& source, const string& destination, double cost, double time) {
        adjacencyList[source].push_back({destination, {cost, time}});
        adjacencyList[destination].push_back({source, {cost, time}});
    }

    Path shortestRouteWithTime(const string& source, const string& destination) {
        Path p;
        p.cost = -1;
        p.time = -1;

        if (adjacencyList.count(source) && adjacencyList.count(destination)) {
            // Temporarily modify the edge weights
            modifyEdgeWeights(source, destination);

            unordered_map<string, double> cost;
            unordered_map<string, double> time;
            unordered_map<string, string> previous;
            previous[source] = "##";
            priority_queue<pair<pair<double, double>, string>, vector<pair<pair<double, double>, string>>, greater<pair<pair<double, double>, string>>> pq;

            for (const auto& city : adjacencyList) {
                if (city.first == source) {
                    cost[city.first] = 0.0;
                    time[city.first] = 0.0;
                } else {
                    cost[city.first] = DBL_MAX;
                    time[city.first] = DBL_MAX;
                }
            }

            pq.push({{0.0, 0.0}, source});

            while (!pq.empty()) {
                string currentCity = pq.top().second;
                double currentCost = pq.top().first.second;
                double currentTime = pq.top().first.first;
                pq.pop();

                if (currentCity == destination) {
                    p.cost = currentCost;
                    p.time = currentTime;
                    vector<string> v;

                    while (previous[currentCity] != "##") {
                        v.push_back(currentCity);
                        currentCity = previous[currentCity];
                    }

                    v.push_back(source);
                    reverse(v.begin(), v.end());
                    p.route = v;
                    restoreEdgeWeights(source, destination);
                    return p;
                }

                for (auto it : adjacencyList[currentCity]) {
                    string nextCity = it.first;
                    double nextCost = it.second.first;
                    double nextTime = it.second.second;

                    if (time[nextCity] > currentTime + nextTime) {
                        time[nextCity] = currentTime + nextTime;
                        cost[nextCity] = currentCost + nextCost;
                        previous[nextCity] = currentCity;
                        pq.push({{time[nextCity], cost[nextCity]}, nextCity});
                    }
                }
            }

            // Restore the original edge weights
            
        }

        return p;
    }

    Path cheapestRoute(const string& source, const string& destination) {
        Path p;
        p.cost = -1;
        p.time = -1;

        if (adjacencyList.count(source) && adjacencyList.count(destination)) {
            // Temporarily modify the edge weights
            modifyEdgeWeights(source, destination);

            unordered_map<string, double> cost;
            unordered_map<string, double> time;
            unordered_map<string, string> previous;
            previous[source] = "##";
            priority_queue<pair<pair<double, double>, string>, vector<pair<pair<double, double>, string>>, greater<pair<pair<double, double>, string>>> pq;

            for (const auto& city : adjacencyList) {
                if (city.first == source) {
                    cost[city.first] = 0.0;
                    time[city.first] = 0.0;
                } else {
                    cost[city.first] = DBL_MAX;
                    time[city.first] = DBL_MAX;
                }
            }

            pq.push({{0.0, 0.0}, source});

            while (!pq.empty()) {
                string currentCity = pq.top().second;
                double currentCost = pq.top().first.first;
                double currentTime = pq.top().first.second;
                pq.pop();

                if (currentCity == destination) {
                    p.cost = currentCost;
                    p.time = currentTime;
                    vector<string> v;

                    while (previous[currentCity] != "##") {
                        v.push_back(currentCity);
                        currentCity = previous[currentCity];
                    }

                    v.push_back(source);
                    reverse(v.begin(), v.end());
                    p.route = v;
                    restoreEdgeWeights(source, destination);
                    return p;
                }

                for (auto it : adjacencyList[currentCity]) {
                    string nextCity = it.first;
                    double nextCost = it.second.first;
                    double nextTime = it.second.second;

                    if (cost[nextCity] > currentCost + nextCost) {
                        cost[nextCity] = currentCost + nextCost;
                        time[nextCity] = currentTime + nextTime;
                        previous[nextCity] = currentCity;
                        pq.push({{cost[nextCity], time[nextCity]}, nextCity});
                    }
                }
            }

            // Restore the original edge weights
            restoreEdgeWeights(source, destination);
        }

        return p;
    }

private:
    void modifyEdgeWeights(const string& source, const string& destination) {
        for (auto& city : adjacencyList) {
            for (auto& connection : city.second) {
                if ((city.first == source && connection.first == destination) ||
                    (city.first == destination && connection.first == source)) {
                    // Keep the original edge weights for the source-destination edge
                    continue;
                }

                // Temporarily reduce the cost by half
                connection.second.first /= 2;

                // Temporarily add 20 to the time
                if(connection.first!=destination)
                connection.second.second += 40;
            }
        }
    }

    void restoreEdgeWeights(const string& source, const string& destination) {
        for (auto& city : adjacencyList) {
            for (auto& connection : city.second) {
                if ((city.first == source && connection.first == destination) ||
                    (city.first == destination && connection.first == source)) {
                    // Keep the original edge weights for the source-destination edge
                    continue;
                }

                // Restore the original cost
                connection.second.first *= 2;

                if(connection.first!=destination)
                connection.second.second -= 40;
            }
        }
    }

    double calculateTotalCost(const vector<string>& pathRoute) {
        double totalCost = 0.0;

        for (size_t i = 0; i < pathRoute.size() - 1; i++) {
            const auto& connections = adjacencyList[pathRoute[i]];
            for (const auto& connection : connections) {
                if (connection.first == pathRoute[i + 1]) {
                    totalCost += connection.second.first; // Consider the modified edge cost
                    break;
                }
            }
        }

        return totalCost;
    }
};

int main() {
    Graph graph;

    graph.addCity("JAIPUR");
    graph.addCity("DELHI");
    graph.addCity("MUMBAI");
    graph.addCity("KOLKATA");
    graph.addCity("CHENNAI");
    graph.addCity("HYDERABAD");
    graph.addCity("BENGALURU");
    graph.addCity("GUWAHATI");
    graph.addCity("CHANDIGARH");
    graph.addCity("AHEMDABAD");
    graph.addCity("LUCKNOW");
    graph.addCity("AMRITSAR");
    graph.addCity("BHOPAL");

    graph.addEdge("JAIPUR", "DELHI", 2439, 45);
    graph.addEdge("JAIPUR", "MUMBAI", 4129, 80);
    graph.addEdge("JAIPUR", "BENGALURU", 5489, 140);
    graph.addEdge("JAIPUR", "HYDERABAD", 4899, 100);
    graph.addEdge("JAIPUR", "KOLKATA", 6029, 160);
    graph.addEdge("JAIPUR", "CHENNAI", 6999, 180);
    graph.addEdge("JAIPUR", "BANGALORE", 5489, 140);
    graph.addEdge("JAIPUR", "CHANDIGARH", 3299, 60);
    graph.addEdge("JAIPUR", "AHEMDABAD", 2979, 55);
    graph.addEdge("JAIPUR", "LUCKNOW", 3699, 65);
    graph.addEdge("JAIPUR", "BHOPAL", 3899, 70);
    graph.addEdge("DELHI", "MUMBAI", 4399, 100);
    
    

    // Find the shortest route with respect to time
    cout<<"********************************************************************************************************\n";
    cout<<"###########################################CHOOSE FLIGHTS ACCORDING TO YOUR PREFERENCE###########################\n";
    string source, destination;
    cout<<"[FROM] ENTER CITY:";
    cin>> source;
    for(auto &x:source){
        if(x>='a'&&x<='z'){
            x=x-32;
        }
    }
    cout<<"[TO] ENTER CITY:";
    cin>>destination;
    for(auto &x:destination){
        if(x>='a'&&x<='z'){
            x=x-32;
        }
    }

    Path shortestRoute = graph.shortestRouteWithTime(source, destination);
    Path cheapestRoute =graph.cheapestRoute(source,destination);
    // Print the shortest route details
    if (shortestRoute.cost != -1) {
        cout << "The route having Shortest Travel Time: " << endl;
        for ( int i=0;i<shortestRoute.route.size()-1;i++) {
            cout <<shortestRoute.route[i]  << " -> ";
        }
        cout<<destination<<endl;
        cout << "Total Cost: Rs:" << shortestRoute.cost <<"+Taxes"<< endl;
        cout << "Total Time: " << (int)shortestRoute.time/60<<" Hours "<<(int)shortestRoute.time%60<<" Minutes "<< endl;
    } else {
        cout << "No route found from source to destination." << endl;
    }

    if (cheapestRoute.cost != -1) {
        cout << "The route having lowest Cost: " << endl;
        for ( int i=0;i<cheapestRoute.route.size()-1;i++) {
            cout <<cheapestRoute.route[i]  << " -> ";
        }
        cout<<destination<<endl;
        cout << "Total Cost: Rs:" << cheapestRoute.cost <<"+Taxes"<< endl;
        cout << "Total Time: " << (int)cheapestRoute.time/60<<" Hours "<<(int)cheapestRoute.time%60<<" Minutes "<< endl;
    } else {
        cout << "No route found from source to destination." << endl;
    }

    return 0;
}
