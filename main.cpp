#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>
#include <map>
#include <queue>
#include <limits>
#include <sstream>
using namespace std;

struct Edge {
    string to;
    int val;
};

// 🎨 Manual lowercase converter
string toLowerManual(string s) {
    for (int i = 0; i < s.size(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] - 'A' + 'a';
    }
    return s;
}

// 🎨 Show all cities from file
void showAllCities() {
    ifstream file("city.txt");
    if (!file.is_open()) {
        cout << "\033[1;31m[Error]\033[0m city.txt not found!\n";
        return;
    }
    cout << "\033[1;36mAvailable Cities:\033[0m\n";
    string name;
    int count = 0;
    while (file >> name) {
        cout << "  -> " << name << "\n";
        count++;
    }
    if (count == 0)
        cout << "  (No cities added yet!)\n";
    cout << endl;
    file.close();
}

// 🗺 Load graph data
void loadGraph(map<string, vector<Edge>>& graph, string mode) {
    ifstream file("path.txt");
    if (!file.is_open()) {
        cout << "\033[1;31mError: path.txt not found!\033[0m\n";
        Sleep(1500);
        return;
    }
    string a, b;
    int t, c;
    while (file >> a >> b >> t >> c) {
        a = toLowerManual(a);
        b = toLowerManual(b);
        if (mode == "cost") {
            graph[a].push_back({b, c});
            graph[b].push_back({a, c});
        } else if (mode == "time") {
            graph[a].push_back({b, t});
            graph[b].push_back({a, t});
        } else {
            int overall_val = static_cast<int>(t * 0.6 + c * 0.4);
            graph[a].push_back({b, overall_val});
            graph[b].push_back({a, overall_val});
        }
    }
    file.close();
}

void printPath(map<string, string>& parent, string start, string end) {
    if (end == start) {
        cout << start;
        return;
    }
    printPath(parent, start, parent[end]);
    cout << " -> " << end;
}

// 🔍 Dijkstra’s shortest path
void pathfinder(map<string, vector<Edge>>& graph, string start, string end) {
    map<string, int> dist;
    map<string, string> parent;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
    dist[start] = 0;
    pq.push({0, start});
    parent[start] = "";
    bool pathFound = false;

    while (!pq.empty()) {
        int d = pq.top().first;
        string u = pq.top().second;
        pq.pop();

        if (d > dist[u] && dist.count(u)) continue;
        if (u == end) {
            pathFound = true;
            break;
        }
        if (graph.find(u) == graph.end()) continue;

        for (auto& edge : graph[u]) {
            string v = edge.to;
            int weight = edge.val;
            int new_dist = d + weight;
            if (dist.find(v) == dist.end() || new_dist < dist[v]) {
                dist[v] = new_dist;
                parent[v] = u;
                pq.push({new_dist, v});
            }
        }
    }

    if (pathFound) {
        cout << "\033[1;32mBest path from " << start << " to " << end 
             << " has a total value of " << dist[end] << ".\033[0m\n";
        cout << "\033[1;36mPath: \033[0m";
        printPath(parent, start, end);
        cout << "\n";
    } else {
        cout << "\033[1;31mNo path found from " << start << " to " << end << ".\033[0m\n";
    }
}

// 🚗 Cost-based path
void costpath() {
    system("cls");
    cout << "\033[1;34m=== BEST COST PATH FINDER ===\033[0m\n\n";
    showAllCities();

    string start, end;
    cout << "\033[1;33mEnter starting city:\033[0m ";
    cin >> start;
    cout << "\033[1;33mEnter destination city:\033[0m ";
    cin >> end;
    start = toLowerManual(start);
    end = toLowerManual(end);

    map<string, vector<Edge>> graph;
    loadGraph(graph, "cost");
    if (graph.empty()) {
        cout << "\033[1;31mNo paths found! Please add paths first.\033[0m\n";
        Sleep(1500);
        return;
    }

    cout << "\033[1;36mFinding best cost route...\033[0m\n";
    Sleep(1000);
    pathfinder(graph, start, end);
    system("pause");
}

