#include "head.h"
#include <istream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <functional>
#include <iostream>
#include <algorithm>
using namespace std;

double Weeklyreport::waitingtime(double a, double b)
{
    return b - a;
}

int Weeklyreport::getlength(vector<person<int> *> report_list)
{
    int length = report_list.size();
    return length;
}

void Weeklyreport::Save(int week, string file_head, bool is_sorted)
{
    vector<string *> file_matrix; // create a vector to store the file name string
    for (int i = 0; i < 3; i++)
    {
        string *file_vector = new string[12];
        for (int j = 0; j < 12; j++)
        {
            file_vector[j] = file_head + to_string(j + 1) + "File" + to_string(i + 1) + ".csv";
        }
        file_matrix.push_back(file_vector);
    }

    string *s1 = file_matrix[0];
    string *s2 = file_matrix[1];
    string *s3 = file_matrix[2];
    vector<int *> head_matrix;

    for (int i = 0; i < 3; i++)
    {
        int *head_vector = new int[12];
        for (int j = 0; j < 12; j++)
        {
            head_vector[j] = 0;
        }
        head_matrix.push_back(head_vector);
    } // generate the file name

    vector<person<int> *> *rl_1 = rl->b_retrieve_I(1); // the vector of status 1 patients
    vector<person<int> *> *rl_2 = rl->b_retrieve_I(2); // the vector of status 2 patients
    vector<person<int> *> *rl_3 = rl->b_retrieve_I(3); // the vector of status 3 patients

    if (is_sorted == true)
    {
        for (int i = 1; i < getlength(*rl_0); i++)
        {
            person<int> *temp_person = rl_0->at(i);
            if (temp_person->status == 1)
            {
                ofstream ofs;
                ofs.open(s1[week - 1], ios::app);
                if (head_matrix[0][week - 1] == 0)
                {
                    // Print table head
                    ofs << "Name,Profession,Age,Risk,Waiting Time" << endl; // create the head of excel
                    head_matrix[0][week - 1] = 1;
                }
                ofs << (*rl_0)[i]->name << "," // write the file using these information
                    << (*rl_0)[i]->prof << ","
                    << (*rl_0)[i]->age << ","
                    << (*rl_0)[i]->risk << ","
                    << waitingtime((*rl_0)[i]->Time[0], current_time) << endl; // curret time - register time
                ofs.close();
            }
            if (temp_person->status == 2)
            {
                ofstream ofs;
                ofs.open(s2[week - 1], ios::app);
                if (head_matrix[0][week - 1] == 0)
                {
                    // Print table head
                    ofs << "Name,Profession,Age,Risk,Waiting Time" << endl; // create the head of excel
                    head_matrix[0][week - 1] = 1;
                }
                ofs << (*rl_0)[i]->name << "," // write the file using these information
                    << (*rl_0)[i]->prof << ","
                    << (*rl_0)[i]->age << ","
                    << (*rl_0)[i]->risk << ","
                    << waitingtime((*rl_0)[i]->Time[0], current_time) << endl; // curret time - register time
                ofs.close();
            }
            if (temp_person->status == 3)
            {
                ofstream ofs;
                ofs.open(s3[week - 1], ios::app);
                if (head_matrix[0][week - 1] == 0)
                {
                    // Print table head
                    ofs << "Name,Profession,Age,Risk,Waiting Time" << endl; // create the head of excel
                    head_matrix[0][week - 1] = 1;
                }
                ofs << (*rl_0)[i]->name << "," // write the file using these information
                    << (*rl_0)[i]->prof << ","
                    << (*rl_0)[i]->age << ","
                    << (*rl_0)[i]->risk << ","
                    << waitingtime((*rl_0)[i]->Time[0], current_time) << endl; // curret time - register time
                ofs.close();
            }
        }
        return;
    }

    if (rl_1 != NULL)
    { // check the NULL pointer
        for (int i = 0; i < getlength(*rl_1); i++)
        {
            // traverse status1, status1 means the first type people without appointments
            ofstream ofs;
            ofs.open(s1[week - 1], ios::app); // open the file
            if (head_matrix[0][week - 1] == 0)
            {
                // Print table head
                ofs << "Name,Profession,Age,Risk,Waiting Time" << endl; // create the head of excel
                head_matrix[0][week - 1] = 1;
            }

            ofs << (*rl_1)[i]->name << "," // write the file using these information
                << (*rl_1)[i]->prof << ","
                << (*rl_1)[i]->age << ","
                << (*rl_1)[i]->risk << ","
                << waitingtime((*rl_1)[i]->Time[0], current_time) << endl; // curret time - register time
            ofs.close();
        }
    }
    if (rl_2 != NULL)
    { // check the NULL pointer
        for (int i = 0; i < getlength(*rl_2); i++)
        // traverse status2, status2 means the second type people with a set appointment
        {
            ofstream ofs;
            ofs.open(s2[week - 1], ios::app); // open the file

            if (head_matrix[1][week - 1] == 0)
            {
                // Print table head
                ofs << "Name,Profession,Age,Risk,Waiting Time" << endl; // create the head of excel
                head_matrix[1][week - 1] = 1;
            }

            int x = rl_2->size();
            person<int> *a_person = (*rl_2)[i];

            ofs << (*rl_2)[i]->name << "," // write the file
                << (*rl_2)[i]->prof << ","
                << (*rl_2)[i]->age << ","
                << (*rl_2)[i]->risk << ","
                << waitingtime((*rl_2)[i]->Time[0], current_time) << endl; // curret time - register time
            ofs.close();
        }
    }
    if (rl_3 != NULL)
    { // check the NULL pointer
        for (int i = 0; i < getlength(*rl_3); i++)
        {
            if ((*rl_3)[i]->Time[2] >= (week - 1) * 7 * 24)
            { // traverse status3, status3 means the 3rd type people who have been treated
                ofstream ofs;
                ofs.open(s3[week - 1], ios::app); // open the file
                if (head_matrix[2][week - 1] == 0)
                {
                    // Print table head
                    ofs << "Name,Profession,Age,Risk,Waiting Time" << endl; // create the head of excel
                    head_matrix[2][week - 1] = 1;
                }

                ofs << (*rl_3)[i]->name << "," // write data to the file
                    << (*rl_3)[i]->prof << ","
                    << (*rl_3)[i]->age << ","
                    << (*rl_3)[i]->risk << ","
                    << waitingtime((*rl_3)[i]->Time[0], (*rl_3)[i]->Time[2]) << endl; // treatment time - register time
                ofs.close();
            }
        }
    }

    // close the file
}

