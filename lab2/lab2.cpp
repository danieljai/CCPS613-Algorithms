// Andy Lee - 500163559
// CCPS 616 - Lab 2

#include <iostream>
#include <math.h>
#include <chrono>
#include <fstream>
#include <random>
#include <cstring>
#include <sstream>
#include <string>
#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

const static int MIN_DIGITS = 10;
const static int MAX_DIGITS = 300;

string mult4(const char *x, const char *y);
int to_int(char c);
// char* repeatZeros(int n);
void repeat_0(char *zeros, int n);
string stringAddition(const char *a, const char *b);
string stringSubtraction(const char *a, const char *b);
string mult3a(const char *x, const char *y);
string mult3b(const char *x, const char *y);
string randomNum(int n);

random_device rd;

int main()
{
    // Resetting output file.
    ofstream myfile;
    myfile.open("CCPS616_Lab2_AndyLee.txt");
    myfile << "";
    myfile.close();

    string rand1, rand2;
    string s1, s2, s3;

    double elapsed_s1 = 0, elapsed_s2 = 0, elapsed_s3 = 0;

    // Printing final results to screen.
    cout << "CCPS616 - Lab 2 - Andy Lee\n";
    cout << "Each result is the average of  10 trials\n\n";
    cout << "k";
    cout << "\t\tmult4() ";
    cout << "\tmult3a() ";
    cout << "\tmult3b() ";
    cout << endl;

    myfile.open("CCPS616_Lab2_AndyLee.txt", ios::app);
    // Printing final results to screen.
    myfile << "CCPS616 - Lab 2 - Andy Lee\n";
    myfile << "Each result is the average of  10 trials\n\n";
    myfile << "k";
    myfile << "\t\tmult4() ";
    myfile << "\tmult3a() ";
    myfile << "\tmult3b() ";
    myfile << endl;
    myfile.close();

    for (int current_digits = MIN_DIGITS; current_digits <= MAX_DIGITS; current_digits++)
    {
        // Resetting timers.
        elapsed_s1 = elapsed_s2 = elapsed_s3 = 0;

        for (int trials = 0; trials < 10; trials++)
        {
            // Time tracking variables.
            duration<double, std::milli> elapsed;
            auto start = high_resolution_clock::now(), finish = high_resolution_clock::now();

            rand1 = randomNum(current_digits);
            rand2 = randomNum(current_digits);

            start = high_resolution_clock::now();
            s1 = mult4(rand1.c_str(), rand2.c_str());
            finish = high_resolution_clock::now();
            elapsed = (finish - start);
            elapsed_s1 += elapsed.count();

            start = high_resolution_clock::now();
            s2 = mult3a(rand1.c_str(), rand2.c_str());
            finish = high_resolution_clock::now();
            elapsed = (finish - start);
            elapsed_s2 += elapsed.count();

            start = high_resolution_clock::now();
            s3 = mult3b(rand1.c_str(), rand2.c_str());
            finish = high_resolution_clock::now();
            elapsed = (finish - start);
            elapsed_s3 += elapsed.count();
        }

        cout << current_digits;
        cout << "\t\t" << elapsed_s1 / (double)10;
        cout << "\t\t" << elapsed_s2 / (double)10;
        cout << "\t\t" << elapsed_s3 / (double)10;
        cout << endl;

        myfile.open("CCPS616_Lab2_AndyLee.txt", ios::app);
        myfile << current_digits;
        myfile << "\t\t" << elapsed_s1 / (double)10;
        myfile << "\t\t" << elapsed_s2 / (double)10;
        myfile << "\t\t" << elapsed_s3 / (double)10;
        myfile << endl;
        myfile.close();
    }
    

    return 0;
}

/*
    Returns a n-digit number as string type.
*/
string randomNum(int n)
{
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 9);

    string result;
    stringstream ss;

    // Avoid starting with 0.
    int num = 0;
    do
    {
        num = dis(gen);
    } while (num == 0);

    ss << num;

    // Generate the rest of the numbers
    for (int i = 1; i < n; i++)
    {
        ss << dis(gen);
    }

    result = ss.str();
    // cout << "Random number: " << result << endl;
    return result;
}

