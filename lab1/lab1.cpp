// Andy Lee - 500163559
// CCPS 616 - Lab 1

#include <iostream>
#include <math.h>
#include <chrono>
#include <fstream>
#include <random>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace std::chrono;

void selectionSort(int *, int, int = 0);
void printArray(int *a, int size);
void mergeSort(int *a, int start, int end);
void merge(int *a, int start, int mid, int end);
void mergeSel(int *a, int start, int end, int t);
double smallest(double a, double b, double c, double d, double e);

// Iterations settings
int nIterations = 5;
int testIterations = 10;

// Threshold for MergeSel. My selection is 4.
int threshold = 4;
int a1_threshold = 2, a2_threshold = 4, a3_threshold = 10, a4_threshold = 12, a5_threshold = 16;

// List of iterations
double n_arr[] = {5e2, 1e3, 5e3, 1e4, 5e4};

int main()
{
    // Resetting output file.
    ofstream myfile;
    myfile.open("CCPS616_Lab1_AndyLee.txt");
    myfile << "";
    myfile.close();

    // Initilize result string rows. Append with row titles.
    string sel_results, merge_results, a1_results, a2_results, a3_results, a4_results, a5_results, bestX_results;
    sel_results.append("Sel\t");
    merge_results.append("Merge\t");
    a1_results.append(to_string(a1_threshold) + "=a1\t");
    a2_results.append(to_string(a2_threshold) + "=a2\t");
    a3_results.append(to_string(a3_threshold) + "=a3\t");
    a4_results.append(to_string(a4_threshold) + "=a4\t");
    a5_results.append(to_string(a5_threshold) + "=a5\t");
    bestX_results.append("bestX\t");

    // Remembering lowest
    int bestX;

    // Set precisions.
    std::cout << std::fixed;
    std::cout << std::setprecision(4);

    // Loop to iterate the list of different "n"s.
    for (int n_arrIdx = 0; n_arrIdx < nIterations; n_arrIdx++)
    {
        int n = n_arr[n_arrIdx];
        cout << "\nTest #" << n_arrIdx+1 << ". n=" << n << ". No. of trials: " << testIterations << "\n";
        // Grand time tracking
        double elapsedTimeSelection = 0, elapsedTimeMerge = 0, elapsedTimeMergeSel_a1 = 0, elapsedTimeMergeSel_a2 = 0,
               elapsedTimeMergeSel_a3 = 0, elapsedTimeMergeSel_a4 = 0, elapsedTimeMergeSel_a5 = 0;

        // Loop to repeat trials.
        for (int testNumber = 0; testNumber < testIterations; testNumber++)
        {
            // Printing test number.
            // ofstream myfile;
            // myfile.open("CCPS616_Lab1_AndyLee.txt", ios::app);
            // myfile << "Trial #" << testNumber << "\n";
            // myfile << "========================================\n";
            // myfile.close();
            // cout << "Trial #" << testNumber << "\n";

            // Array initalization. One array each Sel, Merge and MergeSel variants.
            int arr[n], arr_select[n], arr_merge[n], arr_mergesel_a1[n], arr_mergesel_a2[n], arr_mergesel_a3[n], arr_mergesel_a4[n], arr_mergesel_a5[n];
            // Time tracking variables.
            duration<double, std::milli> elapsed;
            auto start = high_resolution_clock::now(), finish = high_resolution_clock::now();

            // Randomization. Code from source: https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, n * 4);

            for (int idx = 0; idx < n; idx++)
            {
                arr[idx] = arr_select[idx] = arr_merge[idx] = arr_mergesel_a1[idx] = arr_mergesel_a2[idx] = arr_mergesel_a3[idx] = arr_mergesel_a4[idx] = arr_mergesel_a5[idx] = dis(gen);
            }

            // Original Array reporting.
            // printArray(arr, n);

            // Selection Sort reporting.
            start = high_resolution_clock::now();
            selectionSort(arr_select, n);
            finish = high_resolution_clock::now();
            // printArray(arr_select, n);
            elapsed = (finish - start);
            elapsedTimeSelection += elapsed.count();

            // Merge Sort reporting.
            start = high_resolution_clock::now();
            mergeSort(arr_merge, 0, n - 1);
            finish = high_resolution_clock::now();
            // printArray(arr_merge, n);

            elapsed = (finish - start);
            elapsedTimeMerge += elapsed.count();

            // Modified Merge Sort x=a1 reporting.
            start = high_resolution_clock::now();
            mergeSel(arr_mergesel_a1, 0, n - 1, a1_threshold);
            finish = high_resolution_clock::now();
            // printArray(arr_mergesel_a1, n);

            elapsed = (finish - start);
            elapsedTimeMergeSel_a1 += elapsed.count();

            // Modified Merge Sort x=a2 reporting.
            start = high_resolution_clock::now();
            mergeSel(arr_mergesel_a2, 0, n - 1, a2_threshold);
            finish = high_resolution_clock::now();
            // printArray(arr_mergesel_a2, n);

            elapsed = (finish - start);
            elapsedTimeMergeSel_a2 += elapsed.count();

            // Modified Merge Sort x=a3 reporting.
            start = high_resolution_clock::now();
            mergeSel(arr_mergesel_a3, 0, n - 1, a3_threshold);
            finish = high_resolution_clock::now();
            // printArray(arr_mergesel_a3, n);

            elapsed = (finish - start);
            elapsedTimeMergeSel_a3 += elapsed.count();

            // Modified Merge Sort x=a4 reporting.
            start = high_resolution_clock::now();
            mergeSel(arr_mergesel_a4, 0, n - 1, a4_threshold);
            finish = high_resolution_clock::now();
            // printArray(arr_mergesel_a4, n);

            elapsed = (finish - start);
            elapsedTimeMergeSel_a4 += elapsed.count();

            // Modified Merge Sort x=a5 reporting.
            start = high_resolution_clock::now();
            mergeSel(arr_mergesel_a5, 0, n - 1, a5_threshold);
            finish = high_resolution_clock::now();
            // printArray(arr_mergesel_a5, n);

            elapsed = (finish - start);
            elapsedTimeMergeSel_a5 += elapsed.count();
        }

        double a1 = (double)elapsedTimeMergeSel_a1 / (double)testIterations;
        double a2 = (double)elapsedTimeMergeSel_a2 / (double)testIterations;
        double a3 = (double)elapsedTimeMergeSel_a3 / (double)testIterations;
        double a4 = (double)elapsedTimeMergeSel_a4 / (double)testIterations;
        double a5 = (double)elapsedTimeMergeSel_a5 / (double)testIterations;

        // Find lowest X.
        bestX = smallest(a1,a2,a3,a4,a5);

        sel_results.append(to_string((double)(elapsedTimeSelection / (double)testIterations)) + "ms\t");
        merge_results.append(to_string((double)(elapsedTimeMerge / (double)testIterations)) + "ms\t");
        a1_results.append(to_string(a1) + "ms\t");
        a2_results.append(to_string(a2) + "ms\t");
        a3_results.append(to_string(a3) + "ms\t");
        a4_results.append(to_string(a4) + "ms\t");
        a5_results.append(to_string(a5) + "ms\t");
        bestX_results.append(to_string(bestX) + "\t\t");

        
        
        // Average time for this n.
        cout
            << "Average time:\t"
            << elapsedTimeSelection / testIterations
            << "ms\t"
            << elapsedTimeMerge / testIterations
            << "ms\t"
            << elapsedTimeMergeSel_a1 / testIterations
            << "ms\t"
            << elapsedTimeMergeSel_a2 / testIterations
            << "ms\t"
            << elapsedTimeMergeSel_a3 / testIterations
            << "ms\t"
            << elapsedTimeMergeSel_a4 / testIterations
            << "ms\t"
            << elapsedTimeMergeSel_a5 / testIterations
            << "ms\t"
            << bestX
            << " <- bestX\n";
    }

    // Printing final results to screen.
    cout << "\n\nFinal Results:\n====================\n";
    cout << "CCPS616 - Lab 1 - Andy Lee\n";
    cout << "Each result is the average of " << testIterations << " trials\n\n";
    cout << "n = \t5e2\t\t1e3\t\t5e3\t\t1e4\t\t5e4\n";
    cout << sel_results << "\n";
    cout << merge_results << "\n";
    cout << "\n";
    cout << "MergeSel\n";
    cout << a1_results << "\n";
    cout << a2_results << "\n";
    cout << a3_results << "\n";
    cout << a4_results << "\n";
    cout << a5_results << "\n";
    cout << bestX_results << "\n";
    cout << "\n\n";
    cout << "Based on this, MergeSel performed best (we hope) when x = " << bestX;

    // Exporting final results to file.
    myfile.open("CCPS616_Lab1_AndyLee.txt", ios::app);
    myfile << "CCPS616 - Lab 1 – Andy Lee\n";
    myfile << "Each result is the average of " << testIterations << " trials\n\n";
    myfile << "n = \t5e2\t\t1e3\t\t5e3\t\t1e4\t\t5e4\n";
    myfile << sel_results << "\n";
    myfile << merge_results << "\n";
    myfile << "\n";
    myfile << "MergeSel\n";
    myfile << a1_results << "\n";
    myfile << a2_results << "\n";
    myfile << a3_results << "\n";
    myfile << a4_results << "\n";
    myfile << a5_results << "\n";
    myfile << bestX_results << "\n";
    myfile << "\n\n";
    myfile << "Based on this, MergeSel performed best (we hope) when x = " << bestX;
    myfile.close();
    return 0;
}

