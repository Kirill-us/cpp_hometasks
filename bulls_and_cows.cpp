#include "std_lib_facilities.h"
#include <string>
#include <iostream>

int count(const vector<char>& nums, char num)
{
    int k{};

    for (size_t i = 0; i < nums.size(); ++i)
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

pair<int, int> count_bac(const vector<char>& number, const vector<char>& guess)
{
    int bulls = 0;
    int cows = 0;

    for (size_t i = 0; i < number.size(); ++i)
    {
        if (number[i] == guess[i])
            ++bulls;
        else if (count(number, guess[i]) == 1)
            ++cows;
    }

    return {bulls, cows};
}

vector<vector<char>> generate_numbers()
{
    vector<vector<char>> numbers;

    for (int i = 0; i < 10000; ++i)
    {
        string s;
        s = to_string(i);

        if (s.size() != 4)
            continue;

        vector<char> num(s.begin(), s.end());

        if (is_valid(num))
            numbers.push_back(num);
    }

    return numbers;
}


pair<int, int> user_feedback()
{
    int bulls, cows;

    while (true) {
        cout << "How many bulls?\n";
        cin >> bulls;

        cout << "How many cows?\n";
        cin >> cows;

        if (!cin)
            error("invalid input");
        
        if (bulls >= 0 && cows >= 0 && bulls <= 4 && cows <= 4 && bulls+cows <= 4)
            return {bulls, cows};

        cout << "Invalid result. Try again.\n";
    }
}

void computer_guess()
{
    vector<vector<char>> try_num = generate_numbers();
    int attempts = 0;

    while (!try_num.empty())
    {
        vector<char> guess = try_num[0];

        ++attempts;

        for (char c: guess) 
            cout << c;
        cout << endl;
        

        pair<int, int> result = user_feedback();
        int bulls = result.first;
        int cows = result.second;

        if (bulls == 4)
        {
            cout << "Computer guessed your number!\n";
            cout << "Attempts: " << attempts << endl;
            return;
        }

        vector<vector<char>> remaining;

        for (const auto& num : try_num)
        {
            pair<int, int> check = count_bac(num, guess);

            if (check.first == bulls && check.second == cows)
                remaining.push_back(num);
        }

        try_num = remaining;

        if (try_num.empty())
        {
            cout << "Your answers are inconsistent!\n";
            return;
        }

    }
}


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

    cout << "Think of a 4-digit number with unique digits.\n";
    cout << "Computer will try to guess it!\n\n";

    computer_guess();
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
