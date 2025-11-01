#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class SchoolAttendance {
private:
    const int TOTAL_STUDENTS = 50;
    const int TOTAL_SUBJECTS = 11;

    vector<string> subjects = {
        "Mathematics", "English", "Kiswahili", "Chemistry", "Biology",
        "Physics", "Computer", "Geology", "History", "C.R.E","Business"
    };

    // Track attendance: student_id -> (subject_index -> count)
    vector<vector<int>> attendance;

    // Track which subjects each student takes
    vector<vector<int>> studentSubjects;

public:
    SchoolAttendance() {
        // Initialize attendance records with zeros
        attendance.resize(TOTAL_STUDENTS, vector<int>(TOTAL_SUBJECTS, 0));
        studentSubjects.resize(TOTAL_STUDENTS);
        setupStudentSubjects();
    }

    void setupStudentSubjects() {
        cout << "Setting up student subjects...\n";

        // All students take mandatory subjects (0-5)
        for (int student = 0; student < TOTAL_STUDENTS; student++) {
            for (int subject = 0; subject <= 5; subject++) {
                studentSubjects[student].push_back(subject);
            }
        }

        // Assign optional subjects based on capacities
        int physicsCount = 0, geologyCount = 0, historyCount = 0, creCount = 0;

        for (int student = 0; student < TOTAL_STUDENTS; student++) {
            // Physics + 1 humanity (30 students)
            if (student < 30 && physicsCount < 35) {
                studentSubjects[student].push_back(5); // Physics
                physicsCount++;

                // Add one humanity
                if (creCount < 30) {
                    studentSubjects[student].push_back(9); // C.R.E
                    creCount++;
                } else if (historyCount < 40) {
                    studentSubjects[student].push_back(8); // History
                    historyCount++;
                } else if (geologyCount < 20) {
                    studentSubjects[student].push_back(7); // Geology
                    geologyCount++;
                }
            }
            // Two humanities only (20 students)
            else if (student >= 30 && student < 50) {
                int humanities = 0;
                if (creCount < 30) {
                    studentSubjects[student].push_back(9); // C.R.E
                    creCount++;
                    humanities++;
                }
                if (humanities < 2 && historyCount < 40) {
                    studentSubjects[student].push_back(8); // History
                    historyCount++;
                    humanities++;
                }
                if (humanities < 2 && geologyCount < 20) {
                    studentSubjects[student].push_back(7); // Geology
                    geologyCount++;
                }
            }
        }

        cout << "Subject setup completed!\n";
        cout << "Physics: " << physicsCount << "/35\n";
        cout << "Geology: " << geologyCount << "/20\n";
        cout << "History: " << historyCount << "/40\n";
        cout << "C.R.E: " << creCount << "/30\n";
    }

    void markAttendance(int studentId, int subjectId) {
        if (studentId < 1 || studentId > TOTAL_STUDENTS) {
            cout << "Error: Student ID must be between 1 and " << TOTAL_STUDENTS << "\n";
            return;
        }

        if (subjectId < 0 || subjectId >= TOTAL_SUBJECTS) {
            cout << "Error: Subject ID must be between 0 and " << TOTAL_SUBJECTS-1 << "\n";
            return;
        }

        // Check if student takes this subject
        bool takesSubject = false;
        for (int subj : studentSubjects[studentId-1]) {
            if (subj == subjectId) {
                takesSubject = true;
                break;
            }
        }

        if (!takesSubject) {
            cout << "Error: Student " << studentId << " doesn't take " << subjects[subjectId] << "!\n";
            return;
        }

        attendance[studentId-1][subjectId]++;
        cout << "Marked attendance for Student " << studentId
                  << " in " << subjects[subjectId] << "\n";
    }

    void showStudentAttendance(int studentId) {
        if (studentId < 1 || studentId > TOTAL_STUDENTS) {
            cout << "Error: Invalid student ID!\n";
            return;
        }

        cout << "\n=== Attendance for Student " << studentId << " ===\n";
        int total = 0;

        for (int subjectId : studentSubjects[studentId-1]) {
            int count = attendance[studentId-1][subjectId];
            cout << subjects[subjectId] << ": " << count << " sessions\n";
            total += count;
        }
        cout << "Total: " << total << " sessions\n";
    }

    void showSubjectAttendance(int subjectId) {
        if (subjectId < 0 || subjectId >= TOTAL_SUBJECTS) {
            cout << "Error: Invalid subject ID!\n";
            return;
        }

        cout << "\n=== Attendance for " << subjects[subjectId] << " ===\n";
        int total = 0, studentCount = 0;

        for (int student = 0; student < TOTAL_STUDENTS; student++) {
            for (int subj : studentSubjects[student]) {
                if (subj == subjectId) {
                    int count = attendance[student][subjectId];
                    cout << "Student " << student+1 << ": " << count << " sessions\n";
                    total += count;
                    studentCount++;
                    break;
                }
            }
        }

        cout << "Total students: " << studentCount << "\n";
        cout << "Total sessions: " << total << "\n";
    }

    void saveStudentReport(int studentId) {
        if (studentId < 1 || studentId > TOTAL_STUDENTS) {
            cout << "Error: Invalid student ID!\n";
            return;
        }

        string filename = "student_" + to_string(studentId) + "_report.txt";
        ofstream file(filename);

        file << "STUDENT ATTENDANCE REPORT\n";
        file << "Student ID: " << studentId << "\n\n";
        file << "SUBJECT ATTENDANCE:\n";

        int total = 0;
        for (int subjectId : studentSubjects[studentId-1]) {
            int count = attendance[studentId-1][subjectId];
            file << subjects[subjectId] << ": " << count << " sessions\n";
            total += count;
        }

        file << "\nTOTAL SESSIONS: " << total << "\n";
        file.close();
        cout << "Report saved to: " << filename << "\n";
    }

    void saveSubjectReport(int subjectId) {
        if (subjectId < 0 || subjectId >= TOTAL_SUBJECTS) {
            cout << "Error: Invalid subject ID!\n";
            return;
        }

        string filename = subjects[subjectId] + "_report.txt";
        ofstream file(filename);

        file << "SUBJECT ATTENDANCE REPORT\n";
        file << "Subject: " << subjects[subjectId] << "\n\n";
        file << "STUDENT ATTENDANCE:\n";

        int total = 0, studentCount = 0;
        for (int student = 0; student < TOTAL_STUDENTS; student++) {
            for (int subj : studentSubjects[student]) {
                if (subj == subjectId) {
                    int count = attendance[student][subjectId];
                    file << "Student " << student+1 << ": " << count << " sessions\n";
                    total += count;
                    studentCount++;
                    break;
                }
            }
        }

        file << "\nSUMMARY:\n";
        file << "Total students: " << studentCount << "\n";
        file << "Total sessions: " << total << "\n";
        file.close();
        cout << "Report saved to: " << filename << "\n";
    }

    void saveAllReports() {
        ofstream file("school_report.txt");

        file << "SCHOOL WIDE ATTENDANCE REPORT\n\n";

        // Subject summary
        file << "SUBJECT SUMMARY:\n";
        for (int subjectId = 0; subjectId < TOTAL_SUBJECTS; subjectId++) {
            int total = 0, studentCount = 0;
            for (int student = 0; student < TOTAL_STUDENTS; student++) {
                for (int subj : studentSubjects[student]) {
                    if (subj == subjectId) {
                        total += attendance[student][subjectId];
                        studentCount++;
                        break;
                    }
                }
            }
            file << subjects[subjectId] << ": " << studentCount << " students, "
                 << total << " sessions\n";
        }

        file << "\nSTUDENT SUMMARY:\n";
        for (int student = 0; student < TOTAL_STUDENTS; student++) {
            int total = 0;
            for (int subjectId : studentSubjects[student]) {
                total += attendance[student][subjectId];
            }
            file << "Student " << student+1 << ": " << total << " total sessions\n";
        }

        file.close();
        cout << "School report saved to: school_report.txt\n";
    }

    void showSubjects() {
        cout << "\n=== SUBJECTS ===\n";
        for (int i = 0; i < TOTAL_SUBJECTS; i++) {
            cout << i << ". " << subjects[i] << "\n";
        }
    }

    void showStudentSubjects(int studentId) {
        if (studentId < 1 || studentId > TOTAL_STUDENTS) {
            cout << "Error: Invalid student ID!\n";
            return;
        }

        cout << "\nStudent " << studentId << " takes: ";
        for (int subjectId : studentSubjects[studentId-1]) {
            cout << subjects[subjectId] << " ";
        }
        cout << "\n";
    }
};

