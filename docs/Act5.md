# Act 5 - An Exotic Job

地点：**Amazon, Brazil**（West Road、North Road、East Road、Safehouse、Village、Villa，以及火车）。
可用作弊码：`revive` `perso` `reload` `equip`。开局可选 AK-47 或 Revolver。

## 全收集清单
- [ ] Main Missions ×9
- [ ] Secondary Missions ×6
  - [ ] So Precious（10 颗红宝石）
  - [ ] Stealing Thieves（6 个保险箱，附密码）
  - [ ] Jungle Traps（躲过 6 个陷阱）
- [ ] 通关（这一部 Basement 有 bug，见下文）

---

## Main Missions（均为 $2000）

| # | Mission | 类型 | 做法 |
|---|---|---|---|
| 1 | **Without A Map** | Survival | 和 Kiro/Vinnie 对话触发，上 ×2，点船 |
| 2 | **On The Trail** | Investigation | 去 **North Road**，上 ×2，点右边树旁的水坑或水花，找出所有手下 |
| 3 | **Head Over Heels** | Assassination | 消灭所有人，再射断绳子 |
| 4 | **Fireworks** | Assassination | Head Over Heels 后触发。在 Gun Shop 买 **C4 Explosives**（到 **Village** 后右转），去 **East Road**，往左放 2 个炸药，卡车来时按引爆 |
| 5 | **Let Them Go** | Sniping | 去 **Safehouse**，不惊动敌人全部消灭（房子 → 左 → 右 → 打尸体） |
| 6 | **Getting Away** | Assassination | 击杀逃跑的人 |
| 7 | **Catch A Train** | Pursuit | 去 **Village**，上 ×2，右 ×2，直走，点摩托车，消灭所有人（1 个 QTE） |
| 8 | **Taking The Lead** | Precision | 打掉 3 人，再打 2 下拉杆 |
| 9 | **Give It Back** | Massacre | 上 ×5 → 下 → 上 ×6 一路清敌，点 Artefact。Taking The Lead 之后去正门上楼，点 Mr. Sanavan |

---

## Secondary Missions（均为 $500）

| Mission | 做法 |
|---|---|
| **Disturbing Muscles** | **Village** 上 ×2 右转，进 **Bar**，点肌肉男，快速连按 **空格** |
| **Precision Game** | 同一个 Bar，点扔飞刀的人，准星对准靶子时左键 |
| **Modern Fishing** | 刚到 **West Road** 时瞄准左边的小溪，鱼跳起来时射它 |
| **Jungle Traps** | 在 West / North / East Road 探索时会随机出现陷阱，完成 6 次 QTE。躲过 6 个后不再出现 |

### So Precious（10 颗红宝石）
1. 开场打完骑摩托的敌人后，角色左边。
2. 刚到 **West Road**，角色旁的地上（瞄准才看得清）。
3. **West Road** 上 ×2，左边树上，打下来。
4. **West Road** 上 ×4，左边石头上。
5. **North Road** 上，右边地上。
6. **North Road** 上 ×3，左边地上。
7. Head Over Heels 之后左 ×2，左边树上，打下来。
8. **East Road** 左 ×2，右上方的藤蔓上，打下来。
9. **Village** 上 ×2，左上方树上，打下来。
10. **Village** 上 ×2，右 ×2，直走，摩托车旁的牌子左边。

### Stealing Thieves（6 个保险箱，每个 $500）
| 位置 | 密码 |
|---|---|
| **Safehouse**：进去往右，床旁边 | 6 - 22 - 4 |
| **North Road**：Head Over Heels 之后左走，爬梯子，正前方 | 20 - 8 - 0 |
| **Village**：右转（Gun Shop），Gun Shop 右边 | 12 - 1 - 19 |
| **Village**：上 ×2，Clothing Shop 左边 | 18 - 23 - 3 |
| **Village**：上 ×2，右 ×2，进左边房子（先备好枪），左边 | 23 - 9 - 11 |
| **Villa**：往上进正门，楼梯左边 | 2 - 17 - 5 |

开锁方法：转到听见咔哒声就反方向转。比如向左转到 20，再向右转到 8，再向左转到 0。

---

## Basement 的 bug（离线版已修复）
原版 Act 5 里大部分宝物不显示，只剩 Revolver、Vinnie's first pet、Red Mafia tie 和第一块车牌。

**【代码】** 原因有两个：
1. 宝物室里有 3 件宝物检查的是 `bonus_unlocked_5`，而游戏里没有任何地方会设置它，所以永远不显示：**Young Shorty**、**Mobo**、**Police Riot Shield**。
2. Act 2、Act 3 的宝物要看对应那一部的通关标记。原版在线存档到 Act 5 时，这些标记似乎没能带过来。

**【离线版】** `offline\SHW_Act5.swf` 已修复第 1 点：Young Shorty 改成和 Act 4 一样始终显示，Mobo 和 Police Riot Shield 改为检查 Act 4 的通关标记。第 2 点在离线版里不是问题，**只要 Act 1–4 都用同一个用户名玩并通关**，通关标记会随存档一路带到 Act 5，宝物就会全部出现。

---

来源：[Missions in Sift Heads World games](https://siftheads.fandom.com/wiki/Missions_in_Sift_Heads_World_games)、[Act 5 - An Exotic Job](https://siftheads.fandom.com/wiki/Sift_Heads_World:_Act_5_-_An_Exotic_Job)、[Basement](https://siftheads.fandom.com/wiki/Basement_(Sift_Heads_World))
