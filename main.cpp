#include <iostream>
#include <fstream>
using namespace std;

void printWordPortion0(int pL, int move, ostream& outf, char wordPortion[]){
    for (int k = 0; k < pL; k++){
        outf << wordPortion[k + move];
    }
}

void normalSituation(int pL, int lineLength, int& lL, int hyphon, ostream& outf, char wordPortion[], char lastChar, char realLastChar){
    if ((lastChar == ' ' && (realLastChar == '.' || realLastChar == '?')) && pL < lineLength - lL - 1){
        outf << ' ' << ' ';
        lL = lL + 2;
    }
    else if ((lastChar == ' ' && (realLastChar == '.' || realLastChar == '?')) || ((lastChar == ' ' || hyphon >= 2) && pL == lineLength - lL) ){
        outf << '\n';
        lL = 0;
    }
    else if (lastChar == ' ' || hyphon >= 2){
        outf << ' ';
        lL++;
    }
    printWordPortion0(pL, 0, outf, wordPortion);
    lL = lL + pL;
}

void printWordPortion3(int& situation, int lineLength, int& lL, int pL, int hyphon, ostream& outf, char wordPortion[], char lastChar, char realLastChar){
    if (pL <= lineLength){
        outf << '\n';
        printWordPortion0(pL, 0, outf, wordPortion);
        lL = pL;
    }
    else{
        situation = 1; //return 1
        int s = 0;
        int t;
        if ((t = lineLength - lL) > 0){
            if (lastChar == ' ' && (realLastChar == '.' || realLastChar == '?') && (t = lineLength - lL - 2) > 0){
                outf << ' ' << ' ';
            }
            else if (((lastChar == ' ' && realLastChar != '.' && realLastChar != '?') || hyphon >= 2) && (t = lineLength - lL - 1) > 0){
                outf << ' ';
            }
            printWordPortion0(t, 0, outf, wordPortion); //print part of the portion
            s = s + t;
        }
        outf << '\n';
        int q = 0;
        while (q < ((pL - s)/lineLength) - 1){ //print the protion in multiple lines until it fits: print the lines
            printWordPortion0(lineLength, (s + q*lineLength), outf, wordPortion);
            outf << '\n';
            q++;
        }
        if (pL - s < lineLength){
            printWordPortion0((pL - s), s, outf, wordPortion);
            lL = pL - s;
        }
        else if (pL - s - q*lineLength == lineLength || pL - s == lineLength){
            printWordPortion0(lineLength, (s + q*lineLength), outf, wordPortion);
            lL = lineLength;
        }
        else{
            printWordPortion0(lineLength, (s + q*lineLength), outf, wordPortion);
            outf << '\n';
            printWordPortion0((pL - s - (q + 1)*lineLength), (s + (q + 1)*lineLength), outf, wordPortion);
            lL = pL - s - (q + 1)*lineLength;
        } //print the protion in multiple lines until it fits: print the last line
    }
}

int stuff(int lineLength, istream& inf, ostream& outf){
    char c;
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
    int hyphon = 0;
    if (lineLength < 1){
        return 2;
    }
    if (!inf.get(c)){
        return 0;
    }
    if (c != ' ' && c!= '\n' && c != '-'){
        wordPortion[pL] = c;
        start = 1;
        pL++;
    }
    else if (c == '-'){
        outf << '-';
        lL++;
    }
    while (inf.get(c)){
        if ((c != ' ') && (c != '-') && c!= '\n'){ //get the next word portion
            wordPortion[pL] = c;
            start = 1;
            pL++;
        } //the printing only happens when a portion is stored
        else{
            if (c == '-'){
                wordPortion[pL] = c;
                pL++;
                start = 1;
                nextLastChar = '-';
                realNextLastChar = 'i';
            }
            else{
                if (lastChar == '-'){
                    hyphon++;
                }
                nextLastChar = ' ';
                realNextLastChar = wordPortion[pL - 1];
                if (pL == 3 && wordPortion[0] == '#' && wordPortion[1] == 'P' && wordPortion[2] == '#'){
                    if (lL != 0){
                        changeLine = 1;
                    }
                    pL = start = hyphon = 0;
                }
            }
            if (start == 1){
                if (changeLine == 1){ //to avoid returning before the first line and after the last line
                    outf << '\n' << '\n';
                    lL = 0;
                    lastChar = '-';
                }
                if (pL <= lineLength - lL){
                    normalSituation(pL, lineLength, lL, hyphon, outf, wordPortion, lastChar, realLastChar);
                }
                else{
                    printWordPortion3(situation, lineLength, lL, pL,hyphon, outf, wordPortion, lastChar, realLastChar);
                }
                lastChar = nextLastChar;
                realLastChar = realNextLastChar;
                pL = start = changeLine = hyphon = 0;
            }
        }
    }
    if ((pL != 3 || wordPortion[0] != '#' || wordPortion[1] != 'P' || wordPortion[2] != '#') && start == 1 && pL <= lineLength - lL){
        normalSituation(pL, lineLength, lL, hyphon, outf, wordPortion, lastChar, realLastChar);
    }
    else if ((pL != 3 || wordPortion[0] != '#' || wordPortion[1] != 'P' || wordPortion[2] != '#') && start == 1){
        printWordPortion3(situation, lineLength, lL,  pL, hyphon, outf, wordPortion, lastChar, realLastChar);
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
