/* Include */
#include <cstdlib>
#include <iostream>
#include <random>
#include <functional>
#include <map>
#include <string>
#include <filesystem>

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


class Shell {

    public:
        // Attrs
        map<string, function<int(string)>> commands;
        string username;

        // Methods
        // Constructor    
        Shell();
        int Execute(string command, string args);
        // Commands
        int help(string args);
        int hello(string args);
        int exit(string args);
        int clear(string args);
        int echo(string args);
        int native(string args);
        int cd(string args);
        int mkdir(string args);
        int rmdir(string args);
        int rnm(string args);

};

Shell::Shell() {
    // Init
    this->commands.insert(pair<string, function<int(string)>>("hello", bind (&Shell::hello, this, std::placeholders::_1)));
    this->commands.insert(pair<string, function<int(string)>>("exit", bind (&Shell::exit, this, std::placeholders::_1)));
    this->commands.insert(pair<string, function<int(string)>>("help", bind (&Shell::help, this, std::placeholders::_1)));
    this->commands.insert(pair<string, function<int(string)>>("clear", bind (&Shell::clear, this, std::placeholders::_1)));
    this->commands.insert(pair<string, function<int(string)>>("echo", bind (&Shell::echo, this, std::placeholders::_1)));
    this->commands.insert(pair<string, function<int(string)>>("nat", bind (&Shell::native, this, std::placeholders::_1)));
    this->commands.insert(pair<string, function<int(string)>>("cd", bind (&Shell::cd, this, std::placeholders::_1)));
    this->commands.insert(pair<string, function<int(string)>>("mkdir", bind (&Shell::mkdir, this, std::placeholders::_1)));
    this->commands.insert(pair<string, function<int(string)>>("rmdir", bind (&Shell::rmdir, this, std::placeholders::_1)));
    this->commands.insert(pair<string, function<int(string)>>("rnm", bind (&Shell::rnm, this, std::placeholders::_1)));

    this->username = getenv("USER");
}

int Shell::Execute(string command, string args) {
    
    if(this->commands.find(command) != this->commands.end()) {
        return this->commands.at(command)(args);          
    }
    return 1;
}

int Shell::hello(string args) {
    cout << "hello!" << endl;
    return 0;
}

int Shell::echo(string args) {
    cout << args << endl;
    return 0;
}

int Shell::exit(string args) {
    return -1;
}

int Shell::help(string args) {
    cout << "*****************" << endl;
    cout << "List of commands" << endl;
    
    for (const auto &p : this->commands) {
        cout << p.first << endl;
    }
    
    cout << "*****************" << endl;

    return 0;
}

int Shell::clear(string args) {
    printf("\033[H\033[J");
    return 0;
}

int Shell::native(string args) {
    return system(args.c_str());
}

int Shell::cd(string args) {

    cout << "WIP WIP WIP WIP" << endl;

    if(args.empty() || blank(args))
        cout << std::filesystem::current_path() << endl;

    return 0;
}

int Shell::mkdir(string args) { 
    bool success = filesystem::create_directory(filesystem::current_path().string() + "/" + args.c_str()); 
    
    if(!success) {
        cout << "Something wrong!" << endl;
    }
    return 0;
}

int Shell::rmdir(string args) {
    bool success = filesystem::remove(filesystem::current_path().string() + "/" + args.c_str());

    if(!success) {
        cout << "Something wrong!" << endl;
    }
    return 0;
}

int Shell::rnm(string args) {
    
    vector<string> raw = split(args, " ");

    if(raw.size() != 2) {
        cout << "Error: You need to enter the old and the nem name" << endl;
    }
    
    string oold = filesystem::current_path().string() + "/" + raw[0];
    string nnew = filesystem::current_path().string() + "/" + raw[1];

    filesystem::rename(oold, nnew);

    return 0;
}

/* Main Main Main */
int main(int argc, char **argv) {
    Shell commands;
    int fb;

    Color::Modifier red(Color::FG_RED);
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier def(Color::FG_DEFAULT);

    cout << red << "*********************" << def << endl;
    cout << "Terminal initialized" << endl;
    cout << "Welcome " << green << commands.username << def << endl;
    cout << "Type 'help' to know more about this shell" << endl;
    cout << red << "*********************" << def << endl;

    while(1) {
        string command;

        // Input
        cout << green << commands.username << def << ":";
        cout << red << std::filesystem::current_path() << def; 
        cout << " >>> ";

        getline(cin, command);
        

        if(command.empty() || blank(command)) continue;

        // Filter
        vector<string> raw_text = split(command, " ");
        string args = "";
        for(int i = 1; i < raw_text.size(); i++) {
            args += raw_text[i];
            if (i < raw_text.size() - 1) args += " ";
        }

        // Executing commands
        fb = commands.Execute(raw_text[0], args);

        if(fb == 1)
            cout << "Command not found!" << endl;
        if(fb == -1) break;

    }
    return 0;
}

