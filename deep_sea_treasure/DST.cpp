/* 
 * File:   Evo_Agent_DST.h
 * Author: ylinieml
 * Deep Sea Treasure Problem by http://uob-community.ballarat.edu.au/~pvamplew/MORL.html
 * Pareto Transformation Code for Linear Combinations for Concave Areas of Pareto Fronts
 * Simple Evolutionary Agent Structure (Evo_Agent_DST.h)
 * Created on October 15, 2013, 12:27 PM
 */

#include <cstdlib>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <numeric>

#include <stdio.h>
#include <iostream>
using namespace std;

#ifndef VECTOR_INCLUDE
#define VECTOR_INCLUDE
#include <vector>
#endif

/// Small Functions/Macros
#define LYRAND (double)rand()/RAND_MAX
#define SMALL 0.0001
#define HUNDI_ROUND(x) (double)floor(x*100)/100

/// Problem Domain Parameters
#define XMAX 10
#define YMAX 11
#define XSTATES 10
#define YSTATES 11
#define TOTAL_STATES (XSTATES * YSTATES)
#define VELOCITY 1
#define ACTIONS 5


/// Evoluationary Algorithm Parameters
/// PARAM
#define POPULATION 100
#define ELIMINATE 10
#define GENERATIONS 2500
#define STEPS 30
#define STAT_RUNS 30
#define BETA 0.5

#include "Evo_Agent_DST.h"
#include "Trig_Transformation.h"

bool pretty_print = false;

using namespace std;

/// Environment
class deep_sea {
public:
    void start();
    
    vector<int> treasure_x;
    vector<int> treasure_y;
    vector<int> treasure_value;
};

void deep_sea::start() {
    /// Treasure x,y,value as defined by 
    /// http://uob-community.ballarat.edu.au/~pvamplew/MORL.html
    
    treasure_x.push_back(0);
    treasure_x.push_back(1);
    treasure_x.push_back(2);
    treasure_x.push_back(3);
    treasure_x.push_back(4);
    treasure_x.push_back(5);
    treasure_x.push_back(6);
    treasure_x.push_back(7);
    treasure_x.push_back(8);
    treasure_x.push_back(9);
    
    treasure_y.push_back(1);
    treasure_y.push_back(2);
    treasure_y.push_back(3);
    treasure_y.push_back(4);
    treasure_y.push_back(4);
    treasure_y.push_back(4);
    treasure_y.push_back(7);
    treasure_y.push_back(7);
    treasure_y.push_back(9);
    treasure_y.push_back(10);
    
    treasure_value.push_back(1);
    treasure_value.push_back(2);
    treasure_value.push_back(3);
    treasure_value.push_back(5);
    treasure_value.push_back(8);
    treasure_value.push_back(16);
    treasure_value.push_back(24);
    treasure_value.push_back(50);
    treasure_value.push_back(74);
    treasure_value.push_back(124);
}

double vector_median(vector<double> fit){
    double median;
    /// sort vector
    sort(fit.begin(),fit.end());
    /// even or odd size
    int even = (fit.size()+1)%2;
    int odd = fit.size()%2;
    if(even){ /// even case
        int mid = fit.size()/2;
        double m1 = fit.at(mid);
        double m2 = fit.at(mid+1);
        median = (m1+m2)/2;
    }
    if(odd){ /// odd case
        int mid = fit.size()/2;
        median = fit.at(mid);
    }
    return median;
}
double vector_mean(vector<double> fit){
    double sum=accumulate(fit.begin(), fit.end(), 0.0);
    double mean = sum / fit.size();
    return mean;
}

void sense(Evo_Agent* pA) { /// Sense location.
    /// For open-loop evolutionary control, sensing is not necessary.
    /// This space intentionally (functionally) blank.
}

void decide(Evo_Agent* pA, int time) { /// Decide on an action.
    pA->action = pA->get_action(time); /// Take the next action in order. (Open loop control)
}

