/**
 * @brief
 * This program is set to test my skills on the understanding of the c++ programming language
 * The topics will include:
 *      - input/output
 *      - file system
 *      - database
 *      - functions
 *      - if/else
 *      - loops
 *      - some of the data structures (vectors, arrays)
 *
 */

#include <iostream>
#include <mysqlx/xdevapi.h>
#include <vector>
#include <iomanip>
#include <fstream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

void menu_display();
void mysql_insert(string student_name, int student_adm_no, string student_stream, int student_form);
void add_student();
void remove_student();
void display_student();
void add_teacher();
void remove_teacher();
void display_teachers();

int main()
{

    char choice;
    bool is_running = true;

    while (is_running)
    {

        cout << "================================================" << endl;
        cout << endl;
        cout << "School Management System" << endl;
        cout << endl;
        cout << "================================================" << endl;
        cout << endl;

        menu_display();

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case '1':
            add_student();
            break;
        case '2':
            remove_student();
            break;
        case '3':
            display_student();
            break;
        case 'q':
            is_running = false;
            break;
        default:
            cout << "Your choice is invalid" << endl;
            break;
        }
    }

    return 0;
}

void mysql_insert(string student_name, int student_adm_no, string student_stream, int student_form)
{
    using namespace mysqlx;

    Session session("localhost", 33060, "root", "denohgt3", "school_system");
    Schema mydb = session.getSchema("school_system");
    Table student_table = mydb.getTable("student_table");

    student_table.insert("student_name", "student_adm_no", "stream", "student_form").values(student_name, student_adm_no, student_stream, student_form).execute();

    session.close();
}

void menu_display()
{

    cout << "1. Add student." << endl;
    cout << "2. Remove student." << endl;
    cout << "3. View all students." << endl;
    cout << "4. Add teacher." << endl;
    cout << "5. Remove teacher." << endl;
    cout << "6. View all teachers" << endl;
    cout << "7. Enter q to exit." << endl;
    cout << endl;
}

void add_student()
{

    string student_name{""};
    int student_adm_no{};
    string student_stream{""};
    int student_form{};
    int no_of_student{};
    int i{};

    cout << "How many students do you want to add: " << endl;
    cin >> no_of_student;

    while (i < no_of_student)
    {
        cout << "Enter the name of the student: ";
        cin >> student_name;
        cout << "Enter the admission number of the student: ";
        cin >> student_adm_no;
        cout << "Enter the stream of the student: ";
        cin >> student_stream;
        cout << "Enter the form/class of the student: ";
        cin >> student_form;

        mysql_insert(student_name, student_adm_no, student_stream, student_form);

        ++i;
    }
}

void display_student()
{
    using namespace mysqlx;

    Session session("localhost", 33060, "root", "denohgt3", "school_system");
    Schema mydb = session.getSchema("school_system");
    Table student_table = mydb.getTable("student_table");

    RowResult result = student_table.select("*").execute();

    int MAX{25};
    int MAX_{15};

    for (Row row : result)
    {
        for (int i = 0; i < row.colCount(); ++i)
        {
            if (i == 1)
            {
                cout << std::left << std::setw(MAX) << row[i];
            }
            else
            {
                cout << std::left << std::setw(MAX_) << row[i];
            }
        }
        cout << endl;
    }

    session.close();
}

void remove_student() {
    using namespace mysqlx;

    int admission{};
    int no_student{};
    cout << "Enter the number of students you want to remove: ";
    cin >> no_student;
    int i{};

    Session session("localhost", 33060, "root", "denohgt3", "school_system");
    Schema mydb = session.getSchema("school_system");
    Table student_table = mydb.getTable("student_table");

    while (i < no_student) {
        cout << "Enter the admission number of the student you want to remove: ";
        cin >> admission;

        try {
            // Check if the student exists before attempting to remove
            RowResult checkResult = student_table.select("student_adm_no").where("student_adm_no = :adm").bind("adm", admission).execute();

            if (checkResult.count() > 0) {
                // Student exists, proceed with removal
                student_table.remove().where("student_adm_no = :adm").bind("adm", admission).execute();
                cout << "Student with admission number " << admission << " removed successfully." << endl;
            } else {
                cout << "Student with admission number " << admission << " not found." << endl;
            }

        } catch (Error &e) {
            cout << "Database access Error: " << e.what() << endl;
        }

        ++i;
    }
}