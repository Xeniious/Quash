#ifndef TEST
#define TEST

#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <utility>
#include <cstring>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>

#define BASH_EXEC  "/usr/bin/bash"
#define ECHO_EXEC  "/usr/bin/echo"
#define PWD_EXEC "/usr/bin/pwd"

using namespace std;

class test{

private:

//list of processes currently running, used for the JOBS command
vector<int> bg_processes;

char** parse(string in)
{
    string newstring ="";
    vector<string> parsed_input;
    for(int i=0;i<in.length();i++)
    {
        if(in[i]==' ')
        {
            parsed_input.push_back(newstring);
            newstring="";
        }
        else
        {
            newstring+=in[i];
        }
    }
    parsed_input.push_back(newstring);
    
    //interpret commands
    for(int i=0;i<parsed_input.size();i++)
    {
        //if(parsed_input[i] == "echo" || parsed_input[i] == "|" || parsed_input[i] == "<" || parsed_input[i] == ">" || parsed_input[i] == ">>" || parsed_input[i] == "export" || parsed_input[i] == "pwd" ||)
    }
    //return parsed_input;
}

public:

struct command{
    public:

    command(char* com, char** args)
    {
        my_com=com;
        my_args=args;
    };
    ~command()
    {
        
    };

    char** my_args;
    char* my_com;
    virtual void run(){};
};

void run()
{
    cout << "Welcome to Quash!" << endl;
    while(true)
    {
        cout << "[Quash] ";
        string new_input;
        getline(cin, new_input);
        char** commands = parse(new_input);
        cd("testdir");
        pwd();
        echo("echo \"well hello there\"");
        execlp(BASH_EXEC, BASH_EXEC, "-c", "./Infinite_Job", (char *) 0);
        //leave();
    }
}

void executable(string input)
{
    int pid;
    pid=fork();
    if(pid>0)
    {
        wait(NULL);
    }
    else
    {
        char* converted_input=new char[input.size()+1];
        strcpy(converted_input, input.c_str());//free this after exec, so maybe do this op before fork?
        execl(BASH_EXEC, BASH_EXEC, "-c", converted_input, (char *) 0);
    }
}

void echo(string input)
{
    //build the echo input back up
    int pd[2];
    pipe(pd);
    close(pd[0]);
    dup2(pd[1], STDOUT_FILENO);
    int pid;
    pid=fork();
    if(pid>0)
    {
        wait(NULL);
    }
    else
    {
        char* converted_input=new char[input.size()+1];
        strcpy(converted_input, input.c_str());//free this after exec, so maybe do this op before fork?
        execl(BASH_EXEC, BASH_EXEC, "-c", converted_input, (char *) 0);
    }
}

void pwd()
{
    int pid;
    pid=fork();
    if(pid>0)
    {
        wait(NULL);
    }
    else
    {
        string com="pwd";
        char* converted_input=new char[com.size()+1];
        strcpy(converted_input, com.c_str());
        execl(BASH_EXEC, BASH_EXEC, "-c", converted_input, (char *) 0);
    }
}

void cd(string input)
{
    char* path=new char[input.size()+1];
    strcpy(path, input.c_str());
    int success = chdir(path);
}

void export_(string input)
{
    int pid;
    pid=fork();
    if(pid>0)
    {
        wait(NULL);
    }
    else
    {
        char* converted_input=new char[input.size()+1];
        strcpy(converted_input, input.c_str());
        execl(BASH_EXEC, BASH_EXEC, "-c", converted_input, (char *) 0);
    }
}

void leave()
{
    exit(0);
}

void jobs(string input)
{
    
}

void kill(string input)
{

}



void executable(string executable_, vector<string> args_)
{
    //convert arguments
    cout << "executable";
    char** argv=new char*[args_.size()];
    if(args_.size()>0)
    {
        for(int i=0;i<args_.size()-1;i++)
        {
            copy(args_[i].begin(), args_[i].end(), argv[i]);
            cout << argv[i] << endl;
        }
    }
    else
    {
        argv[0]=NULL;
    }

    //fork execution
    int pid;
    pid=fork();
    if(pid==0)
    {
        //WAIT COMMAND FOR WHEN CHILD PROCESS EXITS?
    }
    else
    {
        //DO TASK AND EXIT
        const char* exec_ = executable_.c_str();
        execvp(exec_, argv);
        exit(0);
    }
}

};

#endif