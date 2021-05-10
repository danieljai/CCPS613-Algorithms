// Andy Lee - 500163559
// CCPS 616 - Lab 4

#include <iostream>
#include <math.h>
// #include <chrono>
#include <fstream>
#include <random>
#include <cstring>
#include <sstream>
#include <string>
// #include <climits>
#include <algorithm>

using namespace std;
// using namespace std::chrono;

void printOverbook(vector<pair<int, int>> vec, ofstream &ofs)
{
    cout << "Printing rejected reservations." << endl;
    ofs << "Rejected Reservations." << endl;
    for (auto i = vec.begin(); i != vec.end(); i++)
    {
        cout << "(" << (*i).first << "," << (*i).second << ") ";
        ofs << "(" << (*i).first << "," << (*i).second << ") ";
    }
}

void printRoomSchedule(vector<vector<pair<int, int>>> vec, ofstream &ofs)
{
    cout << "Printing room schedule." << endl;
    ofs << "Room Schedule." << endl;
    int index = 0;
    for (auto i = vec.begin(); i != vec.end(); i++)
    {
        index++;
        cout <<  index << ". ";
        ofs << index << ". ";
        for (auto j = (*i).begin(); j != (*i).end(); j++)
        {
            cout << "(" << (*j).first << "," << (*j).second << ") ";
            ofs << "(" << (*j).first << "," << (*j).second << ") ";
        }
        cout << endl;
        ofs << endl;
    }
    cout << endl;
    ofs << endl;
}

bool compare(const pair<int, int>&i, const pair<int, int>&j)
{
    return i.second < j.second;
}

int main(int argc, char *argv[])
{

    // Setting default filename.
    string filename("bookings.txt");
    string line, tmp_begin, tmp_end;
    int rooms = 0;

    // Take first argument if it exists, otherwise preset with a default filename.
    if (argc > 1)
        filename = (argv[1]);

    // Open graph file.
    ifstream file(filename);
    if (file.is_open())
    {
        getline(file, line);
    }
    else
    {
        cout << "ERROR: unable to open file \"" << filename << "\"." << endl;
        return 0;
    };

    // Get number of rooms.
    istringstream ss_rooms(line);
    ss_rooms >> rooms;
    cout << "rooms: " << rooms << endl;

    // Getting second and third row.
    getline(file, line);
    istringstream iss_1(line);
    getline(file, line);
    istringstream iss_2(line);

    vector<pair<int, int>> vec_booking;

    vector<vector<pair<int, int>>> vec_rooms(rooms);
    vector<pair<int, int>> vec_overbook;

    cout << "Loading bookings data..." << endl;
    // Iterate through pairs of booking.
    while (!(iss_1).eof() || !(iss_2).eof())
    {
        getline(iss_1, tmp_begin, ' ');
        getline(iss_2, tmp_end, ' ');
        cout << "(" << tmp_begin << "," << tmp_end << ")"
             << " ";

        // Pairing in/out days and pushing to vector.
        vec_booking.push_back(make_pair(stoi(tmp_begin),stoi(tmp_end)));

        // sort(vec_booking.begin(), vec_booking.end());
        sort(vec_booking.begin(),vec_booking.end(),compare);

    }

    cout << "\n\n";

    for (auto i = vec_booking.begin(); i != vec_booking.end(); i++)
    {
        cout << "Booking for ";
        cout << "(" << (*i).first << "," << (*i).second << ")"
             << "...\t";

        // Keeping track whether room assigned successfully.             
        bool assignedRoom = false;

        // Counting room number.
        int idx = 0;
        for (auto j = vec_rooms.begin(); j != vec_rooms.end(); j++)
        {
            idx++;
            if ((*j).empty())
            {
                cout << "Room " << idx << " is empty. ";
                (*j).push_back((*i));
                cout << "\t[Assigned]" << endl;
                assignedRoom = true;
                break;
            }
            else
            {
                if ((*j).back().second <= (*i).first)
                {
                    cout << "Room " << idx << " is free. ";
                    (*j).push_back((*i));
                    cout << "\t[Assigned]" << endl;
                    assignedRoom = true;
                    break;
                }
            }
        }

        if (!assignedRoom)
        {
            cout << "\tNo rooms found!\t\t[Rejected]" << endl;
            vec_overbook.push_back((*i));
        }
    }
    

    // Resetting output file.
    ofstream myfile;
    myfile.open("AndyLee_lab4.txt");    
    myfile << "CCPS616 - Lab 4 - Andy Lee\n";
    myfile.close();
    // Printing final results to screen.
    myfile.open("AndyLee_lab4.txt", ios::app);
    printRoomSchedule(vec_rooms,myfile);
    printOverbook(vec_overbook,myfile);    
    myfile << endl;
    myfile.close();
    return 0;
}
