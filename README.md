# mcts_gobang

## 蒙特卡洛树

生成蒙特卡洛树的四个步骤:

### 1.selection(选择)

从根节点出发<b>root</b>出发,向深度方向递归选择最优的子节点直至当前树的某一叶节点。

策略:在蒙特卡洛树的每一父层,优先选择当前父节点下未被探索的子节点,当所有的子节点都被探索以后,选择<b>UCB</b>值最大的子节点;重复进行,直到选择到当前树的某一个叶节点。

### 2.expansion(扩展)

当到达叶节点<b>node</b>时,需要判断该叶节点所对应的游戏状态。如果游戏结束,则直接进入到步骤4,将胜负值反向传播,更新根节点<b>root</b>到当前叶节点路径经过的所有节点;如果游戏没有结束,
则对当前节点进行扩展,将所有可能的行动添加到当前节点下。

### 3.simulation(模拟)

当树到达某个叶节点<b>node</b>并将其扩展以后,蒙特卡洛树需要评价当前选择的节点<b>node</b>的价值,这里就需要使用蒙特卡洛方法来评价此次落子的价值。
从当前棋盘状态开始,采用随机走子的办法进行<b>n</b>次走子,并判断哪位玩家获胜,如果当前落子的玩家获胜,则返回<b>1</b>,失败返回<b>-1</b>,<b>n</b>回合没有决出胜负则返回<b>0</b>。在这里需要注意,从当前状态开始,紧接着走子的对方棋手,因此从当前状态开始判断是否获胜获得的价值实际上是对方棋手获得
的价值,对于当前棋手需要采用价值的相反数对当前节点<b>node</b>进行更新,每次更新节点时,都需要对<b>value</b>取相反数,因为棋手下棋是交替进行的,每个节点的父节点都是对方棋手的走子。

### 4.back propagation(反向传播)

获得当前节点<b>node</b>的价值并反向传播直至根节点,路径中的相邻的两个节点之间的价值互为相反数。

## 蒙特卡洛树玩家

在这里实现了一种简单的蒙特卡洛树玩家,即每一步落子时生成一棵蒙特卡洛树,然后选择蒙特卡洛树第<b>1</b>层节点(根节点位于第<b>0</b>层)中遍历次数最多的节点(行动)。
