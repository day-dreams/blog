#include "bst.h"

void BinarySearchTree::Add(int element)
{
    if (this->root == nullptr)
    {
        this->root = new TreeNode();
        this->root->value = element;
        return;
    }

    auto ite = this->root;
    auto parent = ite;
    while (ite != nullptr)
    {
        parent = ite;
        if (ite->value > element)
        {
            ite = ite->left;
        }
        else
        {
            ite = ite->right;
        }
    }

    auto node = new TreeNode();
    node->value = element;
    node->parent = parent;
    if (parent->value > element)
    {
        parent->left = node;
    }
    else
    {
        parent->right = node;
    }
}

int BinarySearchTree::Remove(TreeNode *node)
{
    /**
     * TODO:
     **/
}

bool BinarySearchTree::findNode(TreeNode* node)    {

}
TreeNode *BinarySearchTree::Find(const int &element)
{
    auto ite = this->root;

    while (ite != nullptr)
    {
        /* code */
        if (ite->value == element)
        {
            return ite;
        }
        else if (ite->value > element)
        {
            if (ite->left == nullptr)
            {
                return nullptr;
            }
            ite = ite->left;
        }
        else if (ite->value < element)
        {
            if (ite->right == nullptr)
            {
                return nullptr;
            }
            ite = ite->right;
        }
    }

    return nullptr;
}

BinarySearchTree::BinarySearchTree(/* args */) {}
BinarySearchTree::~BinarySearchTree()
{
}