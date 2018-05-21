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

void printWordPortion(int& pL, ostream& outf, char wordPortion[]);
void printWordPortion2(int& lL, int& pL, ostream& outf, char wordPortion[]);
void printWordPortion3(int situation, int lineLength, int lL, int pL, ostream& outf, char wordPortion[]);

void printWordPortion(int& pL, ostream& outf, char wordPortion[]){
    for (int k = 0; k < pL; k++){
        outf << wordPortion[k];
    }
}

void printWordPortion2(int& lL, int& pL, ostream& outf, char wordPortion[]){
    lL = 0;
    for (int k = 0; k < pL; k++){
        outf << wordPortion[k];
        lL++;
    }
}

void printWordPortion3(int& situation, int lineLength, int lL, int& lLa, int pL, ostream& outf, char wordPortion[], char lastChar){
    int q;
    int lLb = 0;
    if (pL <= lineLength){
        outf << '\n';
        printWordPortion(pL, outf, wordPortion);
        lLa = pL;
    }
    else{
        situation = 1; //return 1
        int t;
        int s = 0;
        if (lastChar == ' '){
            if ((t = lineLength - lL - 1) > 0){
                outf << ' ';
                printWordPortion2(s, t, outf, wordPortion); //print part of the portion
            }
        }
        else{
            if ((t = lineLength - lL) > 0){
                printWordPortion2(s, t, outf, wordPortion); //print part of the portion
            }
        }
        outf << '\n';
        for (q = 0; q < ((pL - s)/lineLength); q++){ //print the protion in multiple lines until it fits: print the lines
            for (int k = 0; k < lineLength; k++){
                outf << wordPortion[k + s + q*lineLength];
            }
            outf << '\n';
        }
        for (int k = 0; k < (pL - s)%lineLength; k++){ //print the protion in multiple lines until it fits: print the last line
            outf << wordPortion[k + s + q*lineLength];
            lLb++;
        }
        lLa = lLb;
    }
}

int stuff(int lineLength, istream& inf, ostream& outf){
    char c;
    char d;
    char lastChar = '-';
    char nextLastChar;
    char wordPortion[140];
    int pL = 0;
    int lL = 0;
    int situation = 0;
    if (lineLength < 1){
        return 2;
    }
    while (inf.get(c)){
        if (c == '\n'){ //turn enter into space
            d = ' ';
        }
        else{
            d = c;
        }
        if ((d != ' ') && (d != '-')){ //get the next word portion
            wordPortion[pL] = d;
            pL++;
        } //the printing only happens when a portion is stored
        else{
            if (d == ' '){
                nextLastChar = ' ';
            }
            else{
                wordPortion[pL] = d;
                pL++;
                nextLastChar = '-';
            }
            if (pL < lineLength - lL){ //if fit in the line even if adding a space, print the portion
                if (lastChar == ' '){ //if it's the first in a line, or following a portion from the same word
                    if (lastChar == '-'){
                        printWordPortion(pL, outf, wordPortion); //print it right after a space
                    }
                    else{
                        outf << ' ';
                        lL++;
                        printWordPortion(pL, outf, wordPortion); //print it right after
                    }
                }
                else{ //if it's the frist of a word and not the first of a line, print a space before
                    printWordPortion(pL, outf, wordPortion); //print it right after
                }
                lastChar = nextLastChar;
                lL += pL;
            }
            else if (pL == lineLength - lL){ //if fit in line only if not adding a space     problem of repeating '\n's
                if (lastChar == '-'){ //if it's the first in a line, or following a portion from the same word
                    printWordPortion(pL, outf, wordPortion); //print it right after
                    outf << '\n';
                    lastChar = '-';
                    lL = 0;
                }
                else{
                    outf << '\n';
                    printWordPortion2(lL, pL, outf, wordPortion); //print it right after an enter and keep track of lL
                    lastChar = nextLastChar;
                    lL = lL + pL;
                }
            }
            else{ //if not fit in a line
                int lLa;
                printWordPortion3(situation, lineLength, lL, lLa, pL, outf, wordPortion, lastChar);
                lL = lLa;
                lastChar = nextLastChar;
            }
            pL = 0;
        }
    }
    if (pL < lineLength - lL){ //if fit in the line even if adding a space, print the portion
        if (lastChar == ' '){ //if it's the first in a line, or following a space
            if (lastChar == ' '){
                outf << ' ';
                lL++;
                printWordPortion(pL, outf, wordPortion); //print it right after a space
            }
            else{
                printWordPortion(pL, outf, wordPortion); //print it right after
            }
        }
        else{
            printWordPortion(pL, outf, wordPortion);
            outf << '\n';
        }
    }
    else if (pL == lineLength - lL){ //if fit in line only if not adding a space
        if (lastChar == '\n' || lastChar == '-'){ //if it's the first in a line, or following a portion from the same word
            printWordPortion(pL, outf, wordPortion);
            outf << '\n';
        }
        else{
            outf << '\n';
            printWordPortion(pL, outf, wordPortion);
            outf << '\n';
        }
    }
    else{ //if not fit in a line
        int lLa;
        printWordPortion3(situation, lineLength, lL, lLa, pL, outf, wordPortion, lastChar);
        lL = lLa;
        outf << '\n';
    }
    return situation;
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
