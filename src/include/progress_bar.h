#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <iostream>
#include <string>

using namespace std;

class ProgressBar 
{
    public:
        string firstPartOfpBar = "["; //Change these at will (that is why I made them public)
        string lastPartOfpBar = "]";
        string pBarFiller = "|";
        string pBarUpdater = "/-\\|";

        ProgressBar(double _total);

        void reset(double _newTotal);
        void update(double newProgress);
        void print();        

    private:
        int amountOfFiller;
        int pBarLength; //I would recommend NOT changing this
        int currUpdateVal; //Do not change
        double currentProgress; //Do not change
        double neededProgress; //I would recommend NOT changing this
};

#endif // !PROGRESSBAR_H