#ifndef QUASH
#define QUASH

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

class Quash{
    
private:

    vector<string> operations;

    /*the idea behind this structure is being able to create an actual command from the parsing of the input
    then run each command by calling its "run" function. HAS NOT BEEN USED YET*/
    struct command{
        public:

        command()
        {

        };
        ~command()
        {
            args.clear();
        };

        string type;
        vector<string> args;
        virtual void run(){};
    };

    vector<string> parse(string in)
    {
        string newstring ="";
        char* new_word;
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
        parsed_input.push_back(newstring);//for end of file input
        /*for(auto input:parsed_input)
        {
            cout << input << endl;
        }*/
        return parsed_input;
    }

    vector<string> interpret(vector<string> parsed_input)
    {
        vector<string> newcoms;
        string newcom;
        bool args=false;//flag used for reading in multiple arguments
        for(auto com:parsed_input)
        {
            if(com[0]=='.' || com[0]=='/')
            {
                args=true;
                newcoms.push_back("executable_spec");
            }
            else if(com[0]=='|')
            {
                args=false;
                newcoms.push_back("pipe");
            }
            else if(com[0]=='<')
            {
                args=false;
                newcoms.push_back("redir_stdin");
            }
            else if(com[0]=='>')
            {
                args=false;
                if(com.length()==1)
                {
                    newcoms.push_back("truncate");
                }
                else
                {
                    newcoms.push_back("append");
                }
            }
            else
            {
                if(com=="echo")
                {
                    args=true;
                    newcoms.push_back("echo");
                } 
                else if(com=="export" || com=="cd" || com=="pwd" || com=="quit" || com=="exit" || com=="jobs" || com=="kill" || com=="cat")
                {
                    args=false;
                    newcoms.push_back(com);
                }
                else
                {
                    if(args)
                    {
                        newcoms.push_back("arg");
                    }
                    else
                    {
                        args=true;
                        newcoms.push_back("executable");
                    }
                }
            }
        }
        //test printer
        cout << "interpreted:\n";
        for(auto line:newcoms)
        {
            cout << line << endl;
        }
        return newcoms;
    }

    //
    
    /////////////////////
    /*COMMAND FUNCTIONS*/
    /////////////////////
    
    //Echo
    void echo(string input)
    {
        //build the echo input back up
        //int pd[2];
        //pipe(pd);

        int pid;
        pid=fork();
        if(pid>0)
        {
            wait(NULL);
        }
        else
        {
            //dup2(pd[1],1);
            char* converted_input=new char[input.size()+1];
            strcpy(converted_input, input.c_str());//free this after exec, so maybe do this op before fork?
            execl(BASH_EXEC, BASH_EXEC, "-c", converted_input, (char *) 0);
        }
        //dup2(pd[0], 0);
        //close(pd[1]);
    }

    //cd
    void cd(string input)
    {
        char* path=new char[input.size()+1];
        strcpy(path, input.c_str());
        int success = chdir(path);
    }

    //pwd
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

    //export NOT TESTED
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

    //Exit and Quit
    void leave()
    {
        exit(0);
    }

    

    //executables NOT DONE
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
    //specified executables NOT DONE
    void exec_spec(string executable_)
    {
        int pid;
        pid=fork();
        if(pid==0)
        {

        }
        else
        {
            exit(0);
        }
    }



    //will be used for other functions
    void append(char* name, char* val)
    {
        int fd;
        //fd = open(name, O_WRONLY | O_CREAT, 0644);
        char *append_val = val;
        printf("%s\n", append_val);
        //close(fd);
    }

    //clears stdin, no bad input will be used for this project so we probably won't use this but i kept it here just in case
    void clearinput()
    {
        if(false)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    public:
    Quash(){};
    ~Quash(){};

    void run()
    {
        vector<string> commands;
        vector<string> interpreted_commands;
        char** converted_commands;
        cout << "Welcome to Quash!" << endl;
        while(true)
        {
            string new_input;
            getline(cin, new_input);
            commands = parse(new_input);
            interpreted_commands=interpret(commands);

            /*Below is the running of each commands. THIS IS NOT FINISHED AND IS A PROTOTYPE*/
            for(int i=0;i<interpreted_commands.size();i++)
            {
                if(interpreted_commands[i]=="executable")
                {
                    int j=i+1;
                    bool read_in=true;//flag used to find end of args list
                    vector<string> exec_inputs;
                    while(read_in)
                    {
                        if(interpreted_commands[j]=="arg")
                        {
                            j++;
                            exec_inputs.push_back(interpreted_commands[j]);
                        }
                        else
                        {
                            read_in=false;
                        }
                    }  
                    executable(commands[i], exec_inputs);
                }
                else if(interpreted_commands[i]=="echo")
                {
                    int j=i+1;
                    bool read_in=true;//flag used to find end of args list
                    vector<string> echo_inputs;
                    while(read_in)
                    {
                        if(interpreted_commands[j]=="arg")
                        {
                            j++;
                            echo_inputs.push_back(interpreted_commands[j]);
                        }
                        else
                        {
                            read_in=false;
                        }
                    }  
                    echo(echo_inputs);
                }
            }
        }
    }


};

#endif