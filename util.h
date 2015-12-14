//
// Created by 박진성 on 2015. 12. 7..
//

#ifndef SOLARS_UTIL_H
#define SOLARS_UTIL_H

#include <string>
#include <osg/Node>
#include <osg/Group>

template <class T>
T* findNamedNode(const std::string& searchName,
                 osg::Node* currNode)
{
    osg::Group* currGroup;
//    osg::Node* foundNode;

    // check to see if we have a valid (non-NULL) node.
    // if we do have a null node, return NULL.
    if ( !currNode)
    {
        return NULL;
    }

    // We have a valid node, check to see if this is the node we
    // are looking for. If so, return the current node.
    if (currNode->getName() == searchName)
    {
        T *finded = dynamic_cast<T *>(currNode);
        if (finded != NULL)
            return finded;
    }

    // We have a valid node, but not the one we are looking for.
    // Check to see if it has children (non-leaf node). If the node
    // has children, check each of the child nodes by recursive call.
    // If one of the recursive calls returns a non-null value we have
    // found the correct node, so return this node.
    // If we check all of the children and have not found the node,
    // return NULL
    currGroup = currNode->asGroup(); // returns NULL if not a group.
    if ( currGroup )
    {
        for (unsigned int i = 0 ; i < currGroup->getNumChildren(); i ++)
        {
            T *foundNode = findNamedNode<T>(searchName, currGroup->getChild(i));
            if (foundNode)
                return foundNode; // found a match!
        }
        return NULL; // We have checked each child node - no match found.
    }
    else
    {
        return NULL; // leaf node, no match
    }
}

#endif //SOLARS_UTIL_H
