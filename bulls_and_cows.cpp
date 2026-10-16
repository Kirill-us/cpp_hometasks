#include <std_lib_facilities.h>
#include <random>

int count(const vector<char>& nums, char num)
{
    int k{};

    for (int i = 0; i < nums.size(); ++i)
        if (nums[i] == num)
            ++k;
    return k;
}


// void validate(const vector<char>& num) 
// {
//     for (int i = 0; i < num.size(); ++i) 
//     {
//       if (num[i] < '0' || '9' < num[i])
//         error("input is not a digit");
//       if (count(num, num[i]) != 1)
//         error("input are not unique");
//     }
// }

vector<char> user_guess () {
    vector<char> num(4);

    while (true)
    {
        cout << "guess the number\n";
        for (int i = 0; i < num.size(); ++i)
            cin >> num[i];
        
        if (!cin)
            error("invalid input");
        
        bool a = true;

        for (int i = 0; i < num.size(); ++i) 
        {
            if ((num[i] < '0' || num[i] > '9') || (count(num, num[i]) != 1)) {
                cout << "enter a normal number\n";
                a = false;
                break;
            }
        }

        if (a)
            return num;
    }
}

vector<char> random_num()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<char> distrib('0', '9');

    vector<char> num;

    while (num.size() < 4)
    {
        char d = distrib(gen);
        bool dup = false;

        for (int i = 0; i < num.size(); ++i)
            if (num[i] == d)
                dup = true;

        if (!dup)
            num.push_back(d);
    }

    return num;
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
       << "\n"
       << "enter the guessing number:" << endl;

    // vector<char> guessing_num(4);
    // for (int i=0; i < guessing_num.size(); ++i)
    //     cin >> guessing_num[i];
    // if (!cin)
    //     error("invalid input");
    
    // validate(guessing_num);

    // код чтобы пользователь сам вводил загаданное число

    vector<char> guessing_num = random_num();
    for (int i = 0; i < guessing_num.size(); ++i)
        cout << guessing_num[i];
    cout << endl;

    int bulls{0};

    do
    {
        bulls = 0;
        int cows{0};
        vector<char> try_guess = user_guess();

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