/*
    karatsuba down to 21475.
*/
string mult3b(const char *x, const char *y)
{
    string original_x(x);
    string original_y(y);

    // cout << "mult3b() [x*y]:\t" << original_x << " * " << original_y << endl;

    if (original_x.size() <= 5 || original_y.size() <= 5)
    {
        // cout << "running bypass ..." << endl;
        int bypass_x, bypass_y;
        istringstream bypass_ss_x(original_x);
        istringstream bypass_ss_y(original_y);
        bypass_ss_x >> bypass_x;
        bypass_ss_y >> bypass_y;
        // cout << "\tbypass_x: " << bypass_x << "\tbypass_y: " << bypass_y << endl;

        if (bypass_x < 21475 || bypass_y < 21475)
        {
            // Int multiply
            int bypass_int_result = bypass_x * bypass_y;

            // Converting back to string;
            string bypass_string_result;
            stringstream bypass_ss_result;
            bypass_ss_result << bypass_int_result;
            bypass_string_result = bypass_ss_result.str();

            // cout << "\t[BYPASSED the results]: " << bypass_string_result << endl;
            return bypass_string_result;
        }
    }

    int halfway = ceil((min(original_x.size(), original_y.size()) / (double)2));

    // cout << "halfway: " << halfway << endl;

    string a = original_x.substr(0, original_x.size() - halfway);
    string b = original_x.substr(original_x.size() - halfway, original_x.size() - 1);
    string c = original_y.substr(0, original_y.size() - halfway);
    string d = original_y.substr(original_y.size() - halfway, original_y.size() - 1);

    // cout << "a: " << a << " b: " << b << endl;
    // cout << "c: " << c << " d: " << d << endl;

    // cout << "\tz2 = a * c: " << endl;
    string z2 = mult3b(a.c_str(), c.c_str());
    // cout << "\tz0 = b * d: " << endl;
    string z0 = mult3b(b.c_str(), d.c_str());

    string z1_0 = stringAddition(a.c_str(), b.c_str());
    string z1_1 = stringAddition(c.c_str(), d.c_str());
    // cout << "\tz1 = (a + b) * (c + d): " << endl;
    string z1 = mult3b(z1_0.c_str(), z1_1.c_str());
    z1 = stringSubtraction(z1.c_str(), z2.c_str());
    z1 = stringSubtraction(z1.c_str(), z0.c_str());
    // cout << "\tz1 = z1 - z2 - z0: " << z1 << endl;

    // Calculating zeros needed for z1 and z2.
    stringstream s2, s1;
    char lead1[halfway + 1];
    char lead2[halfway * 2 + 1];
    repeat_0(lead1, halfway + 1);
    repeat_0(lead2, halfway * 2 + 1);
    s1 << z1 << lead1;
    s2 << z2 << lead2;

    // Convert back to string
    z1 = s1.str();
    z2 = s2.str();

    // cout << "finals: \t z0: " << z0 << " z1: " << z1 << " z2: " << z2 << endl;

    string result = stringAddition(z0.c_str(), z1.c_str());
    result = stringAddition(result.c_str(), z2.c_str());
    // cout << "result: " << result << endl;
    // string z1 = mutl4(,  stringAddition(c.c_str(), d.c_str())) ;

    return result;
}

