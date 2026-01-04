// include your own comments

#ifndef _USER_H_
#define _USER_H_

#include <string>
#include <map>
#include <stdexcept>
#include "Person.h"

using namespace std;

class NoScoreException: public exception
{
};

class User: public Person
{   
public:
    // constructor should initialise name and registration number using arguments
    // and initialise score map to be empty
    User(const string &name, int regNo);

    // method to return registration number
    int getRegNo() const;

    // method to add the score to the map
    void addScore(float score,string post);

    // method to update the accumulative score map
    // should throw NoScoreException if user has no tweet yet
    bool updateAccumulativeScore(float score);

    // method to set stored scores (used when loading from file)
    void setScores(float accum, float avg);

    float getAccumulativeScore() const;
    float getAverageScore() const;

    void setModeratedPosts(int n);
    int getModeratedPosts() const;

    bool operator<(const User &other) const;

private:
    int regNo;
    string name;
    multimap<string,float> reputation_scores;  // scores map, with each in range 0.0 to 100.0, and a tweet
    float accumulative_reputation_score;
    float average_reputation_score;
    int moderated_posts;

    // friend function to output details of user to stream
    // output to be defined and formated by you
    // if the user has no scores "has no scores" should be output instead of the scores
    friend ostream& operator<<(ostream &str, const User &s);
};

#endif
