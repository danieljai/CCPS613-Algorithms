// Andy Lee - 500163559
// CCPS 616 - Lab 5

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
#include <bitset>
#include <iterator>
using namespace std;

int ks(int k, int w, int **arr_ptr);
void printMatrix(int k, int w, int **arr_ptr);
void reverseMatrix(int k, int w, int **arr_ptr, vector<pair<int, int>> vec, vector<int> &result_vec);
void traverseMatrix(int row, int col, int **arr_ptr, vector<pair<int, int>> vec, vector<int> &result_vec);
string printItems(vector<int> vlist, vector<pair<int, int>> source);

int main(int argc, char *argv[])
{
    // Resetting output file.
    ofstream myfile;
    myfile.open("AndyLee_lab5.txt");    
    myfile << "CCPS616 - Lab 5 - Andy Lee\n";
    myfile.close();

    // Setting default filename.
    string filename("data.txt");
    string line, tmp_begin, tmp_end;
    int sacksize = 0, items = 0;

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

    // Get number of sacksize.
    istringstream ss_sacksize(line);
    ss_sacksize >> sacksize;
    cout << "knapsack size: " << sacksize << endl;

    // Getting second and third row.
    getline(file, line);
    istringstream iss_1(line);
    getline(file, line);
    istringstream iss_2(line);

    vector<pair<int, int>> vec_pairs;

    cout << "Loading data..." << endl;
    // Iterate through pairs.
    while (!(iss_1).eof() || !(iss_2).eof())
    {
        getline(iss_1, tmp_begin, ' ');
        getline(iss_2, tmp_end, ' ');
        cout << "(" << tmp_begin << "," << tmp_end << ")"
             << " ";

        // Pushing pair to vector.
        vec_pairs.push_back(make_pair(stoi(tmp_begin), stoi(tmp_end)));

        // Count number of items.
        items++;
    }

    cout << endl;

    // Sorting pairs and print sorted vector.
    sort(vec_pairs.begin(), vec_pairs.end());
    for (auto v = vec_pairs.begin(); v != vec_pairs.end(); ++v)
    {
        cout << "(" << (*v).first << "," << (*v).second << ")"
             << " ";
    }

    cout << "\t(sorted)" << endl;

    // Global combo/value keepers.
    int best_combo = -1, best_value = -1, best_weight = -1;

    // Brute force.
    cout << "\n* BRUTE FORCE **************************************" << endl;
    for (int i = 0; i < pow(2, items); i++)
    {
        // Assuming it accepts up to 65536 items.
        string inbits = bitset<65536>(i).to_string();
        reverse(inbits.begin(), inbits.end());        

        // Local weight/value counters.
        int weight = 0, value = 0;

        // Iterate char by char. Do something only if bit is '1'.
        for (int j = 0; j < items; j++)
        {
            if (inbits[j] == '1')
            {
                weight += vec_pairs[j].first;
                value += vec_pairs[j].second;
            }

            // cout << inbits[j] << " ";
        }

        // Finding whether this combo is valid and the best
        if (sacksize >= weight && value > best_value)
        {
            best_value = value;
            best_combo = i;
            best_weight = weight;
            cout << "weight = " << weight << "\t\tvalue = " << value;
            cout << " <== current best" << endl;
        }
    }

    cout << endl;

    // Recalling results
    string inbits = bitset<16>(best_combo).to_string();
    vector<int> brute_result_vec;

    reverse(inbits.begin(), inbits.end());
    // cout << "best_combo at position: " << best_combo << endl;
    // cout << "best_combo (bits): " << inbits.substr(0, items) << endl;
    // cout << "best_value: " << best_value << endl;
    // cout << "best_weight: " << best_weight << endl;


    // Compiling indices that was chosen.
    for (int j = 0; j < items; j++)
    {
        if (inbits[j] == '1')
        {
            brute_result_vec.push_back(j);
        }
    }

    myfile.open("AndyLee_lab5.txt", ios::app);
    myfile << printItems(brute_result_vec, vec_pairs);
    myfile << endl;
    myfile.close();

    
    cout << endl;
    cout << "* DYNAMIC PROGRAMMING ******************************" << endl;


    // Initialize 2D array and pointer to first element of each row.
    int arr[items][sacksize + 1];
    int *ptr_arr[items];

    // Assigning pointers of 2D array to a pointer array for easy access.
    for (int i = 0; i < items; i++)
    {
        // arr[i][0] = 0;
        ptr_arr[i] = &arr[i][0];
    }

    for (int i = 0; i < items; i++)
    {
        int item_weight = vec_pairs[i].first;
        int item_value = vec_pairs[i].second;

        for (int max_weight = 0; max_weight <= sacksize; max_weight++)
        {
            if (item_weight == 0 || max_weight == 0)
            {
                arr[i][max_weight] = 0;
            }
            else if (item_weight > max_weight)
            {
                // cout << "2 ";
                arr[i][max_weight] = ks(i - 1, max_weight, ptr_arr);
                // cout << "entry: " << arr[i][max_weight] << endl;
            }
            else
            {
                arr[i][max_weight] = max(ks(i - 1, max_weight, ptr_arr), ks(i - 1, max_weight - item_weight, ptr_arr) + item_value);

                // cout << "entry: " << arr[i][max_weight] << endl;
            }
        }

        // cout << endl;
    }

    printMatrix(items, sacksize, ptr_arr);

    // Go back and find the path of indices chosen.
    vector<int> result_vec;
    reverseMatrix(items, sacksize, ptr_arr, vec_pairs, result_vec);

    cout << endl;
    myfile.open("AndyLee_lab5.txt", ios::app);
    myfile << printItems(result_vec, vec_pairs);
    myfile << endl;
    
    
    // Test if both implementation matches.
    if (result_vec == brute_result_vec)
        cout << "\n**** Both vectors match ****" << endl;
        myfile << "\n**** Both vectors match ****" << endl;
    return 0;

    myfile.close();
}

