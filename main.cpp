//
//  main.cpp
//  CS 31 Project 5
//
//  Created by Guoran Ye on 5/17/18.
//  Copyright © 2018 Guoran Ye. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void changeLine(char& c, int& i, ostream& outf){
    outf << "\n";
    if (c != ' '){
        outf << c;
        i = 1;
    }
    else{
        i = 0;
    }
}

void portion(char firstChars[]; int i){
    
}

int stuff(int lineLength, istream& inf, ostream& outf){
    char c;
    char wordPortion[140];
    int i = 0;
    int j = 0;
    int situation = 0;
    if (lineLength < 1){
        return 2;
    }
    while ( inf.get(c) )
    {
        while (c != ' '){
            wordPortion[j] = c;
            j++;
        }
        if (j <= lineLength){
            for (int k = 0; k < j; k++){
                outf << wordPortion[k];
            }
            
        }
        else{
            situation = 1;
            for (int k = 0; k < lineLength; k++){
                outf << wordPortion[k];
            }
        }
        
        
        if (c == '\n'){
            c = ' ';
        }
        if (i < lineLength - 1){
            outf << c;
            i++;
        }
        else if (i == lineLength - 1){
            if (c != ' '){
                outf << c;
            }
            i++;
        }
        else{
            changeLine(c, i, outf);
        }
    }
    return 0;
}

int main()
{
    const int MAX_FILENAME_LENGTH = 100;
    for (;;)
    {
        cout << "Enter input file name (or q to quit): ";
        char filename[MAX_FILENAME_LENGTH];
        cin.getline(filename, MAX_FILENAME_LENGTH);
        if (strcmp(filename, "q") == 0)
            break;
        ifstream infile(filename);
        if (!infile)
        {
            cerr << "Cannot open " << filename << "!" << endl;
            continue;
        }
        cout << "Enter maximum line length: ";
        int len;
        cin >> len;
        cin.ignore(10000, '\n');
        int returnCode = stuff(len, infile, cout);
        cout << "Return code is " << returnCode << endl;
    }
}
