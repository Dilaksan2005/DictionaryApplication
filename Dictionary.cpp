/*
 * File:     Dictionary.cpp
 * Purpose:  Implementation of the Dictionary class (Tasks 1-3 and the menu).
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */

#include "Dictionary.h"
#include "AlgorithmHelper.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <array>

// The file we try first when the user picks "Load". It should sit next to the .exe
// (or in the project folder when running from Visual Studio).
static const char* DEFAULT_DICTIONARY_FILE = "dictionary_2026S2.txt";

/*
 * Function: Dictionary::Dictionary
 * Purpose:  Constructor. Reserves room in the vector up front so loading
 *           ~105,000 words doesn't keep re-allocating and copying.
 * Input:    none
 * Output:   none (constructs the object)
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
Dictionary::Dictionary()
{
    words.reserve(110000);
}

/*
 * Function: Dictionary::~Dictionary
 * Purpose:  Virtual destructor (nothing to clean up here - the vector looks
 *           after itself - but it must be virtual because AdvancedDictionary
 *           derives from us).
 * Input:    none
 * Output:   none
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
Dictionary::~Dictionary()
{
}

/*
 * Function: Dictionary::readLine
 * Purpose:  Prints a prompt and reads a whole line from the keyboard. I read
 *           whole lines with getline (rather than cin >>) so that typing
 *           something silly like "abc" for a number can't wreck the input
 *           stream, and so stray newlines don't leak into the next prompt.
 * Input:    prompt - text to show the user
 *           output - receives the line, with surrounding whitespace removed
 * Output:   true if a line was read; false if input has ended (Ctrl+Z / Ctrl+D),
 *           which lets the callers bail out instead of looping forever
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
bool Dictionary::readLine(const std::string& prompt, std::string& output) const
{
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line))
    {
        return false;
    }
    output = AlgorithmHelper::trim(line);
    return true;
}

/*
 * Function: Dictionary::parseInteger
 * Purpose:  Safely turns text into an int. stoi alone would happily accept
 *           "3abc" as 3, so I also check that the whole string was used.
 * Input:    text   - what the user typed
 *           number - receives the value on success
 * Output:   true if the text was a valid whole number, otherwise false
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
bool Dictionary::parseInteger(const std::string& text, int& number)
{
    try
    {
        size_t used = 0;
        int value = std::stoi(text, &used);
        if (used != text.size())
        {
            return false;       // there was junk after the digits
        }
        number = value;
        return true;
    }
    catch (...)                 // stoi throws if it's not a number at all, or too big
    {
        return false;
    }
}

/*
 * Function: Dictionary::requireLoaded
 * Purpose:  Tasks 2-5 are pointless on an empty dictionary, so they call this
 *           first. If nothing has been loaded it tells the user what to do.
 * Input:    none
 * Output:   true if the dictionary has words, false (with a message) if not
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
bool Dictionary::requireLoaded() const
{
    if (words.empty())
    {
        std::cout << "\nNo dictionary is loaded yet - please choose option 1 (Load new Dictionary) first.\n";
        return false;
    }
    return true;
}

/*
 * Function: Dictionary::displayMenu
 * Purpose:  Prints the list of tasks. AdvancedDictionary overrides this to
 *           show its longer menu.
 * Input:    none
 * Output:   none (prints to the screen)
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
void Dictionary::displayMenu() const
{
    std::cout << "\n========== DICTIONARY MENU ==========\n"
              << "  1. Load new Dictionary\n"
              << "  2. Search Word\n"
              << "  3. About Dictionary\n"
              << "  " << getExitOption() << ". Exit\n"
              << "=====================================\n";
}

/*
 * Function: Dictionary::getExitOption
 * Purpose:  Says which menu number means "Exit". It's a function (and
 *           virtual) because the advanced menu has more items, so Exit moves
 *           from 4 to 6 and the loop in run() needs to know.
 * Input:    none
 * Output:   the menu number for Exit
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
int Dictionary::getExitOption() const
{
    return 4;
}

/*
 * Function: Dictionary::processChoice
 * Purpose:  Runs the task that matches the user's menu number (Tasks 1-3).
 *           The derived class overrides this, handles its own numbers, and
 *           passes everything else back here.
 * Input:    choice - the number the user entered
 * Output:   none (the chosen task does its own printing)
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
void Dictionary::processChoice(int choice)
{
    switch (choice)
    {
    case 1: loadDictionary();  break;
    case 2: searchWord();      break;
    case 3: aboutDictionary(); break;
    default:
        std::cout << "\nThat isn't a menu option - please pick a number from the list.\n";
        break;
    }
}

/*
 * Function: Dictionary::run
 * Purpose:  The program's main loop. Show the menu, read a number, do the
 *           task, then go straight back to the menu - until the user picks
 *           Exit (or input runs out).
 * Input:    none
 * Output:   none
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
void Dictionary::run()
{
    while (true)
    {
        displayMenu();

        std::string input;
        if (!readLine("Enter the number of your choice: ", input))
        {
            break;      // no more input, so quit rather than spin forever
        }

        int choice = 0;
        if (!parseInteger(input, choice))
        {
            std::cout << "\nPlease type a number, e.g. 1.\n";
            continue;
        }

        if (choice == getExitOption())
        {
            std::cout << "\nGoodbye!\n";
            break;
        }

        processChoice(choice);
    }
}

/*
 * Function: Dictionary::loadFromFile (private)
 * Purpose:  Does the real work of Task 1 for one specific file path.
 *
 *           File layout (from the brief's appendix):
 *               <header line saying how many words there are>
 *               <blank line>
 *               ---
 *               type
 *               name
 *               definition(s) on one line
 *               <blank line>
 *               ---
 *               ...and so on
 *           Rather than counting lines in fours and hoping nothing drifts, I
 *           hunt for each "---" marker and then read the next three lines.
 *           That way a stray blank line can't throw the whole parse out. It
 *           also copes with the very last word, which in the real file has no
 *           blank line or "---" after it.
 *
 *           The file uses Windows (CRLF) line endings, so every line gets its
 *           trailing '\r' stripped - otherwise we'd end up with words like
 *           "aardvark\r" that never match anything.
 * Input:    path - file to open
 * Output:   true if the file opened and at least one word was loaded; false
 *           otherwise. Any previously loaded words are replaced on success.
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
bool Dictionary::loadFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        return false;
    }

    // Load into a temporary vector so a failed load can't wipe a good dictionary
    std::vector<Word> loaded;
    loaded.reserve(110000);

    // ---- header line: "2026-S2 dictionary contains 105160 words" ----
    std::string header;
    std::getline(file, header);
    header = AlgorithmHelper::trim(header);

    // Pull the promised word count out of the header so we can sanity-check ourselves later
    size_t expected = 0;
    size_t countStart = header.find_first_of("0123456789", header.find("contains"));
    if (header.find("contains") != std::string::npos && countStart != std::string::npos)
    {
        try { expected = std::stoul(header.substr(countStart)); }
        catch (...) { expected = 0; }
    }

    // ---- the entries ----
    std::string line;
    while (std::getline(file, line))
    {
        if (AlgorithmHelper::trim(line) != "---")
        {
            continue;       // header gap, blank line, etc. - keep looking for a marker
        }

        std::string type, name, definition;
        if (!std::getline(file, type) || !std::getline(file, name) || !std::getline(file, definition))
        {
            break;          // file ended mid-entry, nothing sensible to add
        }

        // trim() removes the '\r' (and any stray spaces) from each line
        loaded.push_back(Word(AlgorithmHelper::trim(name),
                              AlgorithmHelper::trim(type),
                              AlgorithmHelper::trim(definition)));
    }

    if (loaded.empty())
    {
        return false;
    }

    words.swap(loaded);     // out with the old dictionary, in with the new

    std::cout << "Loaded " << words.size() << " words from \"" << path << "\".\n";
    if (expected != 0 && expected != words.size())
    {
        std::cout << "Warning: the file header promised " << expected
                  << " words but I found " << words.size() << ".\n";
    }
    return true;
}

/*
 * Function: Dictionary::loadDictionary
 * Purpose:  Task 1 - Load new Dictionary. Tries the default file name first;
 *           if that isn't found, asks the user for a path (handy because
 *           Visual Studio's working directory is often not where you'd expect).
 * Input:    none (may prompt for a file path)
 * Output:   none (fills the words vector and prints how many were loaded)
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
void Dictionary::loadDictionary()
{
    std::cout << "\nLoading dictionary...\n";

    if (loadFromFile(DEFAULT_DICTIONARY_FILE))
    {
        return;
    }

    std::cout << "Couldn't open \"" << DEFAULT_DICTIONARY_FILE << "\" in the current folder.\n";

    std::string path;
    if (!readLine("Please type the full path to the dictionary file (or press Enter to cancel): ", path)
        || path.empty())
    {
        std::cout << "Load cancelled.\n";
        return;
    }

    // People often paste paths with quotes around them (Windows "Copy as path") - strip them
    if (path.size() >= 2 && path.front() == '"' && path.back() == '"')
    {
        path = path.substr(1, path.size() - 2);
    }

    if (!loadFromFile(path))
    {
        std::cout << "Sorry, I couldn't load any words from \"" << path << "\".\n";
    }
}

/*
 * Function: Dictionary::searchWord
 * Purpose:  Task 2 - Search Word. Asks for a word (case-insensitive) and
 *           prints "Word Found - with N definitions" followed by the name,
 *           the type in brackets and each definition on its own line.
 *           Otherwise prints "word not found.".
 *           The dictionary is stored in lower case, so I just lower-case what
 *           the user typed. A plain linear scan is plenty fast for ~105k words.
 * Input:    none (prompts for the word)
 * Output:   none (prints the result)
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
void Dictionary::searchWord()
{
    if (!requireLoaded())
    {
        return;
    }

    std::string input;
    if (!readLine("\nEnter a word to search for: ", input))
    {
        return;
    }
    std::string target = AlgorithmHelper::toLower(input);

    for (size_t i = 0; i < words.size(); i++)
    {
        if (words[i].getName() != target)
        {
            continue;
        }

        std::vector<std::string> definitions = words[i].getDefinitions();

        // "\xE2\x80\x93" is the UTF-8 en dash used in the brief's wording
        std::cout << "\nWord Found \xE2\x80\x93 with " << definitions.size()
                  << (definitions.size() == 1 ? " definition" : " definitions") << "\n";
        std::cout << "Word:       " << words[i].getName() << "\n";
        std::cout << "Type:       (" << words[i].getFullType() << ")\n";

        for (size_t d = 0; d < definitions.size(); d++)
        {
            // Number them when there's more than one so they're clearly separated
            if (definitions.size() > 1)
            {
                std::cout << "\nDefinition " << (d + 1) << ": " << definitions[d] << "\n";
            }
            else
            {
                std::cout << "Definition: " << definitions[d] << "\n";
            }
        }
        return;
    }

    std::cout << "\nword not found.\n";
}

/*
 * Function: Dictionary::aboutDictionary
 * Purpose:  Task 3 - About Dictionary. Prints three sets of statistics:
 *             (a) how many words there are of each word type,
 *             (b) how many words start with each letter,
 *             (c) how many words are 1 letter long, 2 letters long, etc.
 *           Word length is the length of the name as written, so a hyphen
 *           counts as a character ("aard-wolf" is 9).
 *           A few entries are prefixes/suffixes that START with a hyphen
 *           ("-able"). They don't begin with a letter, so rather than drop
 *           them silently I count them on their own line - that way the
 *           letter counts add up to the full total.
 * Input:    none
 * Output:   none (prints the statistics)
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
void Dictionary::aboutDictionary()
{
    if (!requireLoaded())
    {
        return;
    }

    std::map<std::string, size_t> typeCounts;      // keyed by full type name -> alphabetical output
    std::array<size_t, 26> letterCounts;
    letterCounts.fill(0);
    size_t startsWithHyphen = 0;
    std::map<size_t, size_t> lengthCounts;         // word length -> how many words

    // One pass over the dictionary gathers all three statistics
    for (size_t i = 0; i < words.size(); i++)
    {
        const std::string& name = words[i].getName();

        typeCounts[words[i].getFullType()]++;
        lengthCounts[name.size()]++;

        if (!name.empty() && name[0] >= 'a' && name[0] <= 'z')
        {
            letterCounts[name[0] - 'a']++;
        }
        else
        {
            startsWithHyphen++;
        }
    }

    std::cout << "\n========== ABOUT THIS DICTIONARY ==========\n";
    std::cout << "Total words: " << words.size() << "\n";

    std::cout << "\n--- Total instances of each word type ---\n";
    for (std::map<std::string, size_t>::const_iterator it = typeCounts.begin(); it != typeCounts.end(); ++it)
    {
        std::cout << "  " << std::left << std::setw(16) << it->first << std::right << std::setw(8) << it->second << "\n";
    }

    std::cout << "\n--- Number of words beginning with each letter ---\n";
    for (int c = 0; c < 26; c++)
    {
        std::cout << "  " << static_cast<char>('a' + c) << std::setw(10) << letterCounts[c] << "\n";
    }
    if (startsWithHyphen > 0)
    {
        // Same column layout as the letters above; these are prefix/suffix entries like "-able"
        std::cout << "  -" << std::setw(10) << startsWithHyphen << "   (entries starting with a hyphen)\n";
    }

    std::cout << "\n--- Number of words of each length (letters) ---\n";
    for (std::map<size_t, size_t>::const_iterator it = lengthCounts.begin(); it != lengthCounts.end(); ++it)
    {
        std::cout << "  " << std::setw(2) << it->first << (it->first == 1 ? " letter :" : " letters:")
                  << std::setw(8) << it->second << "\n";
    }
}
