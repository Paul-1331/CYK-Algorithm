#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <sstream>

using namespace std;

// Represents a rule: A -> BC or A -> a
// Grammar must be CNF
struct Rule{
    string lhs;
    string rhs1;
    string rhs2; // Empty if it's a terminal rule
    bool isTerminal;
};

void cykParser(string input, vector<Rule> grammar, string startSymbol){
    int n = input.length();
    // DP Table: table[length][start_index]
    //table[len][i] = set of variables that can generate substring of length len starting at position of i
    vector<vector<set<string>>> table(n + 1, vector<set<string>>(n));

    // Step 1: Fill the base case (length 1)
    for (int i = 0; i < n; i++){
        string terminal(1, input[i]);
        for (const auto& rule : grammar) {
            if (rule.isTerminal && rule.rhs1 == terminal){
                table[1][i].insert(rule.lhs);
            }
        }
    }

    // Step 2: Fill the table for lengths 2 to n
    for (int len = 2; len <= n; len++){       // Length of substring
        for (int i = 0; i <= n - len; i++){   // Start index
            for (int k = 1; k < len; k++){    // Split point
                // Check combinations of table[k][i] and table[len-k][i+k]
                for (const auto& B : table[k][i]){
                    for (const auto& C : table[len - k][i + k]){
                        for (const auto& rule : grammar){
                            if (!rule.isTerminal && rule.rhs1 == B && rule.rhs2 == C){
                                table[len][i].insert(rule.lhs);
                            }
                        }
                    }
                }
            }
        }
    }

    // Step 3: Check if start symbol is in the top cell
    if (table[n][0].count(startSymbol)){
        cout << "The string \"" << input << "\" is ACCEPTED by the grammar." << endl;
    } 
    else{
        cout << "The string \"" << input << "\" is REJECTED." << endl;
    }
}

bool parseRuleLine(const string& line, Rule& rule) {
    stringstream ss(line);
    vector<string> tokens;
    string token;

    while (ss >> token){
        tokens.push_back(token);
    }

    if (tokens.size() == 2){
        rule = {tokens[0], tokens[1], "", true};   // A a
        return true;
    }

    if (tokens.size() == 3){
        rule = {tokens[0], tokens[1], tokens[2], false};  // A B C
        return true;
    }

    return false;
}

int main(){
    // Example Grammar in CNF:
    // S -> AB | BC, A -> BA | a, B -> CC | b, C -> AB | a
    string S;
    cout<<"Enter the start symbol: ";
    cin>>S;
    cout<<"Enter Grammar in CNF\n";
    cout<<"Enter rules one per line. Type 'done' when finished.\n";
    cout<<"Suggestion: Give Non terminals as capital letters and terminals as small letters\n";
    cout<<"For insertion of rules of type A->BC. Type each variable seperated by space\n";
    cout<<"For insertion of rules of type A->a. Type variable and terminal seperated by space\n";
    getchar();
    vector<Rule> grammar;
    while (true) {
        string line;
        getline(cin, line);

        if (line == "done") {
            break;
        }

        Rule rule;
        if (parseRuleLine(line, rule)) {
            grammar.push_back(rule);
        } else {
            cout << "Invalid rule.\n";
        }
    }
    while(true){
        string input;
        cout<<"Input String: ";
        cin>>input;
        cykParser(input,grammar,S);
    }
    
    return 0;
}
