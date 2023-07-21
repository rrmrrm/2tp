
#pragma once

namespace wordTree{

constexpr int CHILD_MAX = 27;
// Node that represents tree of every available word
//
// if the words are{"a", "ab", "ba"}
// then the tree would look like this:
//       root 
//      /     \
//     a       b
//     | \     |
//     a ' '   a
//             |
//            ' '
// every word has a space after them  like 'ab' -> 'ab '
class Node{
    /// @brief mChildren
    /// each element stores null or a pointer to the next node
    /// a non-null pointer at index 0 means there exists a word which next character is 'a' (1 means 'b' and so on)
    /// a non-null pointer at index 26 (which is the index corresponding to the ' ' char) means the node marks the end of a valid word.
    Node* mChildren[CHILD_MAX];

    Node* mParent = nullptr;
    char mChar = '-';
public:
    Node();
    ~Node();
    Node* nodeAtInd(int aInd) const ;
    Node* nodeAtChar(char aChar) const;
    Node* addNodeAtChar(char aChar, Node* aNewNode);
    bool isWord() const ;
    void markAsWord();
};
}
