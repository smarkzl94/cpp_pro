/*
============================================================================
LeetCode 3. 无重复字符的最长子串（Longest Substring Without Repeating
Characters）
https://leetcode.cn/problems/longest-substring-without-repeating-characters/
============================================================================
题目描述：

  给定一个字符串 s，请你找出其中不含有重复字符的 最长子串 的长度。

  注意：是"子串"（连续），不是"子序列"（可不连续）。

----------------------------------------------------------------------------
示例 1：
  输入：s = "abcabcbb"
  输出：3
  解释：最长无重复子串是 "abc"，长度为 3。

示例 2：
  输入：s = "bbbbb"
  输出：1
  解释：最长无重复子串是 "b"，长度为 1。

示例 3：
  输入：s = "pwwkew"
  输出：3
  解释：最长无重复子串是 "wke"，长度为 3。
        注意 "pwke" 是子序列而非子串，不算。

----------------------------------------------------------------------------
提示（数据范围）：
  0 <= s.length <= 5 * 10^4
  s 由英文字母、数字、符号和空格组成（即可见 ASCII 字符）

----------------------------------------------------------------------------
思路提示（经典的"滑动窗口"登场）：

  朴素思路：枚举每个起点，向后扩张直到遇到重复字符。O(n^2)，会超时吗？
  （5 * 10^4 的平方 = 2.5 * 10^9，必然超时，需要 O(n) 做法。）

  滑动窗口（双指针，推荐）：
    维护窗口 [left, right]，保证窗口内无重复字符。
      - right 从左到右扫一遍；
      - 若 s[right] 已在窗口中，就把 left 右移，
        直到把那个重复字符"挤出"窗口为止；
      - 每一步用窗口长度 right - left + 1 更新答案。

    关键优化：记录每个字符"上一次出现的位置" last[c]。
      当 s[right] 重复时，left 可以直接跳到 last[s[right]] + 1，
      不用一步步挪。（注意还要和当前 left 取 max，为什么？想想 "abba"。）

  数据结构选择：
    - 数组 int last[128]（s 只含可见 ASCII，最快最省）
    - 或 unordered_map<char, int>（更通用）

  复杂度：O(n) 时间（每个字符最多进出窗口一次），O(k) 空间（k 为字符集大小）。
============================================================================
*/

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>

class Solution {
  public:
    int lengthOfLongestSubstring(std::string s) {
        // TODO(你来做)：滑动窗口
        //   1. 准备 last 表：记录字符上次出现的下标（初始化为 -1 表示没出现过）
        //   2. left = 0，right 从 0 扫到 n-1：
        //        - 若 last[s[right]] >= left，说明窗口内已有这个字符，
        //          left 跳到 last[s[right]] + 1
        //        - 更新 last[s[right]] = right
        //        - 用 right - left + 1 更新 ans
        //   3. 返回 ans
        //   边界：空字符串要返回 0
        auto left = 0, right = 0;
        std::unordered_map<char, int> last;
        auto ans = 0;
        while (right < s.size()) {
            if (last.find(s[right]) != last.end() && last[s[right]] >= left) {
                left = last[s[right]] + 1;
            }
            last[s[right]] = right;
            ans = std::max(ans, right - left + 1);
            right++;
        }
        return ans;
    }
};

void test(const std::string &s, int expected) {
    int got = Solution().lengthOfLongestSubstring(s);
    std::cout << (got == expected ? "PASS" : "FAIL") << " -> s = \"" << s
              << "\", expected = " << expected << ", got = " << got << '\n';
}

int main() {
    test("abcabcbb", 3);  // 示例 1
    test("bbbbb", 1);     // 示例 2
    test("pwwkew", 3);    // 示例 3
    test("", 0);          // 空串
    test(" ", 1);         // 单个空格
    test("au", 2);        // 两个不同字符
    test("dvdf", 3);      // "vdf"，left 需要跳过重复的 d
    test("abba", 2);      // 陷阱：left 不能往回退！
    test("tmmzuxt", 5);   // "mzuxt"
    return 0;
}
