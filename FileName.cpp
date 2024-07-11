#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <string>
#include <iomanip>
#include <Windows.h>

using namespace std;

class Course
{
public:
    int id;
    string name;
    int credits;
    string professor;

    Course(int id, string name, int credits, string professor = "") : id(id), name(name), credits(credits), professor(professor) {}
    string get_details()
    {
        return "ID: " + to_string(id) + " | Name: " + name + " | Credits: " + to_string(credits) + " | Professor: " + professor;
    }
};

class Member
{
public:
    int id;
    string name;
    string email;

    Member(int id, const string& name, const string& email)
        : id(id), name(name), email(email) {}

    virtual string get_details() const
    {
        return "ID: " + to_string(id) + ", Name: " + name + ", Email: " + email;
    }
};

class Admin : public Member
{
public:
    Admin(int id, const string& name, const string& email)
        : Member(id, name, email) {}

    string get_details() const override
    {
        return "Admin - " + Member::get_details();
    }
};

class Student : public Member
{
public:
    string major;
    vector<Course*> enrolled_courses;

    Student(int id, string name, string email, string major) : Member(id, name, email), major(major) {}

    string get_details() const override
    {
        return "ID: " + to_string(id) + " | Name: " + name + " | Email: " + email + " | Major: " + major;
    }

    void enroll_in_course(Course* course)
    {
        enrolled_courses.push_back(course);
    }

    void display_enrolled_courses()
    {
        cout << "Enrolled Courses for " << name << ":\n";
        for (auto course : enrolled_courses)
        {
            cout << course->get_details() << endl;
        }
    }
};

class Professor : public Member
{
public:
    string department;
    vector<Course*> teaching_courses;

    Professor(int id, string name, string email, string department) : Member(id, name, email), department(department) {}

    string get_details() const override
    {
        return "ID: " + to_string(id) + " | Name: " + name + " | Email: " + email + " | Department: " + department;
    }

    void add_teaching_course(Course* course)
    {
        teaching_courses.push_back(course);
    }

    void display_teaching_courses()
    {
        cout << "Teaching Courses for " << name << ":\n";
        for (auto course : teaching_courses)
        {
            cout << course->get_details() << endl;
        }
    }
};

class University
{
private:
    vector<Member*> members;
    vector<Course*> courses;
    string admin_password; 

public:
    University() : admin_password("admin123")
    { 
        Admin* defaultAdmin = new Admin(1, "Admin", "admin@example.com");
        members.push_back(defaultAdmin);
    }
    ~University()
    {
        for (auto member : members)
        {
            delete member;
        }
        for (auto course : courses)
        {
            delete course;
        }
    }

    string get_admin_password() const
    {
        return admin_password;
    }

    void add_member(Member* member)
    {
        // Check if the member with the same id already exists
        for (auto existing_member : members)
        {
            if (existing_member->id == member->id)
            {
                cout << "Member with ID " << member->id << " already exists.\n";
                return; // Exit the function without adding the member
            }
        }

        // If member doesn't exist, add it to the vector
        members.push_back(member);
    }

    void edit_member(int id, string name, string email)
    {
        for (auto member : members)
        {
            if (member->id == id)
            {
                member->name = name;
                member->email = email;
            }
        }
    }

    Member* search_member_by_id(int id)
    {
        for (auto member : members)
        {
            if (member->id == id)
            {
                return member;
            }
        }
        return nullptr;
    }

    vector<Member*> search_member_by_name(const string& name)
    {
        vector<Member*> result;
        for (auto member : members)
        {
            if (member->name.find(name) != string::npos)
            {
                result.push_back(member);
            }
        }
        return result;
    }

    void delete_member(int id)
    {
        members.erase(remove_if(members.begin(), members.end(), [id](Member* member)
            { return member->id == id; }),
            members.end());
    }

    void display_members()
    {
        for (auto member : members)
        {
            cout << member->get_details() << endl;
        }
    }

    void add_course(Course* course)
    {
        courses.push_back(course);
    }

    Course* search_course_by_id(int id)
    {
        for (auto course : courses)
        {
            if (course->id == id)
            {
                return course;
            }
        }
        return nullptr;
    }

