//
//  main.cpp
//  CS 31 Project 5
//
//  Created by Guoran Ye on 5/17/18.
//  Copyright © 2018 Guoran Ye. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

void printWordPortion1(int& lL, int pL, int move, ostream& outf, char wordPortion[]){
    for (int k = 0; k < pL; k++){
        outf << wordPortion[k + move];
        lL++;
    }
}

void printWordPortion0(int pL, int move, ostream& outf, char wordPortion[]){
    int i = 0;
    printWordPortion1(i, pL, move, outf, wordPortion);
}

void normalSituation1(int pL, int lineLength, int& lL, ostream& outf, char wordPortion[], char lastChar, char realLastChar){
    if ((lastChar == ' ' && (realLastChar == '.' || realLastChar == '?')) && pL < lineLength - lL - 1){ //if fit in the line even if adding two space, print the portion
        outf << ' ' << ' ';
        lL = lL + 2;
    }
    else if ((lastChar == ' ' && (realLastChar == '.' || realLastChar == '?')) || (lastChar == ' ' && pL == lineLength - lL)){
        outf << '\n';
        lL = 0;
    }
    else if (lastChar == ' '){
        outf << ' ';
        lL++;
    }
    printWordPortion0(pL, 0, outf, wordPortion);
    lL = lL + pL;
}

void printWordPortion3(int& situation, int lineLength, int lL, int& lLa, int pL, ostream& outf, char wordPortion[], char lastChar, char realLastChar){
    if (pL <= lineLength){
        outf << '\n';
        printWordPortion0(pL, 0, outf, wordPortion);
        lLa = pL;
    }
    else{
        situation = 1; //return 1
        int s = 0;
        int t;
        if ((t = lineLength - lL) > 0){
            if (lastChar == ' ' && (realLastChar == '.' || realLastChar == '?') && (t = lineLength - lL - 2) > 0){
                outf << ' ' << ' ';
            }
            else if (lastChar == ' ' && (t = lineLength - lL - 1) > 0){
                outf << ' ';
            }
            printWordPortion1(s, t, 0, outf, wordPortion); //print part of the portion
        }
        outf << '\n';
        int q = 0;
        while (q < ((pL - s)/lineLength) - 1){ //print the protion in multiple lines until it fits: print the lines
            int g = s + q*lineLength;
            printWordPortion0(lineLength, g, outf, wordPortion);
            outf << '\n';
            q++;
        }
        if (pL - s < lineLength){
            int g = pL - s;
            printWordPortion1(lLa, g, s, outf, wordPortion);
        }
        else if (pL - s - q*lineLength == lineLength || pL - s == lineLength){
            int g = s + q*lineLength;
            printWordPortion0(lineLength, g, outf, wordPortion);
            lLa = lineLength;
        }
        else{
            int g = s + q*lineLength;
            printWordPortion0(lineLength, g, outf, wordPortion);
            outf << '\n';
            g = s + (q + 1)*lineLength;
            int h = pL - s - ((q + 1)*lineLength);
            printWordPortion1(lLa, h, g, outf, wordPortion); //print the protion in multiple lines until it fits: print the last line
        }
    }
}

int stuff(int lineLength, istream& inf, ostream& outf){
    char c;
    char d;
    char lastChar = '-';
    char realLastChar = 'i';
    char nextLastChar;
    char realNextLastChar = 'i';
    char wordPortion[140];
    int pL = 0;
    int lL = 0;
    int start = 0;
    int situation = 0;
    int changeLine = 0;
    if (lineLength < 1){
        return 2;
    }
    while (inf.get(c)){
        if (c == '\n'){
            d = ' ';
        }
        else{
            d = c;
        } //turn enter into space
        if ((d != ' ') && (d != '-')){ //get the next word portion
            wordPortion[pL] = d;
            start = 1;
            pL++;
        } //the printing only happens when a portion is stored
        else{
            if (d == '-'){
                start = 1;
                nextLastChar = '-';
                realNextLastChar = 'i';
                wordPortion[pL] = d;
                pL++;
            }            
            else{
                nextLastChar = ' ';
                realNextLastChar = wordPortion[pL - 1];
                if (pL == 3 && wordPortion[0] == '#' && wordPortion[1] == 'P' && wordPortion[2] == '#'){
                    if (lL != 0){
                        changeLine = 1;
                    }
                    pL = 0;
                    start = 0;
                }
            }
            if (start == 1){
                if (changeLine == 1){ //to avoid returning before the first line and after the last line
                    outf << '\n' << '\n';
                    lL = 0;
                    lastChar = '-';
                }
                if (pL <= lineLength - lL){
                    normalSituation1(pL, lineLength, lL, outf, wordPortion, lastChar, realLastChar);
                }
                else{ //if not fit in a line
                    int lLa = 0;
                    printWordPortion3(situation, lineLength, lL, lLa, pL, outf, wordPortion, lastChar, realLastChar);
                    lL = lLa;
                }
                lastChar = nextLastChar;
                realLastChar = realNextLastChar;
                pL = 0;
                start = 0;
                changeLine = 0;
            }
        }
    }
    if ((pL != 3 || wordPortion[0] != '#' || wordPortion[1] != 'P' || wordPortion[2] != '#') && start == 1 && pL <= lineLength - lL){
        normalSituation1(pL, lineLength, lL, outf, wordPortion, lastChar, realLastChar);
    }
    else if ((pL != 3 || wordPortion[0] != '#' || wordPortion[1] != 'P' || wordPortion[2] != '#') && start == 1){ //if not fit in a line
        int lLa = 0;
        printWordPortion3(situation, lineLength, lL, lLa, pL, outf, wordPortion, lastChar, realLastChar);
    }
    outf << '\n';
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
