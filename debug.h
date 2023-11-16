#include <queue>
using namespace std;

template <typename T, typename S>
ostream &operator<<(ostream &os, const pair<T, S> &p)
{
    return os << "(" << p.first << ", " << p.second << ")";
}

template <typename T>
ostream &operator<<(ostream &os, priority_queue<T> c)
{
    bool f = true;
    os << "[";
    while (!c.empty())
    {
        if (!f)
            os << ", ";
        f = false;
        os << c.top();
        c.pop();
    }
    return os << "]";
}

template <typename T>
ostream &operator<<(ostream &os, queue<T> c)
{
    bool f = true;
    os << "[";
    while (!c.empty())
    {
        if (!f)
            os << ", ";
        f = false;
        os << c.front();
        c.pop();
    }
    return os << "]";
}

template <typename C, typename T = decay<decltype(*begin(declval<C>()))>, typename enable_if<!is_same<C, string>::value>::type * = nullptr>
ostream &operator<<(ostream &os, const C &c)
{
    bool f = true;
    os << "[";
    for (const auto &x : c)
    {
        if (!f)
            os << ", ";
        f = false;
        os << x;
    }
    return os << "]";
}

template <typename T>
void debug(string s, T x) { cerr << "\033[1;35m" << s << "\033[0;32m = \033[33m" << x << "\033[0m\n"; }

template <typename T, typename... Args>
void debug(string s, T x, Args... args)
{
    for (int i = 0, b = 0; i < (int)s.size(); i++) {
        if (s[i] == '(' || s[i] == '{')
            b++;
        else if (s[i] == ')' || s[i] == '}')
            b--;
        else if (s[i] == ',' && b == 0)
        {
            cerr << "\033[1;35m" << s.substr(0, i) << "\033[0;32m = \033[33m" << x << "\033[31m | ";
            debug(s.substr(s.find_first_not_of(' ', i + 1)), args...);
            break;
        }
    }
}