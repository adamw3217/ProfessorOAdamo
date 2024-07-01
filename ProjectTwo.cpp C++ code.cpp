#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

class Course {
public:
    std::string title;
    std::vector<std::string> prerequisites;

    // Default constructor
    Course() {}

    // Parameterized constructor
    Course(std::string title, std::vector<std::string> prereqs)
        : title(title), prerequisites(prereqs) {}
};

// Function to prompt for file name and open the file
std::ifstream openCourseFile() {
    std::string filename;
    std::cout << "Enter the file name: ";
    std::cin >> filename;
    std::ifstream file(filename);
    return file;
}

// Load courses from file into the map
void loadCourses(std::map<std::string, Course>& courses) {
    std::ifstream file = openCourseFile();
    if (!file.is_open()) {
        std::cout << "Failed to open file\n";
        return;
    }
    std::string line;
    getline(file, line);  // Skip header line
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string courseNumber, title, token;
        std::vector<std::string> prerequisites;

        getline(ss, courseNumber, ',');
        getline(ss, title, ',');

        while (getline(ss, token, ',')) {
            prerequisites.push_back(token);
        }

        courses.emplace(courseNumber, Course(title, prerequisites));
    }
    file.close();
    std::cout << "Data loaded successfully.\n";
}

// Print all courses
void printCourses(const std::map<std::string, Course>& courses) {
    for (const auto& pair : courses) {
        std::cout << pair.first << " - " << pair.second.title << "\n";
    }
}

// Print course details
void printCourseDetails(const std::map<std::string, Course>& courses) {
    std::string courseNumber;
    std::cout << "Enter course number: ";
    std::cin >> courseNumber;
    auto it = courses.find(courseNumber);
    if (it != courses.end()) {
        std::cout << "Course: " << it->second.title << "\n";
        std::cout << "Prerequisites: ";
        for (const std::string& prereq : it->second.prerequisites) {
            std::cout << prereq << " ";
        }
        std::cout << "\n";
    }
    else {
        std::cout << "Course not found.\n";
    }
}

// Display the menu
void displayMenu() {
    std::cout << "1. Load Data\n";
    std::cout << "2. Print Courses\n";
    std::cout << "3. Course Details\n";
    std::cout << "9. Exit\n";
    std::cout << "Enter option: ";
}

int main() {
    std::map<std::string, Course> courses;
    bool running = true;
    while (running) {
        displayMenu();
        int choice;
        std::cin >> choice;
        switch (choice) {
        case 1:
            loadCourses(courses);
            break;
        case 2:
            printCourses(courses);
            break;
        case 3:
            printCourseDetails(courses);
            break;
        case 9:
            running = false;
            break;
        default:
            std::cout << "Invalid option. Please try again.\n";
        }
    }
    return 0;
}

