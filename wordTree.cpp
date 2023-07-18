#include "wordTree.h"

#include "codealg.h"

#include <assert.h>
// Node for building a tree of the word
// every word will have a space added after them 'asd' -> 'asd '

namespace wordTree{
Node::Node(){
    for(int i = 0 ; i < CHILD_MAX ; ++i){
        mChildren[i] = nullptr;
    }
}
Node* Node::nodeAtInd(int aInd) const {
    assert(aInd>=0 && aInd < CHILD_MAX);
    return mChildren[aInd];
}
Node* Node::nodeAtChar(char aChar) const {
    return nodeAtInd(codealg::charToInt(aChar));
}
Node* Node::addNodeAtChar(char aChar, Node* aNewNode){
    int charCode = codealg::charToInt(aChar);
    return mChildren[charCode] = aNewNode;
}
bool Node::isWord() const {
    // an ending space marks the end of the word
    return nodeAtChar(' ');
}
void Node::markAsWord(){
    if(isWord())
        return;
    addNodeAtChar(' ', new Node);
}
}