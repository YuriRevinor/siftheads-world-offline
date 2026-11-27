# Act 3 - Alonzo's Reinforcement

地点：**Chicago**（Vinnie's Safehouse、Darwen's House、City Hall、Crime Hideout、Business Tower、Gigi restaurant、Bar、Slum Buildings），**Palermo, Italy**（Hotel、The Port、Apart Block、Beach Club、Clothing Shop）。
可用作弊码：`revive` `perso` `reload` `equip`。
**【离线版】** 原版在 Darwen's House 升级的武器属性不存档，读档后会恢复初始值。离线版会保存升级，并带到后面几部。

## 全收集清单
- [ ] Main Missions ×11
- [ ] Secondary Missions ×6
  - [ ] GT part（6 个零件）
  - [ ] Hard Headed（通关后二周目才能做，**离线版可以做**，见下文）
- [ ] 通关，解锁 Act 3 的 Basement 宝物

---

## Main Missions

| # | Mission | 类型 | 奖励 | 做法 |
|---|---|---|---|---|
| 1 | **Telling the truth** | Stealth | $2000 | 在 **Darwen's House** 拿 Tranquilizer Gun，去 **City Hall**，在警察用对讲机之前把他们麻醉，然后和 Mayor 对话 |
| 2 | **On the crime scene** | Investigation | $2000 | 去 **Crime Hideout**，一直往前，点 CD |
| 3 | **Lack of money** | Investigation | $4000 | 去 **Business Tower**，一直往右到阳台，点阳台，用镜头找到 Commissioner（右边车旁，从下往上数第 2 层） |
| 4 | **Introspection 1** | Robbery | $1000 | 到 Palermo 后马上进 **Hotel**，一直往上走到两个房间，进左边房间点 suitcase |
| 5 | **Introspection 2** | Robbery | $1000 | Palermo **The Port**，往右下走，点集装箱左边（先把枪准备好），干掉 6 人拿箱子 |
| 6 | **Introspection 3** | Robbery | $1000 | Palermo **Apart Block**，上到顶楼，点箱子 |
| 7 | **The ambush** | Massacre | $2000 | 回 **City Hall** 点门触发对话，去 **Gigi restaurant** 和 Mayor 对话，消灭所有人 |
| 8 | **Talk or suffer** | Brutality | $2000 | The ambush 后自动触发（审讯小游戏） |
| 9 | **Hidden ways** | Massacre | $2000 | 去 **Bar** 的厕所，进密道：右 4 次 → 左 → 右 |
| 10 | **City forces** | Massacre | $2000 | 见到 Alonzo 之后，干掉 3 个穿护甲的敌人 |
| 11 | **The takedown** | Arrest | $0 | 跟着 Alonzo 走，路上捡起 shield，他的机枪过热停下时开枪 |

---

## Secondary Missions（均为 $1000）

| Mission | 做法 |
|---|---|
| **Shooting range** | Vinnie's Safehouse 的 **Pool Yard**：Kiro 哑铃旁边那个房间（有便条的那个），再进中间有灰色横杆的房间，打出 12/12 |
| **Heavy door** | **Bar** 厕所进密道，往左，有挂锁的房间。把挂锁打烂，进去点箱子。建议在主线 Hidden ways 时顺便做 |
| **Full specs** | 在 **Darwen's House** 把 **Walther P99** 升满：Power 升 8 次（20 → 100），Precision 升 3 次（7 → 1），每次 $250，共 $2750。**【代码】** 游戏只检查 Walther P99，别的枪升满不算。Walther P99 在 Gun Shop 卖 $1000，要带在身上才能升级 |
| **Always a pleasure** | Palermo **Beach Club**，到海滩上点海面，有个人被石头困住，射断树枝 |

