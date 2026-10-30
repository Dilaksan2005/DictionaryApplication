/*
 * File:     Word.h
 * Purpose:  Declares the Word class - one entry from the dictionary file.
 *           A Word is just a small bundle of three strings (name, type and
 *           definition) plus a couple of helpers that make the text nicer to
 *           show the user.
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */

#ifndef WORD_H
#define WORD_H

#include <string>
#include <vector>

class Word
{
public:
    Word();
    Word(const std::string& name, const std::string& type, const std::string& definition);

    const std::string& getName() const;
    const std::string& getType() const;
    const std::string& getDefinition() const;

    // Turns the short code from the file ("n", "adj"...) into the full name ("noun", "adjective"...)
    std::string getFullType() const;

    // Splits the single definition line into its separate definitions
    std::vector<std::string> getDefinitions() const;

    // How many separate definitions this word has
    size_t getDefinitionCount() const;

private:
    std::string name;        // the word itself, lower case, a-z and hyphens only
    std::string type;        // raw type code from the file, e.g. "n" or "adj"
    std::string definition;  // all definitions on ONE line, exactly as in the file
};

#endif