// ⏱ Time-based path
void timepath() {
    system("cls");
    cout << "\033[1;34m=== BEST TIME PATH FINDER ===\033[0m\n\n";
    showAllCities();

    string start, end;
    cout << "\033[1;33mEnter starting city:\033[0m ";
    cin >> start;
    cout << "\033[1;33mEnter destination city:\033[0m ";
    cin >> end;
    start = toLowerManual(start);
    end = toLowerManual(end);

    map<string, vector<Edge>> graph;
    loadGraph(graph, "time");
    if (graph.empty()) {
        cout << "\033[1;31mNo paths found! Please add paths first.\033[0m\n";
        Sleep(1500);
        return;
    }

    cout << "\033[1;36mFinding fastest route...\033[0m\n";
    Sleep(1000);
    pathfinder(graph, start, end);
    system("pause");
}

// 🌟 Overall best path
void overallbest() {
    system("cls");
    cout << "\033[1;34m=== BEST OVERALL PATH FINDER ===\033[0m\n\n";
    showAllCities();

    string start, end;
    cout << "\033[1;33mEnter starting city:\033[0m ";
    cin >> start;
    cout << "\033[1;33mEnter destination city:\033[0m ";
    cin >> end;
    start = toLowerManual(start);
    end = toLowerManual(end);

    map<string, vector<Edge>> graph;
    loadGraph(graph, "overall");
    if (graph.empty()) {
        cout << "\033[1;31mNo paths found! Please add paths first.\033[0m\n";
        Sleep(1500);
        return;
    }

    cout << "\033[1;36mCalculating best overall route...\033[0m\n";
    Sleep(1000);
    pathfinder(graph, start, end);
    system("pause");
}

// 🚦 Menu for finding path
void Alltypeofpath() {
    int choice;
    while (true) {
        cout << "\033[1;35mSelect the way in which you want to find the route:\033[0m\n";
        cout << "1. Best cost path\n";
        cout << "2. Best timing path\n";
        cout << "3. Best overall path\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        string input;
        cin >> input;
        while (input.size() != 1 || input[0] < '0' || input[0] > '9') {
            cout << "\033[1;31mInvalid input. Please enter 1-4:\033[0m ";
            cin >> input;
        }
        choice = input[0] - '0';
        system("cls");
        switch (choice) {
            case 1: costpath(); break;
            case 2: timepath(); break;
            case 3: overallbest(); break;
            case 4: return;
            default: cout << "\033[1;31mInvalid Input!! Please choose 1-4.\033[0m\n"; Sleep(1500);
        }
        system("cls");
    }
}

// 🏙 Check city existence
bool cityExists(const string &city) {
    ifstream file("city.txt");
    if (!file.is_open()) return false;
    string name;
    while (file >> name)
        if (name == city) return true;
    return false;
}

// ➕ Add new city
void newcity() {
    cout << "\033[1;34mEnter the new city name:\033[0m ";
    string city;
    cin >> city;
    city = toLowerManual(city);

    ifstream infile("city.txt");
    bool exists = false;
    string line;
    if (infile.is_open()) {
        while (getline(infile, line)) {
            if (line == city) { exists = true; break; }
        }
        infile.close();
    }

    if (exists) {
        cout << "\033[1;33mCity already exists!\033[0m\n";
        Sleep(1500);
        return;
    }

    fstream file("city.txt", ios::out | ios::app);
    if (!file.is_open()) {
        cout << "\033[1;31mError opening city.txt!\033[0m\n";
        Sleep(1500);
        return;
    }

    file << city << endl;
    cout << "\033[1;32mCity added successfully!\033[0m\n";
    file.close();
    Sleep(1500);
}

// ➕ Add new path
void newpath() {
    showAllCities();
    string place1, place2;
    cout << "\033[1;33mEnter journey starting place:\033[0m ";
    cin >> place1;
    cout << "\033[1;33mEnter journey final place:\033[0m ";
    cin >> place2;
    place1 = toLowerManual(place1);
    place2 = toLowerManual(place2);

    if (!cityExists(place1) || !cityExists(place2)) {
        cout << "\033[1;31mOne or both cities not found in city list!\033[0m\n";
        Sleep(1500);
        return;
    }

    int time, cost;
    cout << "\033[1;34mEnter the time for the journey:\033[0m ";
    while (!(cin >> time) || time < 0) {
        cout << "Invalid time. Try again: ";
        cin.clear(); 
    }
    cout << "\033[1;34mEnter the cost for the journey:\033[0m ";
    while (!(cin >> cost) || cost < 0) {
        cout << "Invalid cost. Try again: ";
        cin.clear(); 
    }

    fstream file("path.txt", ios::out | ios::app);
    if (!file.is_open()) {
        cout << "\033[1;31mError opening path.txt!\033[0m\n";
        Sleep(1500);
        return;
    }

    file << place1 << " " << place2 << " " << time << " " << cost << endl;
    cout << "\033[1;32mNew path added successfully!\033[0m\n";
    file.close();
    Sleep(1500);
}