    vector<Course*> search_course_by_name(const string& name)
    {
        vector<Course*> result;
        for (auto course : courses)
        {
            if (course->name.find(name) != string::npos)
            {
                result.push_back(course);
            }
        }
        return result;
    }

    void delete_course(int id)
    {
        courses.erase(remove_if(courses.begin(), courses.end(), [id](Course* course)
            { return course->id == id; }),
            courses.end());
    }

    void display_courses()
    {
        for (auto course : courses)
        {
            cout << course->get_details() << endl;
        }
    }

    void save_data(const string& member_filename, const string& course_filename)
    {
        ofstream member_file(member_filename, ios::out);
        if (!member_file)
        {
            cerr << "Error opening " << member_filename << " for writing.\n";
            return;
        }

        for (auto member : members)
        {
            if (Student* student = dynamic_cast<Student*>(member))
            {
                member_file << "Student," << student->id << "," << student->name << "," << student->email << "," << student->major << endl;
            }
            else if (Professor* professor = dynamic_cast<Professor*>(member))
            {
                member_file << "Professor," << professor->id << "," << professor->name << "," << professor->email << "," << professor->department << endl;
            }
            else if (Admin* admin = dynamic_cast<Admin*>(member))
            {
                member_file << "Admin," << admin->id << "," << admin->name << "," << admin->email << endl;
            }
        }
        member_file.close();

        ofstream course_file(course_filename, ios::out);
        if (!course_file)
        {
            cerr << "Error opening " << course_filename << " for writing.\n";
            return;
        }

        for (auto course : courses)
        {
            course_file << course->id << "," << course->name << "," << course->credits << "," << course->professor << endl;
        }
        course_file.close();
    }

    void load_data(const string& member_filename, const string& course_filename)
    {
        ifstream member_file(member_filename, ios::in);
        if (!member_file)
        {
            cerr << "Error opening " << member_filename << " for reading.\n";
            return;
        }

        string line, type, name, email, major, department;
        int id;
        while (getline(member_file, line))
        {
            stringstream ss(line);
            getline(ss, type, ',');
            ss >> id;
            ss.ignore();
            getline(ss, name, ',');
            getline(ss, email, ',');

            if (type == "Student")
            {
                getline(ss, major, ',');
                add_member(new Student(id, name, email, major));
            }
            else if (type == "Professor")
            {
                getline(ss, department, ',');
                add_member(new Professor(id, name, email, department));
            }
            else if (type == "Admin")
            {
                add_member(new Admin(id, name, email));
            }
        }
        member_file.close();

        ifstream course_file(course_filename, ios::in);
        if (!course_file)
        {
            cerr << "Error opening " << course_filename << " for reading.\n";
            return;
        }

        string course_name, professor;
        int course_id, credits;
        while (getline(course_file, line))
        {
            stringstream ss(line);
            ss >> course_id;
            ss.ignore();
            getline(ss, course_name, ',');
            ss >> credits;
            ss.ignore();
            getline(ss, professor, ',');
            add_course(new Course(course_id, course_name, credits, professor));
        }
        course_file.close();
    }
};

void clearScreen()
{
    system("cls");
}

void display_admin_info(Member* admin)
{
    cout << "Welcome, Admin!\n";
    cout << "Your information:\n";
    cout << admin->get_details() << endl;
    cout << "----------------------------------\n";
}

void mainMenu();
void memberMenu(University& university);
void courseMenu(University& university);
void addMemberMenu(University& university);
void editMemberMenu(University& university);
void deleteMemberMenu(University& university);
void displayMembersMenu(University& university);
void addCourseMenu(University& university);
void editCourseMenu(University& university);
void deleteCourseMenu(University& university);
void displayCoursesMenu(University& university);
void enrollCourseMenu(University& university);
void displayEnrolledCoursesMenu(University& university);
void assignProfessorMenu(University& university);
void displayProfessorCoursesMenu(University& university);
void admin_login(University& university);
void admin_menu(University& university, Member* admin);

