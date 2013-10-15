/* 
 * File:   Evo_Agent_DST.h
 * Author: ylinieml
 *
 * Created on October 15, 2013, 12:27 PM
 */

#ifndef Evo_Agent_H
#define	Evo_Agent_H

#ifndef VECTOR_INCLUDE
#define VECTOR_INCLUDE
#include <vector.h>
#endif

class Evo_Agent {

    
    
    double raw_fitness; /// raw fitness calculated by treasure, time
    double treasure;
    double time;
    
    double transformed_treasure;
    double transformed_time;
    double transformed_fitness; /// transformed fitness after Pareto Transformation

    void console_1vector(vector<double>);
    void console_2vector(vector<vector<double> >);

    int rand_action();

    double Initial_Q_Value;
    
    vector<int> actions;
    
    void create_action_vector();

public:
    double fitness; /// used for actual evolutionary method
    int get_action(int time);
    int get_fitness();
    void mutate();
    int action;
    void show_action_vector();
    int id;
    
    void start();       //Used before first run of a statistical run / repeat
    void reset();       //Used at start of episode
    
    void set_time(double val);
    void set_treasure(double val);
    double get_time();
    double get_treasure();
    
    void set_raw_fitness(double raw);
    void set_fitness(double fit);
    
    /// Body Values:
    int x;
    int y;
    bool out_of_bounds;
    bool end_episode;
};

void Evo_Agent::start() {
    /// id is set outside of start, in main();
    create_action_vector();
    fitness=-1;
}

void Evo_Agent::reset() {
    x=0;
    y=0;
    out_of_bounds=false;
    end_episode=false;
    treasure=0;
    time=0;
}

void Evo_Agent::create_action_vector(){
    for(int i=0; i<STEPS; i++){
        actions.push_back(rand()%ACTIONS);
    }
}

int Evo_Agent::get_fitness(){
    return fitness;
}

int Evo_Agent::get_action(int time){
    return actions.at(time);
}

void Evo_Agent::set_time(double val){
    time=val;
}

double Evo_Agent::get_time(){
    return time;
}

void Evo_Agent::set_treasure(double val){
    treasure=val;
}

double Evo_Agent::get_treasure(){
    return treasure;
}

void Evo_Agent::set_raw_fitness(double raw){
    raw_fitness=raw;
}

void Evo_Agent::set_fitness(double fit){
    fitness=fit;
}

void Evo_Agent::console_2vector(vector< vector<double> > a) {
    for (int i = 0; i < a.size(); i++) {
        console_1vector(a.at(i));
        cout << endl;
    }
}

void Evo_Agent::console_1vector(vector<double> a) {
    for (int i = 0; i < a.size(); i++) {
        cout << a.at(i);
        cout << "\t";
    }
}

int Evo_Agent::rand_action() {
    int a;
    a = rand() % ACTIONS;
    return a;
}

void Evo_Agent::show_action_vector() {
    int A=actions.size();
    for(int a=0; a<A; a++){
        cout << actions.at(a) << "\t";
    }
    cout << endl;
}

void Evo_Agent::mutate() {
    /// PARAM
    int num_mutations=rand()%STEPS;
    for(int i=0; i<num_mutations; i++){
        actions.at(rand()%actions.size())=rand()%ACTIONS;
    }
}

#endif	/* Evo_Agent_H */