string printItems(vector<int> vlist, vector<pair<int, int>> source)
{
    stringstream ss;
    int best_weight = 0, best_value = 0;

    ss << "Items: ";

    cout << "printItems()\n\tindex: ";
    for (auto i = vlist.begin(); i != vlist.end(); ++i)
    {
        
        int idx = *i;
        cout << idx << ' ';
        ss << "(" << source[idx].first << "," << source[idx].second << ") ";

        best_weight += source[idx].first;
        best_value += source[idx].second;
    }

    // cout << "\n\t" << ss.str() << endl;
    // cout << "\t best_value: " << best_value << endl;
    // cout << "\t best_weight: " << best_weight << endl;

    ss << "\nValue: " << best_value << "\nWeight: " << best_weight << endl;
    cout << "\n" << ss.str();


    return ss.str();
}

int ks(int k, int w, int **arr_ptr)
{
    int result = 0;
    // cout << "k: " << k << "\tw: " << w;

    if (k < 0 || w <= 0)
    {
        // cout << "\tks(result): " << result << "\t";
        return result;
    };

    result = *(arr_ptr[k] + w);
    // cout << "\tks(result): " << result << "\t";
    return result;
}

void printMatrix(int k, int w, int **arr_ptr)
{
    cout << "printMatrix()" << endl;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j <= w; j++)
        {
            cout << *(arr_ptr[i] + j) << " ";
        }

        cout << endl;
    }

    cout << endl;
}

void reverseMatrix(int k, int w, int **arr_ptr, vector<pair<int, int>> vec, vector<int> &result_vec)
{
    cout << "reverseMatrix()" << endl;
    int row = k - 1;
    int col = w;

    traverseMatrix(row, col, arr_ptr, vec, result_vec);
}

void traverseMatrix(int row, int col, int **arr_ptr, vector<pair<int, int>> vec, vector<int> &result_vec)
{
    cout << "Current row: " << row << "\tcol: " << col << "\tvalue: " << *(arr_ptr[row] + col) << endl;
    if (row - 1 < 0 || col <= 0 || *(arr_ptr[row] + col) <= 0)
    {
        cout << "\treached end." << endl;
        if (*(arr_ptr[row] + col) > 0)
        {
            cout << "\tinclude last weight: " << vec[row].first << endl;
            result_vec.insert(result_vec.begin(), row);
        }
    }
    else if (*(arr_ptr[row] + col) == *(arr_ptr[row - 1] + col))
    {
        cout << "\t(going N) \trow: " << row - 1 << endl;
        traverseMatrix(row - 1, col, arr_ptr, vec, result_vec);
    }
    else
    {
        int current_weight = vec[row].first;
        cout << "\t(going NW) \trow: " << row - 1 << "\tcol: " << col - current_weight << "\tweight: " << current_weight << endl;
        result_vec.insert(result_vec.begin(), row);
        traverseMatrix(row - 1, col - current_weight, arr_ptr, vec, result_vec);
    }
}