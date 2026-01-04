#ifndef READPOSTS_H
#define READPOSTS_H

#include <string>
#include <fstream>
#include <map>
#include <stdexcept>
#include <cstdlib>

using namespace std;

class ReadPosts
{
public:
    ReadPosts(const char *filename);

    void close();

    void readAllPosts();

    string findPost();

    string getPost(const string &userID) const;

    const multimap<string, string> &getPosts() const;

private:
    ifstream postfile;
    multimap<string, string> posts;
};

#endif
