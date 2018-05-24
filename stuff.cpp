#include <iostream>
#include <fstream>
using namespace std;

void printWordPortion0(int pL, int move, ostream& outf, char wordPortion[]){
    for (int k = 0; k < pL; k++){
        outf << wordPortion[k + move]; //print the desired number of chars (pL) from a starting position (move)
    }
}

void normalSituation(int pL, int lineLength, int& lL, int hyphon, ostream& outf, char wordPortion[], char lastChar, char realLastChar){
    if ((lastChar == ' ' && (realLastChar == '.' || realLastChar == '?')) && pL < lineLength - lL - 1){
        outf << ' ' << ' '; //if a portion ending with ‘.’ or ‘?’ can fit in the line after adding two spaces
        lL += (2 + pL);
    }
    else if (((lastChar == ' ' && realLastChar != '.' && realLastChar != '?') || hyphon >= 2) && pL < lineLength - lL){ //else if a portion need one space before it and can fit in after adding a space
        //note that hyphon >= 2 means the space that induce this output is at least the second after a hyphon. it it's the first one, then that means there are no space between this word portion and the hyphen preceeding it
        outf << ' ';
        lL += (1 + pL);
    }
    else if (lastChar == ' ' || hyphon >= 2 || pL > lineLength - lL){ //else if a portion cannot fit in the line (or with the space (s) it needs before it)
        outf << '\n';
        lL = pL;
    }
    printWordPortion0(pL, 0, outf, wordPortion);
}

void printWordPortion3(int& situation, int lineLength, int& lL, int pL, int hyphon, ostream& outf, char wordPortion[], char lastChar, char realLastChar){
    situation = 1; //return 1 if a word portion exceeds the max line length, but don't return now, store it till the end
    int t, s = 0, q = 0;
    if ((t = lineLength - lL) > 0){
        if (lastChar == ' ' && (realLastChar == '.' || realLastChar == '?') && (t = lineLength - lL - 2) > 0){ //if there are extra length left in the line after adding two spaces and we need two spaces before the next word portion
            outf << ' ' << ' ';
        }
        else if (((lastChar == ' ' && realLastChar != '.' && realLastChar != '?') || hyphon >= 2) && (t = lineLength - lL - 1) > 0){ //else if there are extra length left in the line after adding one spaces and we need one space before the next word portion
            outf << ' ';
        }
        printWordPortion0(t, 0, outf, wordPortion); //fit as much of the word portion into the line as it can
        s += t;
    }
    outf << '\n';
    for (q = 0; q < ((pL - s)/lineLength) - 1; q++){ //output the rest part of the word portion that fills some complete lines until the portion have some chars left to output but only less than two lines of chars to output
        printWordPortion0(lineLength, (s + q*lineLength), outf, wordPortion);
        outf << '\n';
    }
    if ((t = pL - s) < lineLength){ //if it has less than a line to output
        printWordPortion0((pL - s), s, outf, wordPortion);
    }
    else if (pL - s - q*lineLength == (t = lineLength)){ //if it has exactly a line to output
        printWordPortion0(lineLength, (s + q*lineLength), outf, wordPortion);
    }
    else{ //if it has more than a line to output
        printWordPortion0(lineLength, (s + q*lineLength), outf, wordPortion);
        outf << '\n';
        printWordPortion0((t = pL - s - (q + 1)*lineLength), (s + (q + 1)*lineLength), outf, wordPortion);
    }
    lL = t; //update the current line length to the number of chars printed in the last line
}

int stuff(int lineLength, istream& inf, ostream& outf){
    char wordPortion[140];
    char c, lastChar = 'i', realLastChar = 'i', nextLastChar = 'i', realNextLastChar = 'i';
    int pL = 0, lL = 0, start = 0, situation = 0, changeLine = 0, hyphon = 0;
    if (lineLength < 1) //if the max line length is smaller than 1
        return 2;
    if (!inf.get(c)) //if there is nothing in the input
        return 0;
    if (c != ' ' && c!= '\n' && c != '-'){
        wordPortion[pL] = c;
        start = 1; //take notes that the outputting can begin after hitting a ‘ ‘ or enter or ‘-‘
        pL++;
    }
    else if (c == '-'){
        outf << '-';
        lL++;
        lastChar = '-'; //record that the last char of the previous portion as ‘-‘
    }
    while (inf.get(c)){
        if (c != ' ' && c!= '\n'){
            wordPortion[pL] = c;
            start = 1;
            pL++;
            if (c != '-'){
                continue; //go back to the start of the loop and keep storing. we are not prepared to output until till the end of a word portion
            }
            nextLastChar = '-';
        }
        else{
            if (lastChar == '-'){
                hyphon++; //add one to the number of spaces after the hyphen
            }
            if (start == 1){
                nextLastChar = ' ';
                realNextLastChar = wordPortion[pL - 1]; //also record the second last char of the next previous portion
            }
            if (pL == 3 && wordPortion[0] == '#' && wordPortion[1] == 'P' && wordPortion[2] == '#'){ //if the portion is a paragraph break
                if (lL != 0){ //to ignore consecutive paragraph breaks
                    changeLine = 1; //prepare to break the paragraph before the next portion
                }
                pL = start = 0;
            }
        }
        if (start == 1){
            if (changeLine == 1){ //to avoid returning before the first line and after the last line
                outf << '\n' << '\n';
                lL = hyphon = 0;
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
    hyphon++; //add one to the number of spaces following a hyphen (no need to determine whether the last char of the previous word portion is hyphen because if it’s not, adding one two zero change nothing)
    if ((pL != 3 || wordPortion[0] != '#' || wordPortion[1] != 'P' || wordPortion[2] != '#') && changeLine == 1 && start == 1){ //if we are prepared to break a paragraph and the portion about to print is not a paragraph break itself
        outf << '\n' << '\n';
        lL = hyphon = 0;
        lastChar = '-';
    }
    if ((pL != 3 || wordPortion[0] != '#' || wordPortion[1] != 'P' || wordPortion[2] != '#') && start == 1 && pL <= lineLength){
        normalSituation(pL, lineLength, lL, hyphon, outf, wordPortion, lastChar, realLastChar);
    }
    else if ((pL != 3 || wordPortion[0] != '#' || wordPortion[1] != 'P' || wordPortion[2] != '#') && start == 1){
        printWordPortion3(situation, lineLength, lL,  pL, hyphon, outf, wordPortion, lastChar, realLastChar);
    }
    outf << '\n';
    return situation; //return the value of the function
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
