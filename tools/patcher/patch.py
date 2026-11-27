"""Sift Heads World offline patcher.

Builds offline/<act>.swf from the original SWFs in the repository root:
  * drops the siftheads.net / gamesfree.ca domain lock
  * replaces the server login / register / save calls with a local
    SharedObject store that keeps one save slot per act
  * fixes the Act 5 treasure room bug

Requirements: Python 3.8+, Java 11+ on PATH (or set JAVA), and
JPEXS Free Flash Decompiler (https://github.com/jindrapetrik/jpexs-decompiler).
Point FFDEC_JAR at ffdec.jar, or pass it with --ffdec.

Usage:
  python tools/patcher/patch.py --ffdec path/to/ffdec.jar
"""
import argparse
import glob
import os
import re
import shutil
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
ACTS = ["SHW_Act1_Gamesfree", "SHW_Act2_Gamefree", "SHW_Act3",
        "SHW_Act4_Gamesfree", "SHW_Act5", "SHW_Act6"]

# Injected into frame 1 of every act. Saves live in
# SharedObject "siftheads_world_offline" (path "/", shared by all acts):
#   data.profiles[<lowercase name>].slots["a<act>"] = { game variables }
LIB = r'''_root.url_check = 2;
_global.SHW_VARS = "%VARS%";
_global.SHW_ACT = %ACT%;
_root.shwSO = function()
{
   return SharedObject.getLocal("siftheads_world_offline","/");
};
_root.shwKey = function(n)
{
   var u = String(n);
   if(n == undefined || u == "")
   {
      u = "Player";
   }
   return u.toLowerCase();
};
_root.shwExists = function(n)
{
   var so = _root.shwSO();
   if(so.data.profiles == undefined)
   {
      return false;
   }
   return so.data.profiles[_root.shwKey(n)] != undefined;
};
_root.shwSlots = function(p)
{
   if(p.slots == undefined)
   {
      p.slots = new Object();
      var old = new Object();
      var had = false;
      for(var f in p)
      {
         if(f != "slots" && f != "shw_name")
         {
            old[f] = p[f];
            had = true;
         }
      }
      if(had)
      {
         var a = Number(old.act_playing);
         if(isNaN(a) || a < 1)
         {
            a = 1;
         }
         p.slots["a" + a] = old;
      }
   }
   return p.slots;
};
_root.shwSave = function()
{
   var so = _root.shwSO();
   if(so.data.profiles == undefined)
   {
      so.data.profiles = new Object();
   }
   var k = _root.shwKey(_root.user);
   var p = so.data.profiles[k];
   if(p == undefined)
   {
      p = new Object();
      so.data.profiles[k] = p;
   }
   var slots = _root.shwSlots(p);
   var d = new Object();
   var names = _global.SHW_VARS.split(",");
   var i = 0;
   while(i < names.length)
   {
      var v = _root[names[i]];
      if((names[i].substr(0,9) == "wp_power_" || names[i].substr(0,13) == "wp_precision_") && _root["wp_power_" + names[i].substr(names[i].lastIndexOf("_") + 1)] == 0)
      {
         v = undefined;
      }
      if(v != undefined)
      {
         d[names[i]] = v;
      }
      i++;
   }
   slots["a" + _global.SHW_ACT] = d;
   p.shw_name = String(_root.user);
   so.data.last = k;
   so.flush();
};
_root.shwLoad = function()
{
   var so = _root.shwSO();
   var k = _root.shwKey(_root.user);
   if(so.data.profiles == undefined || so.data.profiles[k] == undefined)
   {
      _root.reaction = "Error";
      return undefined;
   }
   var slots = _root.shwSlots(so.data.profiles[k]);
   var src = undefined;
   var j = _global.SHW_ACT;
   while(j >= 1 && src == undefined)
   {
      src = slots["a" + j];
      j--;
   }
   var fresh = false;
   if(src == undefined)
   {
      j = _global.SHW_ACT + 1;
      while(j <= 7 && src == undefined)
      {
         src = slots["a" + j];
         j++;
      }
      if(src == undefined)
      {
         _root.reaction = "Error";
         return undefined;
      }
      if(_global.SHW_ACT == 1)
      {
         fresh = true;
      }
   }
   _root.user_id = k;
   _root.reaction = "Success";
   if(fresh)
   {
      if(src.cash != undefined)
      {
         _root.cash = src.cash;
      }
      return undefined;
   }
   var p = src;
   var names = _global.SHW_VARS.split(",");
   var i = 0;
   while(i < names.length)
   {
      var n = names[i];
      if(p[n] != undefined)
      {
         _root[n] = p[n];
      }
      else if(n.substr(0,6) == "stats_")
      {
         _root[n] = 0;
      }
      else if(n == "cl_1")
      {
         _root[n] = 2;
      }
      else if(n.substr(0,3) == "cl_")
      {
         _root[n] = 1;
      }
      i++;
   }
   _root.user_id = k;
   _root.reaction = "Success";
};
'''