// ❌ Delete path
void deletepath() {
    showAllCities();
    string place1, place2;
    cout << "\033[1;33mEnter journey starting place:\033[0m ";
    cin >> place1;
    cout << "\033[1;33mEnter journey final place:\033[0m ";
    cin >> place2;
    place1 = toLowerManual(place1);
    place2 = toLowerManual(place2);

    ifstream infile("path.txt");
    if (!infile.is_open()) {
        cout << "\033[1;31mError opening path.txt!\033[0m\n";
        Sleep(1500);
        return;
    }

    vector<string> lines;
    string line;
    bool found = false;
    while (getline(infile, line)) {
        if (line.empty()) continue;
        string a, b;
        int t, c;
        stringstream ss(line);
        if (!(ss >> a >> b >> t >> c)) {
            lines.push_back(line);
            continue;
        }
        if ((a == place1 && b == place2) || (a == place2 && b == place1)) {
            found = true;
            continue;
        }
        lines.push_back(line);
    }
    infile.close();

    ofstream outfile("path.txt", ios::out | ios::trunc);
    for (auto& l : lines) outfile << l << endl;
    outfile.close();

    if (found)
        cout << "\033[1;32mPath deleted successfully!\033[0m\n";
    else
        cout << "\033[1;31mPath not found!\033[0m\n";
    Sleep(1500);
}

// 🧭 Change path menu
void changeinthepath() {
    int choice;
    while (true) {
        cout << "\033[1;35m--- CHANGE IN PATH MENU ---\033[0m\n";
        cout << "1. Add a new city\n";
        cout << "2. Add a new path between cities\n";
        cout << "3. Delete an existing path\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        string input;
        cin >> input;
        while (input.size() != 1 || input[0] < '0' || input[0] > '9') {
            cout << "Invalid input. Enter 1-4: ";
            cin >> input;
        }
        choice = input[0] - '0';
        system("cls");
        switch (choice) {
            case 1: newcity(); break;
            case 2: newpath(); break;
            case 3: deletepath(); break;
            case 4: return;
            default: cout << "\033[1;31mInvalid Input!\033[0m\n"; Sleep(1500);
        }
        system("cls");
    }
}

// 🖥 Header banner
void printHeader() {
    cout << "\033[1;36m----------------------------------------------\033[0m\n";
    cout << "\033[1;36m=                                            =\033[0m\n";
    cout << "\033[1;32m=         SMART ROUTE FINDER SYSTEM          =\033[0m\n";
    cout << "\033[1;36m=                                            =\033[0m\n";
    cout << "\033[1;36m----------------------------------------------\033[0m\n";
}

int main() {
    int choice;
    printHeader();
    Sleep(2000);
    system("cls");

    while (true) {
        cout << "\033[1;35m--- MAIN MENU ---\033[0m\n";
        cout << "1. Find a path\n";
        cout << "2. Modify paths/cities\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        string input;
        cin >> input;
        while (input.size() != 1 || input[0] < '0' || input[0] > '9') {
            cout << "\033[1;31mInvalid input. Please enter 1-3:\033[0m ";
            cin >> input;
        }
        choice = input[0] - '0';
        system("cls");
        switch (choice) {
            case 1: Alltypeofpath(); break;
            case 2: changeinthepath(); break;
            case 3:
                cout << "\033[1;32mThanks for using the app! Goodbye!\033[0m\n";
                Sleep(2000);
                return 0;
            default:
                cout << "\033[1;31mInvalid Input!! Please choose 1-3.\033[0m\n";
                Sleep(1500);
        }
        system("cls");
    }
}
