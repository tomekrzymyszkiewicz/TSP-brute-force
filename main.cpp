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
#include "structures.h"
using namespace std;
using namespace std::chrono;

vector<vector<string>> tasks;
vector<string> results;
string data_file_name = "";
string results_file_name = "";
vector<node> graph_data = vector<node>();
int number_of_current_graph_vertices = 0;
int number_of_current_graph_edges = 0;


struct Result
{
    string structure;
    string operation;
    string graph_type;
    int number_of_vertices;
    int number_of_edges;
    double time_memory;
    int number_of_repeats;
    string test_type;
    Result(string structure, string graph_type, string operation, int number_of_vertices, int number_of_edges, double time_memory, int number_of_repeats, string test_type)
    {
        this->structure = structure;
        this->operation = operation;
        this->graph_type = graph_type;
        this->number_of_vertices = number_of_vertices;
        this->number_of_edges = number_of_edges;
        this->time_memory = time_memory;
        this->number_of_repeats = number_of_repeats;
        this->test_type = test_type;
    }
    string toString()
    {
        return (structure + "," + graph_type + "," + operation + "," + to_string(number_of_vertices) + "," + to_string(number_of_edges) + "," + to_string(time_memory) + "," + to_string(number_of_repeats) + "," + test_type);
    }
};

void save_results(string results_file_name)
{
    std::cout << "Saving results" << endl;
    fstream fout;
    fout.open(results_file_name, ios::out);
    fout << "structure,graph_type,operation,number_of_vertices,number_of_edges,time_memory,number_of_repeats,test_type" << endl;
    for (int i = 0; i < results.size(); i++)
    {
        fout << results[i] << endl;
    }
    fout.close();
    std::cout << "Correctly saved " << results.size() << " results" << endl;
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
    number_of_current_graph_vertices = 0;
    number_of_current_graph_edges = 0;
    while (!fin.eof())
    {
        fin >> loaded_source >> loaded_destination >> loaded_weight;
        if (!fin.eof())
        {
            graph_data.push_back(node(stoi(loaded_source), stoi(loaded_destination), stoi(loaded_weight)));
            number_of_current_graph_edges += 1;
            if (number_of_current_graph_vertices < stoi(loaded_destination) + 1)
                number_of_current_graph_vertices = stoi(loaded_destination) + 1;
            edges_loaded++;
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
    vector<string> row;
    string line;
    fin >> results_file_name;
    while (!fin.eof())
    {
        string file_name, structure, graph_type, operation, number_of_repeats, test_type;
        fin >> file_name >> structure >> graph_type >> operation >> number_of_repeats >> test_type;
        if (file_name.size() == 0 || structure.size() == 0 || graph_type.size() == 0 || operation.size() == 0 || number_of_repeats.size() == 0 || test_type.size() == 0)
        {
            std::cout << "Cannot load this task: " << structure << graph_type << operation << number_of_repeats << test_type;
            break;
        }
        vector<string> task;
        task.push_back(file_name);
        task.push_back(structure);
        task.push_back(graph_type);
        task.push_back(operation);
        task.push_back(number_of_repeats);
        task.push_back(test_type);
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
            string structure = tasks[i][1];
            string graph_type = tasks[i][2];
            string operation = tasks[i][3];
            int number_of_repeats = stoi(tasks[i][4]);
            string test_type = tasks[i][5];
            if (!load_data(graph_file_name))
            {
                std::cout << "Cannot load graph from " << data_file_name << " file." << endl;
            }
            std::cout << "Computing " << operation << " in " << structure << " graph with " << number_of_current_graph_edges << " edges and " << number_of_current_graph_vertices << " vertices repeated " << number_of_repeats << " times; " << test_type << " test" << endl;
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