vector<person<int> *> *Weeklyreport::merge(index_structure<person<int>, long long, int> *rl, vector<person<int> *> *rl_1, vector<person<int> *> *rl_2, vector<person<int> *> *rl_3, vector<person<int> *> *rl_4, vector<person<int> *> *rl_5)
{
    vector<person<int> *> *rl_00 = new vector<person<int> *>;
    if (rl_1 != NULL || rl_2 != NULL || rl_3 != NULL || rl_4 != NULL || rl_5 != NULL)
    { // check the NULL pointer
        if (rl_1 != NULL)
        {
            for (int i = 0; i < getlength(*rl_1); i++)
            {
                (*rl_00).push_back((*rl_1)[i]);
            } // push the vector *rl_1 into *rl_00
        }
        if (rl_2 != NULL)
        {
            for (int i = 0; i < getlength(*rl_2); i++)
            {
                (*rl_00).push_back((*rl_2)[i]);
            } // push the vector *rl_2 into *rl_00
        }
        if (rl_3 != NULL)
        {
            for (int i = 0; i < getlength(*rl_3); i++)
            {
                (*rl_00).push_back((*rl_3)[i]);
            } // push the vector *rl_3 into *rl_00
        }
        if (rl_4 != NULL)
        {
            for (int i = 0; i < getlength(*rl_4); i++)
            {
                (*rl_00).push_back((*rl_4)[i]);
            } // push the vector *rl_4 into *rl_00
        }
        if (rl_5 != NULL)
        {
            for (int i = 0; i < getlength(*rl_5); i++)
            {
                (*rl_00).push_back((*rl_5)[i]);
            } // push the vector *rl_5 into *rl_00
        }
        return rl_00; // get the vector which includes all patients information
    }
    return NULL;
}