void act(Evo_Agent* pA, deep_sea* pE) { //agents make their action
            double x = pA->x;
            double y = pA->y;
            
            /// Begin Insight Bloc
            /*
            cout << "agent " << pA->id << endl;
            cout << "x: " << pA->x << endl;
            cout << "y: " << pA->y << endl;
            cout << "act: " << pA->action << endl;
            //* */
            /// End Insight
            
            /// If the submarine tries to hit the sea floor...
            if(pA->action==4 && x==6 && y==5){pA->set_time(STEPS); pA->set_treasure(0); pA->end_episode=true; return;}
            if(pA->action==4 && x==6 && y==6){pA->set_time(STEPS); pA->set_treasure(0); pA->end_episode=true; return;}
            if(pA->action==4 && x==8 && y==8){pA->set_time(STEPS); pA->set_treasure(0); pA->end_episode=true; return;}
            /// end the episode.

            //Calculate next state
            if(pA->action==0){x=x;y=y;}
            if(pA->action==1){x=x;y=y+VELOCITY;}
            if(pA->action==2){x=x+VELOCITY;y=y;}
            if(pA->action==3){x=x;y=y-VELOCITY;}
            if(pA->action==4){x=x-VELOCITY;y=y;}
            
            /// If the submarine goes out of bounds...
            if(x < 0 || x >= XMAX || y < 0 || y >= YMAX) {
                pA->end_episode=true;
                pA->set_time(STEPS);
                pA->set_treasure(0);
                return;
            }
            /// end the episode.
        
            pA->x = x;
            pA->y = y;
}

void react(Evo_Agent* pA, deep_sea* pE, int time) {
    //if the episode is over, get out.
    if(pA->end_episode==true){return;}
    
    /// if in a reward spot
    int T=pE->treasure_x.size();
    for(int i=0; i<T; i++){
        int X=pE->treasure_x.at(i);
        int Y=pE->treasure_y.at(i);
        if(pA->x==X && pA->y==Y){
            /// get the reward
            pA->set_treasure(pE->treasure_value.at(i));
            pA->set_time(time);
            /// end the episode
            pA->end_episode=true;
            /// and reset back to starting state
            pA->x=0;
            pA->y=0;
            break;
        }
    }
    
    /// if time is up, set rewards.
    if(time == (STEPS - 1)){
        pA->set_treasure(0);
        pA->set_time(time);
    }
}

void report(FILE* pFILE, double value) { /// report to text file
    fprintf(pFILE, "%.5f\t", value);
}
void newline(FILE* pFILE) { /// report to text file
    fprintf(pFILE, "\n");
}

