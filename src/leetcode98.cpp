/*
============================================================================
LeetCode 98. 验证二叉搜索树（Validate Binary Search Tree）
https://leetcode.cn/problems/validate-binary-search-tree/
============================================================================
题目描述：

  给你一个二叉树的根节点 root，判断其是否是一个有效的二叉搜索树。

  有效 二叉搜索树定义如下：
    - 节点的左子树只包含 严格小于 当前节点值的节点
    - 节点的右子树只包含 严格大于 当前节点值的节点
    - 所有左子树和右子树自身必须也是二叉搜索树

----------------------------------------------------------------------------
示例 1：
      2
     / \
    1   3
  输入：root = [2,1,3]
  输出：true

示例 2：
      5
     / \
    1   4
       / \
      3   6
  输入：root = [5,1,4,null,null,3,6]
  输出：false
  解释：根节点的值是 5，但是右子节点 4 的左孩子 3 比 5 小。

----------------------------------------------------------------------------
提示（数据范围）：
  树中节点数目范围在 [1, 10^4] 内
  -2^31 <= Node.val <= 2^31 - 1    <- 注意！节点值可能是 INT_MIN / INT_MAX

----------------------------------------------------------------------------
思路提示（先自己写，写完再看这里的经典陷阱）：

  陷阱：只检查"左孩子 < 自己 < 右孩子"是 不够的！
  上面示例 2 就是反例：4 的左孩子 3 < 4 满足局部条件，
  但 3 在根节点 5 的右子树里，必须 > 5 才行。

  也就是说：每个节点要满足的不是和父节点比较，
  而是落在祖先传下来的一个 取值区间 (lower, upper) 里。

  方法一：递归 + 区间约束
    helper(node, lower, upper)：
      node 为空 -> true
      node->val 必须在 (lower, upper) 开区间内，否则 false
      左子树递归 (lower, node->val)，右子树递归 (node->val, upper)
    注意节点值可能是 INT_MIN/INT_MAX，区间用 long 或用空指针表示无界

  方法二：中序遍历
    BST 的中序遍历结果一定是 严格递增 的。
    中序遍历一遍，检查是否前一个值 < 当前值即可。

  复杂度：O(n) 时间，O(h) 递归栈空间（h 为树高）。
============================================================================
*/

#include <climits>
#include <iostream>
#include <vector>
#include <queue>

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
    // 方法一：递归 + 区间约束
    //
    // 核心观察：一个节点要满足的约束不是"和父节点比"，
    // 而是"落在所有祖先共同限定的开区间 (lower, upper) 里"。
    //   - 往左走：左孩子的值必须 < 当前值，区间上界收紧为 node->val
    //   - 往右走：右孩子的值必须 > 当前值，区间下界收紧为 node->val
    //
    // 以示例 2 为例（5 的右子树里藏了个 3）：
    //   根 5：区间 (-inf, +inf) ✓
    //   右孩子 4：区间 (5, +inf) ✓，继续往下
    //   4 的左孩子 3：区间 (5, 4) -> 3 不在区间里 ✗，当场判定 false
    //
    // 注意区间端点用 long long：节点值可能是 INT_MIN / INT_MAX，
    // 用 int 哨兵会撞边界（测试里的 {2147483647} 就是抓这个的）。
    bool isValidBST_range(TreeNode *root) {
        return checkRange(root, LLONG_MIN, LLONG_MAX);
    }

    // 方法二：中序遍历 + 严格递增检查
    //
    // 核心性质：BST 的中序遍历（左->根->右）结果必然严格递增。
    // 所以只要按中序顺序访问节点，比较"前一个值 prev"和当前值：
    // 一旦出现 prev >= current，就不是 BST。
    //
    // prev 用引用参数在递归间传递；has_prev 处理"第一个节点没有前驱"。
    bool isValidBST_inorder(TreeNode *root) {
        long long prev = 0;
        bool has_prev = false;
        return checkInorder(root, prev, has_prev);
    }

private:
    bool checkRange(TreeNode *node, long long lower, long long upper) {
        if (!node)
            return true; // 空树合法
        if (node->val <= lower || node->val >= upper)
            return false; // 越出祖先给的区间
        // 左子树：上界收紧；右子树：下界收紧
        return checkRange(node->left, lower, node->val) &&
               checkRange(node->right, node->val, upper);
    }

    bool checkInorder(TreeNode *node, long long &prev, bool &has_prev) {
        if (!node)
            return true;
        if (!checkInorder(node->left, prev, has_prev))
            return false; // 左子树已判定失败，提前退出
        // "访问"当前节点：和上一个访问的值比较
        if (has_prev && node->val <= prev)
            return false; // 没有严格递增
        prev = node->val;
        has_prev = true;
        return checkInorder(node->right, prev, has_prev);
    }
};

// ---- 测试辅助 ----

// 用层序数组建树，INT_MIN 哨兵改用空指针语义：用 vector<TreeNode*> 直接搭
// 这里为了简单，提供两种建树方式

// 方式一：层序数组，用一个不可能的标记值表示 null（测试里用不到 INT_MIN 做节点值）
static const long long NULL_MARK = -3000000000LL; // 小于 INT_MIN，保证不和真实值冲突

TreeNode *build(const std::vector<long long> &v) {
    if (v.empty() || v[0] == NULL_MARK) return nullptr;
    std::vector<TreeNode *> nodes(v.size(), nullptr);
    for (size_t i = 0; i < v.size(); ++i)
        if (v[i] != NULL_MARK) nodes[i] = new TreeNode((int)v[i]);
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

void test(const std::vector<long long> &v, bool expected) {
    TreeNode *root = build(v);
    bool got1 = Solution().isValidBST_range(root);
    bool got2 = Solution().isValidBST_inorder(root);
    std::cout << (got1 == expected ? "PASS" : "FAIL") << "[range] "
              << (got2 == expected ? "PASS" : "FAIL") << "[inorder]"
              << " -> expected = " << (expected ? "true" : "false") << '\n';
    freeTree(root);
}

int main() {
    test({2, 1, 3}, true);                               // 示例 1
    test({5, 1, 4, NULL_MARK, NULL_MARK, 3, 6}, false);  // 示例 2：经典陷阱
    test({1}, true);                                     // 单节点
    test({2, 2, 2}, false);                              // 严格小于/大于，相等不行
    test({5, 4, 6, NULL_MARK, NULL_MARK, 3, 7}, false);  // 3 在 5 的右子树里
    test({3, 1, 5, 0, 2, 4, 6}, true);                   // 标准 BST
    test({2147483647}, true);                            // INT_MAX 单节点（边界值陷阱）
    return 0;
}
