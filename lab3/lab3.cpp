// Andy Lee - 500163559
// CCPS 616 - Lab 3

#include <iostream>
#include <math.h>
#include <chrono>
#include <fstream>
#include <random>
#include <cstring>
#include <sstream>
#include <string>
#include <climits>

using namespace std;
using namespace std::chrono;

void printMatrix(int **arr, int size)
{
    cout << "*** printMatrix()" << endl;
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << "\t\t";

        for (int j = 0; j < size; j++)
        {
            cout << *(arr[i] + j) << " ";
        }
        cout << endl;
    }
}

void writeMatrix(ofstream &ofs, int **arr, int size)
{
    // ofs << "*** printMatrix()" << endl;
    for (int i = 0; i < size; i++)
    {
        // ofs << arr[i] << "\t\t";

        for (int j = 0; j < size; j++)
        {
            ofs << *(arr[i] + j) << " ";
        }
        ofs << endl;
    }
}

int lowestUnvisited(int *arr_dist, bool *arr_visited, int size)
{
    // cout << "*** lowestUnvisited()" << endl;
    int temp_i = -1;
    for (int i = 0; i < size; i++)
    {
        if (!(*(arr_visited + i)) && *(arr_dist + i) < INT_MAX)
        {
            if (temp_i < 0)
            {
                temp_i = i;
                continue;
            }

            if (*(arr_dist + temp_i) > *(arr_dist + i))
                temp_i = i;
        }
    }

    return temp_i;
}

void exploreNodeAndUpdate(int **arr, int node, bool *arr_visited, int *arr_dist, int *arr_from, int size)
{
    // cout << "*** exploreNodeAndUpdate() node = ";
    // cout << node << ", " ;
    // cout << endl;
    // cout << " ** then update tracker." << endl;

    // Mark visited.
    *(arr_visited + node) = true;

    // cout << "Explore node and update array" << endl;
    // Search row in original matrix.
    for (int i = 0; i < size; i++)
    {
        // Skip immediately if node has been visited.
        if (*(arr_visited + i))
            continue;

        // If number matrix is > 0 and smaller than number in array distance.
        if (*(arr[node] + i) > 0 && *(arr[node] + i) < *(arr_dist + i))
        {
            *(arr_from + i) = node;
            *(arr_dist + i) = *(arr[node] + i);
        }
    }
}

// Prints result to result matrix.
void compileResult(int **arr_result, int *arr_dist, int *arr_from, int size)
{
    cout << "*** compileResult()" << endl;
    for (int i = 0; i < size; i++)
    {
        cout << i << ".\t";
        // Retrive index point from "from" array.
        int x = *(arr_from + i);
        int y = i;
        int dist = *(arr_dist + i);
        cout << "\tx: " << x << "\ty: " << y << "\tdist: " << dist << endl;
        if (dist < 1)
            continue;
        *(arr_result[x] + y) = dist;
        *(arr_result[y] + x) = dist;
        // *(arr_result[i] + j) = *(arr_dist + i);
    }
}

// Prints tracker in a table.
void showTracker(bool *arr_visit, int *arr_dist, int *arr_from, int size)
{
    cout << endl;
    cout << "*** showTracker()" << endl;
    cout << "Point"
         << "\tVisited?"
         << "\t"
         << "Distance"
         << "\t"
         << "From pt" << endl;
    for (int i = 0; i < size; i++)
    {
        cout << i + 1 << "\t\t";

        cout << *(arr_visit + i);
        cout << "\t\t\t" << *(arr_dist + i);
        if (i == 0)
        {
            cout << "\t\t\t" << *(arr_from + i) + 1;
        }
        else
        {
            if (*(arr_from + i) >= 0)
                cout << "\t\t\t" << *(arr_from + i) + 1;
        }
        cout << endl;
    }
    cout << endl;
}

int main(int argc, char *argv[])
{

    // Setting default filename.
    string filename("graph2.txt");
    string line;
    int vertex = 0, count = 0;

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
    // Gets first line and set it as the vertex.
    istringstream ss_vertex(line);
    ss_vertex >> vertex;
    cout << "vertex: " << vertex << endl;

    // Initialize arrays, and indices
    int arr[vertex][vertex], result[vertex][vertex];
    int *ptr_arr[vertex], *ptr_result[vertex];
    int row = 0, col = 0;

    for (int row = 0; row < vertex; row++)
    {
        ptr_arr[row] = &arr[row][0];
        ptr_result[row] = &result[row][0];

        getline(file, line);
        istringstream iss_line(line);
        // cout << "iss_line --> " << iss_line.str() << "\t\t parsed --> ";
        string c;
        string tmp_str;
        for (int col = 0; col < vertex; col++)
        {
            // Break sentence into space delimited.
            getline(iss_line, tmp_str, ' ');            
            // cout << tmp_str << " ";

            // Convert the extracted value and store in array.
            int val = stoi(tmp_str);
            arr[row][col] = val;

            // Set result to 0 since we have iterators.
            result[row][col] = 0;
        }
        // cout << endl;
    }
    cout << "Print original matrix." << endl;
    printMatrix(ptr_arr, vertex);

    // Initialize tracking variables.
    bool visited[vertex];
    int distance[vertex];
    int from[vertex];

    for (int i = 0; i < vertex; i++)
    {
        distance[i] = (i > 0) ? INT_MAX : 0;
        visited[i] = false;
        from[i] = -1;
    }

    // Prints tracker variables in a table.
    // showTracker(visited, distance, from, vertex);

    cout << "Running algorithm ... " ;
    do
    {
        int node = lowestUnvisited(distance, visited, vertex);
        if (node < 0)
            break;

        exploreNodeAndUpdate(ptr_arr, node, visited, distance, from, vertex);
        // showTracker(visited, distance, from, vertex);

    } while (true);
    cout << "done." << endl;

    cout << "\nPrint final tracker results.";
    showTracker(visited, distance, from, vertex);

    cout << "Reorganize results and store in result matrix." << endl;
    compileResult(ptr_result, distance, from, vertex);

    cout << "\nPrint result matrix." << endl;
    printMatrix(ptr_result, vertex);

    // Resetting output file.
    ofstream myfile;
    myfile.open("graphMST_AndyLee_lab3.txt");
    myfile << "";
    myfile.close();
    // Printing final results to screen.
    myfile.open("graphMST_AndyLee_lab3.txt", ios::app);
    // myfile << "CCPS616 - Lab 3 - Andy Lee\n";
    // myfile << endl;
    myfile << vertex << endl;
    writeMatrix(myfile, ptr_result, vertex);
    myfile << endl;
    myfile.close();
    return 0;
}
