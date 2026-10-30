The Dictionary
Documentation

Author: Dilaksan Gnanasegaran
Date: 30/10/2026


1. HOW TO RUN (VS Code)

Step 1. Open the project folder.
In VS Code choose File > Open Folder and select the CSP2104_Assignment2 folder.
It should contain main.cpp, Dictionary.cpp and dictionary_2026S2.txt.

Step 2. Open the terminal inside VS Code.
Choose Terminal > New Terminal from the top menu (or press Ctrl and the backtick key).
Everything below is typed into this terminal, one line at a time, and you press Enter after each line.
The terminal opens in the project folder already. Type ls to confirm you can see the files.

Step 3. Check the compiler is installed. Type:

    clang++ --version

If a version number appears, carry on. If it says "command not found", type
xcode-select --install (Only for mac, please check on how to do it on windows), wait for it to finish, then try again.

Step 4. Build the program. Type:

    clang++ -std=c++14 -Wall -Wextra *.cpp -o dictionary

No output means success. A file called dictionary is created.

Step 5. Run the program. Type:

    ./dictionary

Notes:
- Run it from the project folder, because the program looks for dictionary_2026S2.txt in the current folder.
  If it cannot find it, it asks for the file path instead of crashing.
- If you change any code, repeat Step 4 before running again.
- Choose option 6 on the menu to exit.
- On Windows with Visual Studio: create a Console App, add all the .h and .cpp files, keep the dictionary
  file in the project folder, then build and run.


2. CLASSES

Word                  Holds name, type and definition. Can expand the type code and split definitions.
Dictionary            Holds the vector of Words, the menu, and Tasks 1 to 3.
AdvancedDictionary    Inherits from Dictionary. Adds Tasks 4 and 5 and the longer menu.
AlgorithmHelper       Static getRandomIndex (plus small helpers toLower and trim).
SimmilarityMeasure    Abstract base class (spelled as in the brief).
HammingDistance, LevenshteinDistance, JaroWinklerDistance    The three measures.

Dictionary declares displayMenu, getExitOption and processChoice as virtual. AdvancedDictionary overrides them,
handles options 4 and 5 itself and passes 1 to 3 back to Dictionary. AdvancedDictionary keeps a vector of
SimmilarityMeasure pointers (created in the constructor, deleted in the destructor). Smart Search loops over
the vector without needing to know which measure it is using.


3. TASK NOTES

Task 1 (Load). Each entry is found by locating its --- line and reading the next three lines. Windows line
endings are removed, and the last entry (which has no trailing ---) loads correctly. The word count in the
file header (105160) is checked against the number loaded.

Task 2 (Search). Case-insensitive. Definitions are split only on a semicolon followed by two spaces, and are
printed numbered on separate lines. Types are shown in full, in brackets, for example (noun).

Word types. Table 2 of the brief was empty in my copy, so I used the codes found in the file:
n = noun, v = verb, adj = adjective, adv = adverb, prep = preposition, pn = proper noun,
n_and_v = noun and verb, misc = miscellaneous.

Task 3 (About). Counts by type, by first letter and by word length. 100 entries start with a hyphen
(for example -able), so they are shown on their own line to keep the totals correct.

Task 4 (Guessing game). Only words with 3 or more letters and no hyphens are used. 5 lives, 3 points per
correct answer, and 1 extra life each time the score passes a multiple of 5. Scores go up in 3s, so 5 is
jumped over rather than hit; I compare score divided by 5 before and after each win. Typing /quit ends the game early.

getRandomIndex. rand() only reaches 32767 on Windows, but there are over 105,000 words. I join four 15-bit
rand() results into one large number, and discard values from the uneven tail before using modulo, so every
index is equally likely. Tested over 1,000,000 draws: the full range was reached evenly.

Task 5 (Smart Search). Shows the five closest words for each measure. Ties are broken by dictionary order.
If the search word exists, it appears first with distance 0.


4. DISTANCE MEASURES

Hamming distance. Counts the positions where two words have different letters. It is normally only for
words of equal length, so for different lengths I count the mismatches in the overlap and add one for each
extra letter on the longer word.

Levenshtein distance. The smallest number of insertions, deletions and substitutions needed to turn one word
into the other.

Jaro-Winkler distance (my own choice). I chose it because it works differently from the other two. It counts
letters that match near each other, penalises swapped letters, and gives a bonus when words start with the
same letters. People rarely get the start of a word wrong when misspelling it, so this suits dictionary
look-ups. It was also designed for short strings like names. I use distance = 1 minus similarity, so 0 means
identical, the same as the other measures. Standard values are used: prefix limit 4, scaling factor 0.1,
bonus only when the Jaro similarity is above 0.7. It matches published examples (MARTHA/MARHTA = 0.9611,
DIXON/DICKSONX = 0.8133).


5. TESTING

Compiled with -Wall -Wextra -pedantic with no warnings, and checked with AddressSanitizer and UBSan with no
problems. Tested: every menu option, invalid input, tasks run before loading, missing file, upper-case
searches, words with one and two definitions, statistics compared with an independent count, a scripted
guessing game (bonus lives, wrong answers, game over), and the distance functions against known values.


6. REFERENCES

Hamming, R. W. (1950). Error detecting and error correcting codes. Bell System Technical Journal, 29(2), 147-160.

Levenshtein, V. I. (1966). Binary codes capable of correcting deletions, insertions and reversals. Soviet Physics Doklady, 10(8), 707-710.

Jaro, M. A. (1989). Advances in record-linkage methodology as applied to matching the 1985 census of Tampa, Florida. Journal of the American Statistical Association, 84(406), 414-420.

Winkler, W. E. (1990). String comparator metrics and enhanced decision rules in the Fellegi-Sunter model of record linkage. Proceedings of the Section on Survey Research Methods, American Statistical Association, 354-359.

Wikipedia contributors. (n.d.). Hamming distance. https://en.wikipedia.org/wiki/Hamming_distance

Wikipedia contributors. (n.d.). Levenshtein distance. https://en.wikipedia.org/wiki/Levenshtein_distance

Wikipedia contributors. (n.d.). Jaro-Winkler distance. https://en.wikipedia.org/wiki/Jaro%E2%80%93Winkler_distance

The code was written by me from these descriptions, with no source code copied. Dictionary data is from the
GCIDE project, under the GNU GPL, as supplied.
