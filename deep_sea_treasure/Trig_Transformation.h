/* 
 * File:   Trig_Transformation.h
 * Author: ylinieml
 *
 * Created on October 16, 2013, 12:44 PM
 */

#ifndef TRIG_TRANSFORMATION_H
#define	TRIG_TRANSFORMATION_H

#ifndef VECTOR_INCLUDE
#define VECTOR_INCLUDE
#include <vector.h>
#endif

#define OBJECTIVES 2

class Trig_Transformation{
    vector<double> utopia;
    vector<double> nadir;
    vector< vector<double> > single_obj_max;
    
    vector<double> input;
    void take_input(vector<double>* coords);
    
    void scale();
    void trig_transform();
    
    vector<double> output;
    void give_output(vector<double>* coords);
    
public:
    void calc_utopia();
    void calc_nadir();
    void take_objective_max(vector<double> coords);
    void execute_transform(vector<double>* pinputs);
};

void Trig_Transformation::take_objective_max(vector<double> coords){
    single_obj_max.push_back(coords);
}

void Trig_Transformation::calc_utopia(){
    for(int o=0; o<OBJECTIVES; o++){
        double best=-999999.0;
        for(int i=0; i<single_obj_max.size(); i++){
            double val = single_obj_max.at(i).at(o);
            if(val > best){
                best = val;
            }
        }
        utopia.push_back(best);
    }
}

void Trig_Transformation::calc_nadir(){
    for(int o=0; o<OBJECTIVES; o++){
        double worst=999999.0;
        for(int i=0; i<single_obj_max.size(); i++){
            double val = single_obj_max.at(i).at(o);
            if(val < worst){
                worst = val;
            }
        }
        nadir.push_back(worst);
    }
}

void Trig_Transformation::take_input(vector<double>* pcoords){
    /// Assign input to class variable.
    input.clear();
    for(int obj=0; obj<OBJECTIVES; obj++){
        input.push_back(pcoords->at(obj));
    }
}

void Trig_Transformation::give_output(vector<double>* pcoords){
    /// Assign output to external variable.
    for(int obj=0; obj<OBJECTIVES; obj++){
        pcoords->at(obj) = output.at(obj);
    }
}

void Trig_Transformation::scale(){
    /// Scale values of objectives to be less than one, with the nadir point taking on (0,0).
    for(int obj=0; obj<OBJECTIVES; obj++){
        double range = utopia.at(obj)-nadir.at(obj);
        input.at(obj) = (input.at(obj) - nadir.at(obj)) / range;
    }
}

void Trig_Transformation::trig_transform(){
    // Only valid for 2-objective operations. May be generalizable with changes.
    // Determine angle with respect to utopia -> x axis.
    double dx,dy,theta,radius,factor;
    factor = 1.5; /// factor = 1.5 guarantees non-concavity. 
    //less than 1.5 allows concavity. 
    //more than 1.5 warps more extremely than strictly necessary
    dx=input.at(0)-1; /// "scaled utopia" lies at 1,1
    dy=input.at(1)-1; /// "scaled utopia" lies at 1,1
    theta=atan2(dy,dx);
    radius = sqrt(dx*dx + dy*dy);
    
    /// input is at (radius,theta) with respect to the utopia point.
    /// We reduce this radius as a function of angle and assign that value to output.
    radius = radius - factor*cos(theta)*sin(theta);
    
    /// Assign transformed values to output vector.
    output.clear();
    output.push_back(1+radius*cos(theta));
    output.push_back(1+radius*sin(theta));
}

void Trig_Transformation::execute_transform(vector<double>* pinputs){
    /// Public method for performing the whole transformation.
    take_input(pinputs);
    //cout << "Inputs: " << input.at(0) << "\t" << input.at(1) << endl;
    scale();
    trig_transform();
    give_output(pinputs);
    //cout << "Scaled Inputs: " << input.at(0) << "\t" << input.at(1) << endl;
    //cout << "Outputs: " << output.at(0) << "\t" << output.at(1) << endl;
}

void trig_testing(){
    FILE* infile; infile = fopen("batch_inputs.txt","w");
    FILE* outfile; outfile = fopen("batch_outputs.txt","w");
    Trig_Transformation T;
    
    vector<double> one;
    vector<double> two; 
    one.push_back(30.0);
    one.push_back(1.0);
    two.push_back(11.0);
    two.push_back(124.0);
    
    T.take_objective_max(one);
    T.take_objective_max(two);
    
    T.calc_utopia();
    T.calc_nadir();
    
    int TIMEMAX=30;
    int TREASUREMAX=124;
    for(int time_test = 11; time_test < TIMEMAX; time_test++){
        for(int treasure_test=1; treasure_test < TREASUREMAX; treasure_test++){
    vector<double> in;
    vector<double>* pin= &in;
    in.push_back(time_test);
    in.push_back(treasure_test);
    
    cout << "Before: ";
    for(int obj=0; obj<OBJECTIVES; obj++){
        cout << in.at(obj) << "\t";
    }
    cout << endl;
    
    fprintf(infile,"%.5f\t", in.at(0));
    fprintf(infile, "%.5f\n", in.at(1));
    
    
    T.execute_transform(pin);
    
    cout << "After: ";
    for(int obj=0; obj<OBJECTIVES; obj++){
        cout << in.at(obj) << "\t";
    }
    cout << endl;
    fprintf(outfile,"%.5f\t", in.at(0));
    fprintf(outfile, "%.5f\n", in.at(1));
    }
    }
    fflush(infile);
    fflush(outfile);
}

#endif	/* TRIG_TRANSFORMATION_H */

