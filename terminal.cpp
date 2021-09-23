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
#include <string.h>

/* Imports */
#include "include/color.hpp"

/* My Headers */
using namespace std;
using namespace std::filesystem;

void t_cd(char **args);
void t_help(char **args);
void t_exit(char **args);

char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

void (*builtin_func[]) (char **) = {
  &t_cd,
  &t_help,
  &t_exit
};

void t_cd(char** args)
{
    chdir(args[1]); 
}

void t_help(char** args) { 
    cout << "*****************" << endl; 
    cout << "List of builtins commands" << endl; 
     
    for (const auto &p : builtin_str) { 
        cout << p << endl; 
    }

    cout << "*****************" << endl; 
 } 

void t_exit(char** args) {
    exit(EXIT_SUCCESS);
} 

int blank(string str) {
    const char * cstr = str.c_str();
    
    for(int i = 0; i < str.length(); i++) {
        if (cstr[i] != ' ')
            return 0;
    }
    return 1;
}

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


void execute(const vector<string> raw_text, char** args) 
{
    for (int i = 0; i < 3; i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

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

        execute(raw_text, args);
    }
    return 0;
}