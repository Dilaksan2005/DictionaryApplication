/*
 * File:     Dictionary.h
 * Purpose:  Declares the Dictionary class. It owns the vector of Words, shows
 *           the menu, reads the user's choice and performs Tasks 1-3
 *           (load, search, statistics).
 *
 *           AdvancedDictionary inherits from this and adds Tasks 4-5, so the
 *           menu-related functions are virtual and the data is protected.
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <vector>
#include "Word.h"

class Dictionary
{
public:
    Dictionary();
    virtual ~Dictionary();

    // Main loop: show menu, get choice, do the task, repeat until the user exits
    void run();

protected:
    // ---- menu handling (virtual so AdvancedDictionary can extend them) ----
    virtual void displayMenu() const;
    virtual int getExitOption() const;
    virtual void processChoice(int choice);

    // ---- Tasks 1 to 3 ----
    void loadDictionary();      // Task 1
    void searchWord();          // Task 2
    void aboutDictionary();     // Task 3

    // ---- helpers shared with the derived class ----
    bool readLine(const std::string& prompt, std::string& output) const;
    bool requireLoaded() const;
    static bool parseInteger(const std::string& text, int& number);

    // The dictionary itself. Protected so AdvancedDictionary can read it.
    std::vector<Word> words;

private:
    bool loadFromFile(const std::string& path);
};

#endif
