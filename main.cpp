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
#include <algorithm>
#include <cctype>
#include <locale>
#include "structures.h"
using namespace std;
using namespace std::chrono;

vector<vector<string>> tasks;
vector<string> results;
string results_file_name = "";
int number_of_current_graph_vertices = 0;
adjacency_matrix current_graph_adjacency_matrix = adjacency_matrix();

struct Result
{
    string graph_name;
    string path;
    int path_weight;
    double time;
    int number_of_repeats;
    Result(string graph_name, string path,int path_weight, double time, int number_of_repeats)
    {
        this->graph_name = graph_name;
        this->path = path;
        this->path_weight = path_weight;
        this->time = time;
        this->number_of_repeats = number_of_repeats;
    }
    string toString()
    {
        // string path_string = "";
        // for (int i = 0; i < number_of_current_graph_vertices; i++)
        // {
        //     path_string += path[i];
        //     path_string += " ";
        // }
        return (graph_name + "," + path + "," + to_string(path_weight) + "," + to_string(time) + "," + to_string(number_of_repeats));
    }
};

void save_results(string results_file_name)
{
    std::cout << "Saving results" << endl;
    fstream fout;
    fout.open(results_file_name, ios::out);
    fout << "graph_name,path,path_weight,time,number_of_repeats" << endl;
    for (int i = 0; i < results.size(); i++)
    {
        fout << results[i] << endl;
    }
    fout.close();
    std::cout << "Correctly saved " << results.size() << " results" << endl;
}

template <typename Out>
void split(const std::string &s, char delim, Out result)
{
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim))
    {
        *result++ = item;
    }
}
std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

// trim from start (in place)
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
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
    string loaded_number_of_vertices;
    getline(fin, loaded_number_of_vertices);
    number_of_current_graph_vertices = stoi(loaded_number_of_vertices);
    current_graph_adjacency_matrix = adjacency_matrix(number_of_current_graph_vertices);
    for (int i = 0; i < number_of_current_graph_vertices; i++)
    {
        string loaded_line_of_matrix = "";
        getline(fin, loaded_line_of_matrix);
        ltrim(loaded_line_of_matrix);
        rtrim(loaded_line_of_matrix);
        vector<string> single_line = split(loaded_line_of_matrix, ' ');
        std::vector<std::string>::iterator it = single_line.begin();
        while (it != single_line.end())
        {
            if (it->length() == 0)
            {
                it = single_line.erase(it);
            }
            else
            {
                ++it;
            }
        }
        for (int j = 0; j < single_line.size(); j++)
        {
            ltrim(single_line[j]);
            rtrim(single_line[j]);
            current_graph_adjacency_matrix.add_edge_dir(i, j, stoi(single_line[j]));
        }
    }
    std::cout << "Loaded correctly graph with " << number_of_current_graph_vertices << " vertices" << endl
              << "Graph:" << endl;
    current_graph_adjacency_matrix.print();
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
    getline(fin, results_file_name);
    while (getline(fin, loaded_line_of_task))
    {
        vector<string> single_line = split(loaded_line_of_task, ' ');
        string graph_file_name, number_of_repeats, shortest_path_weight, shortest_path;
        if (single_line.size() >= 4)
        {
            graph_file_name = single_line[0];
            number_of_repeats = single_line[1];
            shortest_path_weight = single_line[2];
            for (int i = 3; i < single_line.size(); i++)
            {
                shortest_path += single_line[i];
                shortest_path += " ";
            }
        }
        if (graph_file_name.size() == 0 || number_of_repeats.size() == 0 || shortest_path_weight.size() == 0 || shortest_path.size() == 0)
        {
            std::cout << "Cannot load this task: " << graph_file_name << " " << number_of_repeats << " " << shortest_path_weight << " " << shortest_path;
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

vector<int> TSP_brute_force()
{
    int source_vertex = 0;
    vector<int> nodes;
    for (int i = 0; i < number_of_current_graph_vertices; i++)
    {
        if (i != source_vertex)
        {
            nodes.push_back(i);
        }
    }
    int n = nodes.size();
    vector<int> answer_path;
    int TSP_shortest_path = INT_MAX;
    do
    {
        int path_weight = 0;
        int j = source_vertex;
        for (int i = 0; i < n; i++)
        {
            path_weight += current_graph_adjacency_matrix.matrix[j][nodes[i]];
            j = nodes[i];
        }
        path_weight += current_graph_adjacency_matrix.matrix[j][source_vertex];
        if (path_weight < TSP_shortest_path)
        {
            TSP_shortest_path = path_weight;
            answer_path.clear();
            answer_path.push_back(source_vertex);
            for (int i = 0; i < n; i++)
            {
                answer_path.push_back(nodes[i]);
            }
        }
    } while (next_permutation(nodes.begin(), nodes.end()));
    answer_path.push_back(source_vertex);
    answer_path.push_back(TSP_shortest_path);
    return answer_path;
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
            std::cout << endl
                      << "##################################################" << endl
                      << endl;
            string graph_file_name = tasks[i][0];
            int number_of_repeats = stoi(tasks[i][1]);
            string shortest_path_weight = tasks[i][2];
            string shortest_path = tasks[i][3];
            if (!load_data(graph_file_name))
            {
                std::cout << "Cannot load graph from " << graph_file_name << " file." << endl;
            }
            std::cout << "Computing TSP in " << graph_file_name << " graph repeated " << number_of_repeats << " times" << endl;
            if (number_of_current_graph_vertices < 1)
            {
                std::cout << "Cannot execute task. The array must to have at least 1 element.";
            }
            else if (number_of_repeats < 1)
            {
                std::cout << "Cannot execute task. The minimum number of repetitions is 1.";
            }
            else
            {
                vector<int> answer;
                high_resolution_clock::time_point t_start = high_resolution_clock::now();
                for (int j = 0; j < number_of_repeats; j++)
                {
                    answer = TSP_brute_force();
                }
                high_resolution_clock::time_point t_end = high_resolution_clock::now();
                duration<double> time_span = duration_cast<duration<double>>(t_end - t_start);
                int weight = answer[answer.size() - 1];
                string path = "";
                answer.pop_back();
                std::vector<int>::iterator it = answer.begin();
                while (it != answer.end())
                {
                    path += to_string(*it);
                    path += " ";
                    it++;
                }
                ltrim(path);
                rtrim(path);
                cout << "Calculated shortest path: " << path << endl
                     << "Defined shortest path:    " << shortest_path << endl
                     << "Calculated weight: " << weight << endl
                     << "Defined weight:    " << shortest_path_weight << endl
                     << "Time: " << ((double)time_span.count() / (double)number_of_repeats) << " s" << endl;
                Result result = Result(graph_file_name,path,weight,time_span.count(),number_of_repeats);
                results.push_back(result.toString());
            }
        }
    }
    std::cout << endl;
    save_results(results_file_name);
    std::cout << "Press any key to continue...";
    getch();
    return 0;
}