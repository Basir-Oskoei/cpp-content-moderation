#include "User.h"
#include <iostream>

// this creats a user with reg no and inital scores
User::User(const string &name, int regNo) : Person(name)
{
    this->name = name;
    this->regNo = regNo;
    accumulative_reputation_score = 0.0f;
    average_reputation_score = 0.0f;
    moderated_posts = 0;
}

int User::getRegNo() const
{
    return regNo;
}

void User::addScore(float score, string post)
{
    reputation_scores.insert(pair<string, float>(post, score));
}

// updates users scores
bool User::updateAccumulativeScore(float score)
{
    if (reputation_scores.size() == 0)
        throw NoScoreException();

    accumulative_reputation_score += score;
    average_reputation_score = accumulative_reputation_score / reputation_scores.size();
    return true;
}

void User::setScores(float accum, float avg)
{
    accumulative_reputation_score = accum;
    average_reputation_score = avg;
}

float User::getAccumulativeScore() const
{
    return accumulative_reputation_score;
}

float User::getAverageScore() const
{
    return average_reputation_score;
}

void User::setModeratedPosts(int n)
{
    moderated_posts = n;
}

int User::getModeratedPosts() const
{
    return moderated_posts;
}

// outputs the user details
bool User::operator<(const User &other) const
{
    if (accumulative_reputation_score < other.accumulative_reputation_score)
        return true;

    if (accumulative_reputation_score > other.accumulative_reputation_score)
        return false;

    if (accumulative_reputation_score == 0.0f && other.accumulative_reputation_score == 0.0f)
        return moderated_posts > other.moderated_posts;

    return false;
}

ostream& operator<<(ostream &str, const User &s)
{
    str << "Name: " << s.getName() << endl;
    str << "RegNo: " << s.regNo << endl;

    if (s.reputation_scores.size() == 0)
    {
        str << "has no scores" << endl;
    }
    else
    {
        multimap<string, float>::const_iterator it;
        for (it = s.reputation_scores.begin(); it != s.reputation_scores.end(); it++)
        {
            str << it->first << " : " << it->second << endl;
        }
    }

    str << "Accumulative: " << s.accumulative_reputation_score << endl;
    str << "Average: " << s.average_reputation_score << endl;
    str << "Moderated posts: " << s.moderated_posts << endl;
    return str;
}
