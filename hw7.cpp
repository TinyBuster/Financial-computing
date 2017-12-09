#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <map>
using namespace std;

struct Contract {
    int quantity_before_noon;
    int quantity_after_noon;
    vector<double> range_of_price;
    double max_price;
    double min_price;
    double change_in_price;
};

vector<string> splitstring(string , string);

int main(int argc, char** argv) {
    ifstream file;
    int row = 0;
    vector<string> symbol, maturity, date, timestamp;
    vector<double> Price;
    vector<int> number_of_contracts;
    vector< pair<string,string> > symbol_maturity_pair;
    map<pair<string,string>, Contract> contract;
    set<string> symbolList;
    
    string line;
    string delimiter(",");
    string noon("12:00:00:000");
    
    file.open("Trades_2017_10_27.csv");
    if (file.is_open()) {  
        while (!file.eof()) {
            getline(file, line);
            row++;
            
            vector<string> list = splitstring(line, delimiter);
            symbol.push_back(list[0]);
            symbolList.insert(list[0]);
            maturity.push_back(list[1]);
            date.push_back(list[2]);
            timestamp.push_back(list[3]);
            Price.push_back(stof(list[4]));
            number_of_contracts.push_back(stoi(list[5]));
        }
    }
    
    for (int i = 0; i < row; i++) {
        symbol_maturity_pair.push_back(pair<string,string>(symbol[i],maturity[i]));
        
        contract[symbol_maturity_pair[i]].range_of_price.push_back(Price[i]);
        if (timestamp[i] < noon) {
            contract[symbol_maturity_pair[i]].quantity_before_noon += number_of_contracts[i];
        } else {
            contract[symbol_maturity_pair[i]].quantity_after_noon += number_of_contracts[i];
        }
    }
    

    for (map<pair<string,string>, Contract>::iterator it=contract.begin(); it!=contract.end(); ++it) {
        vector<double> price = it -> second.range_of_price;
        it -> second.change_in_price = price[price.size() - 1] - price[0];
        sort(price.begin(), price.end());
        it -> second.min_price = price[0];
        it -> second.max_price = price[price.size() - 1];
    }
    
    cout << "The number of rows in the entire file: " << row << endl << endl;
    cout << "The symbol list: "  << endl;
    for (set<string>::iterator it = symbolList.begin(); it!= symbolList.end(); ++it) {
        cout << *it << " ";
    }   
    cout << endl << endl;
    cout << "The symbol/maturity pair list: "  << endl;
    for (map<pair<string,string>, Contract>::iterator it=contract.begin(); it!= contract.end(); ++it) {
        cout << it -> first.first << " " << it -> first.second << endl;
    }
    
    cout << endl;
    
    cout << "For each symbol/maturity date pair: "  << endl;
    for (map<pair<string,string>, Contract>::iterator it=contract.begin(); it!= contract.end(); ++it) {
        cout << "Pair (" << it -> first.first << ", " << it -> first.second 
                << "): traded before and after noon are " 
                << it -> second.quantity_before_noon << " and " 
                << it -> second.quantity_after_noon << " respectively." << endl 
                << "Range of prices: ("
                << it -> second.min_price << ", " << it -> second.max_price 
                << "), and the change in price is " 
                << it -> second.change_in_price << "."<< endl << endl;
    }
    
    return 0;
}

vector<string> splitstring(string str, string delimiter) {
    vector<string> list;
    string s = string(str);
    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        if (pos) {
            list.push_back(token);
        }
        s.erase(0, pos + delimiter.length());
    }
    list.push_back(s);
    return list;
}


