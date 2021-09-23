/* Include */
#include <cstdlib>
#include <iostream>
#include <random>
#include <functional>
#include <map>
#include <string>

/* My Headers */

using namespace std;


class Shell {

    public:
        // Attrs
        map<string, function<int(string)>> commands;


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

    private:
        // Attrs
        string username;
};

Shell::Shell() {
    // Init
    this->commands.insert(pair<string, function<int(string)>>("hello", bind (&Shell::hello, this, std::placeholders::_1)));
    this->commands.insert(pair<string, function<int(string)>>("exit", bind (&Shell::exit, this, std::placeholders::_1)));
    this->commands.insert(pair<string, function<int(string)>>("help", bind (&Shell::help, this, std::placeholders::_1)));
    this->commands.insert(pair<string, function<int(string)>>("clear", bind (&Shell::clear, this, std::placeholders::_1)));
    this->commands.insert(pair<string, function<int(string)>>("echo", bind (&Shell::echo, this, std::placeholders::_1)));


    this->username = getenv("USER");

    cout << "*********************" << endl;
    cout << "Terminal initialized" << endl;
    cout << "Welcome " << this->username << endl;
    cout << "Type 'help' to know more about this shell" << endl;
    cout << "*********************" << endl;
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


/* Main Main Main */
int main(int argc, char **argv) {
    Shell commands;
    int fb;
    while(1) {
        string command;

        // Input
        cout << ">>> ";
        getline(cin, command);
        
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

