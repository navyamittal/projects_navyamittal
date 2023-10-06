#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <string>
#include <fcntl.h>
#include "Tokenizer.h"
// all the basic colours for a shell prompt
#define RED     "\033[1;31m"
#define GREEN	"\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE	"\033[1;34m"
#define WHITE	"\033[1;37m"
#define NC      "\033[0m"
using namespace std;
char* gettime() 
{
    time_t t;
    time (&t);
    return ctime (&t);
}
int main () {
    //save original srdin and stdout
    int original_stdin = dup(0);
    int original_stdout = dup(1);
    char buffer[1000];;
    string current = getcwd(buffer,sizeof(buffer)); //current directory
    chdir("..");
    string previous = getcwd(buffer,sizeof(buffer)); //store the previous directory 
    chdir(current.c_str());
    vector <int> bgs; //vector of bgs
    for (;;) {
        //implement date/time with 
        //implement username with getenv("USER")
        //implement curdir with getcwd()
        // need date/time, username, and absolute path to current dir

        char buffer[1000];
        char t[256];
        //string current = getcwd(buffer,sizeof(buffer)); //current directory
        //string current = getcwd(buffer,sizeof(buffer)); //current directory
        //chdir("..");
        //string previous = getcwd(buffer,sizeof(buffer)); //store the previous directory 
        //string previous; //store the previous directory 
        //chdir(current.c_str());

        time_t timer = time(NULL);
        struct tm *timeInfo = localtime(&timer);
        strftime(t, 256, "%b %d %T", timeInfo);
        //string absolutePath = path;
        cout << YELLOW << t << " " << YELLOW << getenv("USER") << ":" << YELLOW << current << "$" << NC << " ";
         //implement iteration over vector of bg pid()
        //waitpid() using flag for non-blocking 
        for(unsigned int i = 0; i < bgs.size(); i++)
		{
			if(waitpid(bgs[i],0,WNOHANG) == bgs[i])
			{ 
				cout << "Process: " <<bgs[i] << " ended" <<endl;
				//remove process from list
				bgs.erase(bgs.begin() +i);
				i--; //keeping i at the same exact spot
			}
		}
        
        //cout << YELLOW << "Shell$" << NC << " ";
        
        // get user inputted command
        string input;
        getline(cin, input);
        Tokenizer tknr(input);
        
        // get tokenized commands from user input
        if (tknr.hasError()) {  // continue to next prompt if input had an error
            continue;
        }
        if (input == "exit") {  // print exit message and break out of infinite loop
            cout << RED << "Now exiting shell..." << endl << "Goodbye" << NC << endl;
            break;
        }
        //implement cd with change directory
        //if dir (cd <dir>) is "-" then go to previous working directory else just call change dir with reqd parameters 
        //need a vraible to store previous working directory (it needs to be declared outside the loop)
         if (tknr.commands[0]->args[0] == "cd") {
            // Handle the 'cd' command
            string temp = "";
            if (tknr.commands[0]->args.size() > 1) {
                std::string target = tknr.commands[0]->args[1];
                //cout << "target:    " << target << endl;
                if (target == "-") {
                    // Handle 'cd -' to switch between current and previous directory
                    //cout << "target:    " << target << endl;
                    //cout << "previous:    " << target << endl;
                        chdir(previous.c_str());
                        // temp = previous;
                        // previous = current;
                        // current = temp;
                        std::swap(current, previous);
                    
                } else {
                    // Attempt to change to the specified directory
                    previous = current;
                    chdir(target.c_str());
                    current = getcwd(buffer, sizeof(buffer));
                    //cout << "previous after cd ../../:  " << previous;
                }
            } else {
                // Handle 'cd' without arguments as an error or ignore it
                chdir("..");
                current = getcwd(buffer, sizeof(buffer));
            }
        } else {

        //background process
        
        for (std::vector<Command*>::size_type  i =0; i<tknr.commands.size(); i++){
            int fd[2]; //one for read and one for write 
            pipe(fd);

            bool runInBackground = false; // Flag to determine if the command should run in the background
            if (tknr.commands[i]->isBackground()) {
                // If the last argument is "&", run the command in the background
                //tknr.commands.back()->args.pop_back(); // Remove the "&" from the arguments
                runInBackground = true;
            }

            if(pipe(fd)==-1){
                perror("pipe");
                exit(EXIT_FAILURE);
            }
            //create child to run first command
            pid_t ch = fork();
            //in child Redirect output to write end of pipe
            if (ch == 0) {
            // Redirect output to write end of pipe
                //char* args[] = {(char*) tknr.commands.at(0)->args.at(0).c_str(), nullptr};
                if(i<tknr.commands.size()-1){
                    dup2(fd[1], 1);
                    close(fd[1]);
                }
                
                close(fd[0]); // Close the read end of the pipe on the child side
                // Execute the first command
                //execvp(command->args[0], command->args);
    
                //everything above this and in the for loop is le3
                //io redirection
                //int pos = input.find('>');
                //cout << pos << endl;
                if (tknr.commands[i]->out_file!="")
                {
                    //cout << "HIUFHCHBWJ" << endl;
                    string command = tknr.commands[i]->args[1];
                    //cout << "command  " << command << endl;
                    input= command;
                    int inpfd = open(tknr.commands[i]->out_file.c_str(), O_WRONLY|O_CREAT,S_IWUSR|S_IRUSR);
                    //cout << "execute" << inpfd;
                    dup2(inpfd,1);
                    //close(inpfd);
                }
                //pos =input.find('<');
                if (tknr.commands[i]->in_file!="")
                {
                    string command = tknr.commands[i]->args[1];
                    //string filename = input.substr(pos + 1);//filename
                    input = command;
                    int outfd = open(tknr.commands[i]->in_file.c_str(), O_RDONLY);
                    dup2(outfd,0);
                    //close(outfd);
                }
                std::vector<char*> c_args;
                 for (const std::string& arg : tknr.commands[i]->args) {
                    c_args.push_back(const_cast<char*>(arg.c_str()));
                }
                c_args.push_back(nullptr); // Null-terminate the argument list
                execvp(c_args[0], c_args.data());
            } else{
                //redirect the shell (parents) input to the read end of the pipe
                dup2(fd[0], 0);
                close(fd[1]); // Close the write end of the pipe on the child side
                //wait until the last command finishes
                if (!runInBackground) {
                    wait(NULL);
                } else {
                    // Add the background process PID to the vector
                    bgs.push_back(ch);
                }
            }
        }
        // // print out every command token-by-token on individual lines
        // // prints to cerr to avoid influencing autograder
        // for (auto cmd : tknr.commands) {
        //     for (auto str : cmd->args) {
        //         cerr << "|" << str << "| ";
        //     }
        //     if (cmd->hasInput()) {
        //         cerr << "in< " << cmd->in_file << " ";
        //     }
        //     if (cmd->hasOutput()) {
        //         cerr << "out> " << cmd->out_file << " ";
        //     }
        //     cerr << endl;
        // }
        //for piping 
        //for cmd : commands 
        //  call pipe() to make pipe 
        //  fork() - in child, redirect stdout. in parent redirect stdin
        //add checks for first and last command if first then not redirect stdin fro the parent
        //if last command don't redirect stdout of the child 
        //fork to create child
        //pid_t pid = fork();
        // if (pid < 0) {  // error check
        //     perror("fork");
        //     exit(2);
        // }
        //add check for bg process - add pid to vector if bg and don't waitpid() in parents
        // if (pid == 0) {  // if child, exec to run command
        //     //implement multiple arguments - iterate over args of current command to make and pass into char* array
        //     //char* args[] = {(char*) tknr.commands.at(0)->args.at(0).c_str(), nullptr};
        //     //if current command is redirected then open the file and dup2 std that is being redirected 
        //     // if (execvp(args[0], args) < 0) {  // error check
        //     //     perror("execvp");
        //     //     exit(2);
        //     // }
        // }
        // else {  // if parent, wait for child to finish
        //     int status = 0;
        //     waitpid(pid, &status, 0);
        //     if (status > 1) {  // exit if child didn't exec properly
        //         exit(status);
        //     }
        // }
        //restore stdin and stdout (variable would be outside the loop)
        dup2(original_stdin, 0);
        dup2(original_stdout, 1);
    }
    }

    return 0;
}