void Weeklyreport::Sort(int week)
{

    cout << "\nPlease select a sort method" << endl;
    cout << "Put in '1' if sort by name" << endl;
    cout << "Put in '2' if sort by profession" << endl;
    cout << "Put in '3' if sort by risk" << endl;

    int select = 0;
    cin >> select;
    while (cin.fail())
    {
        cout << "Wrong, you have inputed a wrong type data, please input one of '1', '2', '3'\n"
             << endl;
        cin.clear();
        while (cin.get() != '\n')
        {
            continue;
        }
        cout << "Please input again" << endl;
        cin >> select;
    } // judge if the input data ia int number

    for (int i = 0; i < getlength(*rl_0); i++)
    {
        int Min = i; // Declare the minimum subscript Min
        for (int j = i + 1; j < getlength(*rl_0); j++)
        {
            if (select == 1)
            {
                if (rl_0->at(Min)->name > rl_0->at(j)->name) // compare name
                {
                    Min = j;
                }
            }

            else if (select == 2)
            {
                if (rl_0->at(Min)->prof_id > rl_0->at(j)->prof_id) // compare profession
                {
                    Min = j;
                }
            }

            else if (select == 3)
            {
                if (rl_0->at(Min)->risk_id > rl_0->at(j)->risk_id) // compare risk
                {
                    Min = j;
                }
            }
        }
        if (i != Min)
        {
            swap(rl_0->at(i), rl_0->at(Min));
        }

        // Determine if the initial assumed minimum is the calculated minimum, and if not, exchange data
    }

    cout << "Successful!" << endl;
    this->Save(week, "./Report/SortedWeeklyFile/SortedWeekly", true); // Save the sorted results to a file
}

vector<person<int> *> *Monthlyreport::merge(index_structure<person<int>, long long, int> *rl, vector<person<int> *> *rl_1, vector<person<int> *> *rl_2, vector<person<int> *> *rl_3, vector<person<int> *> *rl_4, vector<person<int> *> *rl_5)
{
    vector<person<int> *> *rl_00 = new vector<person<int> *>;
    if (rl_1 != NULL || rl_2 != NULL || rl_3 != NULL || rl_4 != NULL || rl_5 != NULL)
    { // check the NULL pointer
        if (rl_1 != NULL)
        {
            for (int i = 0; i < getlength(*rl_1); i++)
            {
                (*rl_00).push_back((*rl_1)[i]);
            } // push the vector *rl_1 into *rl_00
        }
        if (rl_2 != NULL)
        {
            for (int i = 0; i < getlength(*rl_2); i++)
            {
                (*rl_00).push_back((*rl_2)[i]);
            } // push the vector *rl_2 into *rl_00
        }
        if (rl_3 != NULL)
        {
            for (int i = 0; i < getlength(*rl_3); i++)
            {
                (*rl_00).push_back((*rl_3)[i]);
            } // push the vector *rl_3 into *rl_00
        }
        if (rl_4 != NULL)
        {
            for (int i = 0; i < getlength(*rl_4); i++)
            {
                (*rl_00).push_back((*rl_4)[i]);
            } // push the vector *rl_4 into *rl_00
        }
        if (rl_5 != NULL)
        {
            for (int i = 0; i < getlength(*rl_5); i++)
            {
                (*rl_00).push_back((*rl_5)[i]);
            } // push the vector *rl_5 into *rl_00
        }
        return rl_00; // get the vector which includes all patients's information
    }
    return NULL;
}

int Monthlyreport::getlength(vector<person<int> *> report_list)
{
    int length = report_list.size();

    return length;
} // all people who have registerred

int Monthlyreport::getlength_m(vector<person<int> *> report_list, int month)
{
    int length = 0;
    for (int i = 0; i < report_list.size(); i++)
    {
        if (rl_0->at(i)->Time[0] >= (month - 1) * 24 * 28)
        {
            length++;
        }
    }
    return length;
} // the number of people who have registerred each month

double Monthlyreport::average_time(vector<person<int> *> report_list, int month)
{
    int treatment_number = 0;
    int sum = 0;
    for (int i = 0; i < getlength(*rl_0); i++)
    {

        if (rl_0->at(i)->status == 3 && rl_0->at(i)->Time[2] >= (month - 1) * 24 * 28)
        {
            double waiting_time = rl_0->at(i)->Time[2] - rl_0->at(i)->Time[0];
            sum += waiting_time;
            treatment_number++;
        }
    }
    double mean = sum / treatment_number;
    return mean;
} // the average waiting time of treated people

