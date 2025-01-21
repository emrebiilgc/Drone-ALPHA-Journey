#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map() {

    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            distanceMatrix[i][j] = 0;
        }
        visited[i] = false;
    }

}


// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open distance data file: " << filename << "\n";
        return;
    }

    std::string line;
    int row = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;

        while (std::getline(ss, cell, ',')) {
            distanceMatrix[row][col] = std::stoi(cell);
            col++;
            if (col >= MAX_SIZE) break;
        }
        row++;
        if (row >= MAX_SIZE) break;
    }

    file.close();

}




// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    return distanceMatrix[provinceA][provinceB] <= maxDistance;
}


// Marks a province as visited
void Map::markAsVisited(int province) {
    if (province >= 0 && province < MAX_SIZE) {
        visited[province] = true;
    } else {
        std::cerr << "Province index out of range: " << province << "\n";
    }
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    if (province >= 0 && province < MAX_SIZE) {
        return visited[province];
    } else {
        std::cerr << "Province index out of range: " << province << "\n";
        return false;
    }
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    for (int i = 0; i < MAX_SIZE; i++) {
        visited[i] = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    int count = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        if (visited[i]) {
            count++;
        }
    }
    return count;
}


// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    if (provinceA >= 0 && provinceA < MAX_SIZE && provinceB >= 0 && provinceB < MAX_SIZE) {
        return distanceMatrix[provinceA][provinceB];
    } else {
        std::cerr << "Province indices out of range: " << provinceA << ", " << provinceB << "\n";
        return -1;
    }
}