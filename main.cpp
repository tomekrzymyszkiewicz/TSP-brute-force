#include <iostream>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <queue>
#include <chrono>
#include <vector>
#include <conio.h>
#include <random>
#include <string>
#include <windows.h>
#include <sstream>
#include <iterator>
#include "structures.h"
using namespace std;
using namespace std::chrono;

vector<vector<string>> tasks;
vector<string> results;
string results_file_name = "";
vector<node> graph_data = vector<node>();
int number_of_current_graph_vertices = 0;
int number_of_current_graph_edges = 0;
adjacency_matrix current_graph_adjacency_matrix = adjacency_matrix();


struct Result
{
    string graph_name;
    int* path;
    double time;
    int number_of_repeats;
    Result(string graph_name, int* path, double time, int number_of_repeats)
    {
        this->graph_name = graph_name;
        this->path = path;
        this->time = time;
        this->number_of_repeats = number_of_repeats;
    }
    string toString()
    {   
        string path_string = "";
        for(int i = 0; i < number_of_current_graph_vertices; i++){
            path_string += path[i];
            path_string += " ";
        }
        return (graph_name + "," + path_string + "," + to_string(time) + "," + to_string(number_of_repeats));
    }
};

void save_results(string results_file_name)
{
    std::cout << "Saving results" << endl;
    fstream fout;
    fout.open(results_file_name, ios::out);
    fout << "graph_name,path,time,number_of_repeats" << endl;
    for (int i = 0; i < results.size(); i++)
    {
        fout << results[i] << endl;
    }
    fout.close();
    std::cout << "Correctly saved " << results.size() << " results" << endl;
}

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

bool load_data(string file_name)
{
    std::cout << "Loading data from " << file_name << " file" << endl;
    ifstream fin;
    fin.open(file_name, ios::in);
    if (!fin.good())
    {
        std::cout << "Data file " << file_name << " not exist" << endl;
        fin.close();
        return false;
    }
    string loaded_source, loaded_destination, loaded_weight;
    int edges_loaded = 0;
    fin >> number_of_current_graph_vertices;
    current_graph_adjacency_matrix = adjacency_matrix(number_of_current_graph_vertices);
    for(int i = 0; i < number_of_current_graph_vertices-1; i++){
        string loaded_line_of_matrix = "";
        getline(fin,loaded_line_of_matrix);
        vector<string> single_line = split(loaded_line_of_matrix,' ');
        for(int j = 0; j < single_line.size(); j++){
            current_graph_adjacency_matrix.add_edge_undir(i,j,stoi(single_line[j]));
        }
    }
    std::cout << "Loaded correctly " << edges_loaded << " edges and " << number_of_current_graph_vertices << " vertices" << endl;
    fin.close();
    return true;
}

void load_config()
{
    std::cout << "Loading config.ini" << endl;
    ifstream fin;
    fin.open("config.ini", ios::in);
    if (!fin.good())
    {
        std::cout << "Config.ini not found" << endl;
        fin.close();
        return;
    }
    string loaded_line_of_task = "";
    getline(fin,results_file_name);
    while (getline(fin,loaded_line_of_task))
    {
        vector<string> single_line = split(loaded_line_of_task,' ');
        string graph_file_name, number_of_repeats, shortest_path_weight, shortest_path;
        if(single_line.size() >= 4){
            graph_file_name = single_line[0];
            number_of_repeats = single_line[1];
            shortest_path_weight = single_line[2];
            for(int i = 3; i < single_line.size(); i++){
                shortest_path += single_line[i];
                shortest_path += " ";
            }
        }
        if (graph_file_name.size() == 0 || number_of_repeats.size() == 0 || shortest_path_weight.size() == 0 || shortest_path.size() == 0)
        {
            std::cout << "Cannot load this task: " << graph_file_name <<" "<< number_of_repeats <<" "<< shortest_path_weight <<" "<< shortest_path;
            break;
        }
        vector<string> task;
        task.push_back(graph_file_name);
        task.push_back(number_of_repeats);
        task.push_back(shortest_path_weight);
        task.push_back(shortest_path);
        tasks.push_back(task);
    }
    fin.close();
    std::cout << "Config loaded correctly" << endl;
    return;
}

int main()
{
    load_config();
    if (tasks.size() == 0)
    {
        std::cout << "No tasks found to be performed." << endl;
    }
    else
    {
        for (int i = 0; i < tasks.size(); i++)
        {
            std::cout << endl;
            string graph_file_name = tasks[i][0];
            int number_of_repeats = stoi(tasks[i][1]);
            string shortest_path_weight = tasks[i][2];
            string shortest_path = tasks[i][3];
            if (!load_data(graph_file_name))
            {
                std::cout << "Cannot load graph from " << graph_file_name << " file." << endl;
            }
            std::cout << "Computing TSP in " << graph_file_name << " graph with " << number_of_current_graph_edges << " edges and " << number_of_current_graph_vertices << " vertices repeated " << number_of_repeats << " times" << endl;
            if (number_of_current_graph_edges < 1)
            {
                std::cout << "Cannot execute task. The array must to have at least 1 element.";
            }
            else if (number_of_repeats < 1)
            {
                std::cout << "Cannot execute task. The minimum number of repetitions is 1.";
            }
            else
            {
                
            }
        }
    }
    std::cout << endl;
    save_results(results_file_name);
    std::cout << "Press any key to continue...";
    getch();
    return 0;
}