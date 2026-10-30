/*
 * File:     Word.cpp
 * Purpose:  Implementation of the Word class (see Word.h).
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */

#include "Word.h"

/*
 * Function: Word::Word()
 * Purpose:  Default constructor - makes an empty word. Mostly here so Word
 *           can sit happily inside STL containers.
 * Input:    none
 * Output:   none (constructs the object)
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
Word::Word()
{
}

/*
 * Function: Word::Word(name, type, definition)
 * Purpose:  Builds a Word from the three pieces of text we read from the file.
 * Input:    name       - the word, e.g. "aardvark"
 *           type       - raw type code, e.g. "n"
 *           definition - the whole definition line (may hold several
 *                        definitions separated by ";  ")
 * Output:   none (constructs the object)
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
Word::Word(const std::string& name, const std::string& type, const std::string& definition)
    : name(name), type(type), definition(definition)
{
}

/*
 * Function: Word::getName
 * Purpose:  Getter for the word's name.
 * Input:    none
 * Output:   const reference to the name (no copy, there are 100k+ of these)
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
const std::string& Word::getName() const
{
    return name;
}

/*
 * Function: Word::getType
 * Purpose:  Getter for the raw type code ("n", "adj", ...).
 * Input:    none
 * Output:   const reference to the type code
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
const std::string& Word::getType() const
{
    return type;
}

/*
 * Function: Word::getDefinition
 * Purpose:  Getter for the raw definition line (all definitions together).
 * Input:    none
 * Output:   const reference to the definition text
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
const std::string& Word::getDefinition() const
{
    return definition;
}

/*
 * Function: Word::getFullType
 * Purpose:  Converts the file's short type code into a readable name.
 *           The brief's Table 2 didn't come through in my copy of the
 *           handout, so this mapping comes from the codes that actually
 *           appear in dictionary_2026S2.txt (n, adj, v, adv, prep, pn,
 *           misc, n_and_v).
 * Input:    none (uses the stored type code)
 * Output:   full type name, e.g. "noun". If we meet a code we don't know,
 *           we just hand back the raw code rather than crash or hide it.
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
std::string Word::getFullType() const
{
    if (type == "n")       return "noun";
    if (type == "v")       return "verb";
    if (type == "adj")     return "adjective";
    if (type == "adv")     return "adverb";
    if (type == "prep")    return "preposition";
    if (type == "pn")      return "proper noun";
    if (type == "n_and_v") return "noun and verb";
    if (type == "misc")    return "miscellaneous";
    return type;
}

/*
 * Function: Word::getDefinitions
 * Purpose:  Breaks the single definition line into individual definitions.
 *           Per the brief, separate definitions are divided by a semicolon
 *           followed by TWO spaces. A semicolon followed by one space is
 *           part of a normal sentence, so we must NOT split on that.
 * Input:    none (uses the stored definition line)
 * Output:   vector of definitions, in file order. Never empty - if the line
 *           has nothing in it we return it as a single entry.
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
std::vector<std::string> Word::getDefinitions() const
{
    const std::string separator = ";  ";   // semicolon + two spaces
    std::vector<std::string> result;

    size_t start = 0;
    while (true)
    {
        size_t found = definition.find(separator, start);

        // Grab the text up to the separator (or to the end if there isn't one)
        std::string piece = (found == std::string::npos)
                                ? definition.substr(start)
                                : definition.substr(start, found - start);

        // Trim stray spaces from both ends of this piece
        size_t first = piece.find_first_not_of(' ');
        size_t last = piece.find_last_not_of(' ');
        if (first != std::string::npos)
        {
            result.push_back(piece.substr(first, last - first + 1));
        }

        if (found == std::string::npos)
        {
            break;                          // that was the last piece
        }
        start = found + separator.size();   // jump past the separator and go again
    }

    if (result.empty())
    {
        result.push_back(definition);       // defensive: always return at least one
    }
    return result;
}

/*
 * Function: Word::getDefinitionCount
 * Purpose:  Tells us how many separate definitions the word has (this is the
 *           XXX in "Word Found - with XXX definitions").
 * Input:    none
 * Output:   number of definitions (always at least 1)
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
size_t Word::getDefinitionCount() const
{
    return getDefinitions().size();
}
