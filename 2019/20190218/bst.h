#if !defined(BINARY_SEARCH_TREE_H)
#define BINARY_SEARCH_TREE_H

struct TreeNode
{
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;
    TreeNode *parent = nullptr;
    int value = 0;
};

class BinarySearchTree
{
  private:
    /* data */
    TreeNode *root = nullptr;


    bool findNode(TreeNode* node);

  public:
    void Add(int element);
    int Remove(TreeNode *node);
    TreeNode *Find(const int &element);

    BinarySearchTree(/* args */);
    ~BinarySearchTree();
};

#endif // BINARY_SEARCH_TREE_H
