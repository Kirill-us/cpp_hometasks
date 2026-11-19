#include "std_lib_facilities.h"
#include <random>

int count(const vector<char>& nums, char num)
{
    int k{};

    for (int i = 0; i < nums.size(); ++i)
        if (nums[i] == num)
            ++k;
    return k;
}

bool is_valid(const vector<char>& number)
{
    for (size_t i = 0; i < number.size(); ++i)
    {
        if (number[i] < '0' || '9' < number[i])
            return false;
        if (count(number, number[i]) != 1)
            return false;
    }
    return true;
}


vector<char> user_guess (const string& prompt) {
    vector<char> num(4);

    while (true)
    {
        cout << prompt << "\n";
        string s;
        cin >> s;
        
        if (!cin)
            error("invalid input");
        
        if (s.size() != 4)
        {
            cout << "enter exactly 4 digits\n";
            continue;
        }

        vector<char> num(s.begin(), s.end());

        if (is_valid(num))
            return num;

        cout << "enter a normal number\n";
    }
}

// vector<char> random_num()
// {
//     random_device rd;
//     mt19937 gen(rd());
//     uniform_int_distribution<char> distrib('0', '9');

//     vector<char> num;

//     while (num.size() < 4)
//     {
//         char d = distrib(gen);
//         bool dup = false;

//         for (int i = 0; i < num.size(); ++i)
//             if (num[i] == d)
//                 dup = true;

//         if (!dup)
//             num.push_back(d);
//     }

//     return num;
// }


int main()
try
{
  cout << "<<Bulls and Cows>>\n"
       << "Computer sets a number of 4 unique digits.\n"
       << "Try to guess it.\n"
       << "<Bull> means right digit in the right position.\n"
       << "<Cow> means right digit in the wrong position.\n"
       << "\n"
       << "game is on" << "\n"
       << "\n";

    vector<char> guessing_num = user_guess("enter the guessing number:");

    // код чтобы пользователь сам вводил загаданное число

    // vector<char> guessing_num = random_num();

    int bulls{0};

    do
    {
        bulls = 0;
        int cows{0};
        vector<char> try_guess = user_guess("guess the number");

        for (int i = 0; i < try_guess.size(); ++i) {
            if (guessing_num[i] == try_guess[i])
                ++bulls;
            else if (count(guessing_num, try_guess[i]) == 1)
                ++cows;
        }
        cout << bulls << " bull(s) and " << cows << " cow(s)" << endl;
    }
    while (bulls != 4);

    cout << "game is over" << endl;
}
catch (exception& e)
{
  cerr << e.what() << endl;
  return 1;
}
catch (...)
{
  cerr << "Oops, something went wrong..." << endl;
  return 2;
}