void mainMenu()
{
    University university;
    university.load_data("members.txt", "courses.txt");

    int choice;
    do
    {
        
        clearScreen();
        cout << "\n\n";
        cout << "\t\t*************************************************\n";
        cout << "\t\t*          Welcome to University Management      *\n";
        cout << "\t\t*************************************************\n";
        cout << "\t\t1. Member Management\n";
        cout << "\t\t2. Course Management\n";
        cout << "\t\t3. Admin Login\n";
        cout << "\t\t0. Exit\n";
        cout << "\n\t\tEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            memberMenu(university);
            break;
        case 2:
            courseMenu(university);
            break;
        case 3:
            admin_login(university);
            break;
        case 0:
            university.save_data("members.txt", "courses.txt");
            return;
        default:
            cout << "Invalid choice! Try again.\n";
            break;
        }
    } while (choice != 0);
}

void memberMenu(University& university)
{
    int choice;
    do
    {
        clearScreen();
        cout << "\n\n";
        cout << "\t\t*************************************************\n";
        cout << "\t\t*         Member Management Menu                *\n";
        cout << "\t\t*************************************************\n";
        cout << "\t\t1. Add Member\n";
        cout << "\t\t2. Edit Member\n";
        cout << "\t\t3. Delete Member\n";
        cout << "\t\t4. Display Members\n";
        cout << "\t\t5. Search Member by ID\n";
        cout << "\t\t6. Search Member by Name\n";
        cout << "\t\t0. Back to Main Menu\n";
        cout << "\n\t\tEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addMemberMenu(university);
            break;
        case 2:
            editMemberMenu(university);
            break;
        case 3:
            deleteMemberMenu(university);
            break;
        case 4:
            displayMembersMenu(university);
            break;
        case 5:
        {
            int id;
            cout << "Enter ID: ";
            cin >> id;
            Member* member = university.search_member_by_id(id);
            if (member)
            {
                cout << member->get_details() << endl;
            }
            else
            {
                cout << "Member not found.\n";
            }
            system("pause");
        }
        break;
        case 6:
        {
            string name;
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);
            vector<Member*> members = university.search_member_by_name(name);
            if (!members.empty())
            {
                for (auto member : members)
                {
                    cout << member->get_details() << endl;
                }
            }
            else
            {
                cout << "No members found with the given name.\n";
            }
            system("pause");
        }
        break;
        case 0:
            return;
        default:
            cout << "Invalid choice! Try again.\n";
            break;
        }
    } while (choice != 0);
}

void courseMenu(University& university)
{
    int choice;
    do
    {
        clearScreen();
        cout << "\n\n";
        cout << "\t\t*************************************************\n";
        cout << "\t\t*         Course Management Menu                *\n";
        cout << "\t\t*************************************************\n";
        cout << "\t\t1. Add Course\n";
        cout << "\t\t2. Edit Course\n";
        cout << "\t\t3. Delete Course\n";
        cout << "\t\t4. Display Courses\n";
        cout << "\t\t5. Search Course by ID\n";
        cout << "\t\t6. Search Course by Name\n";
        cout << "\t\t0. Back to Main Menu\n";
        cout << "\n\t\tEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addCourseMenu(university);
            break;
        case 2:
            editCourseMenu(university);
            break;
        case 3:
            deleteCourseMenu(university);
            break;
        case 4:
            displayCoursesMenu(university);
            break;
        case 5:
        {
            int id;
            cout << "Enter ID: ";
            cin >> id;
            Course* course = university.search_course_by_id(id);
            if (course)
            {
                cout << course->get_details() << endl;
            }
            else
            {
                cout << "Course not found.\n";
            }
            system("pause");
        }
        break;
        case 6:
        {
            string name;
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);
            vector<Course*> courses = university.search_course_by_name(name);
            if (!courses.empty())
            {
                for (auto course : courses)
                {
                    cout << course->get_details() << endl;
                }
            }
            else
            {
                cout << "No courses found with the given name.\n";
            }
            system("pause");
        }
        break;
        case 0:
            return;
        default:
            cout << "Invalid choice! Try again.\n";
            break;
        }
    } while (choice != 0);
}

