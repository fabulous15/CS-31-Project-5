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
        lL += (2 + pL);
    }
    else if (((lastChar == ' ' && realLastChar != '.' && realLastChar != '?') || hyphon >= 2) && pL < lineLength - lL){
        outf << ' ';
        lL += (1 + pL);
    }
    else if (lastChar == ' ' || hyphon >= 2 || pL > lineLength - lL){
        outf << '\n';
        lL = pL;
    }
    printWordPortion0(pL, 0, outf, wordPortion);
}

void printWordPortion3(int& situation, int lineLength, int& lL, int pL, int hyphon, ostream& outf, char wordPortion[], char lastChar, char realLastChar){
    situation = 1; //return 1
    int t, s = 0, q = 0;
    if ((t = lineLength - lL) > 0){
        if (lastChar == ' ' && (realLastChar == '.' || realLastChar == '?') && (t = lineLength - lL - 2) > 0){
            outf << ' ' << ' ';
        }
        else if (((lastChar == ' ' && realLastChar != '.' && realLastChar != '?') || hyphon >= 2) && (t = lineLength - lL - 1) > 0){
            outf << ' ';
        }
        printWordPortion0(t, 0, outf, wordPortion); //print part of the portion
        s += t;
    }
    outf << '\n';
    for (q = 0; q < ((pL - s)/lineLength) - 1; q++){ //print the protion in multiple lines until it fits: print the lines
        printWordPortion0(lineLength, (s + q*lineLength), outf, wordPortion);
        outf << '\n';
    }
    if ((t = pL - s) < lineLength){
        printWordPortion0((pL - s), s, outf, wordPortion);
    }
    else if (pL - s - q*lineLength == (t = lineLength)){
        printWordPortion0(lineLength, (s + q*lineLength), outf, wordPortion);
    }
    else{
        printWordPortion0(lineLength, (s + q*lineLength), outf, wordPortion);
        outf << '\n';
        printWordPortion0((t = pL - s - (q + 1)*lineLength), (s + (q + 1)*lineLength), outf, wordPortion);
    } //print the protion in multiple lines until it fits: print the last line
    lL = t;
}

int stuff(int lineLength, istream& inf, ostream& outf){
    char wordPortion[140];
    char c, lastChar = 'i', realLastChar = 'i', nextLastChar = 'i', realNextLastChar = 'i';
    int pL = 0, lL = 0, start = 0, situation = 0, changeLine = 0, hyphon = 0;
    if (lineLength < 1)
        return 2;
    if (!inf.get(c))
        return 0;
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
        if (c != ' ' && c!= '\n'){ //get the next word portion
            wordPortion[pL] = c;
            start = 1;
            pL++;
            if (c != '-'){
                continue; //the printing only happens when a complete portion is stored
            }
            nextLastChar = '-';
        }
        else{
            if (lastChar == '-'){
                hyphon++;
            }
            if (start == 1){
                nextLastChar = ' ';
                realNextLastChar = wordPortion[pL - 1];
            }
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
            if (pL <= lineLength){
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
    if ((pL != 3 || wordPortion[0] != '#' || wordPortion[1] != 'P' || wordPortion[2] != '#') && start == 1 && pL <= lineLength){
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
        if (!infile){
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
