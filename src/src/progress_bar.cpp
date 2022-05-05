#include "progress_bar.h"

using namespace std;


ProgressBar::ProgressBar(double _total) : neededProgress(_total)
{
    this->pBarLength = 50; //I would recommend NOT changing this
    this->currUpdateVal = 0; //Do not change
    this->currentProgress = 0.0; //Do not change
    (*this).update(0.0);
}

void ProgressBar::reset(double _newTotal)
{
    this->neededProgress = _newTotal;
    this->currUpdateVal = 0; //Do not change
    this->currentProgress = 0.0; //Do not change
    this->amountOfFiller = 0;
}

void ProgressBar::update(double newProgress) 
{
    currentProgress += newProgress;
    amountOfFiller = (int)((this->currentProgress / this->neededProgress) * (double) this->pBarLength);
}

void ProgressBar::print() 
{
    this->currUpdateVal %= this->pBarUpdater.length();            
    cout << "\r" //Bring cursor to start of line
    << this->firstPartOfpBar; //Print out first part of pBar
    
    //Print out current progress
    for (int a = 0; a < this->amountOfFiller; a++) 
    { 
        cout << this->pBarFiller;
    }
    cout << this->pBarUpdater[currUpdateVal];

    for (int b = 0; b < this->pBarLength - this->amountOfFiller; b++) { //Print out spaces
        cout << " ";
    }

    cout << this->lastPartOfpBar //Print out last part of progress bar
    << " (" << this->currentProgress << "/" << this->neededProgress << ")" //This just prints out the fraction
    << flush;
    this->currUpdateVal += 1;
}
