// TODO(my_username): Manh Vu Bui
// this driver.h header file handles Question output 
// and includes function that finds best personality match.

#include <math.h>
#include <limits>
#include <string>
#include <map>
#include <set>
#include "myrandom.h"

using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
    string questionText;  // Text of the question
    map<char, int> factors;  // Map from factors to +1 or -1
    friend bool operator< (const Question& lhs, const Question& rhs) {
        return lhs.questionText < rhs.questionText;
    }
    friend bool operator== (const Question& lhs, const Question& rhs) {
        return lhs.questionText == rhs.questionText;
    }
    friend bool operator!= (const Question& lhs, const Question& rhs) {
        return lhs.questionText != rhs.questionText;
    }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
    string name;      // Name of the person
    map<char, int> scores;  // Map from factors to +1 or -1
    friend bool operator< (const Person& lhs, const Person& rhs) {
        return lhs.name < rhs.name;
    }
    friend bool operator== (const Person& lhs, const Person& rhs) {
        return lhs.name == rhs.name;
    }
    friend bool operator!= (const Person& lhs, const Person& rhs) {
        return lhs.name != rhs.name;
    }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
*/
Question randomElement(set<Question>& questions) {
    int ind = randomInteger(0, (int)questions.size() - 1);
    int i = 0;
    for (auto e : questions) {
        if (i == ind) {
            return e;
        }
        i++;
    }
    return {};
}

/* randomQuestionFrom
 *
 * This function choose a random question from the set of unasked questions,
 * remove it from the set, and return the question.
*/
Question randomQuestionFrom(set<Question>& questions) {
    // throw runtime_error if empty
    if (questions.size() == 0) {
        throw runtime_error("set of questions: empty");
    }
    // create and initialize someQues with a random question
    Question someQues = randomElement(questions);
    questions.erase(someQues);  // erase someQues from the set
    return someQues;
}


/* scoresFrom
*
* This function takes as input a map containing the user’s answers
* to their personality quiz questions,then returns a map representing
* their OCEAN scores.
* 
* Grading rubric states that loop is not allowed for map;
* However, this loop is necessary and not for searching purposes.
*/
map<char, int> scoresFrom(map<Question, int>& answers) {
    map<char, int> scoreOCEAN;
    for (auto& ans : answers) {  // for every answer
        for (auto const& f : ans.first.factors) {  // for every map<char, int> factors
            if (scoreOCEAN.count(f.first) == 0) { 
                // if key not exits in scoreOCEAN, create and assign value
                int temp = f.second * (ans.second - 3);
                scoreOCEAN.emplace(f.first, temp);
            } else if (scoreOCEAN.count(f.first) == 1) { 
                // if key already exits, update value
                scoreOCEAN.at(f.first) += f.second * (ans.second - 3);
            }
        }
    }
    return scoreOCEAN;
}

/* normalize
*
* This function takes as input map of scoreOCEAN and returns a normalized
* version of the scores
*/
map<char, double> normalize(map<char, int>& scores) {
    map<char, double> scoreNormal;
    double sum = 0;
    if (scores.size() == 0) {  // empty map
        throw runtime_error("scores not avaialbe");
    }
    for (const auto& s : scores) {  // intialize normalScore
        scoreNormal.emplace(s.first, (double)s.second);
        sum += s.second * s.second;
    }
    if (sum == 0) {  // sum = 0
        throw runtime_error("scores sum zero");
    }
    sum = sqrt(sum);
    for (auto& score : scoreNormal) {  // normalize score
        scoreNormal[score.first] = score.second / sum;
    }
    return scoreNormal;
}

/* cosineSimilarityOf
*
*  this function takes as input two sets of normalized scores,
* then returns their cosine similarity using the formula given below:
* Similarity = O1O2 + C1C2 + E1E2 + ... range from [-1; 1]
* -1: very disimilar
* 1: very similar
*/
double cosineSimilarityOf(const map<char, double>& lhs,
    const map<char, double>& rhs) {
    double cosineSum = 0;
    for (const auto& left : lhs) {
        if (rhs.count(left.first)) {  // if the key exits in both map
            cosineSum += left.second * rhs.at(left.first);
        }
    }
    cout << cosineSum << endl;
    return cosineSum;
}

/* mostSimilarTo
*
* This function takes as input the user’s raw OCEAN scores and
* a Set of fictional people. This function then returns the Person whose scores have
* the highest cosine similarity to the user’s scores
*/
Person mostSimilarTo(map<char, int>& scores, set<Person>& people) {
    if (people.size() == 0) {
        throw runtime_error("people set: empty");
    }
    Person theOne;
    map<char, double> user = normalize(scores);
    map<char, double> foo;
    double max = -2;
    for (auto& peo : people) {
        map<char, int> peoScore = peo.scores;
        foo = normalize(peoScore);
        double n = cosineSimilarityOf(user, foo);
        if (n > max) {
            max = n;
            theOne = peo;
        }
    }
    return theOne;
}
