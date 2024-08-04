#include "anagram.h"


int main(int argc, char** argv){
    // if (argc < 3)
    // {
    //     printf("You need a valid dictionary file and order.\n");
    //     return -1;
    // }
    printf("Creating dictionary with file %s\n", "test2.txt");


    AnagramDict my_test("test2.txt");

    printf("Traversing using %s order\n", "post");
    my_test.saveAnagrams("post");
    return 0;
}