void selectionSort(int *a, int size, int startIndex)
{

    for (int idx = startIndex; idx < size + startIndex; idx++)
    {

        int minValue = *(a + idx);
        int minIndex = idx;
        // cout << "\t1st index:" << idx << " set minValue = " << minValue << "\n";
        for (int idx2 = idx + 1; idx2 < size + startIndex; idx2++)
        {
            if (minValue > *(a + idx2))
            {
                minValue = *(a + idx2);
                minIndex = idx2;
                // cout << "\t\t2nd index:" << idx2 << " update minValue = " << minValue << "\n";
            }
        }
        // cout << "\t\tSWAP: " << idx << " (" << *(a+idx) << ") <==> " << minIndex <<" (" << minValue << ")\n";
        if (*(a + idx) != *(a + minIndex))
            swap(*(a + idx), *(a + minIndex));

        // printArray(a, startIndex + size);
    }
}

void mergeSort(int *a, int start, int end)
{
    if (end > start)
    {
        int middle = floor(((double)end + (double)start) / 2);
        // cout << "start: " << start << "\tend: " << end /*<< "\tmiddle: " <<  middle*/ << "\n";
        // cout << "First Half\t->\t";
        mergeSort(a, start, middle);
        // cout << "Second Half\n";
        mergeSort(a, middle + 1, end);
        // cout << "after mergesort" << "\t" << "start: " << start << "\tend: " << end << "\n" ;
        merge(a, start, middle, end);
    }
    else
    {
        // cout << "run merge" << "\t" << "start: " << start << "\tend: " << end << "\n" ;
    }
}