int Monthlyreport::appointment_num(vector<person<int> *> report_list, int month)
{
    int appointment_number = 0;
    for (int i = 0; i < getlength(*rl_0); i++)
    {

        if ((rl_0->at(i)->status == 2 || rl_0->at(i)->status == 3) && rl_0->at(i)->Time[1] >= (month - 1) * 24 * 28)
        {
            appointment_number++;
        }
        return appointment_number;
    } // the number of people who have appointment
    return appointment_number;
}

int Monthlyreport::withdraw_num(vector<person<int> *> report_list, int month)
{
    int withdraw_number = 0;
    for (int i = 0; i < getlength(*rl_0); i++)
    {

        if (((rl_0->at(i)->status == 4) || (rl_0->at(i)->status == 5)) && (rl_0->at(i)->Time[3] >= (month - 1) * 28 * 24))
        {
            withdraw_number++;
        }
    }
    return withdraw_number;
} // the number of people who have withdraw

int Monthlyreport::treatment_num(vector<person<int> *> report_list, int month)
{
    int treatment_number = 0;
    for (int i = 0; i < getlength(*rl_0); i++)
    {

        if (rl_0->at(i)->status == 3 && rl_0->at(i)->Time[2] >= (month - 1) * 24 * 28)
        {
            treatment_number++;
        }
    }
    return treatment_number;
} // the number of people who have been treated

int Monthlyreport::register_waiting_num(int id, int month)
{ // register_number-appoint_number
    int register_waiting_number = 0;
    for (int i = 0; i < getlength(*rl_0); i++)
    {

        if (
            rl_0->at(i)->local_id == id && rl_0->at(i)->status != 3 && rl_0->at(i)->status != 4 && rl_0->at(i)->status != 5 && rl_0->at(i)->Time[0] >= (month - 1) * 24 * 28)
        {
            register_waiting_number++;
        }
    }
    return register_waiting_number;
} ////the number of register-waiting people in each local office

int Monthlyreport::intotal_waiting_num(int a, int b, int c)
{ // register_number-treatment_number
    int num = a + b + c;
    return num;
} // the total number of waiting people

void Monthlyreport::Save(int month)
{

    string monthly[] = {"./Report/MonthlyReport/Monthly1_report.txt", "./Report/MonthlyReport/Monthly2_report.txt", "./Report/MonthlyReport/Monthly3_report.txt"};
    string title[] = {"January Report", "February Report", "March Report"};
    // write data to files
    ofstream ofs;
    ofs.open(monthly[month - 1], ios::app);
    ofs << "---" << title[month - 1] << "---" << endl;
    ofs << "Number of people who registered: " << getlength_m(*rl_0, month) << "\n"
        << "Number of people in local registry office 0 who are waiting: " << register_waiting_num(0, month) << "\n"
        << "Number of people in local registry office 1 who are waiting: " << register_waiting_num(1, month) << "\n"
        << "Number of people in local registry office 2 who are waiting: " << register_waiting_num(2, month) << "\n"
        << "Total number of people who are waiting: " << intotal_waiting_num(register_waiting_num(0, month), register_waiting_num(1, month), register_waiting_num(2, month)) << "\n"
        << "Total number of people who have been treated: " << treatment_num(*rl_0, month) << "\n"
        << "The average waiting time: " << average_time(*rl_0, month) << "\n"
        << "Number of people who withdrawed: " << withdraw_num(*rl_0, month) << endl;
    ofs.close(); // close the file
}

