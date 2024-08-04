#include "anagram.h"
#include <iostream>
#include <fstream>

/*
first create constructor which uses getline() to read a word from the textfile and call addWord
then create sortword which just uses std::sort to return sorted string
then create addword which first checks if current word is already lexographically present in tree
    - if it is, check if it is present in corresponding linked list, and if not then add it
    - if not, create a new node with the sorted word as key and list and use insert_node()
then create saveAnagrams which calls one of dfs algorithms and adds it to list
iterate through list and add it to the corresponding text file with newlines
*/
/***
An AnagramDict constructor. This Constructor takes in a filepath
and then creates a binary tree out of all the words inside the 
binary tree. 

Input: 
std::string filename - the path to the file

Output:
N/A
***/
AnagramDict::AnagramDict(std::string filename) {
    filename_ = filename;
    std::string path = "dictionaries/" + filename_;
    std::ifstream file;
    file.open(path.c_str());
    std::string word;
    while (getline(file, word)){ // for each line (word), add it
        addWord(word);
    }
}

/***
An AnagramDict member function. This function sorts the given word in 
lexicographical order

Input: 
std::string word - the word that needs to be sorted

Output:
std::string - the sorted version of the input
***/
std::string AnagramDict::sortWord(std::string word) {
    std::string x = word;
    std::sort(x.begin(), x.end()); // std::sort is in-place
    return x; // since pass by copy can just return param
}

/***
An AnagramDict member function. This function adds a word to the tree.
If the words sorted version already exists in the tree add it to the linked
list of that node. If the sorted word does not exist in the tree create a new
node and insert it into the tree
Duplicated words should not be added to the tree.
Input: 
std::string word - the word that needs to inserted

Output:
N/A
***/
void AnagramDict::addWord(std::string word) {
    Node<std::string, std::list<std::string> >* temp = tree.find_node(sortWord(word)); // look for sorted word in tree
    std::list<std::string> newList;
    if (temp){ // if exists then just add it to linked list
        newList = temp->getData();
        bool exists = false;
        // use iterator in list to check if word already exists in linked list
        for (std::list<std::string>::iterator it = newList.begin(); it != newList.end(); it++)
            if (*it == word)
                exists = true;
        
        if (!exists)
            newList.push_back(word);

        temp->setData(newList);
    }
    else{
        // otherwise create new node (dynamically) and add it to tree using insert_node
        newList.push_back(word);
        Node<std::string, std::list<std::string> > *newNode = new Node<std::string, std::list<std::string> > (sortWord(word), newList);
        tree.insert_node(tree.getRoot(), newNode);
    }
}

/***
An AnagramDict member function. Does a preorder, postorder, or inorder traversal
and then prints out all the anagrams and words.

The output file should be the traversal order of the tree, but only the data on each line. 
View on wiki for more information. 

Input: 
std::string order - The type of order you want to traverse. Can be "pre", "post", "in"

Output:
N/A
***/
void AnagramDict::saveAnagrams(std::string order) {
    std::string path = "output/" + order+"_"+ filename_;
    std::ofstream file;
    file.open(path.c_str());
    if(!file.is_open()) {
        //create new file
        file.open(path.c_str(),std::ios::out);
    }
    std::list<Node<std::string, std::list<std::string> > > words; // empty list with words
    if (order == "pre"){
        tree.pre_order(tree.getRoot(), words);
    }
    else if (order == "post"){
        tree.post_order(tree.getRoot(), words);
    }
    else if (order == "in"){
        tree.in_order(tree.getRoot(), words);
    }
    // iterator of type list which has string and list of string 
    for (std::list<Node<std::string, std::list<std::string> > >::iterator it = words.begin(); it != words.end(); ++it) {
        // iterator which has list of strings to write to file
        std::list<std::string> dataList = it->getData();
        for (std::list<std::string>::iterator jt = dataList.begin(); jt != dataList.end(); ++jt) {
            file << *jt << ' ';
        }
        file << std::endl; // newline after every node
    }
}