int main() {        
    srand(time(NULL));
    FILE* pFILE_fit;
    FILE* pFILE_time;
    FILE* pFILE_treasure;
    pFILE_fit=fopen("fitness.txt","w");
    pFILE_time=fopen("time.txt","w");
    pFILE_treasure=fopen("treasure.txt","w");
    
    Trig_Transformation T;
    vector<double> one;
    vector<double> two; 
    one.push_back(30); /// time
    one.push_back(1); /// treasure
    two.push_back(11); /// time
    two.push_back(124); /// treasure
    
    T.take_objective_max(one);
    T.take_objective_max(two);
    T.calc_nadir();
    T.calc_utopia();
    
    double TIME_WEIGHT = BETA;

        for(int stat_run=0; stat_run < STAT_RUNS; stat_run++) {
            cout << "Begin Stat Run " << stat_run << " of "  << STAT_RUNS << endl;
            
            deep_sea environment;
            environment.start();
            deep_sea* pE = &environment; /// pointer to Environment

            vector<Evo_Agent> Agents;
            vector<Evo_Agent>* pVA = &Agents; /// pointer to Vector of Agents

            for (int i = 0; i < POPULATION; i++) {
                Evo_Agent EA;
                EA.start();
                EA.id=i;
                pVA->push_back(EA);
            }

            for (int gen = 0; gen < GENERATIONS; gen++){
                // environment.start_epsiode(); /// No changes necessary between episodes or generations (deterministic)
                
                if (gen % (GENERATIONS / 10) == 0) {
                    cout << "Run No." << stat_run << " is " << (double) gen / GENERATIONS * 100 << " % Complete!" << endl;
                }
                
                /// For each population member in pA, execute 1 round of the DST domain:
                for (int mem=0; mem<POPULATION; mem++) {
                    Evo_Agent* pA = &pVA->at(mem);
                    pA->reset();
                for (int time = 0; time < STEPS; time++) {
                    if(pA->end_episode){break;}
                    sense(pA);
                    decide(pA,time);
                    act(pA, pE);
                    react(pA, pE,time);
               }
               }
                
                /// Combine multiple objectives into scalar fitness
                for (int a = 0; a < pVA->size(); a++) {
                    /// <PARAM>
                    /// <Linear Combination of Objectives>
                    // pVA->at(a).fitness = (STEPS - pVA->at(a).get_time())*TIME_WEIGHT + pVA->at(a).get_treasure()*(1-TIME_WEIGHT);
                    /// <Treasure Only>
                    // pVA->at(a).fitness = pVA->at(a).get_treasure();
                    /// <Time Only>
                    //pVA->at(a).fitness = (STEPS - pVA->at(a).get_time());
                    /// <Trigonometric Transformation>
                    vector<double> MO;
                    vector<double>* pMO;
                    MO.push_back(STEPS - pVA->at(a).get_time());
                    MO.push_back(pVA->at(a).get_treasure());
                    pMO = &MO;
                    T.execute_transform(pMO);
                    pVA->at(a).transformed_fitness =  MO.at(0)*TIME_WEIGHT + MO.at(1)*(1-TIME_WEIGHT);
                    pVA->at(a).fitness = pVA->at(a).transformed_fitness;
                }
                
                /// eliminate worst-performing agents:
                /// build vector of fitnesses
                vector<double> fit;
                vector<double> times;
                vector<double> treasures;
                
                for (int a = 0; a < pVA->size(); a++) {
                    fit.push_back(pVA->at(a).get_fitness());
                    times.push_back(pVA->at(a).get_time());
                    treasures.push_back(pVA->at(a).get_treasure());
                }
                /// determine mean, median for reporting
                double generation_median = vector_median(fit);
                double generation_mean = vector_mean(fit);
                double median_time = vector_median(times);
                double median_treasure = vector_median(treasures);
                if(gen % 100 == 0){
                cout << "Generation\t" << gen << "\ttime:\t" << median_time << "\ttreasure:\t" << median_treasure << "\tfitness:\t" << generation_median << endl;
                }
                
                /// always eliminate worst-performing solutions.
                for(int e=0; e<ELIMINATE; e++){
                    double minfit = *min_element(fit.begin(), fit.end());
                    int spot = min_element(fit.begin(), fit.end()) - fit.begin();
                    // kill this fitness;
                    fit.erase(fit.begin() + spot);
                    // kill this agent
                    pVA->erase(pVA->begin() + spot);
                }
                
                /// duplicate best-performing agents
                for(int d=0; d<ELIMINATE; d++){
                    double maxfit = *max_element(fit.begin(), fit.end());
                    int spot = max_element(fit.begin(), fit.end()) - fit.begin();
                    /// <PARAM>
                    /// to reduce the rate of convergence, we select the best one (spot) 50% of the time,
                    /// and the other 50% of the time we select a random survivor.
                    if(rand()%2){spot=rand()%pVA->size();}
              
                    /// create exact copy
                    pVA->push_back(pVA->at(spot));
                    /// mutate
                    pVA->back().mutate();
                }

               //For production-level graphs
               if (pretty_print) {
                   report(pFILE_fit,generation_mean); /// report every result
                   report(pFILE_time,median_time); // Report every result
                   report(pFILE_treasure,median_treasure); // Report every result
               } else {                
                   //For Coarse Results
                   if (gen % (GENERATIONS / 100) == 0) {
                       report(pFILE_fit,generation_mean); // Report only occasionally
                       report(pFILE_time,median_time); // Report only occasionally
                       report(pFILE_treasure,median_treasure); // Report only occasionally
                   }
               }
            }

            //Start a new line in output file for next run
            fprintf(pFILE_fit,"\n");
            fprintf(pFILE_time,"\n");
            fprintf(pFILE_treasure,"\n");
        }
    fclose(pFILE_time);
    fclose(pFILE_treasure);
    fclose(pFILE_fit);
    return 0;
}

