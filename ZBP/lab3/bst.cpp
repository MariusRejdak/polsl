// Marius Rejdak
// Informatyka, mgr, OS1

/*

Running:
#!/bin/fish
g++ -Wall -W -std=c++11 -o3 -o bst bst.cpp; and ./bst

*/

#include "bst.h"

template <class _Key, class _Compare>
BST<_Key,_Compare>::BST(const _Compare& comp) : comp(comp), root(nullptr)
{
    //empty
}

template <class _Key, class _Compare>
bool BST<_Key,_Compare>::insert(_Key value)
{
    if (find(value))
        return false;

    shared_ptr e = shared_ptr(new Node(value)), y = nullptr, x = root;

    while (x != nullptr)
    {
        y = x;
        if (comp(e->value, x->value))
            x = x->left;
        else
            x = x->right;
    }
    e->parent = y;

    if (y == nullptr)
    {
        root = e;
    }
    else
    {
        if (comp(e->value, y->value))
            y->left = e;
        else
            y->right = e;
    }
    return true;
}

template <class _Key, class _Compare>
bool BST<_Key,_Compare>::find(_Key value)
{
    shared_ptr x = root;
    while ((x != nullptr) && !compe(value, x->value))
    {
        if (comp(value, x->value))
            x = x->left;
        else
            x = x->right;
    }
    return !((x == nullptr) || !compe(x->value, value));
}

template <class _Key, class _Compare>
bool BST<_Key,_Compare>::erase(_Key value)
{
    shared_ptr x = nullptr;
    x = getNode(value);
    if ((x != nullptr) && compe(x->value, value))
    {
        shared_ptr y = nullptr;
        if ((x->left == nullptr) || (x->right == nullptr))
        {
            y = x;
        }
        else
        {
            y = next(x);
        }

        if (y->left != nullptr)
        {
            x = y->left;
        }
        else
        {
            x = y->right;
        }

        if (x != nullptr)
        {
            x->parent = y->parent;
        }

        if (y->parent == nullptr)
        {
            root = x;
        }
        else
        {
            if (y == y->parent->left)
            {
                y->parent->left = x;
            }
            else 
            {
                y->parent->right = x;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

template <class _Key, class _Compare>
typename BST<_Key,_Compare>::key_type& BST<_Key,_Compare>::min(void)
{
    return root->min();
}

template <class _Key, class _Compare>
typename BST<_Key,_Compare>::key_type& BST<_Key,_Compare>::max(void)
{
    return root->max();
}

template <class _Key, class _Compare>
typename BST<_Key,_Compare>::shared_ptr BST<_Key,_Compare>::next(shared_ptr x)
{
    if (x->right != nullptr)
    {
       return getNode(x->right->min());
    }
    shared_ptr y = x->parent;
    while ((y != nullptr) && (x == y->right))
    {
        x = y;
        y = y->parent;
   }
   return y;
}

template <class _Key, class _Compare>
typename BST<_Key,_Compare>::shared_ptr BST<_Key,_Compare>::getNode(key_type value)
{
    shared_ptr x = root;
    while ((x != nullptr) && !compe(value, x->value))
    {
        if (comp(value, x->value))
        {
            x = x->left;
        }
        else 
        {
            x = x->right;
        }
    }
   return x;
}

template <class _Key, class _Compare>
BST<_Key,_Compare>::Node::Node(key_type x) : value(x)
{
    //empty
}

template <class _Key, class _Compare>
typename BST<_Key,_Compare>::key_type& BST<_Key,_Compare>::Node::min()
{
    if (left != nullptr)
        return left->min();
    else
        return value;
}

template <class _Key, class _Compare>
typename BST<_Key,_Compare>::key_type&BST<_Key,_Compare>::Node::max()
{
    if (right != nullptr)
        return right->max();
    else
        return value;
}

#include <iostream>

using namespace std;

/*int main()
{
    BST<int> testBST;
    testBST.insert(13);
    testBST.insert(13);
    testBST.insert(3);
    testBST.insert(4);
    testBST.insert(12);
    testBST.insert(5);
    testBST.insert(14);
    testBST.insert(16);
    //testBST.insert(17);

    int x = testBST.min();
    int y = testBST.max();
    cout << testBST.find(3) << " " << testBST.find(4) << " " << x  << " " << y<< endl;
    testBST.erase(4);
    x = testBST.min();
    y = testBST.max();
    cout << testBST.find(3) << " " << testBST.find(4) << " " << x  << " " << y<< endl;
    return 0;
}*/