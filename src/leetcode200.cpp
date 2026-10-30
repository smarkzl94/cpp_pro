/*
============================================================================
LeetCode 200. 岛屿数量（Number of Islands）
https://leetcode.cn/problems/number-of-islands/
============================================================================
题目描述：

  给你一个由 '1'（陆地）和 '0'（水）组成的二维网格 grid，
  请你计算网格中岛屿的数量。

  岛屿总是被水包围，并且每座岛屿只能由 水平方向或垂直方向
  上相邻的陆地连接形成（对角线不算！）。
  此外，你可以假设网格的四个边均被水包围。

----------------------------------------------------------------------------
示例 1：
  输入：grid = [
    ['1','1','1','1','0'],
    ['1','1','0','1','0'],
    ['1','1','0','0','0'],
    ['0','0','0','0','0']
  ]
  输出：1

示例 2：
  输入：grid = [
    ['1','1','0','0','0'],
    ['1','1','0','0','0'],
    ['0','0','1','0','0'],
    ['0','0','0','1','1']
  ]
  输出：3

----------------------------------------------------------------------------
提示（数据范围）：
  m == grid.length
  n == grid[i].length
  1 <= m, n <= 300
  grid[i][j] 的值为 '0' 或 '1'

----------------------------------------------------------------------------
思路提示（网格上的 DFS / BFS，二选一）：

  整体框架（两种方法通用）：
    遍历每个格子，遇到 '1' 就 岛屿计数 +1，
    然后从它出发做一次搜索，把 整座岛 都标记掉（淹没），
    保证每座岛只被数一次。

  方法一：DFS 淹没法（推荐先写这个）
    从 (r, c) 出发递归访问 上下左右 四个邻居：
      - 越界 或 当前格子是 '0' -> 直接返回
      - 否则把格子改成 '0'（淹没，相当于打"已访问"标记），
        再递归四个方向
    小技巧：方向数组 dx[4]={-1,1,0,0}, dy[4]={0,0,-1,1}

  方法二：BFS 淹没法
    和 LC 102 一样的队列套路，遇到 '1' 就把坐标入队，
    弹出时扩展四个方向的陆地邻居入队并淹没。
    这里不需要分层，所以不用锁 sz。

  注意：本题直接修改 grid 当"已访问"标记，省一个 visited 数组。
        （面试时如果面试官不允许改输入，就换成 visited[m][n]。）

  复杂度：O(m*n) 时间，每个格子最多访问一次；
          DFS 最坏 O(m*n) 递归深度，BFS 最坏 O(m*n) 队列空间。
============================================================================
*/

#include <iostream>
#include <vector>
#include <queue>

class Solution {
public:
    /*
    // ================================================================
    // 错误示范：数"左上角"法（已被反例证伪，留作警示）
    //
    // 思路：每个上方、左方都不是 '1' 的陆格子，当作一座新岛。
    // 反例 1（阶梯形，内部）：        反例 2（环形，合并补丁也救不了）：
    //   0 1 1                         1 1 1
    //   1 1 0   -> 1 座岛被数成 2     1 0 1
    //                                 1 1 1   -> (2,2) 的上/左本就同属一岛
    // 根本原因：连通性是非局部信息，一个格子属于哪座岛取决于
    // 可能绕很远的路径，只看上/左邻居的局部规则无法判断。
    // 数连通分量必须遍历：DFS / BFS / 并查集，没有捷径。
    // ================================================================
    int numIslands(std::vector<std::vector<char>> &grid) {
        auto nums = grid[0][0] - '0';
        for (size_t index = 1; index < grid.size(); ++index) {
            if (grid[index - 1][0] - '0' == 0 && grid[index][0] - '0') {
                nums += 1;
            }
        }
        for (size_t index = 1; index < grid[0].size(); ++index) {
            if (grid[0][index - 1] - '0' == 0 && grid[0][index] - '0') {
                nums += 1;
            }
        }

        for (size_t row = 1; row < grid.size(); ++row) {
            for (size_t col = 1; col < grid[0].size(); ++col) {
                if (grid[row - 1][col] == '1' || grid[row][col - 1] == '1') {
                    continue;
                } else if (grid[row][col] == '1') {
                    nums += 1;
                }
            }
        }

        return nums;
    }
    */