int main() {
    SchoolAttendance school;
    int choice;

    cout << "=== SCHOOL ATTENDANCE SYSTEM ===\n";
    cout << "50 Students | 10 Subjects\n\n";

    do {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. Mark Attendance\n";
        cout << "2. View Student Attendance\n";
        cout << "3. View Subject Attendance\n";
        cout << "4. Save Student Report\n";
        cout << "5. Save Subject Report\n";
        cout << "6. Save All Reports\n";
        cout << "7. Show Subjects List\n";
        cout << "8. Show Student's Subjects\n";
        cout << "9. Exit\n";
        cout << "Choose option: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int studentId, subjectId;
                cout << "Enter student ID (1-50): ";
                cin >> studentId;
                cout << "Enter subject ID (0-10): ";
                cin >> subjectId;
                school.markAttendance(studentId, subjectId);
                break;
            }
            case 2: {
                int studentId;
                cout << "Enter student ID (1-50): ";
                cin >> studentId;
                school.showStudentAttendance(studentId);
                break;
            }
            case 3: {
                int subjectId;
                cout << "Enter subject ID (0-10): ";
                cin >> subjectId;
                school.showSubjectAttendance(subjectId);
                break;
            }
            case 4: {
                int studentId;
                cout << "Enter student ID (1-50): ";
                cin >> studentId;
                school.saveStudentReport(studentId);
                break;
            }
            case 5: {
                int subjectId;
                cout << "Enter subject ID (0-10): ";
                cin >> subjectId;
                school.saveSubjectReport(subjectId);
                break;
            }
            case 6:
                school.saveAllReports();
                break;
            case 7:
                school.showSubjects();
                break;
            case 8: {
                int studentId;
                cout << "Enter student ID (1-50): ";
                cin >> studentId;
                school.showStudentSubjects(studentId);
                break;
            }
            case 9:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 9);

    return 0;
}
