/*
============================================================================
LeetCode 70. 爬楼梯（Climbing Stairs）
https://leetcode.cn/problems/climbing-stairs/
============================================================================
题目描述：

  假设你正在爬楼梯。需要 n 阶你才能到达楼顶。
  每次你可以爬 1 或 2 个台阶。你有多少种不同的方法可以爬到楼顶呢？

----------------------------------------------------------------------------
示例 1：
  输入：n = 2
  输出：2
  解释：有两种方法可以爬到楼顶。
    1. 1 阶 + 1 阶
    2. 2 阶

示例 2：
  输入：n = 3
  输出：3
  解释：有三种方法可以爬到楼顶。
    1. 1 阶 + 1 阶 + 1 阶
    2. 1 阶 + 2 阶
    3. 2 阶 + 1 阶

----------------------------------------------------------------------------
提示（数据范围）：
  1 <= n <= 45

----------------------------------------------------------------------------
思路提示（动态规划第一课）：

  关键一问：最后一步之前，你在哪？
    要到第 n 阶，最后一步要么是从第 n-1 阶迈 1 步，
    要么是从第 n-2 阶迈 2 步。别无可能。

  于是设 f(n) = 爬到第 n 阶的方法数，则：
    f(n) = f(n-1) + f(n-2)      <- 状态转移方程
    f(1) = 1, f(2) = 2          <- 边界（base case）

  看出来了吗？这就是 斐波那契数列 换了个马甲。

  三种实现层次（建议都写一遍，体会 DP 的优化路径）：
    1. 纯递归 f(n) = f(n-1) + f(n-2)
       —— 会超时！想想为什么？（f(n-3) 被重复算了几次？画一下递归树）
    2. 记忆化递归 / 数组递推：开一个数组 dp[]，从小到大填表
       —— O(n) 时间，O(n) 空间
    3. 滚动变量：递推只依赖前两项，用两个变量滚动即可
       —— O(n) 时间，O(1) 空间（最优解）

  复杂度：O(n) 时间，O(1) 空间（滚动变量版）。
============================================================================
*/

#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    // 方法一：纯递归（已实现——正确但 O(2^n)，n=45 要跑 5 秒+）
    int climbStairs(int n) {
        if (n == 0) {
            return 0;
        } else if (n == 1) {
            return 1;
        } else if (n == 2) {
            return 2;
        } else {
            return climbStairs(n - 1) + climbStairs(n - 2);
        }
    }

    // 方法二：递推 + 滚动变量（O(n) 时间，O(1) 空间）
    int climbStairs_dp(int n) {
        // TODO(你来做)：
        //   1. 特判 n <= 2 直接返回 n
        //   2. prev2 = f(1) = 1, prev1 = f(2) = 2
        //   3. 从 i = 3 递推到 n：
        //        cur = prev1 + prev2
        //        prev2 = prev1, prev1 = cur
        //   4. 返回 prev1
        vector<int> vec (n+1);
        for (size_t index = 0; index < n+1; ++index) {
            if (index <= 2) {
                vec[index] = index;
            } else {
                vec[index] = vec[index - 1] + vec[index - 2];
            }
        }
        return vec[n];
    }
};

void test(int n, int expected) {
    int got_dp = Solution().climbStairs_dp(n);
    std::cout << (got_dp == expected ? "PASS" : "FAIL") << "[dp] -> n = " << n
              << ", expected = " << expected << ", got = " << got_dp;
    if (n <= 10) {  // 递归版只在小数据上验证，n=45 要跑 5 秒+
        int got_rec = Solution().climbStairs(n);
        std::cout << "  " << (got_rec == expected ? "PASS" : "FAIL") << "[recursive]";
    }
    std::cout << '\n';
}

int main() {
    test(1, 1);  // 只有 1 种
    test(2, 2);  // 示例 1
    test(3, 3);  // 示例 2
    test(4, 5);  // 斐波那契
    test(5, 8);
    test(10, 89);
    test(45, 1836311903);  // 最大值，int 刚好装得下（比 INT_MAX 小一点）
    return 0;
}
