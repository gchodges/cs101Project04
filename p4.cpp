#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

uint64_t djb2(string str) {
    const char *ptr = str.c_str();
    uint64_t hash = 5381;
    int c;
    while ((c = *ptr++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

class Person {
public:
    string id;
    string first;
    string last;
    string email;
    string phone;
    string city;
    string state;
    string postalCode;

    void setID(string input){
        id =input;
    }

    void setFirst(string input){
        first =input;
    }

    void setLast(string input){
        last =input;
    }

    void setEmail(string input){
        email =input;
    }

    void setPhone(string input){
        phone =input;
    }

    void setCity(string input){
        city =input;
    }

    void setState(string input){
        state =input;
    }

    void setPostal(string input){
        postalCode =input;
    }

    string findKey(string key){
        if (key == "Id"){
            return id;
        }
        else if(key == "FirstName"){
            return first;
        }
        if (key == "LastName"){
            return last;
        }
        else if(key == "Email"){
            return email;
        }
        if (key == "Phone"){
            return phone;
        }
        else if(key == "City"){
            return city;
        }
        if (key == "State"){
            return state;
        }
        else if(key == "PostalCode"){
            return postalCode;
        }
    }

    ~Person(){
    }
};

class HashTableEntry{
    public:
        int size;
        string key;
        int index;
        vector <Person> people;
        HashTableEntry(){
            size = 0;
            people.resize(size);
            key = "EMPTY";
        }
};

class HashTable {
    private:
        int tableSize;
        string key;
        vector <HashTableEntry> hashTable;
    public:
        void printPerson(Person person){
    cout << person.id << "," << person.first << "," << person.last << "," << person.email << "," << person.phone << "," << person.city << "," <<
    person.state << "," <<  person.postalCode << endl;

}
        
        HashTable(int size, string key){
            tableSize = size;
            this->key = key;
            hashTable.resize(tableSize);
        };
        
        void lookup(string lookupVal){
            uint64_t hashKeyNum = djb2(lookupVal);              //create hasKey
            int hash = hashKeyNum % tableSize;                  //create original index
            int currIndex = 0;                                  //create probing method index
            for (int probe = 0; probe < tableSize;probe++){
                //cout << "in for loop" << endl;
                currIndex = (hash + probe + (probe*probe)) % tableSize;         //quadratic probing
                if (hashTable[currIndex].key == lookupVal){
                    //cout << hashTable[currIndex].size << endl;
                    cout << "Id,FirstName,LastName,Email,Phone,City,State,PostalCode" << endl;
                    for (int p = 0; p<hashTable[currIndex].size;p++){
                        printPerson(hashTable[currIndex].people.at(p));     //use my PrintPerson function
                    }
                    break;
                }
                if (probe == tableSize -1){             //whole table has been probed
                    cout << "No results" << endl;
                }
            }
        }

        void addToTable(Person person){
            //cout << "in beginning of add to table func" << endl;
            string stringKey = person.findKey(key);         //find the value of the key in the person ie if key is state find California
            uint64_t hashKeyNum = djb2(stringKey);          //create hash key
            int hash = hashKeyNum % tableSize;              //mod by table size
            int currIndex = 0;
            for (int probe = 0; probe < tableSize;probe++){         //probe
                //cout << "in for loop" << endl;
                currIndex = (hash + probe + (probe*probe)) % tableSize;
                //cout << currIndex << endl;
                //cout << hashTable[currIndex].key << endl;
                if (hashTable[currIndex].people.size() == 0){           //if an empty spot is found, add person
                    //cout << "in first if statement" << endl;
                    hashTable[currIndex].people.push_back(person);
                    hashTable[currIndex].size++;
                    hashTable[currIndex].key = stringKey;
                    hashTable[currIndex].index = currIndex;
                    break;
                }
                else{
                    if(hashTable[currIndex].key == stringKey){                  //if there is a collision, but it is the correct key, add
                        hashTable[currIndex].people.push_back(person);
                        hashTable[currIndex].size++;
                        break;
                    }
                }
            }
            
        }
        
        void printTable(){
            for (int i = 0;i<tableSize;i++){//iterate through table and print all people
                
                if(hashTable[i].people.size()!=0){    
                    cout << hashTable[i].index << ": " <<  hashTable[i].key << " (" << hashTable[i].size << ")," << endl;
                }
            }

        }
};

void getVal(istream &is, string &str) {
    char ch;
    string line;

    is >> ch;
    getline(is, line);

    str = ch + line;
}

bool isValidKey(const string &key) {
    string validKeys[8] = { "Id", "FirstName", "LastName", "Email", "Phone", "City", "State", "PostalCode" };
    for (int i = 0; i < 8; i++) {
        if (key == validKeys[i]) {
            return true;
        }
    }
    return false;
}

// Person setPerson(string token, int index,int num){       //i tried to use a setPerson function, but the iss didnt work
//     Person* person = new Person;
//     if (num == 0)
//     person->setID(token);
//     if (num == 1) 
//     person->setFirst(token);
//     if (num == 2) 
//     person->setLast(token);
//     if (num == 3) 
//     person->setEmail(token);
//     if (num == 4)
//     person->setPhone(token);
//     if (num == 5) 
//     person->setCity(token);
//     if (num == 6) 
//     person->setState(token);
//     if (num == 7)
//     person->setPostal(token);
// }

void printPerson(Person person){
    cout << person.id << "," << person.first << "," << person.last << "," << person.email << "," << person.phone << "," << person.city << "," <<
    person.state << "," <<  person.postalCode << endl;

}

int main(int argc, char **argv) {
    if (argc != 4) {
        cout << "Usage: ./a.out filename.txt table_size key" << endl;
        return 1;
    }

    string filename = argv[1];
    int tableSize = stoi(argv[2]);
    string key = argv[3];

    ifstream file(filename);
    if (!file) {
        cout << "Unable to open " << filename << endl;
        return 2;
    }

    if (!isValidKey(key)) {
        cout << "Invalid key: " << key << endl;
        return 3;
    }
    
    vector <Person> people;

    // This is an example of how to retreive the tokens from the input file
    // You will need to modify this to fit your needs to build the hash table
    string line, token;
    Person person;
    int index = 0,num=0;
    getline(file, line); // consume header line
    while (getline(file, line)) {
            istringstream iss(line);
            while(getline(iss, token, '\t')){
                
                // if (num < 8){                        switch actually works and is more efficient
                //     if (num == 0){
                //         cout << "setting id" << endl;
                //         person.setID(token);         i dont know why but this code was causing my people vector to access outside of range on 22
                //     }
                //     if (num == 1){ 
                //         cout << "setting first" << endl;
                //         person.setFirst(token);}
                //     if (num == 2){ 
                //         cout << "setting last" << endl;
                //         person.setLast(token);
                //     }
                //     if (num == 3){ 
                //         cout << "setting email" << endl;
                //         person.setEmail(token);
                //     }
                //     if (num == 4){
                //         cout << "setting phone" << endl;
                //         person.setPhone(token);
                //     }
                //     if (num == 5){ 
                //         cout << "setting city" << endl;
                //         person.setCity(token);
                //     }
                //     if (num == 6){ 
                //         cout << "setting state" << endl;
                //         person.setState(token);
                //     }
                //     if (num == 7){
                //         cout << "setting postal" << endl;
                //         person.setPostal(token);
                //     }
                //     num++;
                //     }
                // else{
                    
                //     people.push_back(person);
                //     cout << people.size() << endl;
                //     num = 0;
                // }
            switch (num)        //create a switch with case being int num. num  starts at zero and iterates, essentially, this switch sets all 8 values for each person 
            {
            case 0:
                person.setID(token);
                break;
            case 1:
                person.setFirst(token);
                break;
            case 2:
                person.setLast(token);
                break;
            case 3:
                person.setEmail(token);
                break;
            case 4:
                person.setPhone(token);
                break;
            case 5:
                person.setCity(token);
                break;
            case 6:
                person.setState(token);
                break;
            case 7:
                person.setPostal(token);
                break;
            }
            if (num < 7)
            {
                num++;
            }
            else//when the full person is populated, push back into people ane reset num
            {
                
                people.push_back(person);
                num = 0;
            }
            }
            
        index++;
    }

    HashTable h(tableSize,key);//create the hash table of table size

    for (int j = 0;j<index;j++){
        h.addToTable(people.at(j));//add all people to table
    }

    

    cout << "Commands:" << endl << "\tprint" << endl << "\tlookup <key>" << endl << "\tquit" << endl;
    string cmd, val;
    while (1) {
        cout << endl << "Enter a command:" << endl;
        cin >> cmd;
        if (cmd == "quit") {
            break;
        }
        else if (cmd == "print") {
            h.printTable();
        }
        else if (cmd == "lookup") {
            getVal(cin, val);
            // lookup code here
            
            h.lookup(val);
        }
        else {
            getline(cin, val);
            cout << "Invalid command" << endl;
        }
    }

    return 0;
}