void addMemberMenu(University& university) {
    clearScreen();
    int id;
    string name, email, major, department;
    int memberType;

    cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left corner
    cout << "**********************************************\n";
    cout << "*              Add Member                     *\n";
    cout << "**********************************************\n";
    cout << "Select Member Type:\n";
    cout << "1. Student\n";
    cout << "2. Professor\n";
    cout << "Enter your choice: ";
    cin >> memberType;

    cout << "Enter ID: ";
    cin >> id;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Email: ";
    getline(cin, email);

    if (memberType == 1) {
        cout << "Enter Major: ";
        getline(cin, major);
        university.add_member(new Student(id, name, email, major));
        cout << "\nStudent added successfully.\n";
    }
    else if (memberType == 2) {
        cout << "Enter Department: ";
        getline(cin, department);
        university.add_member(new Professor(id, name, email, department));
        cout << "\nProfessor added successfully.\n";
    }
    else {
        cout << "\nInvalid member type.\n";
    }
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void editMemberMenu(University& university) {
    clearScreen();
    int id;
    string name, email;

    cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left corner
    cout << "**********************************************\n";
    cout << "*              Edit Member                    *\n";
    cout << "**********************************************\n";
    cout << "Enter ID: ";
    cin >> id;
    cout << "Enter New Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter New Email: ";
    getline(cin, email);

    university.edit_member(id, name, email);
    cout << "\nMember edited successfully.\n";
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void deleteMemberMenu(University& university) {
    clearScreen();
    int id;

    cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left corner
    cout << "**********************************************\n";
    cout << "*              Delete Member                  *\n";
    cout << "**********************************************\n";
    cout << "Enter ID: ";
    cin >> id;

    university.delete_member(id);
    cout << "\nMember deleted successfully.\n";
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void displayMembersMenu(University& university) {
    clearScreen();

    cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left corner
    cout << "**********************************************\n";
    cout << "*              Display Members                *\n";
    cout << "**********************************************\n";
    university.display_members();
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void addCourseMenu(University& university) {
    clearScreen();
    int id, credits;
    string name, professor;

    cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left corner
    cout << "**********************************************\n";
    cout << "*              Add Course                     *\n";
    cout << "**********************************************\n";
    cout << "Enter ID: ";
    cin >> id;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Credits: ";
    cin >> credits;
    cout << "Enter Professor: ";
    cin.ignore();
    getline(cin, professor);

    university.add_course(new Course(id, name, credits, professor));
    cout << "\nCourse added successfully.\n";
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void editCourseMenu(University& university) {
    clearScreen();
    int id, credits;
    string name, professor;

    cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left corner
    cout << "**********************************************\n";
    cout << "*              Edit Course                    *\n";
    cout << "**********************************************\n";
    cout << "Enter ID: ";
    cin >> id;
    cout << "Enter New Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter New Credits: ";
    cin >> credits;
    cout << "Enter New Professor: ";
    cin.ignore();
    getline(cin, professor);

    Course* course = university.search_course_by_id(id);
    if (course) {
        course->name = name;
        course->credits = credits;
        course->professor = professor;
        cout << "\nCourse edited successfully.\n";
    }
    else {
        cout << "\nCourse not found.\n";
    }
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void deleteCourseMenu(University& university) {
    clearScreen();
    int id;

    cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left corner
    cout << "**********************************************\n";
    cout << "*              Delete Course                  *\n";
    cout << "**********************************************\n";
    cout << "Enter ID: ";
    cin >> id;

    university.delete_course(id);
    cout << "\nCourse deleted successfully.\n";
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void displayCoursesMenu(University& university) {
    clearScreen();

    cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left corner
    cout << "**********************************************\n";
    cout << "*              Display Courses                *\n";
    cout << "**********************************************\n";
    university.display_courses();
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void enrollCourseMenu(University& university) {
    clearScreen();
    int student_id, course_id;

    cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left corner
    cout << "**********************************************\n";
    cout << "*              Enroll Course                  *\n";
    cout << "**********************************************\n";
    cout << "Enter Student ID: ";
    cin >> student_id;
    cout << "Enter Course ID: ";
    cin >> course_id;

    Student* student = dynamic_cast<Student*>(university.search_member_by_id(student_id));
    if (student) {
        Course* course = university.search_course_by_id(course_id);
        if (course) {
            student->enroll_in_course(course);
            cout << "\nStudent enrolled in course successfully.\n";
        }
        else {
            cout << "\nCourse not found.\n";
        }
    }
    else {
        cout << "\nStudent not found.\n";
    }
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void displayEnrolledCoursesMenu(University& university) {
    clearScreen();
    int student_id;

    cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left corner
    cout << "**********************************************\n";
    cout << "*              Display Enrolled Courses       *\n";
    cout << "**********************************************\n";
    cout << "Enter Student ID: ";
    cin >> student_id;

    Student* student = dynamic_cast<Student*>(university.search_member_by_id(student_id));
    if (student) {
        student->display_enrolled_courses();
    }
    else {
        cout << "\nStudent not found.\n";
    }
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void assignProfessorMenu(University& university) {
    clearScreen();
    int course_id, professor_id;

    cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left corner
    cout << "**********************************************\n";
    cout << "*              Assign Professor to Course     *\n";
    cout << "**********************************************\n";
    cout << "Enter Course ID: ";
    cin >> course_id;
    cout << "Enter Professor ID: ";
    cin >> professor_id;

    Course* course = university.search_course_by_id(course_id);
    if (course) {
        Professor* professor = dynamic_cast<Professor*>(university.search_member_by_id(professor_id));
        if (professor) {
            course->professor = professor->name;
            professor->add_teaching_course(course);
            cout << "\nProfessor assigned to course successfully.\n";
        }
        else {
            cout << "\nProfessor not found.\n";
        }
    }
    else {
        cout << "\nCourse not found.\n";
    }
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void displayProfessorCoursesMenu(University& university) {
    clearScreen();
    int professor_id;

    cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left corner
    cout << "**********************************************\n";
    cout << "*              Display Professor's Courses    *\n";
    cout << "**********************************************\n";
    cout << "Enter Professor ID: ";
    cin >> professor_id;

    Professor* professor = dynamic_cast<Professor*>(university.search_member_by_id(professor_id));
    if (professor) {
        professor->display_teaching_courses();
    }
    else {
        cout << "\nProfessor not found.\n";
    }
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void admin_login(University& university)
{
    clearScreen();
    int id;
    string password;
    cout << "Enter Admin ID: ";
    cin >> id;
    cout << "Enter Admin Password: ";
    cin >> password;

    Member* member = university.search_member_by_id(id);
    if (Admin* admin = dynamic_cast<Admin*>(member))
    {

        if (password == university.get_admin_password())
        { // استفاده از get_admin_password()
            admin_menu(university, admin);
        }
        else
        {
            cout << "Incorrect password.\n";
            system("pause");
        }
    }
    else
    {
        cout << "Admin not found.\n";
        system("pause");
    }
}

void admin_menu(University& university, Member* admin)
{
    int choice;
    do
    {
        clearScreen();
        display_admin_info(admin);
        cout << "**********************************************\n";
        cout << "*              Admin Menu                     *\n";
        cout << "**********************************************\n";
        cout << "1. Member Management\n";
        cout << "2. Course Management\n";
        cout << "3. Enroll Course\n";
        cout << "4. Display Enrolled Courses\n";
        cout << "5. Assign Professor to Course\n";
        cout << "6. Display Professor's Courses\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            memberMenu(university);
            break;
        case 2:
            courseMenu(university);
            break;
        case 3:
            enrollCourseMenu(university);
            break;
        case 4:
            displayEnrolledCoursesMenu(university);
            break;
        case 5:
            assignProfessorMenu(university);
            break;
        case 6:
            displayProfessorCoursesMenu(university);
            break;
        case 0:
            return;
        default:
            cout << "Invalid choice! Try again.\n";
            break;
        }
    } while (choice != 0);
}

int main()
{
    mainMenu();
    return 0;
}

