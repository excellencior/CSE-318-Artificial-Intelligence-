#include <bits/stdc++.h>
#include "Node.h"
using namespace std;

std::ofstream fout ("graph.csv");

#define vt vector
#define pb push_back
#define buying 0
#define maint 1
#define doors 2
#define persons 3
#define lug_boot 4
#define safety 5
#define class_val 6

map<string, int> nameDecode = {{"buying", buying}, {"maint", maint}, {"doors", doors}, {"persons", persons}, {"lug_boot", lug_boot}, {"safety", safety}};
map<string, vt<string>> attrValueMap;
vt<string> frame80, frame20;
vt<vt<string>> allExamples, trainExamples, testExamples;
Node *head;

vt<string> splitString(string s, char delim) {
    vt<string> stringarr;
    stringstream ss(s);
    string word;
    while (!ss.eof()) {
        getline(ss, word, delim);
        stringarr.pb(word);
    }
    return stringarr;
}

double getRealRandom(double limit) {
    static std::random_device rd;
	static mt19937 gen(rd());

    uniform_real_distribution <> dis(0.0, limit);

    double random_number = dis(gen);

    return random_number;
}

double calculateStandardDeviation(vector<double> x, double mean, int itrCount) {
    double num;
    for (double val: x) {
        num += (val - mean) * (val - mean);
    }
    num = (1.0 * num) / itrCount;
    return std::sqrt(num);
}

void printattrs(AttrValStat *attr) {
    for (pair p: attr->classCount) {
        fout << p.first << ": ";
        for (pair pp: p.second) {
            fout << pp.first << ", " << pp.second << endl;
        }
        fout << endl;
    }
}

AttrValStat* getAttrStat(int attr, vt<vt<string>> examples) {
    AttrValStat *atval = new AttrValStat();

    for (vt vect: examples) {
        string attrval = vect[attr]; // choosing the attribute column from the examples

        atval->freqCount[attrval]++; // frequencies of that attribute values
        
        string className = vect[class_val]; // classval column
        atval->classCount[attrval][className]++;
    }

    return atval;
}

void print(vt<vt<string>> frame) {
    int i=1;
    cout << "\n===================Size of examples: " << frame.size() << endl;
    for (auto v: frame) {
        cout << i << ": ";
        cout << v.size() << endl;
        for (string a: v)
            cout << a << " || ";
        cout << endl;
        i++;
    }
}

void convertToVectors() {
    for (string s: frame80) {
        trainExamples.push_back(splitString(s, ','));
    }
    for (string s: frame20) {
        testExamples.push_back(splitString(s, ','));
    }
}

// a=>val, b=>base , log4(a)
double logany(double a, int b = 4) {
    return log2(a)/log2(b);
}

void clearContainers() {
    trainExamples.clear();
    testExamples.clear();
    frame20.clear();
    frame80.clear();
}

// Division of dataset (80/20) uniform distribution
void loadSets(vt<string> dataframe) {
    int nexamples = 0.8 * dataframe.size();

    // Trainset -> 80% of dataframe
    for (int i=0; i<nexamples; i++) {
        frame80.pb(dataframe[i]);
    }
    for (int i=nexamples; i<dataframe.size(); i++) {
        frame20.pb(dataframe[i]);
    }

    convertToVectors();
}

void loadAttrValueMap() {
    attrValueMap["buying"] = vector<string>{"low", "med", "high", "vhigh"};
    attrValueMap["maint"] = vector<string>{"low", "med", "high", "vhigh"};
    attrValueMap["doors"] = vector<string>{"2", "3", "4", "5more"};
    attrValueMap["persons"] = vector<string>{"2", "4", "more"};
    attrValueMap["lug_boot"] = vector<string>{"small", "med", "big"};
    attrValueMap["safety"] = vector<string>{"low", "med", "high"};
}

string pluralityValue(Node* node) {
    // cout << "No more examples" << endl;
    vt<vt<string>> examples = node->getExamples();
    int unacc, acc, good, vgood;
    unacc = acc = good = vgood = 0;

    string s;
    for (vt v: examples) {
        s = v[class_val]; // buying(0), maint(1), doors(2), persons(3), lug_boot(4), safety(5), class(6)
        if (s == "unacc") unacc++;
        else if (s == "acc") acc++;
        else if (s == "good") good++;
        else if (s == "vgood") vgood++;
    }

    int maxval = max(max(max(unacc, acc), good), vgood);

    string verdict = "";
    if (maxval == unacc) verdict = "unacc";
    else if (maxval == acc) verdict = "acc";
    else if (maxval == good) verdict = "good";
    else verdict = "vgood";

    return verdict;
}

// pair<sameclassf. = true/false, classification (if true)>
string checkclassification(Node *node) {
    if (node->getExamples().size() == 0) return "notpure";

    vt<vt<string>> examples = node->getExamples();
    string classValue = examples[0][class_val]; // 6th value (class name) of the first example
    
    for (vt v: examples) {
        string s = v[class_val];
        if (s != classValue) {
            return "notpure";
        }
    }

    return classValue;
}