void merge(int *a, int start, int mid, int end)
{
    // cout << "merge() index:" << "\t" << "Start: " << start << "\tmid: " << mid << "\tend: " << end << "\n" ;
    int tmparr[(end + 1 - start)];
    int l1 = start;
    int l2 = mid + 1;

    for (int i = 0; i < end + 1 - start; i++)
    {

        if ((*(a + l1) <= *(a + l2) && l1 != -1) || (l2 == -1))
        {
            // cout << "compare:\t" << "L1:" << l1 << " (" << *(a+l1) << ")\t\t" << "L2:" << l2 << " " << *(a+l2) << "\n";
            tmparr[i] = *(a + l1);
            if (l1 != mid)
            {
                l1++;
            }
            else
            {
                l1 = -1;
            };
        }
        else /*if (l2 != -1)*/
        {
            // cout << "compare:\t" << "L1:" << l1 << " " << *(a+l1) << "\t\t" << "L2:" << l2 << " (" << *(a+l2) << ")\n";
            tmparr[i] = *(a + l2);
            if (l2 != end)
            {
                l2++;
            }
            else
            {
                l2 = -1;
            };
        }
    }

    for (int i = start; i < end + 1; i++)
    {
        *(a + i) = tmparr[i - start];
    }

    // printArray(tmparr, end+1-start);
    // printArray(a, end+1);
}

void mergeSel(int *a, int start, int end, int t)
{
    if (end > start && (end - start) >= t)
    {
        int middle = floor(((double)end + (double)start) / 2);
        // cout << "Condition met. " << "\t" << start << ".." << end;
        // cout << "\tmid=" <<  middle << ".\tRun self again...\n";
        mergeSel(a, start, middle, t);
        mergeSel(a, middle + 1, end, t);
        // cout << "[RUN merge]" << "\t" << "start: " << start << "\tmid: " << middle << "\tend: " << end << "\n" ;
        merge(a, start, middle, end);
    }
    else
    {
        // cout << "[RUN selection] " << "\t" << "start: " << start << "\tend: " << end << "\n" ;
        selectionSort(a, end - start + 1, start);
    }
}

// Function to print array to screen.
void printArray(int *a, int size)
{
    string result = "";
    for (int i = 0; i < size; i++)
    {
        result.append(to_string(*(a + i)));
        result.append(" ");
    }
    cout << "\n" << result << "\n";

    // ofstream myfile;
    // myfile.open("CCPS616_Lab1_AndyLee.txt", ios::app);
    // myfile << result << "\n\n";
    // myfile.close();
}

double smallest(double a, double b, double c, double d, double e) {
    // cout << "fixed:\n" << std::fixed << a << " "<< b << " "<< c << " "<< d << " " << e << "\n";
    int num = a1_threshold;
    double tmp = a;
    double epsilon  = 0.00001;

    if (tmp-b > epsilon) {tmp = b; num = a2_threshold; }
    if (tmp-c > epsilon) {tmp = c; num = a3_threshold;}
    if (tmp-d > epsilon) {tmp = d; num = a4_threshold;}
    if (tmp-e > epsilon) {tmp = e; num = a5_threshold;}

    // cout << "tmp: " << tmp << "\n";
    
    return num;
}