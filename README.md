# cpp-content-moderation

A C++ content moderation system with user roles, reputation tracking, and file-based persistence.

## Features
- User and manager roles
- Text moderation using blocked-word filtering
- Reputation and moderation tracking per user
- File-based storage using standard C++ streams

## Build and Run
```bash
g++ -std=c++11 -Wall main.cpp Person.cpp Manager.cpp User.cpp ReadPost.cpp -o main
./main