double getEntropy(string attrName, vt<vt<string>> examples) {
    // Initial entropy, required for selecting the root attribute

    int unacc = 0, acc = 0, good = 0, vgood = 0;
    double entropy = 0.0;
    double denom = examples.size();

    if (attrName == "head") {
        string classValue;
        for (vt v: examples) {
            classValue = v[class_val]; // classvalue column

            if (classValue == "unacc") unacc++;
            else if (classValue == "acc") acc++;
            else if (classValue == "good") good++;
            else if (classValue == "vgood") vgood++;
        }

        if (unacc > 0) entropy += ( (1.0*unacc)/(1.0*denom) ) * logany( (1.0*denom)/(1.0*unacc) );
        if (acc > 0) entropy += ( (1.0*acc)/(1.0*denom) ) * logany( (1.0*denom)/(1.0*acc) );
        if (good > 0) entropy += ( (1.0*good)/(1.0*denom) ) * logany( (1.0*denom)/(1.0*good) );
        if (vgood > 0) entropy += ( (1.0*vgood)/(1.0*denom) ) * logany( (1.0*denom)/(1.0*vgood) );
        
        return entropy;
    }
    else {
        double remainder = 0.0;
        int attr = nameDecode[attrName];
        // stat => statistics of the selected attribute (frequencies of the values, frequencies of the classvalues of the values)
        AttrValStat *stat = getAttrStat(attr, examples);

        // Iterating over the values of the selected attribute
        for (pair valFreq: stat->freqCount) {
            string valueName = valFreq.first;
            double wt = (1.0 * valFreq.second) / denom;

            unacc = stat->classCount[valueName]["unacc"];
            acc = stat->classCount[valueName]["acc"];
            good = stat->classCount[valueName]["good"];
            vgood = stat->classCount[valueName]["vgood"];

            int totalClassCount = unacc + acc + good + vgood; // required for calculation of entropy

            if (unacc > 0) entropy += ( (1.0*unacc)/(1.0*totalClassCount) ) * logany( (1.0*totalClassCount)/(1.0*unacc) );
            if (acc > 0) entropy += ( (1.0*acc)/(1.0*totalClassCount) ) * logany( (1.0*totalClassCount)/(1.0*acc) );
            if (good > 0) entropy += ( (1.0*good)/(1.0*totalClassCount) ) * logany( (1.0*totalClassCount)/(1.0*good) );
            if (vgood > 0) entropy += ( (1.0*vgood)/(1.0*totalClassCount) ) * logany( (1.0*totalClassCount)/(1.0*vgood) );

            remainder += (wt * entropy * 1.0);
            entropy = 0.0;
        }

        return remainder;
    }

    return -1; // not reached
}

// Return the next node with the highest important == lowest entropy == highest gain [greedy algo]
Node* importance(set<string> attributes, vt<vt<string>> examples) {
    string maxGainAttr;
    double maxGain = INT_MIN, selectedNodeEntropy = 0;
    double parentEntropy = getEntropy("head", examples);
    // Calculate the entropy for the current examples
    // attributes => Remaining ones
    for (string attr: attributes) {
        double attrEntropy = getEntropy(attr, examples);
        double gain = parentEntropy - attrEntropy;

        if (gain > maxGain) {
            maxGain = gain;
            maxGainAttr = attr;
            selectedNodeEntropy = attrEntropy;
        }
    }
    Node* child = new Node(maxGainAttr);

    return child;
}

// Return a decision
Node* DecisionTreeLearning(set<string> attributes, Node *node, Node *parent, string branchName = "") {
    string checkclass = checkclassification( node );

    if ( node->getExamples().size() == 0 ) {
        string verdict = pluralityValue(parent);
        Node* t = new Node("leaf");
        t->setBranchName(branchName);
        t->verdict = verdict;
        return t;
    }
    else if ( checkclass != "notpure" ) {
        Node* t = new Node("leaf");
        t->setBranchName(branchName);
        t->verdict = checkclass;
        return t;
    }
    else if ( attributes.size() == 0 ) {
        string verdict = pluralityValue(node);
        Node* t = new Node("leaf");
        t->setBranchName(branchName);
        t->verdict = verdict;
        return t;
    }
    else {
        Node* newnode = importance(attributes, node->getExamples());
        newnode->setBranchName(branchName); // This newnode is selected through branch "branchName"

        attributes.erase(newnode->getName()); // take the attributes except this
        // fout << "Attributes size: " << attributes.size() << endl;
        // this newnode is a selected attribute, now branch over for all the values this attribute can have
        for (string branchName: attrValueMap[newnode->getName()]) {
            int attr = nameDecode[newnode->getName()];

            // From the examples, select only the ones for which the branch happens
            vt<vt<string>> newExamples;
            for (vt vect: node->getExamples()) {
                string av = vect[attr];
                if (av == branchName) {
                    newExamples.push_back(vect);
                }
            }
            newnode->setExamples(newExamples);

            Node *child = DecisionTreeLearning(attributes, newnode, node, branchName);
            newnode->addChild(child);
        }

        return newnode;
    }
}