void report_by_choice::choice()
{

    int select = 0;
    while (select != -1)
    { // set a while loop
        cout << "Please choose the style you want to check '1' for medical status, '2' for register day,  '3' for register hour,   '4' for treatment day\nEnter -1 to exist" << endl;
        cin >> select;
        if (select == -1)
        {
            break;
        }
        while (cin.fail())
        {
            cout << "Wrong, you have inputed a wrong type data, please input one of '1' '2' '3' or '4'\n"
                 << endl;
            cin.clear();
            while (cin.get() != '\n')
            {
                continue;
            }
            cout << "Please input again" << endl;
            cin >> select;
        } // judge if the input data is int number

        if (select == 1)
        { // if 1, jump to the print_by_m function
            print_by_m();
        }
        else if (select == 2)
        { // if 2, jump to the print_by_r function
            print_by_r();
        }
        else if (select == 3)
        { // if 3, jump to the print_by_hour function
            print_by_hour();
        }
        else if (select == 4)
        { // if 4, jump to the print_by_t function
            print_by_t();
        } // all people who have registerred
    }
}

int report_by_choice::getlength(vector<Medical_status *> report_list)
{
    int length = report_list.size();
    return length;
}

int report_by_choice::getlength(vector<Registration *> report_list)
{
    int length = report_list.size();
    return length;
} // all people who have registerred

int report_by_choice::getlength(vector<appointment *> report_list)
{
    int length = report_list.size();
    return length;
} // all people who have registerred

void report_by_choice::print_by_m()
{
    int select = -3;
    while (select != -1)
    { // set a while loop

        cout << "Please choose medical status '0' '1' '2' or '3'\nEnter -1 to exist\n"
             << endl;
        cin >> select;
        if (select == -1)
        {
            break;
        }
        while (cin.fail())
        {
            cout << "Wrong, you have inputed a wrong type data, please input one of '0','1', '2', '3'\n"
                 << endl;
            cin.clear();
            while (cin.get() != '\n')
            {
                continue;
            }
            cout << "Please input again" << endl;
            cin >> select;
        } // judge if the input data is int number

        vector<Medical_status *> *rl_M = rl_m->b_retrieve_I(select); // retrieve the choice
        if (rl_M != NULL)
        { // check the NULL pointer
            string choice[] = {"./Report/Medical_Status_0.csv", "./Report/Medical_Status_1.csv", "./Report/Medical_Status_2.csv", "./Report/Medical_Status_3.csv"};
            // write data to files
            ofstream ofs;
            ofs.open(choice[select]);
            ofs << "ID,Risk Status"
                << "\n";
            for (int i = 0; i < getlength(*rl_M); i++)
            {
                // ofstream ofs;
                // ofs.open(choice[select], ios::app);
                ofs << (*rl_M)[i]->id << "," // write data to the file
                    << (*rl_M)[i]->risk << "\n";
            }
            ofs.close(); // close the file
        }

        else
        {
            cout << "No such medical status person\n";
        }
    }
}

void report_by_choice::print_by_t()
{
    int select = -3;

    while (select != -1)
    {
        cout << "Please input '0-83' to choose the day\nEnter -1 to exit" << endl; // input the day
        cin >> select;
        if (select == -1)
        {
            break;
        }
        while (cin.fail())
        {
            cout << "Wrong, you have inputed a wrong type data, please input one of '0-83'\n"
                 << endl;
            cin.clear();
            while (cin.get() != '\n')
            {
                continue;
            }
            cout << "Please input again" << endl;
            cin >> select;
        }                                                                     // judge if the input data ia int number
        cout << "Please input '0' '1' '2' to choose the hospital id" << endl; // input the hospital
        int sel = -3;
        cin >> sel;
        while (cin.fail())
        {
            cout << "Wrong, you have inputed a wrong type data, please input one of '0' '1' or '3'\n"
                 << endl;
            cin.clear();
            while (cin.get() != '\n')
            {
                continue;
            }
            cout << "Please input again" << endl;
            cin >> sel;
        } // judge if the input data ia int number

        ofstream ofs;
        ofs.open("./Report/Choose_by_Treattime.csv");
        ofs << "ID, Hospitcal ID, Status, Treatment Day, Treatment Type"
            << "\n";
        if (sel == 0)
        {
            vector<appointment *> *rl_T = rl_t0->b_retrieve_I(select); // retrieve the choice
            // write data to files
            if (rl_T != NULL)
            {

                for (int i = 0; i < getlength(*rl_T); i++)
                {
                    ofs << (*rl_T)[i]->pri_key << "," // write data to the file
                        << (*rl_T)[i]->hospital_id << ","
                        << (*rl_T)[i]->status << ","
                        << (*rl_T)[i]->date << ","
                        << (*rl_T)[i]->treatment << endl;
                }
            }
        }
        else if (sel == 1)
        {
            vector<appointment *> *rl_T = rl_t1->b_retrieve_I(select); // retrieve the choice
            // write data to files
            if (rl_T != NULL)
            {
                for (int i = 0; i < getlength(*rl_T); i++)
                {
                    ofs << (*rl_T)[i]->pri_key << "," // write data to the file
                        << (*rl_T)[i]->hospital_id << ","
                        << (*rl_T)[i]->status << ","
                        << (*rl_T)[i]->date << ","
                        << (*rl_T)[i]->treatment << endl;
                }
            }
        }
        else if (sel == 2)
        {
            vector<appointment *> *rl_T = rl_t2->b_retrieve_I(select); // retrieve the choice
            // write data to files
            if (rl_T != NULL)
            {
                for (int i = 0; i < getlength(*rl_T); i++)
                {
                    ofs << (*rl_T)[i]->pri_key << "," // write data to the file
                        << (*rl_T)[i]->hospital_id << ","
                        << (*rl_T)[i]->status << ","
                        << (*rl_T)[i]->date << ","
                        << (*rl_T)[i]->treatment << endl;
                }
            }
        }
        ofs.close();
        cout << "File generated in Report directory" << endl;
    }
}

