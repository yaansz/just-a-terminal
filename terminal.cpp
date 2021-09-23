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

#define DEBUG

class BuiltInCommands {

    public:

    BuiltInCommands();
    int cd(char **args);
    int help(char **args);
    int exit(char **args);
    int execute(string cmd, char **args);

    private:
        
    map<string, function<int(char **)>> commands;
};

BuiltInCommands::BuiltInCommands() {
    this->commands.insert(pair<string, function<int(char **)>>("cd", bind (&BuiltInCommands::cd, this, std::placeholders::_1)));
    this->commands.insert(pair<string, function<int(char **)>>("help", bind (&BuiltInCommands::help, this, std::placeholders::_1)));
    this->commands.insert(pair<string, function<int(char **)>>("exit", bind (&BuiltInCommands::exit, this, std::placeholders::_1)));
}

int BuiltInCommands::cd(char** args) {
    chdir(args[1]);

    return 0; 
}

int BuiltInCommands::help(char** args) { 
    cout << "*****************" << endl; 
    cout << "List of builtins commands" << endl; 
     
     for (const auto &p : this->commands) { 
        cout << p.first << endl;
    }

    cout << "*****************" << endl; 

    return 0;
 } 

int BuiltInCommands::exit(char** args) {
    #ifdef DEBUG
    cout << "Returning -1" << endl;
    #endif
    return -1;
} 

int BuiltInCommands::execute(const string cmd, char** args) 
{
    if(this->commands.find(cmd) != this->commands.end()) {
        #ifdef DEBUG
        cout << "Executing command: " << cmd << endl;
        #endif
        return this->commands.at(cmd)(args);
    }

    return 1;
}

int execute(BuiltInCommands bic, const string cmd, char** args) 
{   

    int bic_return = bic.execute(cmd, args);
    // found in built in
    if(bic_return == 0) return 0;
    else if(bic_return == -1) return bic_return;

    cout << "bic_return: " << bic_return << endl;

    pid_t pid = fork();
    
    if(pid == -1)
        cout << "Error to fork" << endl;

    // Running child proccess
    else if(pid == 0) {
        if(execvp(cmd.c_str(), args) < 0) {
            cout << "Error to exec the command" << endl;
        }
        // Killing it 🤬😡😡😠
        exit(0);
    // Wait until the child process is killed
    } else {
        wait(NULL);
    }

    return 0;
}

/* Helpful */
int blank(string str) {
    const char * cstr = str.c_str();
    
    for(int i = 0; i < str.length(); i++) {
        if (cstr[i] != ' ')
            return 0;
    }
    return 1;
}

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

    BuiltInCommands bic;
    int exec_return;

    while(1) {
        string command;

        // Input
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

        exec_return = execute(bic, raw_text[0], args);

        if(exec_return == -1) break;
    }
    return 0;
}