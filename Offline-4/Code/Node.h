#pragma once
#ifndef DECISION_TREE_NODE
#define DECISION_TREE_NODE
#include <bits/stdc++.h>
using namespace std;

class AttrValStat {
    public:
        map<string, int> freqCount;
        // count of attrVal (vhigh, high, low....)
        map<string, map<string, int>> classCount;
        // vhigh -> (unacc_count, acc_count, good_count, vgood_count)
        // low -> ....
        // more, 5more .. -> ....
        // ----------------------------------------
        // unacc_count => map["unacc"] = how many unacc are present in vihigh/low/.....
};

class Node {
    private:
        vector<Node*> childList; // Maximum of 4 child possible from any node
        string name, branchName;
        vector<vector<string>> examples;

    public:
        string verdict;

        Node(string name) {
            this->name = name;
            this->branchName = "";
        }

        void addChild(Node *child) {
            childList.push_back(child);
        }

        vector<Node*> getChildList() {
            return this->childList;
        }
        
        string getName() {
            return this->name;
        }

        void setExamples(vector<vector<string>> examples) {
            this->examples.clear();
            for (vector v: examples) {
                vector<string> vect(v);
                this->examples.push_back(vect);
            }
        }

        vector<vector<string>> getExamples() {
            return this->examples;
        }

        void setBranchName(string name) {
            this->branchName = name;
        }

        string getBranchName() {
            return this->branchName;
        }
};

#endif