void report_by_choice::print_by_r()
{
    int select = -3;
    cout << "Please input '0-83'as the registration date to look into\nEnter -1 to exit" << endl;
    cin >> select;
    while (cin.fail())
    {
        cout << "Wrong, you have inputed a wrong type data, please input one of '0-83'\n"
             << endl;
        cin.clear();
        while (cin.get() != '\n')
        {
            continue;
        }
        cout << "Please input again" << endl;
        cin >> select;
    } // judge if the input data is int number

    vector<Registration *> *rl_R = rl_r->b_retrieve_I(select); // retrieve the choice

    // write data to files
    if (rl_R != NULL)
    {
        ofstream ofs;
        ofs.open("./Report/Choose_by_regday.csv");
        ofs << "ID,Hospital ID,Registration Day"
            << "\n";
        for (int i = 0; i < getlength(*rl_R); i++)
        {
            ofs << (*rl_R)[i]->id << "," // write data to the file
                << (*rl_R)[i]->hospital_id[0] << ","
                << (*rl_R)[i]->Day << endl;
        }
        ofs.close(); // close the file
        cout << "File generated in Report directory" << endl;
    }
}

void report_by_choice::print_by_hour()
{
    int select1 = -3;
    int select2 = -3;
    while (select1 != -1 && select2 != -1)
    {
        cout << "Please input the begin hour '0-1992'\nEnter -1 to exit" << endl;
        cin >> select1;
        if (select1 == -1)
        {
            break;
        }
        while (cin.fail())
        {
            cout << "Wrong, you have inputed a wrong type data, please input one of '0-1992'\n"
                 << endl;
            cin.clear();
            while (cin.get() != '\n')
            {
                continue;
            }
            cout << "Please input again" << endl;
            cin >> select1;
        } // judge if the input data ia int number

        cout << "Please input the end hour '0-1992' NOTICE! this number should >= the begin hour, enter -1 to exit" << endl;
        cin >> select2;
        while (cin.fail())
        {
            cout << "Wrong, you have inputed a wrong type data, please input one of '0-1992'\n"
                 << endl;
            cin.clear();
            while (cin.get() != '\n')
            {
                continue;
            }
            cout << "Please input again" << endl;
            cin >> select2;
        } // judge if the input data is int number

        vector<Registration *> *rl_R = rl_r->query(select1, select2); // retrieve the choice

        // write data to files
        if (rl_R != NULL)
        {
            ofstream ofs;
            ofs.open("./Report/Choose_by_reghour_range.csv");
            ofs << "ID,Hospital,Registration Hour"
                << "\n";
            for (int i = 0; i < getlength(*rl_R); i++)
            {
                ofs << (*rl_R)[i]->id << "," // write data to the file
                    << (*rl_R)[i]->hospital_id[0] << ","
                    << (*rl_R)[i]->time << endl;
            }
            ofs.close(); // close the file
            cout << "File generated in Report directory" << endl;
        }
    }
}
