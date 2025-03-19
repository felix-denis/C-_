/**
 * @file
 * school_management_system.cpp
 *
 * @author
 * Denis mugambi
 *
 * @
 *
 * @brief
 * The program is a prototype of a school management system where by the admin can be able to monitor the presence/number of the students in the school and also the
 * teachers in the school
 * The admin is able to ::
 *  -add student/teacher
 *  -remove student/teacher
 *  -view all student/teacher
 *  -for the teachers the admin can also assign classes to the teachers
 *  -create a csv file from the database 
 *
 *
 * This program is set to test my skills on the understanding of the c++ programming language
 * The topics will include:
 *      - input/output
 *      - file system
 *      - database(mysqlx)
 *      - functions
 *      - if/else
 *      - loops
 *
 *
 * @date
 *      started on Tue Mar 18
 *
 *      ended on
 *
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
void mysql_insert_student(string student_name, int student_adm_no, string student_stream, int student_form);
void mysql_insert_teachers(string teacher_name, int teacher_id, string assigned_class, string subjects_teaching);
void add_student();
void remove_student();
void display_student();
void add_teacher();
void remove_teacher();
void display_teachers();
void create_csv();

int main()
{

    char choice;
    bool is_running = true;

    while (is_running)
    {

        cout << "================================================" << endl;
        cout << endl;
        cout << "********    School Management System     *******" << endl;
        cout << endl;
        cout << "================================================" << endl;
        cout << endl;

        menu_display();

        cout << "Enter choice: ";
        cin >> choice;
        cout << endl;

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
        case '4':
            add_teacher();
            break;
        case '5':
            remove_teacher();
            break;
        case '6':
            display_teachers();
            break;

        case '7':
            create_csv();
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

void mysql_insert_teachers(string teacher_name, int teacher_id, string assigned_class, string subjects_teaching)
{
    using namespace mysqlx;

    Session session("mysqlx://root:denohgt@localhost:33060/school_system");
    Schema mydb = session.getSchema("school_system");
    Table teachers_table = mydb.getTable("teachers_table");

    teachers_table.insert("teacher_name", "teacher_id", "assigned_class", "subjects_teaching").values(teacher_name, teacher_id, assigned_class, subjects_teaching).execute();

    session.close();
}

void mysql_insert_student(string student_name, int student_adm_no, string student_stream, int student_form)
{
    using namespace mysqlx;

    Session session("mysqlx://root:denohgt3@localhost:33060/school_system");
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
    cout << "7. Create a csv file." << endl;
    cout << "8. Enter q to exit." << endl;
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
        if (i == 0)
        {
            cout << "Enter the name of the student: ";
            std::getline(cin >> std::ws, student_name);
        }
        else
        {
            cout << "Enter the name of the next student: ";
            std::getline(cin >> std::ws, student_name);
        }

        cout << "Enter the admission number of the student: ";
        cin >> student_adm_no;
        cout << "Enter the stream of the student: ";
        std::getline(cin >> std::ws, student_stream);
        cout << "Enter the form/class of the student(1-9): ";
        cin >> student_form;

        cout << endl;
        cout << "*************************************************" << endl;
        cout << endl;

        mysql_insert_student(student_name, student_adm_no, student_stream, student_form);

        ++i;
    }
}

void display_student()
{
    using namespace mysqlx;

    Session session("mysqlx://root:denohgt3@localhost:33060/school_system");
    Schema mydb = session.getSchema("school_system");
    Table student_table = mydb.getTable("student_table");

    RowResult result = student_table.select("*").execute();

    int MAX{25};
    int MAX_{15};

    cout << std::left << std::setw(MAX_) << "" << std::setw(MAX) << "Name" << std::setw(MAX_) << "Adm/Reg No" << std::setw(MAX_) << "Stream" << std::setw(MAX_) << "Form" << endl;
    cout << "---------------------------------------------------------------------------------" << endl;

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
    cout << endl;

    session.close();
}

void remove_student()
{
    using namespace mysqlx;

    int admission{};
    int no_student{};
    cout << "Enter the number of students you want to remove: ";
    cin >> no_student;
    int i{};

    Session session("mysqlx://root:denohgt3@localhost:33060/school_system");
    Schema mydb = session.getSchema("school_system");
    Table student_table = mydb.getTable("student_table");

    while (i < no_student)
    {
        cout << "Enter the admission number of the student you want to remove: ";
        cin >> admission;

        try
        {

            RowResult checkResult = student_table.select("student_adm_no").where("student_adm_no = :adm").bind("adm", admission).execute();

            if (checkResult.count() > 0)
            {
                student_table.remove().where("student_adm_no = :adm").bind("adm", admission).execute();
                cout << "Student with admission number [" << admission << "] removed successfully." << endl;
            }
            else
            {
                cout << "Student with admission number [" << admission << "] not found." << endl;
            }
        }
        catch (Error &e)
        {
            cout << "Database access Error: " << e.what() << endl;
        }

        ++i;
    }
}

void add_teacher()
{
    string teacher_name{""};
    int teacher_id{};
    string assigned_class{""};
    string subjects_teaching{""};
    int no_of_teachers{};
    int i{};

    cout << "How many teachers do you want to add: " << endl;
    cin >> no_of_teachers;

    while (i < no_of_teachers)
    {
        if (i == 0)
        {
            cout << "Enter the name of the teacher: ";
            std::getline(cin >> std::ws, teacher_name);
        }
        else
        {
            cout << "Enter the name of the next teacher: ";
            std::getline(cin >> std::ws, teacher_name);
        }

        cout << "Enter the school Id number of the teacher: ";
        cin >> teacher_id;
        cout << "Class to assign: ";
        std::getline(cin >> std::ws, assigned_class);
        cout << "Enter the subjects the teacher is teaching: ";
        std::getline(cin >> std::ws, subjects_teaching);

        cout << endl;
        cout << "**************************************************" << endl;
        cout << endl;

        mysql_insert_teachers(teacher_name, teacher_id, assigned_class, subjects_teaching);

        ++i;
    }
}

void remove_teacher()
{
    using namespace mysqlx;

    int teacher_id{};
    int no_of_teachers{};
    cout << "Enter the number of teachers you want to remove: ";
    cin >> no_of_teachers;
    int i{};

    Session session("mysqlx://root:denohgt3@localhost:33060/school_system");
    Schema mydb = session.getSchema("school_system");
    Table teacher_table = mydb.getTable("teachers_table");

    while (i < no_of_teachers)
    {
        cout << "Enter the school Id number of the teacher you want to remove: ";
        cin >> teacher_id;

        try
        {

            RowResult checkResult = teacher_table.select("teacher_id").where("teacher_id = :id").bind("id", teacher_id).execute();

            if (checkResult.count() > 0)
            {
                teacher_table.remove().where("teacher_id = :id").bind("id", teacher_id).execute();
                cout << endl;
                cout << "Teacher with school Id number [" << teacher_id << "] removed successfully." << endl;
            }
            else
            {
                cout << endl;
                cout << "Teacher with school Id number [" << teacher_id << "] not found." << endl;
            }
        }
        catch (Error &e)
        {
            cout << "Database access Error: " << e.what() << endl;
        }

        ++i;
    }
}

void display_teachers()
{
    using namespace mysqlx;

    Session session("mysqlx://root:denohgt3@localhost:33060/school_system");
    Schema mydb = session.getSchema("school_system");
    Table teacher_table = mydb.getTable("teachers_table");

    RowResult result = teacher_table.select("*").execute();

    int MAX{25};
    int MAX_{15};

    cout << std::left << std::setw(MAX) << "Name" << std::setw(MAX_) << "Teacher Id" << std::setw(MAX_) << "Class Assigned" << std::setw(MAX) << "Subjects" << endl;
    cout << "---------------------------------------------------------------------" << endl;

    for (Row row : result)
    {
        for (int i = 0; i < row.colCount(); ++i)
        {
            if (i == 0 || i == 3)
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
    cout << endl;

    session.close();
}

void create_csv()
{
    using namespace mysqlx;

    std::string file_name{""};
    std::string type{""};

    cout << "Is the file you want to create of teachers or students? enter -> (teachers / students): ";
    cin >> type;

    cout << "Enter the name of the file you want to create: ";
    cin >> file_name;

    Session session("mysqlx://root:denohgt3@localhost:33060/school_system");
    Schema mydb = session.getSchema("school_system");

    if (type == "teachers")
    {
        Table teachers_table = mydb.getTable("teachers_table");
        RowResult result = teachers_table.select("*").execute();

        std::ofstream file(file_name + ".csv");
        if (file.is_open())
        {
            
            for (Row row : result)
            {

                for (int i = 0; i < row.colCount(); ++i)
                {
                    if (i == row.colCount() - 1)
                    {
                        file << row[i];
                    }
                    else
                    {
                        file << row[i] << ",";
                    }
                }
                cout << endl;
            }
            file.close();

            cout << "File " << file_name << ".csv was created succesfully" << endl;
        }
    }
    else if (type == "students")
    {
        Table student_table = mydb.getTable("student_table");
        RowResult result = student_table.select("*").execute();

        std::ofstream file(file_name + ".csv");
        if (file.is_open())
        {
            int i{};
            for (Row row : result)
            {

                for (int i = 0; i < row.colCount(); ++i)
                {
                    if (i == row.colCount() - 1)
                    {
                        file << row[i] << endl;
                    }
                    else
                    {
                        file << row[i] << ",";
                    }
                }
                cout << endl;
            }

            file.close();

            cout << "File " << file_name << ".csv was created succesfully" << endl;
        }
    }
}