MAP_FIX = {
    "SHW_Act2_Gamefree": (   # Act 2 takes place in Chicago and Tokyo
        ("DefineSprite_2798/frame_2/PlaceObject2_2797_5", "palermo", 1),
        ("DefineSprite_2798/frame_2/PlaceObject2_2797_15", "shanghai", 1),
        ("DefineSprite_2798/frame_2/PlaceObject2_2797_25", "stpetersburg", 1),
    ),
    "SHW_Act4_Gamesfree": (  # Act 4: St. Petersburg (a new game starts with it unlocked)
        ("DefineSprite_3150/frame_2/PlaceObject2_3149_6", "palermo", 1),
        ("DefineSprite_3150/frame_2/PlaceObject2_3149_26", "stpetersburg", 2),
    ),
    "SHW_Act5": (            # Act 5: Amazon has its own always-unlocked button
        ("DefineSprite_3270/frame_2/PlaceObject2_3269_6", "palermo", 1),
        ("DefineSprite_3270/frame_2/PlaceObject2_3269_26", "stpetersburg", 1),
    ),
}


def rd(p):
    with open(p, encoding="utf-8") as f:
        return f.read()


def sub1(text, old, new, label):
    t2, c = re.subn(old, new, text, count=1, flags=re.S)
    if c < 1:
        sys.exit(f"[{label}] pattern not found: {old[:80]}")
    return t2


class FFDec:
    def __init__(self, jar, java):
        self.cmd = [java, "-jar", jar]
        # FFDec writes its settings to %APPDATA%\JPEXS (Windows) or ~/.local/share/JPEXS.
        # Remove that folder afterwards, but only if this run created it.
        base = os.environ.get("APPDATA") or os.path.expanduser("~/.local/share")
        self.cfg = os.path.join(base, "JPEXS")
        self.cfg_existed = os.path.exists(self.cfg)

    def run(self, *args):
        r = subprocess.run(self.cmd + list(args), capture_output=True, text=True,
                           encoding="utf-8", errors="replace")
        if r.returncode != 0:
            sys.exit("ffdec failed:\n" + r.stdout + r.stderr)
        return r.stdout + r.stderr

    def cleanup(self):
        if not self.cfg_existed:
            shutil.rmtree(self.cfg, ignore_errors=True)


