# 离线补丁脚本

用仓库根目录的原版 SWF 重新生成 `offline\` 里的离线版。仓库里已经带有生成好的文件，只有修改补丁内容时才需要运行它。

## 需要
- Python 3.8+
- Java 11+（在 PATH 里，或用环境变量 `JAVA` 指定）
- [JPEXS Free Flash Decompiler](https://github.com/jindrapetrik/jpexs-decompiler) 的 `ffdec.jar`（用 26.x 测试过）

## 运行
在仓库根目录执行：

```
python tools/patcher/patch.py --ffdec <ffdec.jar 的路径>
```

也可以设置环境变量 `FFDEC_JAR` 代替 `--ffdec`。`--out` 可以改输出目录，默认是 `offline\`。

脚本会在系统临时目录里反编译、修改、重新导入脚本，结束后删除临时文件。FFDec 运行时会创建配置目录 `%APPDATA%\JPEXS`；如果运行前这个目录不存在，脚本结束后会把它删掉。

## 改了什么
每一部：
- **frame 1：** 去掉域名检查，注入本地存档函数 `shwSave` / `shwLoad` / `shwExists`。
- **frame 27：** 登录时改为读本地存档。
- **frame 29、42：** 新建账号和游戏内存档改为写本地存档。
- **注册界面：** 用户名查重改为本地检查，邮箱可以留空。

另外：
- Act 5 修复宝物室里永远不显示的 3 件宝物。
- Act 2/4/5 的世界地图里，两个城市按钮共用同一段场景编号，从上一部继承来的城市解锁标记会让错的按钮显示为已解锁。把这些按钮固定为锁定或解锁（见 `MAP_FIX`）。

存档格式：SharedObject `siftheads_world_offline`（路径 `/`，各部共用），结构为 `data.profiles[<小写用户名>].slots["a<部>"]`。
