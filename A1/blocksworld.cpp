#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <limits>
#include <queue>
#include <map>

using namespace std;

class Node{
public:
    vector<string> state;
    int alpha;
    int beta;
    int path_cost;
    vector<Node> children;

    Node(){}

    Node(vector<string> stacks){
        state = stacks;
        path_cost = 0;
        alpha = numeric_limits<int>::max();
        beta = numeric_limits<int>::min();
    }

    Node(vector<string> stacks, int g){
        state = stacks;
        path_cost = g + 1;
        alpha = numeric_limits<int>::max();
        beta = numeric_limits<int>::min();
    }

    void generateChildren(){
        for(int i = 0; i < state.size(); i++){
            char top_of_stack = state.at(i)[state.at(i).size() - 1];
            state.at(i).pop_back();
            for(int j = 0; j < state.size(); j++){
                if(j != i){
                    state.at(j) += top_of_stack;
                    Node c(state, path_cost);
                    children.push_back(c);
                    state.at(j).pop_back();
                }
            }
            state.at(i).push_back(top_of_stack);
        }
    }
};

int processArguments(string &filename, string &h, int &max_iter, int argc,char* args[]){
// Ensure file has been provided
    try{
        if(argc < 2){
            throw 0;
        }

        filename = args[1];
        h = "H0";
        max_iter = 100000;

        // Check if more arguments have been passed
        if(argc > 2){
            int s = -1;
            for(int i = 2; i < argc; i++){
                if(strcmp(args[i],"-H") == 0){
                    if(i+1 < argc){
                        h = args[i+=1];
                    } 
                    else {
                        throw 1;
                    }
                }
                else if(strcmp(args[i],"-MAX_ITERS") == 0){
                    if(i+1 < argc){
                        max_iter = stoi(args[i+=1]);
                    } 
                    else {
                        throw 2;
                    }
                }
                else {
                    throw 3;
                }
            }

        }
        return -1;
    }
    catch(int e){
        switch(e){
            case 0:
                cout << "Error: No file provided" << endl;
                break;
            case 1:
                cout << "Error: No heuristic specified" << endl;
                break;
            case 2:
                cout << "Error: No max iterations specified" << endl;
                break;
            case 3:
                cout << "Error: Invaild argument provided" << endl;
                break;

            }
        return 0;
    }
}
int processFile(string filename, vector<string> &stacks, vector<string> &goals){
    int num_stacks;
    int num_blocks;
    int num_moves = -1; 

    ifstream file(filename);
    file >> num_stacks;
    file >> num_blocks;
    file >> num_moves;

    string buffer;
    getline(file, buffer);  // Line containing data lengths
    getline(file, buffer);  // Line containing delimitor
    for(int i = 0; i < num_stacks; i++){
        getline(file, buffer);
        stacks.push_back(buffer);
    }

    getline(file, buffer);  // Line containing delimitor
    for(int i = 0; i < num_stacks; i++){
        getline(file, buffer);
        goals.push_back(buffer);
    } 

    return num_moves; 
}

struct nodeCmp{
    bool operator()(const Node n, const Node m) const {
        return n.path_cost > m.path_cost;
    }
};
Node bestFirstSearch(vector<string> problem, vector<string> goal){
    Node initial(problem);
    
    priority_queue<Node, vector<Node>, nodeCmp> frontier;
    frontier.push(initial);
    
    map<vector<string>, Node> reached;
    reached[problem] = initial;
    iterator<vector<string>, Node> it;
    
    while(!frontier.empty()){
        Node n = frontier.top();
        frontier.pop();
        if(n.state == goal){
            return n;
        }
        n.generateChildren();
        for(int i = 0; i < n.children.size(); i++){
            Node s = n.children.at(i);
            if(reached.find(s.state) == reached.end() || s.path_cost < reached.at(s.state).path_cost){
                reached[s.state] = s;
                frontier.push(s);
            }

        }

    }
    Node n;
    return n;
    
}
int main(int argc, char* args[]){
    string filename;
    string h;
    int max_iter;
    vector<string> goals;
    vector<string> stacks;

    if(!processArguments(filename, h, max_iter, argc, args))
        return 0;

    int num_moves = processFile(filename, stacks, goals);

    cout << "Goal: " << endl;
    Node test = bestFirstSearch(stacks, goals);
    for(int i = 0; i < test.state.size(); i++){
        cout << test.state.at(i) << endl;
    }


    
}