def patch_act(ff, act, act_no, work, out_dir):
    swf = os.path.join(ROOT, act + ".swf")
    src = os.path.join(work, "src", act)
    ff.run("-export", "script", src, swf)
    src = os.path.join(src, "scripts")
    edits = {}

    # variables saved per act = what the act used to load from the server + treasure flags
    f27 = rd(os.path.join(src, "frame_27", "DoAction.as"))
    vars_ = re.findall(r"_root\.([a-z0-9_]+) = Number\(", f27)
    vars_ += [f"bonus_unlocked_{i}" for i in range(2, 13)]
    # Acts 3-6: Darwen's weapon upgrades (power/precision per weapon) were never saved.
    # Weapons with power 0 don't exist in that act and are skipped by shwSave, so a
    # later act keeps its own defaults for weapons it adds.
    if act_no >= 3:
        f20 = rd(os.path.join(src, "frame_20", "DoAction.as"))
        vars_ += re.findall(r"_root\.(wp_p(?:ower|recision)_\d+) = ", f20)
    vars_ = list(dict.fromkeys(vars_))

    # 1. frame 1: drop the domain lock, inject the save library
    p = "frame_1/DoAction_3"
    lib = LIB.replace("%VARS%", ",".join(vars_)).replace("%ACT%", str(act_no))
    edits[p] = sub1(rd(os.path.join(src, p + ".as")),
                    r"onEnterFrame = function\(\)\s*\{\s*urlStart.*?\n\};\n",
                    lambda m: lib, act + " f1")

    # 2. frame 27: login / load
    t = sub1(f27, r'loadVariablesNum\("http://[^"]*load\.php\?submit=1",0,"POST"\);',
             "_root.shwLoad();", act + " f27a")
    t = sub1(t, r"_loc2_ = new LoadVars\(\);.*?_loc2_\.load\([^)]*\);",
             "_root.reaction_received = 2;", act + " f27b")
    edits["frame_27/DoAction"] = t

    # 3/4. frame 29 (new profile) and frame 42 (in-game save)
    for p, lab in (("frame_29/DoAction", "submit=1"), ("frame_42/DoAction", "submit=2")):
        edits[p] = sub1(rd(os.path.join(src, p + ".as")),
                        r'loadVariablesNum\("http://[^"]*save\.php\?' + lab + r'",0,"POST"\);',
                        "_root.shwSave();", act + " " + p)

    # 5. registration sprite: local username check, e-mail optional
    reg = [f for f in glob.glob(os.path.join(src, "DefineSprite_*", "frame_4", "DoAction.as"))
           if "load.php?submit=2" in rd(f)]
    if len(reg) != 1:
        sys.exit(f"[{act}] registration sprite not found: {reg}")
    sp = os.path.basename(os.path.dirname(os.path.dirname(reg[0])))
    edits[f"{sp}/frame_4/DoAction"] = '_root.reaction = "";\n'
    edits[f"{sp}/frame_49/DoAction"] = (
        'stop();\nif(_root.shwExists(user))\n{\n   _root.reaction = "Username_Exist";\n}\n'
        'else\n{\n   _root.reaction = "OK";\n}\ngotoAndStop(50);\n')
    p = f"{sp}/frame_3/DoAction"
    t = rd(os.path.join(src, p + ".as"))
    old = 'if(!(cour != "" && cour.indexOf("@") != -1 && cour.indexOf(".") != -1))'
    if old in t:
        edits[p] = t.replace(old, "if(false)", 1)

    # 6. Act 5 treasure room: three treasures test bonus_unlocked_5, which is never set
    if act == "SHW_Act5":
        base = "DefineSprite_1789/frame_1/PlaceObject2_%s/CLIPACTIONRECORD onClipEvent(load)"
        for obj, new in (("1772_20", None),                 # Young Shorty (Act 1): always shown, as in Act 4
                         ("1786_37", "bonus_unlocked_4"),   # Mobo (Act 4)
                         ("1788_39", "bonus_unlocked_4")):  # Police Riot Shield (Act 4)
            p = base % obj
            t = rd(os.path.join(src, p + ".as"))
            if "_root.bonus_unlocked_5 != 2" not in t:
                sys.exit(f"[{act}] unexpected script in {p}")
            edits[p] = "onClipEvent(load){\n}\n" if new is None else t.replace("bonus_unlocked_5", new)

    # 7. world map: in Acts 2/4/5 two city buttons share one level range, and the city flags
    #    carried over from the previous act unlock the wrong one (e.g. "Palermo" leads to Tokyo
    #    in Act 2). Pin those buttons: 1 = locked, 2 = unlocked.
    for obj, flag, state in MAP_FIX.get(act, ()):
        p = f"{obj}/CLIPACTIONRECORD onClipEvent(enterFrame)"
        t = rd(os.path.join(src, p + ".as"))
        old = f"gotoAndStop(_root.{flag}_unlocked);"
        if old not in t:
            sys.exit(f"[{act}] unexpected script in {p}")
        edits[p] = t.replace(old, f"gotoAndStop({state});")

    # 8. Act 2 second playthrough: ob_20_9 / ob_20_10 mark the Yuuma and Black Fox videos as
    #    played; the Fox video unlocks Crossing The Line (ob_11). The end-of-act restart does
    #    not clear them, so mission 5 never unlocks again. Clear them on restart, and on load
    #    while ob_11 is still locked (repairs saves that are already stuck).
    if act == "SHW_Act2_Gamefree":
        reset = ["_root.ob_20_9 = 1;", "_root.ob_20_10 = 1;"]
        p = "DefineSprite_3208/frame_22/DoAction"
        edits[p] = sub1(rd(os.path.join(src, p + ".as")), r"\n([ \t]*)_root\.ob_11 = 1;\n",
                        lambda m: m.group(0) + "".join(m.group(1) + l + "\n" for l in reset),
                        act + " restart")
        p = "frame_43/DoAction"
        edits[p] = sub1(rd(os.path.join(src, p + ".as")), r"\nstop\(\);\nonEnterFrame = function\(\)",
                        lambda m: "\nif(_root.ob_11 == 1)\n{\n" + "".join("   " + l + "\n" for l in reset)
                        + "}" + m.group(0), act + " load")

    imp = os.path.join(work, "import", act)
    for p, t in edits.items():
        fn = os.path.join(imp, "scripts", *p.split("/")) + ".as"
        os.makedirs(os.path.dirname(fn), exist_ok=True)
        with open(fn, "w", encoding="utf-8") as f:
            f.write(t)
    ff.run("-importScript", swf, os.path.join(out_dir, act + ".swf"), imp)
    print(f"{act}: {len(edits)} scripts patched, {len(vars_)} saved variables")


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--ffdec", default=os.environ.get("FFDEC_JAR"), help="path to ffdec.jar")
    ap.add_argument("--java", default=os.environ.get("JAVA", "java"), help="java executable")
    ap.add_argument("--out", default=os.path.join(ROOT, "offline"), help="output folder")
    a = ap.parse_args()
    if not a.ffdec or not os.path.isfile(a.ffdec):
        sys.exit("ffdec.jar not found: pass --ffdec or set FFDEC_JAR")

    os.makedirs(a.out, exist_ok=True)
    ff = FFDec(a.ffdec, a.java)
    work = tempfile.mkdtemp(prefix="shw_patch_")
    try:
        for i, act in enumerate(ACTS, 1):
            patch_act(ff, act, i, work, a.out)
    finally:
        shutil.rmtree(work, ignore_errors=True)
        ff.cleanup()


if __name__ == "__main__":
    main()
