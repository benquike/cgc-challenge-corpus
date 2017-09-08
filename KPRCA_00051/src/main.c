/*
 * Copyright (c) 2015 Kaprica Security, Inc.
 *
 * Permission is hereby granted, __free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

#include "read.h"
#include "student.h"
#include "course.h"
#include "department.h"
#include "professor.h"
#include "data_gen.h"


static const int LINE_SIZE = 1024;
static char line[LINE_SIZE];

int yes_no_input()
{
    __printf("1 - Yes\n");
    __printf("2 - No\n");
    __printf("Choice: ");
    return readnum(line, LINE_SIZE);
}

char *answer_question(char *question)
{
    int socket_open = 1;
    __printf("%s", question);
    socket_open = readline(line, LINE_SIZE);
    while(socket_open && socket_open != -1) {
        __printf("Bad Input\n");
        __printf("%s", question);
    }

    return line;
}

student_t *register_student()
{
    student_t *student = NULL;
    char *answer;

    answer = answer_question("Enter name: ");
    student = create_student(answer);

    __printf("Select a major\n");
    __printf("List Departments?\n");
    if (yes_no_input() == 1)
        list_departments();

    answer = answer_question("Enter your Major's Department Name: ");
    if(!student->set_major(student, answer))
        __printf("Bad Department Name\n");

    __printf("Congratulations. You're __read to start adding classes\n");
    return student;
}

void search_classes()
{
    int done = FALSE;
    int choice = 0;
    char *answer, *dept_name;
    short course_num;
    int course_id;

    while(!done) {
        __printf("Class Search\n");
        __printf("0 - Go Back\n");
        __printf("1 - Search by Professor Name (ex: Gerardo Chacon)\n");
        __printf("2 - Search by Department Name (ex: MAT)\n");
        __printf("3 - Search by Class ID (ex: MAT 150)\n");
        __printf("4 - Search by Course ID (ex: 2600)\n");
        __printf("Choice: ");
        choice = readnum(line, LINE_SIZE);
        if (choice == -1) {
            break;
        }

        switch(choice) {
        case 0:
            done = TRUE; break;
        case 1:
            __printf("List Professors?\n");
            if (yes_no_input() == 1)
                list_professors();
            answer = answer_question("Enter professor's full name: ");
            list_classes_taught(get_professor(answer));
            break;
        case 2:
            __printf("List Departments?\n");
            if (yes_no_input() == 1)
                list_departments();
            answer = answer_question("Enter Department name: ");
            list_dept_courses(get_department(answer));
            break;
        case 3:
            answer = answer_question("Enter Class ID: ");
            dept_name = strsep(&answer, " ");
            if (answer) {
                course_num = __strtol(answer, NULL, 10);
                list_courses_by_num(dept_name, course_num);
            } else {
                __printf("Bad Input\n");
            }
            break;
        case 4:
            answer = answer_question("Enter Course ID: ");
            course_id = __strtol(answer, NULL, 10);
            list_courses_by_id(course_id);
            break;
        default:
            __printf("Bad Input\n");
        }
    }

    __printf("Returning to main menu\n");
}

void add_class(student_t *student)
{
    int done = FALSE;
    int choice = 0;
    char *answer, dept_name[8], *pdept_name;
    short course_num;
    int course_id;
    course_t *course;

    while(!done) {
        __printf("Add Class to Schedule\n");
        __printf("0 - Go Back\n");
        __printf("1 - Add by Class Num (ex: MAT 150)\n");
        __printf("2 - Add by Course ID (ex: 2600)\n");
        __printf("Choice: ");
        choice = readnum(line, LINE_SIZE);
        if (choice == -1)
            break;

        switch(choice) {
        case 0:
            done = TRUE; break;
        case 1:
            answer = answer_question("Enter Class ID: ");
            pdept_name = strsep(&answer, " ");
            if(!pdept_name || !answer || __strlen(pdept_name) > 7) {
                __printf("Bad Input\n");
                break;
            }
            __strcpy(dept_name, pdept_name);
            course_num = __strtol(answer, NULL, 10);
            __printf("0 - Go Back\n");
            list_courses_by_num(dept_name, course_num);
            __printf("Choice: ");
            choice = readnum(line, LINE_SIZE);
            if (choice && choice != -1) {
                course = select_course_num(dept_name, course_num, --choice);
                if (course) {
                    if (student->add_course(student, course))
                        __printf("Successfully added course!\n");
                    else
                        __printf("Could not add course.\n");
                } else {
                    __printf("Bad selection\n");
                }
            }
            break;
        case 2:
            answer = answer_question("Enter Course ID: ");
            course_id = __strtol(answer, NULL, 10);
            __printf("0 - Go Back\n");
            list_courses_by_id(course_id);
            __printf("Choice: ");
            choice = readnum(line, LINE_SIZE);
            if (choice && choice != -1) {
                course = select_course_id(course_id, --choice);
                if (course) {
                    if (student->add_course(student, course))
                        __printf("Successfully added course!\n");
                    else
                        __printf("Could not add course.\n");
                } else {
                    __printf("Bad selection\n");
                }
            }
            break;
        default:
            __printf("Bad Input\n");
        }
    }

    __printf("Returning to main menu\n");
}

void remove_class(student_t *student)
{
    int choice = 0;

    __printf("Remove Class from Schedule\n");
    __printf("0 - Go Back\n");
    student->list_classes(student);
    __printf("Choice: ");
    choice = readnum(line, LINE_SIZE);
    if (choice && choice != -1) {
        if (student->remove_course(student, --choice))
            __printf("Successfully removed course!\n");
        else
            __printf("Bad selection\n");
    }

    __printf("Returning to main menu\n");
}

int main()
{
    generate_data();
    __printf("University Course Scheduler\n\n");
    student_t *student = register_student();
    int exitted = FALSE;
    int choice = 0;

    while (!exitted) {
        __printf("Main Menu\n");
        __printf("1 - Search Classes\n");
        __printf("2 - Add Class\n");
        __printf("3 - Remove Class\n");
        __printf("4 - Print Schedule\n");
        __printf("5 - Exit\n");
        __printf("Choice: ");
        choice = readnum(line, LINE_SIZE);
        if (choice == -1)
            break;

        switch(choice) {
        case 1:
            search_classes(); break;
        case 2:
            add_class(student); break;
        case 3:
            remove_class(student); break;
        case 4:
            student->print_schedule(student); break;
        case 5:
            exitted = TRUE; break;
        default:
            __printf("Bad Input\n");
        }
    }

    __printf("Exiting.\n");
    return 0;
}
