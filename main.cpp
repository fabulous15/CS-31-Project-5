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
        if (d != ' ' || d != '-'){ //get the next word portion
            wordPortion[pL] = d;
            pL++;
            lL++;
        } //the printing only happens when a portion is stored
        else{
            if (d == '-'){
                wordPortion[pL] = d;
                nextLastChar = '-';
                pL++;
                lL++;
            }
            else{
                nextLastChar = ' ';
            }
            if (pL < lineLength - lL){ //if fit in the line even if adding a space, print the portion
                if (lastChar == '\n' || lastChar == '-'){ //if it's the first in a line, or following a portion from the same word
                    for (int k = 0; k < pL; k++){ //print it right after
                        outf << wordPortion[k];
                    }
                    lastChar = nextLastChar;
                }
                else{ //if it's the frist of a word and not the first of a line, print a space before
                    outf << ' ';
                    lL++;
                    for (int k = 0; k < pL; k++){ //print it right after a space
                        outf << wordPortion[k];
                    }
                    lastChar = nextLastChar;
                }
            }
            else if (pL == lineLength - lL){ //if fit in line only if not adding a space
                if (lastChar == '\n' || lastChar == '-'){ //if it's the first in a line, or following a portion from the same word
                    for (int k = 0; k < pL; k++){ //print it right after
                        outf << wordPortion[k];
                    }
                    outf << '\n';
                    lastChar = '\n';
                }
                else{
                    outf << '\n';
                    lL = 0;
                    for (int k = 0; k < pL; k++){  //print it right after an enter
                        outf << wordPortion[k];
                        lL++;
                    }
                    lastChar = nextLastChar;
                }
            }
            else{ //if not fit in a line
                int q;
                int lLb = 0;
                if (lL == 0){
                    situation = 1; //return 1
                }
                for (int k = 0; k < lineLength - lL; k++){ //print part of the portion
                    outf << wordPortion[k];
                }
                outf << '\n';
                for (q = 0; q < ((pL - lineLength + lL)/lineLength); q++){ //print the protion in multiple lines until it fits: print the lines
                    for (int k = 0; k < lineLength; k++){
                        outf << wordPortion[k + lineLength - lL + q*lineLength];
                        outf << '\n';
                    }
                }
                for (int k = 0; k < (pL - lineLength + lL)%lineLength; k++){ //print the protion in multiple lines until it fits: print the last line
                    outf << wordPortion[k + lineLength - lL + q*lineLength];
                    lLb++;
                }
                lastChar = nextLastChar;
                lL = lLb;
            }
            pL = 0;
        }
    }
    if (pL < lineLength - lL){ //if fit in the line even if adding a space, print the portion
        if (lastChar == '\n' || lastChar == '-'){ //if it's the first in a line, or following a portion from the same word
            for (int k = 0; k < pL; k++){ //print it right after
                outf << wordPortion[k];
            }
            outf << '\n';
        }
        else{ //if it's the frist of a word and not the first of a line, print a space before
            outf << ' ';
            lL++;
            for (int k = 0; k < pL; k++){ //print it right after a space
                outf << wordPortion[k];
            }
            outf << '\n';
        }
    }
    else if (pL == lineLength - lL){ //if fit in line only if not adding a space
        if (lastChar == '\n' || lastChar == '-'){ //if it's the first in a line, or following a portion from the same word
            for (int k = 0; k < pL; k++){ //print it right after
                outf << wordPortion[k];
            }
            outf << '\n';
        }
        else{
            outf << '\n';
            lL = 0;
            for (int k = 0; k < pL; k++){  //print it right after an enter
                outf << wordPortion[k];
                lL++;
            }
            outf << '\n';
        }
    }
    else{ //if not fit in a line
        int q;
        int lLb = 0;
        if (lL == 0){
            situation = 1; //return 1
        }
        for (int k = 0; k < lineLength - lL; k++){ //print part of the portion
            outf << wordPortion[k];
        }
        outf << '\n';
        for (q = 0; q < ((pL - lineLength + lL)/lineLength); q++){ //print the protion in multiple lines until it fits: print the lines
            for (int k = 0; k < lineLength; k++){
                outf << wordPortion[k + lineLength - lL + q*lineLength];
                outf << '\n';
            }
        }
        for (int k = 0; k < (pL - lineLength + lL)%lineLength; k++){ //print the protion in multiple lines until it fits: print the last line
            outf << wordPortion[k + lineLength - lL + q*lineLength];
            lLb++;
            outf << '\n';
        }
        lL = lLb;
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