### GT part（6 个零件）
1. **Vinnie's Safehouse**：Basement 附近。
2. Chicago，**Crime Hideout** 附近的 **Slum Buildings**：右边。
3. **Business Tower**：从正门进，盆栽旁边。
4. Palermo **Beach Club**：进去上楼，沙发旁的地上。
5. Palermo **Clothing Shop**：刚到时店门左边。
6. Chicago **Gigi restaurant**：进楼一直走到垃圾桶，在垃圾桶旁边。

### Hard Headed（通关后的隐藏支线，$1000）
Wiki 说这个任务要有账号才能做，现在拿不到。实际上它是一个**二周目任务**：通关后把被关在 Safehouse 牢房里的 Alonzo 审问一遍。原版只有注册账号才能存档、读档进入二周目，所以 Wiki 才会那么说。**【离线版】** 用本地存档就能做。

#### 做法
1. 把 Act 3 打通，打完最后的 **The takedown**。
2. 片尾出现 **Continue** 时点它。游戏会自动存档并回到主菜单。
3. 在 Act 3 主菜单点 **LOAD GAME**，用**同一个用户名**登录。游戏会从开场重新开始（二周目），主线全部重置，Hard Headed 保持可做状态。
4. 打完开场，回到 **Vinnie's Safehouse**，**先别去做 Telling the truth**。
5. 走到 **Kiro 的哑铃**所在的房间，进左边贴着便条的那扇门，到走廊。
6. 在走廊里进**左侧墙上靠里的那扇门**（近处的黑门是回去的路），到牢房。
7. 牢房里能看到 **Alonzo**，点他会播放审问过场，完成 **Hard Headed**，奖励 $1000。

#### 注意
- **一定要在完成 Telling the truth 之前做。** 二周目一旦完成 Telling the truth，Alonzo 就不会再出现在牢房里，只能再通关一次重来。
- 一周目里牢房是空的，Hard Headed 在菜单里一直显示未完成，这是正常的。
- 第 2 步的 Continue 必须点。存档里的“Hard Headed 可做”状态就是在这一步写入的。

#### 代码依据【代码】
| 环节 | 位置 | 内容 |
|---|---|---|
| 解锁 | `DefineSprite_3398` frame 2637（The takedown 结尾） | `ob_10 = 3`；若 `ob_20 != 3` 则设 `ob_20 = 2` |
| 进入二周目 | 主时间轴 frame 44 | 把 `ob_1`–`ob_10` 重置为 1，`ob_20` 不变。Continue（`DefineSprite_1139`）设 `bonus_unlocked_3 = 2` 并跳到 frame 42 存档 |
| 存档 | frame 42 | 离线版调用 `shwSave()`。帧上的存档动画 `DefineSprite_1127` 在第 150 帧设 `got2go = 2`，然后回主菜单，不依赖服务器 |
| 读档 | frame 43 | `ob_1 == 1` 时从开场（`lvl = 10`）重新开始 |
| 路线 | `DefineSprite_2448`（各场景） | Level 7（哑铃房间）的左门 → Level 46（走廊），走廊的门 `DefineButton2_1785` → Level 39（牢房） |
| 触发 | Level 39 的 `DefineSprite_1775` | 当 `ob_20 != 1 && ob_2 != 3`（Telling the truth 未完成）时显示 Alonzo 和按钮 `DefineButton2_1771` |
| 完成 | 按钮 → frame 74 播放视频 17（`DefineSprite_3399`） | 结尾设 `ob_20 = 3`，`mission_reward = 1000` |

以上是读代码得出的结论，还没有实际打一遍验证。

---

## Basement
Act 3 通关后新增宝物：Kung Fu staff、Femme fatale、Yakuza's vase、Triad's Son's helmet。

---

来源：[Missions in Sift Heads World games](https://siftheads.fandom.com/wiki/Missions_in_Sift_Heads_World_games)、[Act 3 - Alonzo's Reinforcement](https://siftheads.fandom.com/wiki/Sift_Heads_World:_Act_3_-_Alonzo%27s_Reinforcement)。Hard Headed 的分析依据 `SHW_Act3.swf` 脚本。
