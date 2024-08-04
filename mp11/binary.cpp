#include "binary.h"
using namespace std;

/*
first create constructors, getters, and setters for Node and BinaryTree class
then create insert_node which recursively goes through the tree to insert a node if necessary
then create find_node_parent which checks if current nodes children are the key if not recursively go to 
where the key could be based on comparing it with current node and check that, if current node is null return NULL
then create delete_node which follows pseudocode to delete a node while updating tree properly
then create dfs algorithms by first calling left, right, or root, then choose remaining
 - recursively call left,right but can just add to list of the root
connect the public functions to private helper functions b/c within the class you have access to *root
write destructor which calls delete_node on all the nodes of the graph until tree is deleted
*/
/***
A Tree Node constructor. Set all the
member variables (key_, data_, left, right)
of the Node class. 

Input: 
T key - The key for the node
Y data - The actual value from the node

Output:
N/A
***/
template <class T, class Y>
Node<T, Y>::Node(T key, Y data){
    key_ = key;
    data_ = data;
    left = NULL;
    right = NULL;
} 

/***
A Node member getter function. This returns the
key of the node. 

Input: 
N/A

Output:
T - returns the key of the node that is of type T
***/
template <class T, class Y>
T Node<T, Y>::getKey(){
    return key_;
}

/***
A Node member getter function. This returns the
actual data of the node. 

Input: 
N/A

Output:
Y - Returns the data of the node that is of type Y
***/
template <class T, class Y>
Y Node<T, Y>::getData(){
    return data_;
}

/***
A Node member setter function. This sets the key
of a Node.

Input: 
T key - Sets the key of the Node of type T

Output:
N/A
***/
template <class T, class Y>
void Node<T, Y>::setKey(T key){
    key_ = key;
}

/***
A Node member setter function. This sets the actual
data of a Node.

Input: 
Y data - Sets the value of the Node of type Y

Output:
N/A
***/
template <class T, class Y>
void Node<T, Y>::setData(Y data){
    data_ = data;
}


/***
The BinaryTree constructor. Implicitly set the 
values of the member variables


Input: 
N/A

Output:
N/A
***/

template <class T, class Y>
BinaryTree<T,Y>::BinaryTree(){
    root = NULL;
}


/***
A BinaryTree Member fuction. This function takes in two arguments
and inserts a node. If the key already exists in the tree do nothing.

Input: 
Node<T,Y> *parent_node - The current root node
Node<T,Y> *data - The Node that you want to insert into the tree

Output:
N/A
***/
template <class T, class Y>
void BinaryTree<T,Y>::insert_node(Node<T,Y> *parent_node, Node<T,Y> *data){
    if (data == NULL)
        return;
    if (root == NULL){ // if root is null set root to data
        root = data;
        return; 
    }
    
    if (parent_node->getKey() == data->getKey()) // if already there no need to insert
        return;

    if (parent_node->getKey() > data->getKey()){ // if less than current root and left is null insert there
        if (parent_node->left == NULL){
            parent_node->left = data;
            return;
        }
        else{
            insert_node(parent_node->left, data); // otherwise recursion to insert in left subtree
        }
    }
    else {
        if (parent_node->right == NULL){ // same for if greater than
            parent_node->right = data;
            return;
        }
        else{
            insert_node(parent_node->right, data);
        }
    }
}

/***
A BinaryTree Member fuction. This function takes in one arguments
and finds the parent of a node given a key.
If the key does not exist in the tree return NULL.

Input: 
T key - The key of the node you are searching for.

Output:
Node<T,Y> * - returns the parent of the node you are searching for
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::find_node_parent(T key){
    return find_node_parent(root, key); // call helper function but now can use root parameter
}

/***
A BinaryTree Member fuction. This function takes in one arguments
and finds the node given a key.
If the key does not exist in the tree return NULL.

Input: 
T key - The key of the node you are searching for.

Output:
Node<T,Y> * - returns the node you are searching for
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::find_node(T key){
    return find_node(root, key);  // call helper function but now can use root parameter
}

/***
A BinaryTree Member fuction. This function deletes a node in
the binary tree. If the key does not exist do not do anything.

Input: 
T key - The key of the node you are trying to delete.

Output:
N/A
***/
template <class T, class Y>
void BinaryTree<T,Y>::delete_node(T key){
    delete_node(root, key); // call helper function but now can use root parameter
}


