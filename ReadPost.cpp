#include "ReadPost.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

ReadPosts::ReadPosts(const char *filename)
{
    postfile.open(filename);
    if (!postfile)
    {
        cerr << "Failed to open " << filename << endl;
        exit(1);
    }

    srand(static_cast<unsigned>(time(NULL)));
}

void ReadPosts::close()
{
    if (postfile.is_open())
    {
        postfile.close();
    }
}

void ReadPosts::readAllPosts()
{
    string line;

    while (getline(postfile, line))
    {
        if (line.length() == 0)
            continue;

        size_t t = line.find('\t');
        if (t == string::npos)
            continue;

        string userID = line.substr(0, t);
        string rest = line.substr(t + 1);

        posts.insert(pair<string, string>(userID, rest));
    }
}

string ReadPosts::findPost()
{
    if (posts.size() == 0)
        return "";

    int n = rand() % posts.size();

    multimap<string, string>::iterator it = posts.begin();
    for (int i = 0; i < n; i++)
        it++;

    return it->first + "\t" + it->second;
}

string ReadPosts::getPost(const string &userID) const
{
    multimap<string, string>::const_iterator it = posts.find(userID);

    if (it == posts.end())
        return "";

    return it->first + "\t" + it->second;
}

const multimap<string, string> &ReadPosts::getPosts() const
{
    return posts;
}
