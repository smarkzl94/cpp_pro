/*
============================================================================
LeetCode 102. 二叉树的层序遍历（Binary Tree Level Order Traversal）
https://leetcode.cn/problems/binary-tree-level-order-traversal/
============================================================================
题目描述：

  给你二叉树的根节点 root，返回其节点值的 层序遍历。
  （即逐层地、从左到右访问所有节点。）

----------------------------------------------------------------------------
示例 1：
      3
     / \
    9  20
      /  \
     15   7
  输入：root = [3,9,20,null,null,15,7]
  输出：[[3],[9,20],[15,7]]

示例 2：
  输入：root = [1]
  输出：[[1]]

示例 3：
  输入：root = []
  输出：[]

----------------------------------------------------------------------------
提示（数据范围）：
  树中节点数目在范围 [0, 2000] 内
  -1000 <= Node.val <= 1000

----------------------------------------------------------------------------
思路提示（经典的 BFS + 队列）：

  层序遍历 = 广度优先搜索（BFS），核心工具是 队列（FIFO）：

    1. root 入队；
    2. 循环直到队列为空：
         - 记录当前队列长度 sz = queue.size()，
           这就是"当前这一层"的节点个数（关键！）；
         - 连续弹出 sz 个节点，收集它们的值，组成一层的结果；
         - 每弹出一个节点，把它的左右孩子（非空）入队——它们是下一层；
    3. 把每层结果 push 进答案。

  为什么需要 sz？（思考题）
    如果不在进入循环前锁定 sz，队列会在遍历过程中混入下一层的节点，
    就分不清层的边界了。

  复杂度：O(n) 时间（每个节点进出队列各一次），O(n) 空间。

  进阶思考：如果题目改成"之"字形锯齿遍历（LC 103），你会怎么改？
============================================================================
*/

#include <iostream>
#include <queue>
#include <vector>

// LeetCode 官方二叉树定义
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    std::vector<std::vector<int>> levelOrder(TreeNode *root) {
        // TODO(你来做)：BFS + 队列
        //   1. root 为空直接返回 {}
        //   2. queue<TreeNode*>，root 入队
        //   3. while 队列非空：
        //        sz = queue.size()
        //        vector<int> level
        //        循环 sz 次：弹出队首 -> 收值 -> 左右孩子入队
        //        level 推入 ans
        //   4. 返回 ans
        if (!root) {
            return {};
        }
        std::queue<TreeNode *> q;
        q.push(root);
        std::vector<std::vector<int>> ans;
        ans.push_back({});
        auto level = 0;
        ans[level] = {};

        auto len = q.size();
        while (!q.empty()) {
            auto node = q.front();
            q.pop();
            ans[level].push_back(node->val);
            if (node->left) {
                q.push(node->left);
            }
            if (node->right) {
                q.push(node->right);
            }
            if (--len == 0) {
                len = q.size();
                level++;
                if (len!=0) {
                    ans.push_back({});
                    ans[level] = {};
                }
            }
        }

        return ans;
    }
};

// ---- 测试辅助 ----

// 用层序数组建树，-1 表示空节点（LeetCode 风格）
TreeNode *build(const std::vector<int> &v) {
    if (v.empty() || v[0] == -1) return nullptr;
    std::vector<TreeNode *> nodes(v.size(), nullptr);
    for (size_t i = 0; i < v.size(); ++i)
        if (v[i] != -1) nodes[i] = new TreeNode(v[i]);
    for (size_t i = 0; i < v.size(); ++i) {
        if (!nodes[i]) continue;
        size_t l = 2 * i + 1, r = 2 * i + 2;
        if (l < v.size()) nodes[i]->left = nodes[l];
        if (r < v.size()) nodes[i]->right = nodes[r];
    }
    return nodes[0];
}

void freeTree(TreeNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

void test(const std::vector<int> &v, const std::vector<std::vector<int>> &expected) {
    TreeNode *root = build(v);
    auto got = Solution().levelOrder(root);
    bool ok = got == expected;
    std::cout << (ok ? "PASS" : "FAIL") << " -> got [";
    for (size_t i = 0; i < got.size(); ++i) {
        std::cout << (i ? ", [" : "[");
        for (size_t j = 0; j < got[i].size(); ++j) std::cout << (j ? "," : "") << got[i][j];
        std::cout << "]";
    }
    std::cout << "]\n";
    freeTree(root);
}

int main() {
    test({3, 9, 20, -1, -1, 15, 7}, {{3}, {9, 20}, {15, 7}}); // 示例 1
    test({1}, {{1}});                                         // 示例 2：单节点
    test({}, {});                                             // 示例 3：空树
    test({1, 2, 3, 4, 5}, {{1}, {2, 3}, {4, 5}});             // 完全二叉树
    test({1, 2, -1, 3, -1,-1,-1, 4}, {{1}, {2}, {3}, {4}});          // 左斜链
    return 0;
}