/***
A BinaryTree helper fuction. This function deletes a node in
the binary tree. If the key does not exist in the tree return NULL.

Input: 
T key - The key of the node you are trying to delete.
Node<T,Y>*node - The root of the current subtree

Output:
Node<T,Y> * - The new root since the root can be deleted.
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::delete_node(Node<T,Y>* node, T key){
    if (node == NULL) // node to be deleted doesn't exist
        return node;
    
    else if (key < node->getKey()) // its somewhere in left subtree
        node->left = delete_node(node->left, key);
    else if (key > node->getKey())
        node->right = delete_node(node->right, key); // somewhere in right
    else{ // found node to be deleted
        if (!node->left && !node->right){ // doesn't have any children, just delete it
            delete node;
            return NULL;
        }
        else if (!node->left){ // doesn't have left child, just put right child in spot
            Node<T,Y> *temp = node->right;
            delete node;
            return temp;
        }
        else if (!node->right){ // doesn't have right child, put left in spot
            Node<T,Y> *temp = node->left;
            delete node;
            return temp;
        }
        else{ // has both child
            Node<T,Y> *temp = node->right; 
            while(temp->left) // replace deleted node with left most node of right subtree
                temp = temp->left;

            node->setKey(temp->getKey()); // swap values, dont actually delete yet
            node->setData(temp->getData());
            node->right = delete_node(node->right, temp->getKey()); // now delete the node that was used to swap values
        }
    }
    return node; // after recursive calls just pass the node back up 
}
/***
A BinaryTree helper fuction. This function finds the parent node of
a node with a given key for the binary tree. If the key does not exist in the tree return NULL.

Input: 
T key - The key of the node you are trying to find.
Node<T,Y>*node - The root of the current subtree

Output:
Node<T,Y> * - Returns the parent of the key you were searching for
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::find_node_parent(Node<T,Y> *node,T key){
    if (node == NULL) // if reached end of subtree no parent :(
        return NULL;
    // if left or right child has key, this is the parent
    if ((node->left != NULL && node->left->getKey() == key) || (node->right != NULL && node->right->getKey() == key)) 
        return node;

    // otherwise if greater, call on left subtree
    if (node->getKey() > key){
        return find_node_parent(node->left, key);
    }
    // othwerise call on right subtree
    else{
        return find_node_parent(node->right, key);
    }
}

/***
A BinaryTree helper fuction. This function finds the node with 
a given key for the binary tree. If the key does not exist in 
the tree return NULL.

Input: 
T key - The key of the node you are trying to find.
Node<T,Y>*node - The root of the current subtree

Output:
Node<T,Y> * - Returns the Node in the Tree
              for the key you were searching for
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::find_node(Node<T,Y> *node,T key){
    // recursively go left or right based on key until reached node with same key (return node) or end of tree (return NULL)
    if (node == NULL)
        return NULL;
    if (node->getKey() == key){
        return node;
    }
    if (node->getKey() > key){
        return BinaryTree<T,Y>::find_node(node->left, key);
    }
    else{
        return BinaryTree<T,Y>::find_node(node->right, key);
    }
}

/***
A BinaryTree member fuction. This function performs
pre_order traversal on the tree and stores the nodes
inside a list

Input: 
Node<T,Y> *node - The current root of the subtree
List<Node<T,Y>> - A list to store all the nodes.

Output:
N/A
***/
template <class T, class Y>
void BinaryTree<T,Y>::pre_order(Node<T,Y> *node, std::list<Node<T,Y> > &list){
    if (node == NULL)
        return;

    // pre-order has root, left, right, algorithm gave in lecture
    list.push_back(*node);
    pre_order(node->left, list);
    pre_order(node->right, list);

}

/***
A BinaryTree member fuction. This function performs
in_order traversal on the tree and stores the nodes
inside a list

Input: 
Node<T,Y> *node - The current root of the subtree
List<Node<T,Y>> - A list to store all the nodes.

Output:
N/A
***/
template <class T, class Y>
void BinaryTree<T,Y>::in_order(Node<T,Y> *node, std::list<Node<T,Y> > &list){
    if (node == NULL)
        return;

    // in-order has left, root, right, algorithm gave in lecture
    in_order(node->left, list);
    list.push_back(*node);
    in_order(node->right, list);
}

/***
A BinaryTree member fuction. This function performs
post_order traversal on the tree and stores the nodes
inside a list

Input: 
Node<T,Y> *node - The current root of the subtree
List<Node<T,Y>> - A list to store all the nodes.

Output:
N/A
***/
template <class T, class Y>
void BinaryTree<T,Y>::post_order(Node<T,Y> *node, std::list<Node<T,Y> > &list){
    if (node == NULL)
        return;

    // post-order has left, right, root algorithm gave in lecture
    post_order(node->left, list);
    post_order(node->right, list);
    list.push_back(*node);
}

/***
A BinaryTree getter fuction. This function gets
the current root of the binary tree.

Input: 
N/A
Output:
Outputs the root of the binary tree
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::getRoot(){
   return root;
}


/***
The BinaryTree destructor. Make sure all
the TreeNodes that have been created get destroyed
and make sure there are no memory leaks.

Input: 
N/A

Output:
N/A
***/
template <class T, class Y>
BinaryTree<T,Y>::~BinaryTree(){
    // better than using dfs algorithm as long as delete_node() works because tree will continue to update until tree empty
    while (root != NULL)
        root = delete_node(root, root->getKey()); 
}

template class Node<int, std::string>;
template class Node<std::string, std::string>;
template class Node<int, int>;
template class Node<double, double>;
template class Node<std::string, long>;
template class Node<std::string, std::list<std::string> >;
template class Node<std::string, int >;

template class BinaryTree<std::string, std::list<std::string> >;
template class BinaryTree<std::string, std::string>;
template class BinaryTree<int, int>;
template class BinaryTree<int, std::string>;
template class BinaryTree<std::string, int>;