    // 方法二：DFS 淹没法
    int numIslands_dfs(std::vector<std::vector<char>> &grid) {
        // TODO(你来做)：
        //   1. 遍历每个格子 (r, c)
        //   2. 若 grid[r][c] == '1'：计数 +1，并调用 dfs 淹没整座岛
        //   3. 返回计数
        auto nums = 0;
        for (uint row = 0; row < grid.size(); ++row) {
            for (uint col = 0; col < grid[0].size(); ++col) {
                if (grid[row][col] == '1') {
                    nums += 1;
                    dfs(grid, row, col);
                }
            }
        }
        return nums;
    }

    // 方法三：BFS 淹没法
    int numIslands_bfs(std::vector<std::vector<char>> &grid) {
        // TODO(你来做)：
        //   1. 遍历每个格子 (r, c)
        //   2. 若 grid[r][c] == '1'：计数 +1，坐标入队后做 BFS 淹没整座岛
        //      （队列里放坐标，可以用 pair<int,int>，也可以把 r*n+c 编码成 int）
        //   3. 返回计数
                auto nums = 0;
        for (uint row = 0; row < grid.size(); ++row) {
            for (uint col = 0; col < grid[0].size(); ++col) {
                if (grid[row][col] == '1') {
                    nums += 1;
                    bfs(grid, row, col);
                }
            }
        }
        return nums;
    }

private:
      void bfs(std::vector<std::vector<char>> &grid, int row, int col) {
        std::queue<std::pair<int, int>> q;
        q.push({row, col});

        while(!q.empty()){
          auto [r,c] = q.front();
          q.pop();
          if (grid[r][c] == '0') {
            continue;
          }
          grid[r][c] = '0';
          if (r - 1 >= 0) {
            q.push({r - 1, c});
          }
          if (r < grid.size() - 1) {
            q.push({r + 1, c});
          }
          if (c - 1 >= 0) {
            q.push({r, c - 1});
          }
          if (c < grid[0].size() - 1) {
            q.push({r, c + 1});
          }
        }
        }
      
      void dfs(std::vector<std::vector<char>> &grid, int row, int col) {
        if (grid[row][col] == '0') {
            return;
        }
        grid[row][col] = '0';
        if (row - 1 >= 0) {
            dfs(grid, row - 1, col);
        }
        if (row < grid.size() - 1) {
            dfs(grid, row + 1, col);
        }
        if (col - 1 >= 0) {
            dfs(grid, row, col - 1);
        }
        if (col < grid[0].size() - 1) {
            dfs(grid, row, col + 1);
        }
    }
};

void test(const std::vector<std::vector<char>> &grid, int expected) {
    // 每种方法单独拷贝一份 grid（淹没法会修改输入）
    auto g2 = grid;
    int got2 = Solution().numIslands_dfs(g2);
    auto g3 = grid;
    int got3 = Solution().numIslands_bfs(g3);
    std::cout << (got2 == expected ? "PASS" : "FAIL") << "[dfs] "
              << (got3 == expected ? "PASS" : "FAIL") << "[bfs]"
              << " -> expected = " << expected << '\n';
}

int main() {
    // 示例 1：一整块大陆
    test({{'1', '1', '1', '1', '0'},
          {'1', '1', '0', '1', '0'},
          {'1', '1', '0', '0', '0'},
          {'0', '0', '0', '0', '0'}},
         1);
    // 示例 2：三座岛
    test({{'1', '1', '0', '0', '0'},
          {'1', '1', '0', '0', '0'},
          {'0', '0', '1', '0', '0'},
          {'0', '0', '0', '1', '1'}},
         3);
    // 全是水
    test({{'0', '0'}, {'0', '0'}}, 0);
    // 全是陆地
    test({{'1', '1'}, {'1', '1'}}, 1);
    // 单个格子
    test({{'1'}}, 1);
    // 对角线不相连：两座岛
    test({{'1', '0'}, {'0', '1'}}, 2);
    // L 形：向下再向左延伸，必须能"向左淹到第 0 列"
    test({{'0', '1'}, {'1', '1'}}, 1);
    return 0;
}
