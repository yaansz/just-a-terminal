/* Include */
#include <cstdlib>
#include <iostream>
#include <random>
#include <functional>
#include <map>
#include <string>
#include <filesystem>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Imports */
#include "include/color.hpp"

/* My Headers */
using namespace std;
using namespace std::filesystem;

/* Helpful */
vector<string> split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

int blank(string str) {
    const char * cstr = str.c_str();
    
    for(int i = 0; i < str.length(); i++) {
        if (cstr[i] != ' ')
            return 0;
    }
    return 1;
}


/* Main Main Main */
int main(int argc, char **argv) {
    Color::Modifier red(Color::FG_RED);
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier def(Color::FG_DEFAULT);

    cout << red << "*********************" << def << endl;
    cout << "Terminal initialized" << endl;
    cout << "Welcome " << green << getenv("USER") << def << endl;
    cout << "Type 'help' to know more about this shell" << endl;
    cout << red << "*********************" << def << endl;

    while(1) {
        string command;

        // Input
        cout << endl;
        cout << green << getenv("USER") << def << ":";
        cout << red << std::filesystem::current_path() << def; 
        cout << " >>> ";

        getline(cin, command);
        
        if(command.empty() || blank(command)) continue;

        // Filter
        vector<string> raw_text = split(command, " ");
        
        vector<char*> pointerVec(raw_text.size());
        for(unsigned i = 0; i < raw_text.size(); ++i)
            pointerVec[i] = raw_text[i].data();
        char** args = pointerVec.data();

        // built in
        if(raw_text[0].compare("cd") == 0) {
                cout << " here " << endl;
                chdir(args[1]); 
                continue;    
        }

        // Exec
        pid_t pid = fork();
        
        if(pid == -1)
            cout << "Error to fork" << endl;
        else if(pid == 0) {
            if(execvp(raw_text[0].c_str(), args) < 0) {
                cout << "Error to exec the command" << endl;
            }
            exit(0);
        } else {
            wait(NULL);
        }
    }
    return 0;
}

