//
// This is example of starting code from Chapter 7.8 "Variables" of
// "Software - Principles and Practice using C++" by Bjarne Stroustrup
//

#include "std_lib_facilities.h"

struct Token
{
  char kind;
  int value;
  string name;

  Token(char ch) : kind{ch}, value{0} {}

  Token(char ch, int val) : kind{ch}, value{val} {}

  Token(char ch, string id) : kind{ch}, name{id} {}
};

class Token_stream
{
  bool full{false};
  Token buffer{'\0'};

public:
  Token_stream() {}

  Token get ();
  void putback (Token t);
  void ignore (char);
};

void Token_stream::putback(Token t)
{
  if (full)
    error("putback() into a full buffer");

  buffer = t;
  full = true;
}

constexpr char quit = 'q';
constexpr char print = ';';
constexpr char number = '8';
constexpr char name = 'a';
constexpr char let = 'L';
constexpr char and_and = 'A';
constexpr char or_or = 'O';

const string prompt = "> ";
const string result = "= ";
const string declkey = "let";
const string quitkey = "quit";

Token Token_stream::get()
{
  if (full)
  {
    full = false;
    return buffer;
  }

  char ch;
  cin >> ch;

  switch (ch)
  {
  case '(':
  case ')':
  case '+':
  case '-':
  case '*':
  case '/':
  case '%':
  case print:
  case '=':
  case '!':
  case '~':
  case '^':
    return Token{ch};
  case '&':
  {
    char ch2;
    if (cin.get(ch2) && ch2 == '&')
        return Token{and_and};
    cin.putback(ch2);
    return Token{'&'};
}
  case '|':
  {
    char ch2;
    if (cin.get(ch2) && ch2 == '|')
        return Token{or_or};
    cin.putback(ch2);
    return Token{'|'};
}

  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  {
    cin.putback(ch);
    int val;
    cin >> val;
    return Token{number, val};
  }

  default:
    if (isalpha(ch))
    {
      string s;
      s += ch;
      while (cin.get(ch) && (isalpha(ch) || isdigit(ch)))
        s += ch;
      cin.putback(ch);

      if (s == declkey)
        return Token{let};

      if (s == quitkey)
        return Token{quit};

      return Token{name, s};
    }
    error("bad token");
  }
}

void Token_stream::ignore(char c)
{
  if (full && c == buffer.kind)
  {
    full = false;
    return;
  }
  full = false;

  for (char ch; cin >> ch;)
  {
    if (ch == c)
      return;
  }
}

struct Variable
{
  string name;
  int value;

  Variable(string n, int v) : name{n}, value{v} {}
};

vector<Variable> var_table;

int get_value (const string& s)
{
  for (const auto& var : var_table)
  {
    if (var.name == s)
      return var.value;
  }

  error("get: undefined name ", s);
}

void set_value (const string& s, int d)
{
  for (auto& var : var_table)
  {
    if (var.name == s)
    {
      var.value = d;
      return;
    }
  }

  error("set: undefined name ", s);
}

bool is_declared (const string& s)
{
  for (const auto& var : var_table)
  {
    if (var.name == s)
      return true;
  }

  return false;
}

int define_name (const string& var, int val)
{
  if (is_declared(var))
    error(var, " declared twice");

  var_table.push_back(Variable{var, val});

  return val;
}

Token_stream ts;

int expression(); // ||
int and_expr(); // &&
int bitor_expr(); // |
int bitxor_expr(); // ^
int bitand_expr(); // &
int add_expr(); // +-
int term(); // *
int primary(); // унарные () числа

int primary ()
{
  Token t = ts.get();
  switch (t.kind)
  {
  case '(':
  {
    int d = expression();
    t = ts.get();
    if (t.kind != ')')
      error("'(' expected");
    return d;
  }

  case '-':
    return -primary();
  case '+':
    return +primary();
  case '!': 
    return !primary();
  case '~':
    return ~primary();

  case number:
    return t.value;

  case name:
    return get_value(t.name);

  default:
    error("primary expected");
  }
}

int term ()
{
  int left = primary();

  while (true)
  {
    Token t = ts.get();

    switch (t.kind)
    {
    case '*':
      left *= primary();
      break;

    case '/':
    {
      int d = primary();
      if (d == 0)
        error("divide by zero");
      left /= d;
      break;
    }

    case '%':
    {
      int d = primary();
      if (d == 0)
        error("'%': divide by zero");
      left %= d;
      break;
    }

    default:
      ts.putback(t);
      return left;
    }
  }
}

int add_expr()
{
    int left = term();
    while (true)
    {
        Token t = ts.get();
        switch (t.kind)
        {
        case '+':
            left += term();
            break;
        case '-':
            left -= term();
            break;
        default:
            ts.putback(t);
            return left;
        }
    }
}

int bitand_expr()
{
    int left = add_expr();
    while (true)
    {
        Token t = ts.get();
        if (t.kind == '&')
            left &= add_expr();
        else
        {
            ts.putback(t);
            return left;
        }
    }
}

int bitxor_expr()
{
    int left = bitand_expr();
    while (true)
    {
        Token t = ts.get();
        if (t.kind == '^')
            left ^= bitand_expr();
        else
        {
            ts.putback(t);
            return left;
        }
    }
}

int bitor_expr()
{
    int left = bitxor_expr();
    while (true)
    {
        Token t = ts.get();
        if (t.kind == '|')
            left |= bitxor_expr();
        else
        {
            ts.putback(t);
            return left;
        }
    }
}

int and_expr()
{
    int left = bitor_expr();
    while (true)
    {
        Token t = ts.get();
        if (t.kind == and_and)
        {
            int right = bitor_expr();
            left = (left != 0 && right != 0) ? 1 : 0;
        }
        else
        {
            ts.putback(t);
            return left;
        }
    }
}

int expression ()
{
  int left = and_expr();

  while (true)
  {
    Token t = ts.get();
    if (t.kind == or_or)
    {
      int right = and_expr();
      left = (left != 0 || right != 0) ? 1 : 0;
    }
    else
    {
      ts.putback(t);
      return left;
    }
  }
}

int declaration ()
{
  Token t = ts.get();
  if (t.kind != name)
    error("name expected in declaration");

  string var = t.name;
  if (is_declared(var))
    error(var, " declared twice");

  t = ts.get();
  if (t.kind != '=')
    error("'=' missing in declaration of ", var);

  return define_name(var, expression());
}

int statement ()
{
  Token t = ts.get();
  switch (t.kind)
  {
  case let:
    return declaration();
  default:
    ts.putback(t);
    return expression();
  }
}

void clean_up_mess () { ts.ignore(print); }

void calculate ()
{
  while (true)
    try
    {
      cout << prompt;
      Token t = ts.get();
      while (t.kind == print)
        t = ts.get();
      if (t.kind == quit)
        return;

      ts.putback(t);
      cout << result << statement() << endl;
    }
    catch (runtime_error& e)
    {
      cerr << e.what() << endl;
      clean_up_mess();
    }
}

int main ()
try
{
  calculate();
}
catch (exception& e)
{
  cerr << "exception: " << e.what() << endl;
  return 1;
}
catch (...)
{
  cerr << "Oops, unknown exception" << endl;
  return 2;
}
