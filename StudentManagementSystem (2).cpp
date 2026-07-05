/*
    STUDENT MANAGEMENT SYSTEM
    --------------------------
    Language   : C++
    Purpose    : Console-based application to manage student records
                 using file handling and menu-driven operations.

    Key Features:
        1. Add student information
        2. Update student information
        3. Delete student information
        4. Display student information (single / all)
        5. Persistent storage using file handling (students.txt)

    Expected Outcome:
        A functional application that allows users to add, update,
        delete, and display student information with persistent data
        storage for reliable record management.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

// ---------------------------------------------------------
// Student class : represents a single student record
// ---------------------------------------------------------
class Student {
public:
    int roll;
    string name;
    int age;
    string department;
    float marks;

    Student() : roll(0), age(0), marks(0.0f) {}

    Student(int r, string n, int a, string dept, float m)
        : roll(r), name(n), age(a), department(dept), marks(m) {}

    // Convert student record to a single line for file storage
    string toFileString() const {
        ostringstream oss;
        oss << roll << "," << name << "," << age << ","
            << department << "," << marks;
        return oss.str();
    }

    // Parse a line from the file back into a Student object
    static Student fromFileString(const string &line) {
        Student s;
        stringstream ss(line);
        string token;

        getline(ss, token, ',');
        s.roll = stoi(token);

        getline(ss, token, ',');
        s.name = token;

        getline(ss, token, ',');
        s.age = stoi(token);

        getline(ss, token, ',');
        s.department = token;

        getline(ss, token, ',');
        s.marks = stof(token);

        return s;
    }

    void display() const {
        cout << left
             << setw(8)  << roll
             << setw(20) << name
             << setw(6)  << age
             << setw(15) << department
             << setw(8)  << marks << endl;
    }
};

// ---------------------------------------------------------
// StudentManagementSystem class : handles all operations
// ---------------------------------------------------------
class StudentManagementSystem {
private:
    const string FILENAME = "students.txt";
    vector<Student> students;

    // Load all records from file into memory
    void loadFromFile() {
        students.clear();
        ifstream fin(FILENAME);
        string line;
        if (fin.is_open()) {
            while (getline(fin, line)) {
                if (!line.empty())
                    students.push_back(Student::fromFileString(line));
            }
            fin.close();
        }
    }

    // Save all records from memory back to file
    void saveToFile() {
        ofstream fout(FILENAME, ios::trunc);
        for (const auto &s : students) {
            fout << s.toFileString() << "\n";
        }
        fout.close();
    }

    // Check whether a roll number already exists
    bool rollExists(int roll) {
        for (const auto &s : students)
            if (s.roll == roll) return true;
        return false;
    }

    // Clear invalid cin state after bad input
    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

public:
    StudentManagementSystem() {
        loadFromFile(); // Persistent storage: load existing data on startup
    }

    // -------------------- ADD --------------------
    void addStudent() {
        int roll, age;
        string name, dept;
        float marks;

        cout << "\n--- Add Student ---\n";

        cout << "Enter Roll Number: ";
        while (!(cin >> roll)) {
            cout << "Invalid input. Enter a numeric Roll Number: ";
            clearInputBuffer();
        }

        if (rollExists(roll)) {
            cout << "A student with Roll Number " << roll
                 << " already exists. Use Update instead.\n";
            return;
        }

        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);

        cout << "Enter Age: ";
        while (!(cin >> age)) {
            cout << "Invalid input. Enter a numeric Age: ";
            clearInputBuffer();
        }

        cin.ignore();
        cout << "Enter Department: ";
        getline(cin, dept);

        cout << "Enter Marks: ";
        while (!(cin >> marks)) {
            cout << "Invalid input. Enter numeric Marks: ";
            clearInputBuffer();
        }

        students.push_back(Student(roll, name, age, dept, marks));
        saveToFile(); // persist immediately
        cout << "Student record added successfully!\n";
    }

    // -------------------- UPDATE --------------------
    void updateStudent() {
        int roll;
        cout << "\n--- Update Student ---\n";
        cout << "Enter Roll Number of student to update: ";
        while (!(cin >> roll)) {
            cout << "Invalid input. Enter a numeric Roll Number: ";
            clearInputBuffer();
        }

        for (auto &s : students) {
            if (s.roll == roll) {
                string name, dept;

                cin.ignore();
                cout << "Enter new Name (" << s.name << "): ";
                getline(cin, name);
                if (!name.empty()) s.name = name;

                cout << "Enter new Age (" << s.age << "): ";
                string ageInput;
                getline(cin, ageInput);
                if (!ageInput.empty()) {
                    try { s.age = stoi(ageInput); } catch (...) {}
                }

                cout << "Enter new Department (" << s.department << "): ";
                getline(cin, dept);
                if (!dept.empty()) s.department = dept;

                cout << "Enter new Marks (" << s.marks << "): ";
                string marksInput;
                getline(cin, marksInput);
                if (!marksInput.empty()) {
                    try { s.marks = stof(marksInput); } catch (...) {}
                }

                saveToFile(); // persist changes
                cout << "Student record updated successfully!\n";
                return;
            }
        }

        cout << "No student found with Roll Number " << roll << ".\n";
    }

    // -------------------- DELETE --------------------
    void deleteStudent() {
        int roll;
        cout << "\n--- Delete Student ---\n";
        cout << "Enter Roll Number of student to delete: ";
        while (!(cin >> roll)) {
            cout << "Invalid input. Enter a numeric Roll Number: ";
            clearInputBuffer();
        }

        for (size_t i = 0; i < students.size(); i++) {
            if (students[i].roll == roll) {
                students.erase(students.begin() + i);
                saveToFile(); // persist changes
                cout << "Student record deleted successfully!\n";
                return;
            }
        }

        cout << "No student found with Roll Number " << roll << ".\n";
    }

    // -------------------- DISPLAY ALL --------------------
    void displayAll() {
        cout << "\n--- All Student Records ---\n";
        if (students.empty()) {
            cout << "No records found.\n";
            return;
        }

        cout << left
             << setw(8)  << "Roll"
             << setw(20) << "Name"
             << setw(6)  << "Age"
             << setw(15) << "Department"
             << setw(8)  << "Marks" << endl;
        cout << string(57, '-') << endl;

        for (const auto &s : students)
            s.display();
    }

    // -------------------- DISPLAY ONE --------------------
    void displayOne() {
        int roll;
        cout << "\n--- Search Student ---\n";
        cout << "Enter Roll Number: ";
        while (!(cin >> roll)) {
            cout << "Invalid input. Enter a numeric Roll Number: ";
            clearInputBuffer();
        }

        for (const auto &s : students) {
            if (s.roll == roll) {
                cout << left
                     << setw(8)  << "Roll"
                     << setw(20) << "Name"
                     << setw(6)  << "Age"
                     << setw(15) << "Department"
                     << setw(8)  << "Marks" << endl;
                cout << string(57, '-') << endl;
                s.display();
                return;
            }
        }

        cout << "No student found with Roll Number " << roll << ".\n";
    }

    // -------------------- MENU --------------------
    void run() {
        int choice;

        do {
            cout << "\n=====================================\n";
            cout << "     STUDENT MANAGEMENT SYSTEM\n";
            cout << "=====================================\n";
            cout << "1. Add Student\n";
            cout << "2. Update Student\n";
            cout << "3. Delete Student\n";
            cout << "4. Display All Students\n";
            cout << "5. Search Student by Roll Number\n";
            cout << "6. Exit\n";
            cout << "=====================================\n";
            cout << "Enter your choice: ";

            while (!(cin >> choice)) {
                cout << "Invalid input. Enter a number between 1-6: ";
                clearInputBuffer();
            }

            switch (choice) {
                case 1: addStudent();   break;
                case 2: updateStudent(); break;
                case 3: deleteStudent(); break;
                case 4: displayAll();   break;
                case 5: displayOne();   break;
                case 6:
                    cout << "\nExiting... All data has been saved to '"
                         << FILENAME << "'.\n";
                    cout << "Thank you for using Student Management System!\n";
                    break;
                default:
                    cout << "Invalid choice. Please select between 1 and 6.\n";
            }

        } while (choice != 6);
    }
};

// ---------------------------------------------------------
// Main function
// ---------------------------------------------------------
int main() {
    StudentManagementSystem sms;
    sms.run();
    return 0;
}
