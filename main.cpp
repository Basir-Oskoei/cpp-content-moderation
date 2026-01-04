#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cstdlib>

#include "User.h"
#include "Manager.h"
#include "ReadPost.h"

using namespace std;

// makes the charectors in a string to lowercase
void lowerString(string &s)
{
    for (int i = 0; i < (int)s.length(); i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] - 'A' + 'a';
    }
}

// remoives punctuation 
string stripEnds(const string &w)
{
    int a = 0;
    int b = (int)w.length() - 1;

    while (a <= b)
    {
        char c = w[a];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
            break;
        a++;
    }

    while (b >= a)
    {
        char c = w[b];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
            break;
        b--;
    }

    if (a > b)
        return "";

    return w.substr(a, b - a + 1);
}

// counts the letters
int letterCount(const string &w)
{
    int c = 0;
    for (int i = 0; i < (int)w.length(); i++)
    {
        if ((w[i] >= 'A' && w[i] <= 'Z') || (w[i] >= 'a' && w[i] <= 'z'))
            c++;
    }
    return c;
}

string censorToken(const string &token)
{
    string result = token;

    int a = 0;
    int b = (int)token.length() - 1;

    while (a <= b)
    {
        char c = token[a];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
            break;
        a++;
    }

    while (b >= a)
    {
        char c = token[b];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
            break;
        b--;
    }

    for (int i = a; i <= b; i++)
        result[i] = 'X';

    return result;
}

// for cencoring the blocked words
string moderateText(const string &text, const vector<string> &blocked, bool &moderated)
{
    moderated = false;

    stringstream ss(text);
    string token;
    string out = "";

    while (ss >> token)
    {
        string w = stripEnds(token);

        bool bad = false;
        if (w != "" && letterCount(w) > 1)
        {
            lowerString(w);
            for (int i = 0; i < (int)blocked.size(); i++)
            {
                if (w == blocked[i])
                    bad = true;
            }
        }

        if (bad)
        {
            moderated = true;
            out += censorToken(token);
        }
        else
        {
            out += token;
        }

        out += " ";
    }

    if (moderated)
        out += "#moderatedpost";

    return out;
}

// creats a user object
bool parseLine(const string &line, User &u)
{
    stringstream ss(line);

    int regNo;
    string first, last;
    float accum, avg;

    if (ss >> regNo >> first >> last >> accum >> avg)
    {
        string name = first + " " + last;
        u = User(name, regNo);
        u.setScores(accum, avg);
        return true;
    }
    return false;
}

// does the same as above just creats a manager object
bool parseLine(const string &line, Manager &m)
{
    stringstream ss(line);

    int regNo;
    string first, last;

    if (ss >> regNo >> first >> last)
    {
        float test;
        if (!(ss >> test))
        {
            string name = first + " " + last;
            m = Manager(name, regNo);
            return true;
        }
    }
    return false;
}


// Loads users or managers from the accounts file
template <class T>
void login(const char *filename, vector<T> &accounts)
{
    ifstream in(filename);
    if (!in)
    {
        cout << "Error: cannot open accounts file" << endl;
        exit(1);
    }

    string line;
    while (getline(in, line))
    {
        if (line.length() == 0)
            continue;

        T obj("", 0);
        if (parseLine(line, obj))
            accounts.push_back(obj);
    }

    in.close();
}

int findUserByName(const vector<User> &users, const string &name)
{
    for (int i = 0; i < (int)users.size(); i++)
    {
        if (users[i].getName() == name)
            return i;
    }
    return -1;
}

int findManagerByName(const vector<Manager> &managers, const string &name)
{
    for (int i = 0; i < (int)managers.size(); i++)
    {
        if (managers[i].getName() == name)
            return i;
    }
    return -1;
}

int findUserByReg(const vector<User> &users, int regNo)
{
    for (int i = 0; i < (int)users.size(); i++)
    {
        if (users[i].getRegNo() == regNo)
            return i;
    }
    return -1;
}

// this shows the manager menu and deals with the words that have been blocked
void managerMenu(vector<string> &blocked)
{
    int choice = 0;

    while (choice != 3)
    {
        cout << endl;
        cout << "1. View Blocked Content" << endl;
        cout << "2. Add Blocked Content" << endl;
        cout << "3. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
        {
            if (blocked.size() == 0)
                cout << "No blocked words" << endl;
            else
            {
                for (int i = 0; i < (int)blocked.size(); i++)
                    cout << blocked[i] << endl;
            }
        }
        else if (choice == 2)
        {
            string w;
            cout << "Enter word: ";
            cin >> w;
            lowerString(w);
            blocked.push_back(w);
        }
    }
}

// saves a post to the database
void appendPost(int regNo, const string &postID, const string &text, const string &dt)
{
    ofstream out("Post_database.txt", ios::app);
    if (!out)
    {
        cout << "Error: cannot open posts file" << endl;
        return;
    }

    out << regNo << "\t" << postID << "\t" << text << "\t" << dt << endl;
    out.close();
}