string dfs(Node* node, Node* parent, vt<string> vect) {
    // base case => No more branch to take
    if (node->verdict != "") 
        return node->verdict;

    int selAttr = nameDecode[node->getName()];
    string attributeValue = vect[selAttr];
    // Search which branch to take based on the test data
    for (Node* n: node->getChildList()) {
        // fout <<"sel: "<< n->getName() << " | " << attributeValue << endl;
        if (n->getBranchName() == attributeValue) {
            return dfs(n, node, vect);
        }
    }

    return node->verdict;
}

double runTestExamples(vt<string> frame, set<string> attributes, bool printToConsole) {
    vector<double> x;
    int itrCount = 20;
    double totalAccuracy = 0.0;
    for (int i=0; i<itrCount; i++) {
        std::default_random_engine e(getRealRandom(1000));
        std::shuffle(std::begin(frame), std::end(frame), e);

        clearContainers();
        loadSets(frame);

        head = new Node("head");
        head->setExamples(trainExamples);

        head = DecisionTreeLearning(attributes, head, NULL);

        double accuracy = 0;
        // dfs on the testSet
        for (vt vect: testExamples) {
            // Search on this data
            if (vect[class_val] == dfs(head, NULL, vect)) {
                accuracy++;
            }
        }

        if (printToConsole) {    
            cout << "Run ======== " << (i+1) << " ======== " << endl;
            cout << "Total Test Examples: " << testExamples.size() << endl;
            cout << "Accurate Results: " << accuracy << endl;
        }

        accuracy = ((1.0 * accuracy)/testExamples.size())*100;
        if (printToConsole) cout << "Accuracy: " << accuracy << " %" << endl;

        x.push_back(accuracy);
        totalAccuracy += accuracy;
    }
    
    double meanAccuracy = (1.0 * totalAccuracy)/itrCount;
    if (printToConsole) {
        cout << "\nMean Accuracy (" << itrCount << " runs): " << meanAccuracy << " %" << endl;
        double sd = calculateStandardDeviation(x, meanAccuracy, itrCount);
        cout << "\nStandard Deviation (" << itrCount << " runs): " << sd << " %" << endl;
    }

    return meanAccuracy;
}

void printToCsv(vt<string> frame, set<string> attributes) {
    vector<double> x;
    int itrCount = 20;
    double totalAccuracy = 0.0;
    for (int i=0; i<itrCount; i++) {

        trainExamples.clear();
        for (string s: frame) {
            trainExamples.push_back(splitString(s, ','));
        }

        head = new Node("head");
        head->setExamples(trainExamples);

        head = DecisionTreeLearning(attributes, head, NULL);
        // cout << "Head node: " << head->getName() << endl;

        double accuracy = 0;
        // dfs on the testSet
        for (vt vect: testExamples) {
            // Search on this data
            if (vect[class_val] == dfs(head, NULL, vect)) {
                accuracy++;
            }
        }

        accuracy = ((1.0 * accuracy)/testExamples.size())*100;

        x.push_back(accuracy);
        totalAccuracy += accuracy;
    }
    
    double meanAccuracy = (1.0 * totalAccuracy)/itrCount;

    fout << frame.size() << ',' << meanAccuracy << "%" << endl;
}

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(false);

    string rowDat;
    vt<string> totalframe;
    fstream cin ("car.data");
    while (cin >> rowDat) {
        totalframe.pb(rowDat);
    }

    loadAttrValueMap();
    set<string> attributes;
    attributes.insert("buying");
    attributes.insert("maint");
    attributes.insert("doors");
    attributes.insert("persons");
    attributes.insert("lug_boot");
    attributes.insert("safety");

    int incrAmt = 50;
    bool printToConsole = true, printToCSV = true;

    runTestExamples(totalframe, attributes, printToConsole);

    vt<string> incrementalTrainFrame;
    int i = 0;
    for (string data: totalframe) {
        if (i % 200 == 0 && i) {
            printToCsv(incrementalTrainFrame, attributes);
        }

        i++;
        if (i > (totalframe.size() * 0.8)) break;
        incrementalTrainFrame.push_back(data);
    }
    printToCsv(incrementalTrainFrame, attributes);
    
    // runTestExamples(totalframe, attributes, true); 

    return 0;
}

/*
--class values

unacc, acc, good, vgood

--attributes

buying:   vhigh, high, med, low.
maint:    vhigh, high, med, low.
doors:    2, 3, 4, 5more.
persons:  2, 4, more.
lug_boot: small, med, big.
safety:   low, med, high.
*/