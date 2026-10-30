/*
 * File:     AdvancedDictionary.h
 * Purpose:  Declares AdvancedDictionary, which extends Dictionary (inheritance)
 *           with Task 4 (Guessing Game) and Task 5 (Smart Search) and
 *           shows the updated, longer menu.
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */

#ifndef ADVANCEDDICTIONARY_H
#define ADVANCEDDICTIONARY_H

#include <vector>
#include "Dictionary.h"
#include "SimmilarityMeasure.h"

class AdvancedDictionary : public Dictionary
{
public:
    AdvancedDictionary();
    ~AdvancedDictionary() override;

    // This class owns raw pointers, so copying it would lead to a double
    // delete. Nobody needs to copy it, so I just forbid it.
    AdvancedDictionary(const AdvancedDictionary&) = delete;
    AdvancedDictionary& operator=(const AdvancedDictionary&) = delete;

protected:
    void displayMenu() const override;
    int getExitOption() const override;
    void processChoice(int choice) override;

private:
    void guessingGame();    // Task 4
    void smartSearch();     // Task 5

    // The brief asks for a vector of pointers to the measure objects.
    // We create them in the constructor and delete them in the destructor.
    std::vector<SimmilarityMeasure*> measures;
};

#endif
