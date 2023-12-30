// TODO(my_username): Manh Vu Bui
// Project 2: Personality Tests

#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "driver.h"

using namespace std;

/* openFile() : this function open 'filename' file,
* and check for its validity 
* Reused code from Project 1, cool feature to have.
 */
bool openFile(ifstream& inFile, string& filename) {
	// open file
	inFile.open(filename);
	// check if file is valid
	if (!inFile.is_open()) {
		cout << "Error: unable to open '" << filename << "'" << endl;
		return false;
	} else {
		return true;
	}
}

/* toStrMap:
*
* this function takes in a string and 
* extract name and map within that string
 */
void toStrMap(string& text, string& title, map<char, int>& mapFoo) {
	stringstream ss(text);
	getline(ss, text, '.');
	if (!ss.fail()) {
		title = text;  // extract name
		while (ss >> text) {  // extract map key and corresponding value
			int foo1 = stoi(text.substr(2));
			mapFoo.emplace(text.at(0), foo1);
		}
	}
}


/* getInput:
*
* this function get questions from the text file
* and add it into the question library
 */
void getInput(set<Question>& quesLib) {
	ifstream infile;
	string filename = "questions.txt";
	openFile(infile, filename);
	while (!infile.eof()) {
		Question newQues;
		string name;
		map<char, int> someMap;

		string text;
		getline(infile, text);
		if (infile.fail()) break; //  terminate at empty line
		toStrMap(text, name, someMap);  // extract name and map
		newQues.questionText = name + ".";
		newQues.factors = someMap;
		quesLib.insert(newQues);
	}

	infile.close();
}

/* getNumOfQuestion:
*
* this function welcome and ask user to input 
* the number of question.
 */
int getNumOfQuestion() {
	int n;
	cout << "Welcome to the Personality Quiz!\n" << endl;
	cout << "Choose number of questions: ";
	cin >> n;
	cout << endl;
	return n;
}

/* askQuestion:
*
* this function prints question 
* and update map of userAnswers
 */
void askQuestion(set<Question>& quesLib,
	map<Question, int>& userAnswers) {
	Question ques;
	int n;
	cout << "How much do you agree with this statement?" << endl;
	ques = randomQuestionFrom(quesLib);
	cout << "\"" << ques.questionText << "\"\n" << endl;
	cout << "1. Strongly disagree" << endl;
	cout << "2. Disagree" << endl;
	cout << "3. Neutral" << endl;
	cout << "4. Agree" << endl;
	cout << "5. Strongly agree\n" << endl;
	cout << "Enter your answer here (1-5): ";
	cin >> n;
	cout << endl;
	userAnswers.emplace(ques, n);
}

/* findfilename:
*
* this function takes in the category number and return 
* the name of the corresponding *.people file.
* this function also helps print the category choosing options.
 */
string findFilename(int& n) {
	switch (n) {
	case 1:
		return "BabyAnimals";
	case 2:
		return "Brooklyn99";
	case 3:
		return "Disney";
	case 4:
		return "Hogwarts";
	case 5:
		return "MyersBriggs";
	case 6:
		return "SesameStreet";
	case 7:
		return "StarWars";
	case 8:
		return "Vegetables";
	case 9:
		return "mine";
	default:
		throw runtime_error("name not found");
	}
}

/* findMatch:
*
* this function takes in user chosen category, their answers
* and return the name of matched character.
 */
string findMatch(int& n, map<Question, int>& userAnswers) {
	map<char, int> userScores = scoresFrom(userAnswers);

	string filename = findFilename(n) + ".people";
	set<Person> people;
	ifstream infile;
	openFile(infile, filename);

	while (!infile.eof()) {
		Person newPerson;
		string name;
		map<char, int> someMap;

		string text;
		getline(infile, text);  // process each individual line of people
		if (infile.fail()) break;
		toStrMap(text, name, someMap);  // extract name and map
		newPerson.name = name;
		newPerson.scores = someMap;
		people.insert(newPerson);
	}
	infile.close();
	return mostSimilarTo(userScores, people).name;
}

/* chooseCategory:
*
* this function prompts user to choose a category
* they would like to be identified as.
 */
void chooseCategory(map<Question, int>& userAnswers) {
	int n = -1;
	while (true) {
		for (int i = 1; i < 10; i++) {
			cout << i << ". " << findFilename(i) << endl;
		}
		cout << "0. To end program.\n" << endl;
		cout << "Choose test number (1-9, or 0 to end): ";
		cin >> n;
		if (n == 0) {
			cout << "Goodbye!" << endl;
			break;
		}
		cout << "You got " << findMatch(n, userAnswers) << "!\n" << endl;
	}
}

// main:
int main() {
	// Generate question library
	set<Question> quesLib;
	map<Question, int> userAnswers;
	getInput(quesLib);
	int n = getNumOfQuestion();
	while (n > 0) {
		askQuestion(quesLib, userAnswers);
		n--;
	}
	chooseCategory(userAnswers);
	return 0;
}
