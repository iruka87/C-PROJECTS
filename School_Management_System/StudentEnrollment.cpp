#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;

// Structure to represent a Course
struct Course {
    string courseID;
    string courseName;

    // For saving to file: convert course to string format
    string toString() const {
        return courseID + "|" + courseName;
    }

    // For loading from file: parse string into course
    static Course fromString(const string& str) {
        Course c;
        size_t pos = str.find('|');
        if (pos != string::npos) {
            c.courseID = str.substr(0, pos);
            c.courseName = str.substr(pos + 1);
        }
        return c;
    }
};

// Structure to represent a Student
struct Student {
    string studentID;
    string name;
    vector<string> enrolledCourses;  // Stores only course IDs

    // Convert student to string for file saving
    string toString() const {
        string result = studentID + "|" + name;
        for (const auto& courseID : enrolledCourses) {
            result += "|" + courseID;
        }
        return result;
    }

    // Parse string and load into student object
    static Student fromString(const string& str) {
        Student s;
        stringstream ss(str);
        string token;
        vector<string> tokens;

        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 2) {
            s.studentID = tokens[0];
            s.name = tokens[1];
            for (size_t i = 2; i < tokens.size(); ++i) {
                s.enrolledCourses.push_back(tokens[i]);
            }
        }
        return s;
    }
};

// Global data: list of all students and available courses
vector<Student> students;
vector<Course> courses;
const string DATA_FILE = "enrollment_data.txt";  // File for persistence

// Function prototypes
void loadData();
void saveData();
void displayMenu();
void addStudent();
void addCourse();
void enrollStudent();
void displayStudents();
void displayCourses();

// Main function
int main() {
    loadData();  // Load existing data from file at startup

    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();  // Clear input buffer

        switch (choice) {
            case 1: addStudent(); break;
            case 2: addCourse(); break;
            case 3: enrollStudent(); break;
            case 4: displayStudents(); break;
            case 5: displayCourses(); break;
            case 6:
                saveData();
                cout << "Data saved. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 6);

    return 0;
}

// Load students and courses from file
void loadData() {
    ifstream file(DATA_FILE);
    if (!file.is_open()) {
        cout << "No previous data found. Starting fresh.\n";
        return;
    }

    string line;
    bool readingCourses = true;  // First section: courses, then students

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line == "---") {
            readingCourses = false;  // Switch from courses to students
            continue;
        }

        if (readingCourses) {
            Course c = Course::fromString(line);
            courses.push_back(c);
        } else {
            Student s = Student::fromString(line);
            students.push_back(s);
        }
    }

    file.close();
    cout << "Data loaded successfully from " << DATA_FILE << "\n";
}

// Save all students and courses to file
void saveData() {
    ofstream file(DATA_FILE);
    if (!file.is_open()) {
        cout << "Error: Could not save data!\n";
        return;
    }

    // Save courses first
    for (const auto& c : courses) {
        file << c.toString() << "\n";
    }
    file << "---\n";  // Separator

    // Save students
    for (const auto& s : students) {
        file << s.toString() << "\n";
    }

    file.close();
    cout << "Data saved to " << DATA_FILE << "\n";
}

// Display main menu
void displayMenu() {
    cout << "\n=== Student Enrollment System ===\n";
    cout << "1. Add Student\n";
    cout << "2. Add Course\n";
    cout << "3. Enroll Student in Course\n";
    cout << "4. Display All Students\n";
    cout << "5. Display All Courses\n";
    cout << "6. Save and Exit\n";
    cout << "================================\n";
}

// Add a new student
void addStudent() {
    Student s;
    cout << "Enter Student ID: ";
    getline(cin, s.studentID);
    cout << "Enter Student Name: ";
    getline(cin, s.name);

    // Check for duplicate student ID
    for (const auto& stu : students) {
        if (stu.studentID == s.studentID) {
            cout << "Student with this ID already exists!\n";
            return;
        }
    }

    students.push_back(s);
    cout << "Student added successfully.\n";
}

// Add a new course
void addCourse() {
    Course c;
    cout << "Enter Course ID: ";
    getline(cin, c.courseID);
    cout << "Enter Course Name: ";
    getline(cin, c.courseName);

    // Check for duplicate course ID
    for (const auto& course : courses) {
        if (course.courseID == c.courseID) {
            cout << "Course with this ID already exists!\n";
            return;
        }
    }

    courses.push_back(c);
    cout << "Course added successfully.\n";
}

// Enroll a student in a course (with duplicate check)
void enrollStudent() {
    string studentID, courseID;
    cout << "Enter Student ID: ";
    getline(cin, studentID);
    cout << "Enter Course ID: ";
    getline(cin, courseID);

    // Find student pointer
    Student* studentPtr = nullptr;
    for (auto& s : students) {
        if (s.studentID == studentID) {
            studentPtr = &s;
            break;
        }
    }

    if (!studentPtr) {
        cout << "Student not found!\n";
        return;
    }

    // Check if course exists
    bool courseExists = false;
    for (const auto& c : courses) {
        if (c.courseID == courseID) {
            courseExists = true;
            break;
        }
    }
    if (!courseExists) {
        cout << "Course not found!\n";
        return;
    }

    // === YOUR ORIGINAL LOGIC ENHANCED WITH COMMENTS ===
    // Check if student is already enrolled in the course
    if (find(studentPtr->enrolledCourses.begin(), studentPtr->enrolledCourses.end(), courseID)
        != studentPtr->enrolledCourses.end()) {
        cout << "Student already enrolled in this course.\n";
        return;
    }
    // ==================================================

    // If not enrolled, add the course
    studentPtr->enrolledCourses.push_back(courseID);
    cout << "Student enrolled successfully in course " << courseID << ".\n";
}

// Display all students and their enrolled courses
void displayStudents() {
    if (students.empty()) {
        cout << "No students registered.\n";
        return;
    }

    cout << "\n--- List of Students ---\n";
    for (const auto& s : students) {
        cout << "ID: " << s.studentID << ", Name: " << s.name << "\n";
        cout << "  Enrolled Courses: ";
        if (s.enrolledCourses.empty()) {
            cout << "None";
        } else {
            for (size_t i = 0; i < s.enrolledCourses.size(); ++i) {
                // Find course name by ID
                string courseName = "Unknown";
                for (const auto& c : courses) {
                    if (c.courseID == s.enrolledCourses[i]) {
                        courseName = c.courseName;
                        break;
                    }
                }
                cout << s.enrolledCourses[i] << " (" << courseName << ")";
                if (i < s.enrolledCourses.size() - 1) cout << ", ";
            }
        }
        cout << "\n";
    }
}

// Display all available courses
void displayCourses() {
    if (courses.empty()) {
        cout << "No courses available.\n";
        return;
    }

    cout << "\n--- List of Courses ---\n";
    for (const auto& c : courses) {
        cout << "ID: " << c.courseID << ", Name: " << c.courseName << "\n";
    }
}