/*
    karatsuba down to n=1
*/
string mult3a(const char *x, const char *y)
{
    string original_x(x);
    string original_y(y);

    // cout << "mult3a() [x*y]:\t" << original_x << " * " << original_y << endl;

    if (original_x.size() <= 1 || original_y.size() <= 1)
    {
        // cout << "running bypass ..." << endl;
        int bypass_x, bypass_y;
        istringstream bypass_ss_x(original_x);
        istringstream bypass_ss_y(original_y);
        bypass_ss_x >> bypass_x;
        bypass_ss_y >> bypass_y;
        // cout << "\tbypass_x: " << bypass_x << "\tbypass_y: " << bypass_y << endl;

        // Int multiply
        int bypass_int_result = bypass_x * bypass_y;

        // Converting back to string;
        string bypass_string_result;
        stringstream bypass_ss_result;
        bypass_ss_result << bypass_int_result;
        bypass_string_result = bypass_ss_result.str();

        // cout << "\t[BYPASSED the results]: " << bypass_string_result << endl;
        return bypass_string_result;
    }

    int halfway = ceil((min(original_x.size(), original_y.size()) / (double)2));

    // cout << "halfway: " << halfway << endl;

    string a = original_x.substr(0, original_x.size() - halfway);
    string b = original_x.substr(original_x.size() - halfway, original_x.size() - 1);
    string c = original_y.substr(0, original_y.size() - halfway);
    string d = original_y.substr(original_y.size() - halfway, original_y.size() - 1);

    // cout << "a: " << a << " b: " << b << endl;
    // cout << "c: " << c << " d: " << d << endl;

    string z2 = mult3a(a.c_str(), c.c_str());
    string z0 = mult3a(b.c_str(), d.c_str());

    string z1_0 = stringAddition(a.c_str(), b.c_str());
    string z1_1 = stringAddition(c.c_str(), d.c_str());
    string z1 = mult3a(z1_0.c_str(), z1_1.c_str());
    z1 = stringSubtraction(z1.c_str(), z2.c_str());
    z1 = stringSubtraction(z1.c_str(), z0.c_str());

    // Calculating zeros needed for z1 and z2.
    stringstream s2, s1;
    char lead1[halfway + 1];
    char lead2[halfway * 2 + 1];
    repeat_0(lead1, halfway + 1);
    repeat_0(lead2, halfway * 2 + 1);
    s1 << z1 << lead1;
    s2 << z2 << lead2;

    // Convert back to string
    z1 = s1.str();
    z2 = s2.str();

    // cout << "finals: \t z0: " << z0 << " z1: " << z1 << " z2: " << z2 << endl;

    string result = stringAddition(z0.c_str(), z1.c_str());
    result = stringAddition(result.c_str(), z2.c_str());
    // cout << "result: " << result << endl;
    // string z1 = mutl4(,  stringAddition(c.c_str(), d.c_str())) ;

    return result;
}

/*
    bit by bit multiplication
*/
string mult4(const char *x, const char *y)
{

    string original_x(x);
    string original_y(y);
    string reverse_x(x);
    string reverse_y(y);

    reverse(reverse_x.begin(), reverse_x.end());
    reverse(reverse_y.begin(), reverse_y.end());

    // cout << "mult4() [x*y]:\t" << original_x << " * " << original_y << endl;
    // cout << "mult4(): " << reverse_x << " reverse: " << reverse_y << endl;

    // Assign pointers to work on digit by digit
    const char *reverse_ptr_x = reverse_x.c_str();
    const char *reverse_ptr_y = reverse_y.c_str();

    int tmp_multi = 1, carry_over = 0;
    string out_string, result;
    stringstream ss;

    result = "0";

    for (int x_idx = 0; x_idx < reverse_x.size(); x_idx++)
    {
        ss.str(std::string());
        if (x_idx != 0)
        {
            char test[x_idx + 1];
            repeat_0(test, x_idx + 1);
            ss << test;
            // cout << "getting zeros " << test << "]" << endl;
        }

        for (int y_idx = 0; y_idx < reverse_y.size(); y_idx++)
        {
            // Multiply two digit
            // if (reverse_x.size() > idx) tmp_add += (int)((int)*(r_a_ptr + idx)-(int)'0');
            // if (reverse_x.size() > idx) tmp_add += (int)((int)*(r_b_ptr + idx)-(int)'0');
            tmp_multi = 1;
            tmp_multi *= (int)((int)*(reverse_ptr_x + x_idx) - (int)'0');
            tmp_multi *= (int)((int)*(reverse_ptr_y + y_idx) - (int)'0');

            // Add and reset carry over.
            tmp_multi += carry_over;
            carry_over = 0;

            // cout << "[1] " << tmp_multi << endl;

            // If sum is 10 or larger. Subtract 10 and carry 1 digit over.
            if (tmp_multi >= 10)
            {
                carry_over = (tmp_multi - tmp_multi % 10) / 10;
                tmp_multi = tmp_multi - 10 * carry_over;
            }

            // cout << "[2] " << tmp_multi << "carry over: " << carry_over << endl;
            ss << tmp_multi;
            // cout << "[3] ss: " << ss << endl;
        }

        // Final check to carryover left over values.
        if (carry_over != 0)
        {
            ss << carry_over;
            carry_over = 0;
        }

        out_string = ss.str();
        reverse(out_string.begin(), out_string.end());
        // cout << "\tsubtotal... " << out_string << endl;

        result = stringAddition(result.c_str(), out_string.c_str());
    }

    // cout << "mult4() RESULT: " << result << endl;
    return result;
}

int to_int(char c) { return c - 48; }

