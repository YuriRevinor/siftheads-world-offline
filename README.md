# Sift Heads World 离线版

把 Flash 游戏 **Sift Heads World**（Act 1–6）改成可以离线运行、离线存档的版本。附带便携式 Flash 播放器、存档管理器和全收集攻略。

原版的在线功能（注册账号、云存档）依赖的服务器早已关闭，导致无法存档，也无法把进度带到下一部。这个项目用补丁把存档改为本地保存，其余游戏内容保持原样。

## 快速开始
1. 双击 `Play_Act1.bat` ~ `Play_Act6.bat` 开始对应的一部。
2. 第一次玩选 **NEW GAME → CREATE PROFILE** 注册一个用户名（邮箱可留空，密码不校验）。
3. 之后每一部都用**同一个用户名**点 **LOAD GAME**，进度会自动从上一部继承。

更详细的说明见 [说明.txt](说明.txt)。

## 目录结构
| 路径 | 内容 |
|---|---|
| `SHW_*.swf` | 原版游戏文件，未改动 |
| `offline\` | 打过离线补丁的 Act 1–6 |
| `player\` | 便携版 [Ruffle](https://ruffle.rs/) Flash 播放器。配置、缓存、存档都放在 `player\data\`，不写系统目录 |
| `Play_Act*.bat`、`play.bat` | 启动脚本 |
| `SaveManager.exe` | 存档管理器，可查看和删除各部的存档槽 |
| `tools\SaveManager\` | 存档管理器源码（C++/CLI + WinForms）和构建脚本 |
| `tools\patcher\` | 离线补丁脚本，用原版 SWF 生成 `offline\`（见其中的 README） |
| `docs\` | Act 1–6 全收集攻略 |
| `说明.txt` | 使用说明 |

存档文件在 `player\data\saves\localhost\siftheads_world_offline.sol`，备份它即可。

## 离线补丁改了什么
- **去掉域名锁：** 原版只允许在 siftheads.net / gamesfree.ca 上注册和存档。
- **本地存储代替服务器：** 注册、登录、存档改为读写本地 SharedObject。
- **分部存档：** 同一用户名在每一部各有一个存档槽。进入某一部时，优先读这一部的存档；没有就继承前一部的进度。回头重玩前面的章节，不会覆盖后面章节的进度。
- **修复 Act 5 宝物室：** 原版有几件宝物永远不显示，已修复。
- **修复 Act 2/4/5 地图：** 原版读档后，地图上的城市按钮可能显示已解锁却去错地方（如 Act 2 点 Palermo 进了 Tokyo），已修复。
- **修复 Act 2 二周目：** 原版二周目完成主线 4 后主线 5 无法触发，已修复（已卡住的存档读档即可恢复）。
- **保存武器升级：** 原版 Act 3–6 在 Darwen's House 升级的武器属性不存档，读档后恢复初始值。离线版会保存，并带到下一部。
- **其他：** 外部网页链接已禁用。

`SHW_Ultimatum.swf`（Act 7）本身就是本地存档，没有改动。

## 攻略
[docs/README.md](docs/README.md)：每一部的主线、支线、收集品位置、作弊码、Basement 宝物、武器和服装，整理自 [Sift Heads Wiki](https://siftheads.fandom.com/wiki/Sift_Heads_World)，并对照游戏代码做了补充，例如 Act 3 隐藏任务 Hard Headed 的做法。

## 说明
本项目仅用于保存和游玩已停止服务的老游戏。游戏版权归原作者 Pyrozen / Chriss Martin 所有。
