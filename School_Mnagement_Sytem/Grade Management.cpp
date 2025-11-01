#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>

using namespace std;

class GradeManager {
private:
    struct StudentGrade {
        string name;
        int id;
        map<string, double> grades;

        // Initialize all subjects with 0.0 grade
        void initializeSubjects() {
            vector<string> subjects = {
                "Maths", "English", "Kiswahili", "Chem", "Bio",
                "Phyc", "Computer", "Geo/Hist", "C.R.E", "Business"
            };

            for (const auto& subject : subjects) {
                grades[subject] = 0.0;
            }
        }

        // Set grade for a specific subject
        bool setGrade(const string& subject, double grade) {
            if (grades.find(subject) != grades.end() && grade >= 0 && grade <= 100) {
                grades[subject] = grade;
                return true;
            }
            return false;
        }

        // Get grade for a specific subject
        double getGrade(const string& subject) const {
            auto it = grades.find(subject);
            if (it != grades.end()) {
                return it->second;
            }
            return -1.0; // Indicates subject not found
        }

        // Calculate average grade
        double calculateAverage() const {
            if (grades.empty()) return 0.0;

            double total = 0.0;
            for (const auto& pair : grades) {
                total += pair.second;
            }
            return total / grades.size();
        }

        // Get grade letter
        string getGradeLetter(double grade) const {
            if (grade >= 80) return "A";
            else if (grade >= 75) return "A-";
            else if (grade >= 70) return "B+";
            else if (grade >= 65) return "B";
            else if (grade >= 60) return "B-";
            else if (grade >= 55) return "C+";
            else if (grade >= 50) return "C";
            else if (grade >= 45) return "C-";
            else if (grade >= 40) return "D+";
            else if (grade >= 35) return "D";
            else if (grade >= 30) return "D-";
            else return "E";
        }

        // Display individual student grade report
        void displayGradeReport() const {
            cout << "\n=== GRADE REPORT ===" << endl;
            cout << "Name: " << name << endl;
            cout << "ID: " << id << endl;
            cout << "----------------------------------------" << endl;
            cout << left << setw(15) << "SUBJECT" << setw(10) << "GRADE" << "LETTER" << endl;
            cout << "----------------------------------------" << endl;

            for (const auto& pair : grades) {
                cout << left << setw(15) << pair.first
                     << setw(10) << fixed << setprecision(1) << pair.second
                     << getGradeLetter(pair.second) << endl;
            }

            double average = calculateAverage();
            cout << "----------------------------------------" << endl;
            cout << left << setw(15) << "AVERAGE" << setw(10) << fixed << setprecision(1) << average
                 << getGradeLetter(average) << endl;
        }
    };

    vector<StudentGrade> students;

public:
    // Add a new student for grade management
    void addStudent(const string& name, int id) {
        // Check if ID already exists
        for (const auto& student : students) {
            if (student.id == id) {
                cout << "Error: Student ID already exists!" << endl;
                return;
            }
        }

        StudentGrade newStudent;
        newStudent.name = name;
        newStudent.id = id;
        newStudent.initializeSubjects();

        students.push_back(newStudent);
        cout << "Student added successfully for grade management!" << endl;
    }

    // Set grades for all subjects for a student
    void setAllGrades(int studentId) {
        StudentGrade* student = findStudent(studentId);
        if (!student) {
            cout << "Student not found!" << endl;
            return;
        }

        cout << "\nEntering grades for: " << student->name << endl;
        cout << "Enter grades (0-100) for all subjects:" << endl;

        vector<string> subjects = {
            "Maths", "English", "Kiswahili", "Chem", "Bio",
            "Phyc", "Computer", "Geo/Hist", "C.R.E", "Business"
        };

        for (const auto& subject : subjects) {
            double grade;
            cout << subject << ": ";
            cin >> grade;

            if (student->setGrade(subject, grade)) {
                cout << "  ✓ Grade set: " << grade << endl;
            } else {
                cout << "  ✗ Invalid grade! Using previous value." << endl;
            }
        }
        cout << "All grades updated successfully!" << endl;
    }

    // Update grade for a specific subject
    void updateSubjectGrade(int studentId, const string& subject, double grade) {
        StudentGrade* student = findStudent(studentId);
        if (!student) {
            cout << "Student not found!" << endl;
            return;
        }

        if (student->setGrade(subject, grade)) {
            cout << "Grade for " << subject << " updated to: " << grade << endl;
        } else {
            cout << "Failed to update grade! Invalid subject or grade value." << endl;
        }
    }