/*
    reverse two string a and b
    do while a and b != "\0"
    if a or b = "\0" then turn on flag and skip
    do simple addition
*/
string stringAddition(const char *a, const char *b)
{

    string reverse_a(a);
    string reverse_b(b);
    // cout << "stringAddition(): " << reverse_a << " + " << reverse_b << " = ";
    reverse(reverse_a.begin(), reverse_a.end());
    reverse(reverse_b.begin(), reverse_b.end());

    // Assign pointers to work on digit by digit
    const char *r_a_ptr = reverse_a.c_str();
    const char *r_b_ptr = reverse_b.c_str();

    int maxsize = max(reverse_a.size(), reverse_b.size());

    // bool a_done = false, b_done = false;
    int tmp_add = 0, carry_over = 0;

    string out_string;
    stringstream ss;

    for (int idx = 0; idx < maxsize; idx++)
    {

        // resetting calculator
        tmp_add = 0;

        // Add two single digits along with carry over.
        if (reverse_a.size() > idx)
            tmp_add += (int)((int)*(r_a_ptr + idx) - (int)'0');
        if (reverse_b.size() > idx)
            tmp_add += (int)((int)*(r_b_ptr + idx) - (int)'0');

        // Add and reset carry over.
        tmp_add += carry_over;
        carry_over = 0;

        // If sum is 10 or larger. Subtract 10 and carry 1 digit over.
        if (tmp_add >= 10)
        {
            carry_over = (tmp_add - tmp_add % 10) / 10;
            tmp_add = tmp_add - 10;
        }
        // cout << "carry over: " << carry_over << endl;
        ss << tmp_add;
    }

    // Final check to carryover left over values.
    if (carry_over != 0)
    {
        ss << carry_over;
        carry_over = 0;
    }

    out_string = ss.str();
    reverse(out_string.begin(), out_string.end());
    // cout << out_string << endl;

    return out_string;
}

/*
    reverse two string a and b
    do while a and b != "\0"
    if a or b = "\0" then turn on flag and skip
    do simple addition
*/
string stringSubtraction(const char *a, const char *b)
{

    string reverse_a(a);
    string reverse_b(b);
    // cout << "stringSubtraction(): " << reverse_a << " - " << reverse_b << " = ";
    reverse(reverse_a.begin(), reverse_a.end());
    reverse(reverse_b.begin(), reverse_b.end());

    // Assign pointers to work on digit by digit
    const char *r_a_ptr = reverse_a.c_str();
    const char *r_b_ptr = reverse_b.c_str();

    int maxsize = max(reverse_a.size(), reverse_b.size());
    // if ((int)stoi(reverse_a) < (int)stoi(reverse_b))
    //     cout << "error reversed!" << endl;

    // bool a_done = false, b_done = false;
    int tmp_sub = 0, carry_over = 0;

    string out_string;
    stringstream ss;

    for (int idx = 0; idx < maxsize; idx++)
    {

        // resetting calculator
        tmp_sub = 0;

        // Add two single digits along with carry over.
        if (reverse_a.size() > idx)
            tmp_sub += (int)((int)*(r_a_ptr + idx) - (int)'0');
        if (reverse_b.size() > idx)
            tmp_sub += (-1) * (int)((int)*(r_b_ptr + idx) - (int)'0');

        // Add and reset carry over.
        tmp_sub += carry_over;
        carry_over = 0;

        // If sum is 10 or larger. Subtract 10 and carry 1 digit over.
        if (tmp_sub < 0)
        {
            carry_over = -1;
            tmp_sub = tmp_sub + 10;
        }
        // cout << "carry over: " << carry_over << endl;
        ss << tmp_sub;
    }

    // Final check to carryover left over values.
    if (carry_over != 0)
    {
        ss << carry_over;
        carry_over = 0;
    }

    out_string = ss.str();
    reverse(out_string.begin(), out_string.end());

    // Remove leading zeros.
    out_string.erase(0, min(out_string.find_first_not_of('0'), out_string.size() - 1));

    // cout << out_string << endl;

    return out_string;
}

/* 
    needs to be delete after done with it.
*/
char *repeats(int n)
{
    char *x = new char('0');
    return x;
}

void repeat_0(char *zeros, int size)
{
    /*
        returns zeros used to adding leading zeros at the end.
    */
    if (size <= 1)
        *zeros = '\0';
    for (int i = 0; i < size - 1; i++)
    {
        // *(zeros+i) = '0';
        zeros[i] = '0';
    }
    zeros[size - 1] = '\0';
}