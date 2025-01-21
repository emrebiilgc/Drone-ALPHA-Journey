# Drone-ALPHA-Journey
Finding the best route for a cargo drone
The Drone ALPHA Journey Through Türkiye program is a logistics simulation implemented in C++, designed to navigate a drone through Turkish provinces while maximizing coverage under constraints such as fuel limits and inaccessible areas. The program uses stack and queue data structures for efficient route planning and backtracking.

The stack enables backtracking when the drone encounters dead-ends, while the queue organizes potential next moves in a prioritized manner. The map of provinces, represented as a 2D matrix, provides distance data, ensuring the drone adheres to a maximum travel distance of 250 km per move. Priority provinces are given precedence in exploration, while weather-restricted areas are avoided. The program marks visited provinces to prevent revisits and loops.

The exploration process begins in Ankara and proceeds step-by-step, updating the route based on constraints and stopping when all reachable provinces are visited. At the end, the program outputs the total distance covered, the provinces visited, and the status of priority locations. The project emphasizes dynamic memory management, efficient use of stack and queue structures, and adherence to complex constraints, making it a robust simulation of real-world logistics challenges.
