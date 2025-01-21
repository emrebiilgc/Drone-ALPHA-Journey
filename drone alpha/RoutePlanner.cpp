#include "RoutePlanner.h"
#include <iostream>
#include <fstream>
#include <regex>

// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {

    // Load the distance matrix from the distance_data file
    map.loadDistanceData(distance_data);

    // Mark all provinces as unvisited
    map.resetVisited();

    // Load priority provinces from the priority_data file
    loadPriorityProvinces(priority_data);

    // Load weather-restricted provinces from the restricted_data file
    loadWeatherRestrictedProvinces(restricted_data);
}


// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open priority provinces file: " << filename << "\n";
        return;
    }

    std::string line;
    std::regex regexPattern(R"((.*)\((\d+)\))");
    std::smatch match;

    while (std::getline(file, line) && numPriorityProvinces < MAX_PRIORITY_PROVINCES) {
        if (std::regex_search(line, match, regexPattern)) {
            try {
                int provinceIndex = std::stoi(match[2].str());
                priorityProvinces[numPriorityProvinces++] = provinceIndex;
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid province index in line: " << line << "\n";
            } catch (const std::out_of_range& e) {
                std::cerr << "Error: Province index out of range in line: " << line << "\n";
            }
        } else {
            std::cerr << "Error: Line does not match expected format: " << line << "\n";
        }
    }

    file.close();
}



// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open restricted provinces file: " << filename << "\n";
        return;
    }

    // It is almost same fucntion as loadpriority only difference between them marking cities as weatherresctrt or priorty


    std::string line;
    std::regex regexPattern(R"((.*)\((\d+)\))");
    std::smatch match;

    while (std::getline(file, line) && numWeatherRestrictedProvinces < MAX_WEATHER_RESTRICTED_PROVINCES) {
        if (std::regex_search(line, match, regexPattern)) {
            try {
                int provinceIndex = std::stoi(match[2].str());
                weatherRestrictedProvinces[numWeatherRestrictedProvinces++] = provinceIndex;
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid province index in line: " << line << "\n";
            } catch (const std::out_of_range& e) {
                std::cerr << "Error: Province index out of range in line: " << line << "\n";
            }
        } else {
            std::cerr << "Error: Line does not match expected format: " << line << "\n";
        }
    }

    file.close();
}



// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    for (int i = 0; i < numPriorityProvinces; ++i) {
        if (priorityProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    for (int i = 0; i < numWeatherRestrictedProvinces; ++i) {
        if (weatherRestrictedProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {

    stack.push(startingCity);
    map.markAsVisited(startingCity);
    route.push_back(startingCity);


    while (!stack.isEmpty()) {

        int currentProvince = stack.peek();
        exploreFromProvince(currentProvince);
    }

    totalDistanceCovered = 0;
    for (size_t i = 1; i < route.size(); ++i) {
        int from = route[i - 1];
        int to = route[i];
        totalDistanceCovered += map.getDistance(from, to);
    }
    displayResults();
}

// Adding the cities. passing the ones without range and visited.
// If some city is priority we add it as enqueue priorty citiy
void RoutePlanner::enqueueNeighbors(int province) {
    for (int i = 0; i < MAX_SIZE; ++i) {

        if (!map.isWithinRange(province, i, maxDistance)) {
            continue;
        }

        if (map.isVisited(i)) {
            continue;
        }
        
        // if (queue.contains(province)) {continue;} i tried to implement this but it cost -4 points to my score

        if (isPriorityProvince(i)) {
            queue.enqueuePriority(i);
            continue;
        }

        queue.enqueue(i);
    }
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    enqueueNeighbors(province);


    //While there are cities we can go
    //Checking if we can. if we can we go and add check the next city's neighbors to queue
    while (!queue.isEmpty()) {
        int neighbor = queue.dequeue();

        if (!map.isWithinRange(province, neighbor, maxDistance)) {
            continue;
        }

        if (map.isVisited(neighbor)) {
            continue;
        }

        if (isWeatherRestricted(neighbor)) {
            std::cout << "Province " << cities[neighbor] <<" is weather-restricted. Skipping." << "\n";
            continue;
        }

        // if (queue.contains(neighbor)) {continue;} i tried to implement this but it cost -4 points to my score

        stack.push(neighbor);
        map.markAsVisited(neighbor);
        route.push_back(neighbor);

        //we go to do neighbors until we left nowhere to go
        exploreFromProvince(neighbor);
        return;
    }

    //if nowhere to go backtracking starts
    if (!stack.isEmpty()) {
        backtrack();
    }
}





void RoutePlanner::backtrack() {

        stack.pop();

}


bool RoutePlanner::isExplorationComplete() const {
    return stack.isEmpty() && queue.isEmpty();
}

void RoutePlanner::displayResults() const {
    std::cout << "----------------------------\n";
    std::cout << "Journey Completed!\n";
    std::cout << "----------------------------\n";
    std::cout << "Total Number of Provinces Visited: " << route.size() << "\n";


    std::cout << "Total Distance Covered: " << totalDistanceCovered << " km\n";

    std::cout << "Route Taken:\n";
    for (size_t i = 0; i < route.size(); ++i) {
        std::cout << cities[route[i]];
        if (i < route.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << "\n";

    std::cout << "Priority Provinces Status:\n";

    int priorityVisited = 0;
    for (int i = 0; i < numPriorityProvinces; ++i) {
        int province = priorityProvinces[i];
        std::cout << "- " << cities[province] << " (";
        if (map.isVisited(province)) {
            std::cout << "Visited";
            priorityVisited++;
        } else {
            std::cout << "Not Visited";
        }
        std::cout << ")\n";
    }

    std::cout << "Total Priority Provinces Visited: " << priorityVisited << " out of " << numPriorityProvinces << "\n";

    if (priorityVisited == numPriorityProvinces) {
        std::cout << "Success: All priority provinces were visited.\n";
    } else {
        std::cout << "Warning: Not all priority provinces were visited.\n";
    }
}