    // Display grade report for a specific student
    void displayStudentGrades(int studentId) {
        StudentGrade* student = findStudent(studentId);
        if (student) {
            student->displayGradeReport();
        } else {
            cout << "Student not found!" << endl;
        }
    }

    // Display grades for all students
    void displayAllGrades() {
        if (students.empty()) {
            cout << "\nNo students in the system!" << endl;
            return;
        }

        cout << "\n=== ALL STUDENTS GRADES ===" << endl;
        for (const auto& student : students) {
            cout << "\nStudent: " << student.name << " (ID: " << student.id << ")" << endl;
            cout << "Subjects: ";
            for (const auto& grade : student.grades) {
                cout << grade.first << ": " << grade.second << " ";
            }
            cout << "\nAverage: " << fixed << setprecision(1) << student.calculateAverage() << endl;
        }
    }

    // Calculate and display class average for each subject
    void displaySubjectAverages() {
        if (students.empty()) {
            cout << "\nNo students in the system!" << endl;
            return;
        }

        cout << "\n=== SUBJECT AVERAGES ===" << endl;
        vector<string> subjects = {
            "Maths", "English", "Kiswahili", "Chem", "Bio",
            "Phyc", "Computer", "Geo/Hist", "C.R.E", "Business"
        };

        cout << left << setw(15) << "SUBJECT" << setw(10) << "AVERAGE"
             << setw(10) << "HIGHEST" << setw(10) << "LOWEST" << endl;
        cout << "----------------------------------------------------" << endl;

        for (const auto& subject : subjects) {
            double total = 0.0;
            double highest = 0.0;
            double lowest = 100.0;
            int count = 0;

            for (const auto& student : students) {
                double grade = student.getGrade(subject);
                if (grade >= 0) {
                    total += grade;
                    if (grade > highest) highest = grade;
                    if (grade < lowest) lowest = grade;
                    count++;
                }
            }

            if (count > 0) {
                double average = total / count;
                cout << left << setw(15) << subject
                     << setw(10) << fixed << setprecision(1) << average
                     << setw(10) << highest
                     << setw(10) << lowest << endl;
            }
        }
    }

    // Find student by ID
    StudentGrade* findStudent(int id) {
        for (auto& student : students) {
            if (student.id == id) {
                return &student;
            }
        }
        return nullptr;
    }

    // Get student count
    int getStudentCount() const {
        return students.size();
    }
};

// Grade Management Menu
void displayGradeMenu() {
    cout << "\n=== GRADE MANAGEMENT SYSTEM ===" << endl;
    cout << "1. Add Student" << endl;
    cout << "2. Set All Grades for Student" << endl;
    cout << "3. Update Single Subject Grade" << endl;
    cout << "4. View Student Grade Report" << endl;
    cout << "5. View All Students' Grades" << endl;
    cout << "6. View Subject Averages" << endl;
    cout << "7. Show Student Count" << endl;
    cout << "8. Exit" << endl;
    cout << "Enter your choice (1-8): ";
}

// Demo function to show usage
void demonstrateGradeManagement() {
    GradeManager gm;

    // Add some demo students
    gm.addStudent("John Doe", 1001);
    gm.addStudent("Jane Smith", 1002);
    gm.addStudent("Mike Johnson", 1003);

    // You can now use the menu to interact with the grade manager
}

int main() {
    GradeManager gradeManager;
    int choice;

    do {
        displayGradeMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                string name;
                int id;
                cout << "Enter student name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter student ID: ";
                cin >> id;
                gradeManager.addStudent(name, id);
                break;
            }
            case 2: {
                int id;
                cout << "Enter student ID: ";
                cin >> id;
                gradeManager.setAllGrades(id);
                break;
            }
            case 3: {
                int id;
                string subject;
                double grade;
                cout << "Enter student ID: ";
                cin >> id;
                cout << "Enter subject: ";
                cin.ignore();
                getline(cin, subject);
                cout << "Enter new grade: ";
                cin >> grade;
                gradeManager.updateSubjectGrade(id, subject, grade);
                break;
            }
            case 4: {
                int id;
                cout << "Enter student ID: ";
                cin >> id;
                gradeManager.displayStudentGrades(id);
                break;
            }
            case 5:
                gradeManager.displayAllGrades();
                break;
            case 6:
                gradeManager.displaySubjectAverages();
                break;
            case 7:
                cout << "Total students: " << gradeManager.getStudentCount() << endl;
                break;
            case 8:
                cout << "Exiting Grade Management System. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 8);

    return 0;
}