// counts the posts that will be modirated
int moderatedCountForUser(ReadPosts &rp, int regNo, const vector<string> &blocked)
{
    stringstream ss;
    ss << regNo;
    string key = ss.str();

    const multimap<string, string> &posts = rp.getPosts();
    pair<multimap<string, string>::const_iterator, multimap<string, string>::const_iterator> range;
    range = posts.equal_range(key);

    int c = 0;
    for (multimap<string, string>::const_iterator it = range.first; it != range.second; it++)
    {
        bool mod;
        moderateText(it->second, blocked, mod);
        if (mod)
            c++;
    }

    return c;
}

// shows the top 10 users with the worst reputaton 
void showBottom10(vector<User> usersCopy)
{
    sort(usersCopy.begin(), usersCopy.end());

    int limit = usersCopy.size();
    if (limit > 10)
        limit = 10;

    for (int i = 0; i < limit; i++)
    {
        cout << usersCopy[i].getName() << "  ";
        cout << usersCopy[i].getAccumulativeScore() << "  ";
        cout << usersCopy[i].getModeratedPosts() << endl;
    }
}

// Shows the user menu and handles all user actions
void userMenu(vector<User> &users, int userIndex, vector<string> &blocked)
{
    ReadPosts rp("Post_database.txt");
    rp.readAllPosts();

    int choice = 0;

    while (choice != 4)
    {
        cout << endl;
        cout << "1. Read/Write Post" << endl;
        cout << "2. View Your Statistics" << endl;
        cout << "3. Display Top 10 Most Problematic Users" << endl;
        cout << "4. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
        {
            cout << "1. Read a post" << endl;
            cout << "2. Write a post" << endl;
            cout << "Choice: ";
            int c2;
            cin >> c2;

            if (c2 == 1)
            {
                int done = 0;
                while (!done)
                {
                    string full = rp.findPost();
                    if (full == "")
                    {
                        cout << "No posts available" << endl;
                        done = 1;
                    }
                    else
                    {
                        size_t t = full.find('\t');
                        string author = full.substr(0, t);
                        string rest = full.substr(t + 1);

                        bool mod;
                        cout << author << "  " << moderateText(rest, blocked, mod) << endl;

                        cout << "1. Read another post" << endl;
                        cout << "2. Report post" << endl;
                        cout << "3. Exit" << endl;
                        cout << "Choice: ";
                        int c3;
                        cin >> c3;

                        if (c3 == 1)
                            done = 0;
                        else if (c3 == 2)
                        {
                            int authorReg = atoi(author.c_str());
                            int ai = findUserByReg(users, authorReg);

                            if (ai != -1)
                            {
                                users[ai].addScore(0.0f, rest);
                                try
                                {
                                    users[ai].updateAccumulativeScore(-1.0f);
                                }
                                catch (NoScoreException &e)
                                {
                                }
                            }

                            cout << "Reported" << endl;
                        }
                        else
                            done = 1;
                    }
                }
            }
            else if (c2 == 2)
            {
                cout << "Write post (one line): " << endl;
                cin.ignore();
                string content;
                getline(cin, content);

                if (content.length() > 140)
                    content = content.substr(0, 140);

                bool mod;
                string moderatedText = moderateText(content, blocked, mod);

                string postID = "0";
                string dt = "0";

                appendPost(users[userIndex].getRegNo(), postID, moderatedText, dt);

                if (mod)
                    users[userIndex].setModeratedPosts(users[userIndex].getModeratedPosts() + 1);

                cout << "Posted" << endl;
            }
        }
        else if (choice == 2)
        {
            cout << users[userIndex] << endl;
        }
        else if (choice == 3)
        {
            for (int i = 0; i < (int)users.size(); i++)
                users[i].setModeratedPosts(moderatedCountForUser(rp, users[i].getRegNo(), blocked));

            showBottom10(users);
        }
    }

    rp.close();
}

// helps to start the program and handles the user or the manager log ins
int main()
{
    vector<User> users;
    vector<Manager> managers;
    vector<string> blocked;

    login<User>("users.txt", users);
    login<Manager>("users.txt", managers);

    cout << "Enter name: ";
    string first, last;
    cin >> first >> last;
    string name = first + " " + last;

    int mi = findManagerByName(managers, name);
    if (mi != -1)
    {
        cout << "Manager logged in: " << name << endl;
        managerMenu(blocked);
        return 0;
    }

    int ui = findUserByName(users, name);
    if (ui == -1)
    {
        try
        {
            throw runtime_error("User not found");
        }
        catch (runtime_error &e)
        {
            cout << e.what() << endl;
            cout << "Creating new user in memory" << endl;
        }

        int regNo;
        cout << "Enter registration number: ";
        cin >> regNo;

        users.push_back(User(name, regNo));
        ui = (int)users.size() - 1;
    }

    cout << "User logged in: " << name << endl;
    userMenu(users, ui, blocked);

    return 0;
}
