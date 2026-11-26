# Sift Heads World 全收集攻略（Act 1–6）

整理自 [Sift Heads Wiki (Fandom)](https://siftheads.fandom.com/wiki/Sift_Heads_World)，并对照本目录游戏 SWF 的反编译脚本做了核对和补充。游戏名词（任务名、地点、武器、人物）保留英文，方便和游戏内对照。

| 文件 | 内容 |
|---|---|
| [Act1.md](Act1.md) | Act 1 - Deadly Newcomer |
| [Act2.md](Act2.md) | Act 2 - The Treacherous Return |
| [Act3.md](Act3.md) | Act 3 - Alonzo's Reinforcement |
| [Act4.md](Act4.md) | Act 4 - Cold Memories |
| [Act5.md](Act5.md) | Act 5 - An Exotic Job |
| [Act6.md](Act6.md) | Act 6 - Illicit Association |

标记说明：
- **【代码】**：网上没有、或与网上说法不同，是从游戏脚本里查到的。
- **【离线版】**：和本目录 `offline\` 离线补丁有关的说明。

---

## 通用说明

### 任务分类
每一部的任务分两类，在游戏里的 **Objectives** 菜单查看：
- **Main Missions**：主线，按剧情推进。
- **Secondary Missions**：支线（Facultative），给奖金，全收集主要就是收齐这些。

### 收集与存档的关系【离线版】
- 原版大部分进度，比如 Basement 宝物和 Act 1 的 Thompson，要靠在线账号把进度带到下一次游戏。离线版用本地存档代替，用法：**每一部都用同一个用户名，通关后在下一部点 LOAD GAME 登录**。
- 每一部通关时游戏会自动存档，并把这一部的通关标记（`bonus_unlocked_N`）写进存档。
- 想在通关后回到同一部补任务：在那一部点 **LOAD GAME**，用同一个用户名登录。

### 存档管理器【离线版】
游戏根目录的 **`SaveManager.exe`** 可以查看和删除各部的存档槽。

**什么时候用：** 某一部一旦有了自己的存档，在那一部点 LOAD GAME 就只读它，不会再继承上一部。比如你回头补完了 Act 2 的支线，想让 Act 3 带上新的进度，就把 Act 3 的存档槽删掉，再在 Act 3 点 LOAD GAME，进度会重新从 Act 2 继承。

**用法：**
1. **先关闭游戏**，游戏运行时工具会拒绝删除。
2. 双击 `SaveManager.exe`，列表里是每个用户名在各部的存档槽（用户名、Act、金钱）。
3. 勾选要删除的存档槽，点 **删除选中**，确认即可。

**注意：**
- 删除前会自动备份为 `siftheads_world_offline.sol.<日期-时间>.bak`，放在存档同目录。想恢复的话，把备份文件改回 `siftheads_world_offline.sol` 即可。
- 某个用户名的存档槽**全部删光**时，这个用户名也会一起删除，之后可以用它重新 CREATE PROFILE。
- 备注栏显示“旧格式”的，是还没被游戏迁移成分部存档的老存档，删除它等于删除这个用户。
- 源码在 `tools\SaveManager\`，运行 `build.bat` 可以重新编译。

### Cheats（作弊码）
游戏中依次按 **6、5、7** 打开输入框，输入代码后按 Enter。

| Code | 效果 | 可用的 Act |
|---|---|---|
| `revive` | 回满血 | 1–6 |
| `perso` | 切换角色（Vinnie / Kiro / Shorty） | 1–6 |
| `money` | +$5000 | 1、2 |
| `treasure` | 直接打开 Basement（The Cave） | 1、2【代码：Act 2 也有】 |
| `halloween` | 秘密服装 | 1、2 |
| `burst` | 换成 Shotgun（SPAS-12） | 1、2 |
| `snipe` | 换成狙击枪 | 1、2 |
| `reload` | 当前武器补满弹药 | 3–6 |
| `equip` | 当前武器换成 Desert Eagle | 3–6 |

Act 2 另有几个 Wiki 没写的代码，见 [Act2.md](Act2.md#cheats)。

> 用作弊码完成 **One Day Pay**（攒到 $25000）这类支线是可行的，Wiki 也明确提到了。

---

## Basement / The Cave（宝物室）

Basement 在 Vinnie 的 Safehouse 里，**通关一次后解锁**。Act 1 可以用 `treasure` 作弊码提前打开。首次进入 Act 1 的 Basement 会完成支线 **Hidden Memories**（$1000）。

宝物会随着各部的通关逐步出现：

| Treasure | 出现的 Act | 出处 |
|---|---|---|
| Young Shorty | Act 1 | 照片，出自 Sift Heads 4 |
| Vinnie's car license | Act 1 | Vinnie 第一辆车 Cuda 的车牌（Sift Heads） |
| Vinnie's first pet | Act 1 | Sift Heads 0 |
| Vinnie's first gun | Act 1 | Smith & Wesson 左轮 |
| Red Mafia tie | Act 1 | Sift Heads |
| Shorty's high heels | Act 2 | |
| Vinnie's knife | Act 2 | Sift Heads 3 |
| Kiro's ride（实为 Kienji's katana） | Act 2 | Sift Renegade。游戏里名字写错了 |
| Alonzo's right hand | Act 2 | Act 1 开场被 Kiro 砍下的手 |
| Kung Fu staff | Act 3 | Sift Renegade |
| Femme fatale | Act 3 | Shorty 的红裙 |
| Yakuza's vase | Act 3 | Act 2 偷来的花瓶 |
| Triad's Son's helmet | Act 3 | Sift Heads 2 |
| Mobo | Act 4 | Vinnie 的泰迪熊（Sift Heads 0） |
| Police Riot Shield | Act 4 | Act 3 打 Alonzo 用的盾 |
| A target | Act 1 就有，Act 6 才能互动 | |
| Little souvenir | Act 1 就有，Act 6 才能互动 | |

已知问题：
- 原版 Act 5 有 bug，大部分宝物不显示。**【离线版】** 已修复，详见 [Act5.md](Act5.md#basement-的-bug离线版已修复)。
- **【离线版】** 宝物能否显示取决于前面各部的通关标记，所以要每一部都用同一个用户名，并用 LOAD GAME 继承存档。
- Act 6 通关一次后，不用进 Cave 也能直接查看 Treasures。

---

## Weapons（武器）

每次最多带 3 把武器，每把 3 个弹匣。在各地 **Gun Shop** 购买，在 Safehouse 的 **Armory** 里装备。

| Weapon | 弹药 | 射击方式 | 价格 | 出现的 Act / 备注 |
|---|---|---|---|---|
| Glock | 12+24 | Semi-Auto | $1,000 | 1–6 |
| Walther P99 | 12+24 | Semi-Auto | $1,000 | 1–6 |
| Beretta 92F | 15+30 | Semi-Auto | $1,500 | 1–6。瞄准时的模型其实是 Desert Eagle |
| Desert Eagle | 10+20 | Semi-Auto | $3,000 | 1–6 |
| Revolver | 6+24 | Revolver | $2,000 | 4–6。St. Petersburg 的 Gun Shop |
| Tranquilizer Gun | 12+24 | Semi-Auto | $2,000 | 3、4、6。麻醉枪，潜入任务必备 |
| Uzi | 30+60 | Burst | $4,000 | 1–6 |
| Thompson | 50+150 | Burst | $8,000 | Act 1：**通关一次后**（或用 `treasure`）在 Chicago Gun Shop 出售，子弹无限。Act 6 也有 |
| SPAS-12 | 8+16 | Pump-Action | $5,000 | 1–6 |
| AA-12 | 32+16 | Automatic | $35,000 | 2–6 |
| M16 | 32+64 | Automatic | $6,000 | 1–6 |
| AK-47 | 30+90（Act 4）/ 60+90 | Automatic | $4,000 | 4–6。Act 5 开局自带 |
| Dragunov | 14+28 | Semi-Auto | $7,000 | 1–3。Act 4 起没有地方卖 |
| M21 | 10+20 | Semi-Auto | $9,000 | 1–6。Act 5、6 里被错标为 "Dragunov" |
| Barrett | 1+20 | Single-shot | $10,000 | 1–6 |
| Crossbow | 10+30 | Arrows | $4,000 | 2–6 |
| Kunai | 无限 | Thrown | $1,000 | 1–6 |
| Shurikens 4B | 无限 | Thrown | $2,000 | 1–6 |
| Shurikens 6B | 无限 | Thrown | $4,000 | 1–6 |
| C4 Explosives | — | Explosive | $500 | 只能在 Act 5 买，任务 Fireworks 用 |

Act 3 起，可以在 **Darwen's House** 升级武器的 Precision 和 Power。Act 3 的支线 Full Specs 要求把任意一把枪升满。

---

## Clothes（服装）

在各地 **Clothing Shop** 购买。编号全系列通用，同一编号三位角色（Vinnie / Kiro / Shorty）各有自己的造型，换角色时衣服编号不变。Act 3 起买过的衣服会存进 Safehouse 的衣柜，可以随时换回。下表按游戏代码整理：

| 编号 | 地点 | 价格 |
|---|---|---|
| Original（1） | 初始 | $0 |
| Style 2–7 | Chicago Clothing Shop（Act 1–5） | 各 $500 |
| BONUS STYLE（8） | Chicago Clothing Shop，**通关 Act 1 后**出现 | $0 |
| Style 9–11 | Tokyo Clothing Shop（Act 2） | 各 $500 |
| Style 12 | Tokyo Clothing Shop（Act 2） | $1000 |
| PANDA SUIT（13） | Tokyo Clothing Shop，**仅 Kiro**，通关 Act 2 后出现 | $0 |
| BONUS（13） | Palermo Clothing Shop，**仅 Vinnie**，通关 Act 3 后出现 | $0 |
| Style 16–18 | Palermo Clothing Shop（Act 3） | 各 $500 |
| Style 19–21 | St. Petersburg Clothing Shop（Act 4） | 各 $500 |
| Style 10–25 | Amazon Village 的商店（Act 5），Style 22 仅 Vinnie | 多为 $500 |
| Style 2–28 | Act 6 各城市的 Clothing Shop 分别出售一部分 | 各 $500 |

和服装有关的支线只有两个：Act 1 的 Well Dressed（要穿 BONUS STYLE）和 Act 2 的 Dress 4 Success（要穿 Tokyo 的 Style 9–12），见各部攻略。

另外还有 `halloween` 等作弊码服装，见上面的作弊码表和 [Act2.md](Act2.md#cheats)。

---

## 来源
- [Sift Heads World](https://siftheads.fandom.com/wiki/Sift_Heads_World)
- [Missions in Sift Heads World games](https://siftheads.fandom.com/wiki/Missions_in_Sift_Heads_World_games)
- [Basement (Sift Heads World)](https://siftheads.fandom.com/wiki/Basement_(Sift_Heads_World))
- [Weapons in Sift Heads World](https://siftheads.fandom.com/wiki/Weapons_in_Sift_Heads_World)
- [Clothes in Sift Heads World](https://siftheads.fandom.com/wiki/Clothes_in_Sift_Heads_World)
- 各部的条目：[Act 1](https://siftheads.fandom.com/wiki/Sift_Heads_World:_Act_1_-_Deadly_Newcomer)、[Act 2](https://siftheads.fandom.com/wiki/Sift_Heads_World:_Act_2_-_The_Treacherous_Return)、[Act 3](https://siftheads.fandom.com/wiki/Sift_Heads_World:_Act_3_-_Alonzo%27s_Reinforcement)、[Act 4](https://siftheads.fandom.com/wiki/Sift_Heads_World:_Act_4_-_Cold_Memories)、[Act 5](https://siftheads.fandom.com/wiki/Sift_Heads_World:_Act_5_-_An_Exotic_Job)、[Act 6](https://siftheads.fandom.com/wiki/Sift_Heads_World:_Act_6_-_Illicit_Association)

Wiki 内容采用 CC-BY-SA 许可。
