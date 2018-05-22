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

void printWordPortion2(int& lL, int pL, ostream& outf, char wordPortion[]){
    int i = 0;
    printWordPortion1(lL, pL, i, outf, wordPortion);
}

void printWordPortion(int pL, ostream& outf, char wordPortion[]){
    int i = 0;
    printWordPortion0(pL, i, outf, wordPortion);
}

void printFirstPart(int& s, int lineLength, int lL, ostream& outf, char wordPortion[], char lastChar, char realLastChar){
    int t;
    if (lastChar == ' ' && (realLastChar == '.' || realLastChar == '?')){
        if ((t = lineLength - lL - 2) > 0){
            outf << ' ' << ' ';
            printWordPortion2(s, t, outf, wordPortion); //print part of the portion
        }
    }
    else if (lastChar == ' '){
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
}

void printWordPortion3(int& situation, int lineLength, int lL, int& lLa, int pL, ostream& outf, char wordPortion[], char lastChar, char realLastChar){
    int q;
    int lLb = 0;
    if (pL <= lineLength){
        outf << '\n';
        printWordPortion(pL, outf, wordPortion);
        lLa = pL;
    }
    else{
        situation = 1; //return 1
        int s = 0;
        printFirstPart(s, lineLength, lL, outf, wordPortion, lastChar, realLastChar);
        q = 0;
        while (q < ((pL - s)/lineLength) - 1){ //print the protion in multiple lines until it fits: print the lines
            int g = s + q*lineLength;
            printWordPortion0(lineLength, g, outf, wordPortion);
            q++;
            outf << '\n';
        }
        if (pL - s < lineLength){
            int g = pL - s;
            printWordPortion1(lLb, g, s, outf, wordPortion);
        }
        else if (pL - s - q*lineLength == lineLength || pL - s == lineLength){
            int g = s + q*lineLength;
            printWordPortion0(lineLength, g, outf, wordPortion);
            lLb = lineLength;
        }
        else{
            int g = s + q*lineLength;
            printWordPortion0(lineLength, g, outf, wordPortion);
            outf << '\n';
            q++;
            g = s + q*lineLength;
            int h = pL - s - (q*lineLength);
            printWordPortion1(lLb, h, g, outf, wordPortion); //print the protion in multiple lines until it fits: print the last line
        }
        lLa = lLb;
    }
}

void normalSituation1(int pL, int lineLength, int& lL, ostream& outf, char wordPortion[], char lastChar, char realLastChar){
    if (pL < lineLength - lL - 1){ //if fit in the line even if adding two space, print the portion
        if (lastChar == ' '){
            if (realLastChar == '.' || realLastChar == '?'){
                outf << ' ' << ' ';
                lL = lL + 2;
                printWordPortion(pL, outf, wordPortion);
            }
            else{
                outf << ' ';
                lL++;
                printWordPortion(pL, outf, wordPortion);
            }
        }
        else{
            printWordPortion(pL, outf, wordPortion); //print it right after
        }
        lL = lL + pL;
    }
    else if (pL == lineLength - lL - 1){ //if fit in the line even if adding a space, but not two, print the portion
        if (lastChar == ' '){ //if it's the first in a line, or following a portion from the same word
            if (realLastChar == '.' || realLastChar == '?'){
                outf << '\n';
                printWordPortion2(lL, pL, outf, wordPortion); //print it right after an enter and keep track of lL
                lL = pL;
            }
            else{
                outf << ' ';
                printWordPortion(pL, outf, wordPortion); //print it right after a space
                lL = lL + pL;
            }
        }
        else{ //if it's the frist of a word and not the first of a line, print a space before
            printWordPortion(pL, outf, wordPortion); //print it right after
            lL = lL + pL;
        }
    }
    else if (pL == lineLength - lL){ //if fit in line only if not adding a space     problem of repeating '\n's
        if (lastChar == '-'){ //if it's the first in a line, or following a portion from the same word
            printWordPortion(pL, outf, wordPortion); //print it right after
            lL = lL + pL;
        }
        else{
            outf << '\n';
            printWordPortion2(lL, pL, outf, wordPortion); //print it right after an enter and keep track of lL
            lL = pL;
        }
    }
}

int para(char wordPortion[], int pL){
    if ((pL%3) != 0){
        return 0;
    }
    if (pL == 0){
        return 0;
    }
    for (int j = 0; j < pL; j = j + 3){
        if (wordPortion[j] != '#'){
            return 0;
        }
    }
    for (int j = 1; j < pL; j = j + 3){
        if (wordPortion[j] != 'P'){
            return 0;
        }
    }
    for (int j = 2; j < pL; j = j + 3){
        if (wordPortion[j] != '#'){
            return 0;
        }
    }
    return 1;
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
                if (para(wordPortion, pL) == 1){
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
                    int lLa;
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
    if (start == 1){
        if (pL <= lineLength - lL){
            normalSituation1(pL, lineLength, lL, outf, wordPortion, lastChar, realLastChar);
        }
        else{ //if not fit in a line
            int q;
            if (pL <= lineLength){
                outf << '\n';
                printWordPortion(pL, outf, wordPortion);
            }
            else{
                situation = 1; //return 1
                int s = 0;
                printFirstPart(s, lineLength, lL, outf, wordPortion, lastChar, realLastChar);
                q = 0;
                while (q < ((pL - s)/lineLength) - 1){ //print the protion in multiple lines until it fits: print the lines
                    int g = s + q*lineLength;
                    printWordPortion0(lineLength, g, outf, wordPortion);
                    q++;
                    outf << '\n';
                }
                if (pL - s < lineLength){
                    int g = pL - s;
                    printWordPortion0(g, s, outf, wordPortion);
                }
                else if (pL - s - q*lineLength == lineLength || pL - s == lineLength){
                    int g = s + q*lineLength;
                    printWordPortion0(lineLength, g, outf, wordPortion);
                    nextLastChar = '-';
                }
                else{
                    int g = s + q*lineLength;
                    printWordPortion0(lineLength, g, outf, wordPortion);
                    outf << '\n';
                    q++;
                    g = s + q*lineLength;
                    int h = pL - s - (q*lineLength);
                    printWordPortion0(h, g, outf, wordPortion); //print the protion in multiple lines until it fits: print the last line
                }
            }
        }
        outf << '\n';
    }
    else{
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
