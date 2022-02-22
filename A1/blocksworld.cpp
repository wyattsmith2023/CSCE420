// Heuristic used is if we find a block that needs to be moved
// we add the number of blocks ontop of it including itself and 
// if a goal stack is taller than the corresponding stack we add
// the difference

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <limits>
#include <queue>
#include <map>
#include <typeinfo>

using namespace std;

int iterations;
int max_q;
vector<string> goal_state;
string h_code;

class Node{
public:
    vector<string> state;
    int path_cost;
    vector<Node> children;
    vector<string> moves;

    Node(){}

    Node(vector<string> stacks){
        state = stacks;
        path_cost = 0;
    }

    Node(vector<string> stacks, int g){
        state = stacks;
        path_cost = g + 1;
    }

    int heuristic(){
        int h = 0;
        // best: B16
        if(h_code == "H1"){
            for(int i = 0; i < state.size(); i++){
                if(state.at(i).size() < goal_state.at(i).size()){
                    h += (goal_state.at(i).size() - state.at(i).size());
                }
                for(int j = 0; j < state.at(i).size(); j++){
                    if(j < goal_state.at(i).size()){
                        if(state.at(i).at(j) != goal_state.at(i).at(j)){
                            h++;
                        }
                    }
                }
            }
        } else if (h_code == "H2"){
            for(int i = 0; i < state.size(); i++){
                if(state.at(i).size() < goal_state.at(i).size()){
                    h += (goal_state.at(i).size() - state.at(i).size());
                }
                for(int j = 0; j < state.at(i).size(); j++){
                    if(j < goal_state.at(i).size()){
                        if(state.at(i).at(j) != goal_state.at(i).at(j)){
                            h += (state.at(i).size() - state.at(i).find(state.at(i).at(j)));
                        }
                    }
                }
            }
        }
        return h;
    }

    void generateChildren(){
        vector<string> child_state = state;
        for(int i = 0; i < child_state.size(); i++){
            if(child_state.at(i).size() == 0){
                continue;
            }
            char top_of_stack = child_state.at(i).at(child_state.at(i).size() - 1);
            child_state.at(i).pop_back();
            for(int j = 0; j < child_state.size(); j++){
                if(j != i){
                    child_state.at(j) += top_of_stack;
                    Node c(child_state, path_cost);
                    for(int i = 0; i < moves.size(); i++){
                        c.moves.push_back(moves.at(i));
                    }
                    c.moves.push_back("------- Move " + to_string(path_cost) + ", h(n)=" + to_string(heuristic()) + " f(n)=" + to_string(path_cost + heuristic()) +  " -------");
                    for(int i = 0; i < state.size(); i++){
                        c.moves.push_back(state.at(i));
                    }
                    children.push_back(c);
                    child_state.at(j).pop_back();
                }
            }
           child_state = state;
        }
    }
};

int processArguments(string &filename, string &h, int &max_iter, bool &display_moves, int argc,char* args[]){
// Ensure file has been provided
    try{
        if(argc < 2){
            throw 0;
        }

        filename = args[1];
        h = "H2";
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
                else if(strcmp(args[i],"-MOVES") == 0){
                         display_moves = true;
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
    bool operator()(Node n, Node m) const {
        return n.path_cost + n.heuristic() >= m.path_cost + m.heuristic();
    }
};
Node bestFirstSearch(vector<string> problem, vector<string> goal, int max){
    Node initial(problem);
    
    priority_queue<Node, vector<Node>, nodeCmp> frontier;
    frontier.push(initial);
    
    map<vector<string>, Node> reached;
    reached[problem] = initial;
    iterator<vector<string>, Node> it;
    
    iterations = 0;
    max_q = 1;
    while(!frontier.empty() && iterations < max){
        if(frontier.size() > max_q){
            max_q = frontier.size();
        }
        iterations++;
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
    return initial;
}

void printMoves(Node n){
    for(int i = 0; i < n.moves.size(); i++){
        cout << n.moves.at(i) << endl;
    }
        cout << "------- Move " << n.path_cost << ", h(n)=" << n.heuristic() << " f(n)=" << (n.path_cost + n.heuristic()) <<  " -------" << endl;
        for(int i = 0; i < n.state.size(); i++){
            cout << n.state.at(i) << endl;
        }
}

int main(int argc, char* args[]){
    string filename;
    int max_iter;
    vector<string> goals;
    vector<string> stacks;
    bool display_moves = false;

    if(!processArguments(filename, h_code, max_iter, display_moves, argc, args))
        return 0;

    int num_moves = processFile(filename, stacks, goals);
    goal_state = goals;

    Node goal = bestFirstSearch(stacks, goals, max_iter);

    cout << "statistics: " << filename << " method ";
    if(h_code == "H0")
        cout << "BFS";
    else 
        cout << "Astar";
    cout << " planlen ";
    if(goal.path_cost != 0 && iterations <= max_iter){
        cout << goal.path_cost << " iter " << iterations << " maxq " << max_q << endl;
    } else {
        cout << "FAILED" << " iter " << iterations << " maxq " << max_q << endl;
    }


    if(display_moves)
        printMoves(goal);
    

}