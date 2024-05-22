// microwitharg 9
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

string purifyargument(string s){
    string res = "";
    for(size_t i=0;i<s.length();i++){
        if((s[i] != '&') && (s[i] != ',')){
            res += s[i];    
        }
    }
    return res;
}

int main()
{
    // Initialize MDT and MNT 
    map<int,pair<string,int>> MNT;
    map<int,string> MDT;
    unordered_multimap<string, pair<string,string>> ALA;
    vector<string> expandedcode;
    int mdtidx = 10;
    int mntidx = 1;

    // Prepare to start reading the text file
    fstream myfile;
    myfile.open("input.txt",ios::in);
    if(myfile.is_open()){
        string line;
        int checkflag = 2;
        // start reading the file -> It's either Macro def or .Code expansion 
        while(getline(myfile,line)){
            istringstream iss(line);
            vector<string> words;
            while(iss){
                string tempstring;
                iss >> tempstring;
                words.push_back(tempstring);
            }
            int wordsinline = words.size() - 1;//empty object to remove -1
            if(words[0] == "macro"){
                if(wordsinline > 2){
                    // this means it has arguments - update into ALA first
                    string macroname = words[1];
                    int count = 1;
                    for(int i=2;i<wordsinline;i++){
                        string w = purifyargument(words[i]);
                        string numarg = "#" + to_string(count++);
                        ALA.insert({macroname,{w,numarg}});
                    }
                    // Update MNT now 
                    MNT[mntidx] = make_pair(words[1],mdtidx);
                    mntidx++;
                    checkflag = 2;
                    continue;
                }
                else{
                    // it does not have any arguments - simply update MNT
                    MNT[mntidx] = make_pair(words[1],mdtidx);
                    mntidx++;
                    checkflag = 2;
                    continue;
                }
            }
            else if(words[0] == ".Code"){
                // WRITE MODE
                checkflag = 4;
                continue;
            }

            // Check flags and write accordinly 
            if(checkflag == 2){
                // Update MDT
                MDT[mdtidx] = line;
                mdtidx++;
            }

            else if(checkflag == 4){
                // write stuff into expanded code vector. Two cases, either complete macro or simple instruction
                mntidx = 1;
                bool flag = false;
                for(size_t i=0;i<MNT.size();i++){
                    if(MNT[mntidx].first == words[0]){
                        flag = true;
                        mdtidx = MNT[mntidx].second;
                        break;
                    }
                    mntidx++;
                }

                // CASE 1 - It's A macro 
                if(flag){
                    // two cases here also - either it has args or it doesn't
                    if(wordsinline>1){
                        // first update ALA with actual values 
                        int count = 1;
                        for(int i=1;i<wordsinline;i++){
                            string toupdate = purifyargument(words[i]);
                            string tosearch = "#" + to_string(count);
                            for(auto &it:ALA){
                                //cout << it.first << endl;
                                if(it.first == words[0] && it.second.second == tosearch){
                                    it.second.second = toupdate;
                                    //cout << it.second.second << endl;
                                }
                            }
                            count++;
                        }
                        // secondly, while reading from MDT, update and data and pushback on the fly
                        string wrd = MDT[mdtidx];
                        while(wrd != "mend"){
                            istringstream ss(wrd);
                            string w1,w2;
                            ss >> w1 >> w2;
                            w2 = purifyargument(w2);
                            //cout << w2 << endl;
                            bool foundinala = false;
                            for(auto &it:ALA){
                                if(it.second.first == w2){
                                    //cout << it.second.second << endl;
                                    foundinala = true;
                                    w2 = it.second.second;
                                    // cout << w2 << endl;
                                    // cout << it.second.second << endl;
                                }
                            }
                            if(foundinala){
                                string finalst = w1 + " " + w2;
                                expandedcode.push_back(finalst);
                            }
                            else{
                                expandedcode.push_back(wrd);
                            }
                            mdtidx++;
                            wrd = MDT[mdtidx];
                        }
                    }else{
                        string wrd = MDT[mdtidx];
                        while(wrd != "mend"){
                            expandedcode.push_back(wrd);
                            mdtidx++;
                            wrd = MDT[mdtidx];
                        }
                    }

                }
                // CASE 2 - It's just an instruction 
                else{
                    // simply push 
                    expandedcode.push_back(line);
                }
            }
        }        

        myfile.close();

        // print expanded code :
        cout << "\n Expanded Code : " << endl;
        for(size_t i=0;i<expandedcode.size();i++){
            cout << expandedcode[i] << endl;
        }

        // print MNT 
        mntidx = 1;
        mdtidx = MNT[mntidx].second;
        cout << "\n Macro Name Table (MNT) : " << endl;
        cout << "Macro Name \t MDT index " << endl;
        for(size_t i=0;i<MNT.size();i++){
            cout << MNT[mntidx].first << " \t \t "<< MNT[mntidx].second << endl; 
            mntidx++;
        }

        // print MDT
        cout << "\n Macro Defination Table (MDT) : " << endl;
        cout << "MDT Index \t Card " << endl; 
        for(size_t i=0;i<MDT.size();i++){
            cout << mdtidx << " \t \t "<< MDT[mdtidx] << endl;
            mdtidx++;
        }

        // Print ALA 
        cout << "\n Argument List Array : " << endl;
        cout << "Macro Name \t Formal Parameter " << "\t" << "Actual Parameter" << endl;
        for(auto it:ALA){
            cout << it.first << "\t \t \t" << it.second.first <<"\t \t"<<it.second.second<<endl;
        }
    }
    else{
        cout << "No such file exists ! " << endl;
    }
    return 0;
}