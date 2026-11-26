// Sift Heads World 离线版存档管理器
// 列出每个用户名在各 Act 的存档槽，并可删除选中的存档槽。
// C++/CLI + WinForms，运行在系统自带的 .NET Framework 4.x 上。
// 构建方法见 build.bat（引用的程序集由 build.bat 的 /FU 提供）。


using namespace System;
using namespace System::IO;
using namespace System::Text;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

// ---------------------------------------------------------------- AMF0 ----

ref class Amf;

ref class Prop {
public:
    String^ name;
    Amf^ value;
    Prop(String^ n, Amf^ v) : name(n), value(v) {}
};

// 一个 AMF0 值。对象和 ECMA 数组保留属性顺序，写回时字节不变。
ref class Amf {
public:
    Byte type;
    double num;
    bool flag;
    String^ str;
    List<Prop^>^ props;     // 3 object, 8 ECMA array
    List<Amf^>^ items;      // 10 strict array
    array<Byte>^ raw;       // 11 date 的原始 10 字节

    Amf^ Get(String^ key) {
        if (props == nullptr) return nullptr;
        for each (Prop^ p in props) if (p->name == key) return p->value;
        return nullptr;
    }
    bool Remove(String^ key) {
        if (props == nullptr) return false;
        for (int i = 0; i < props->Count; i++)
            if (props[i]->name == key) { props->RemoveAt(i); return true; }
        return false;
    }
    bool IsObject() { return props != nullptr; }
};

ref class Reader {
    array<Byte>^ b;
public:
    int pos;
    Reader(array<Byte>^ data, int start) : b(data), pos(start) {}
    bool AtEnd() { return pos >= b->Length; }
    Byte U8() { return b[pos++]; }
    int U16() { int v = (b[pos] << 8) | b[pos + 1]; pos += 2; return v; }
    int U32() { int v = (b[pos] << 24) | (b[pos + 1] << 16) | (b[pos + 2] << 8) | b[pos + 3]; pos += 4; return v; }
    String^ Str(int n) { String^ s = Encoding::UTF8->GetString(b, pos, n); pos += n; return s; }
    double Num() {
        array<Byte>^ t = gcnew array<Byte>(8);
        for (int i = 0; i < 8; i++) t[i] = b[pos + 7 - i];
        pos += 8;
        return BitConverter::ToDouble(t, 0);
    }
    List<Prop^>^ Props() {
        List<Prop^>^ list = gcnew List<Prop^>();
        while (true) {
            String^ k = Str(U16());
            if (k->Length == 0 && b[pos] == 9) { pos++; return list; }
            list->Add(gcnew Prop(k, Value()));
        }
    }
    Amf^ Value() {
        Amf^ v = gcnew Amf();
        v->type = U8();
        switch (v->type) {
        case 0: v->num = Num(); break;
        case 1: v->flag = U8() != 0; break;
        case 2: v->str = Str(U16()); break;
        case 3: v->props = Props(); break;
        case 5: case 6: break;
        case 8: U32(); v->props = Props(); break;
        case 10: {
            int n = U32();
            v->items = gcnew List<Amf^>();
            for (int i = 0; i < n; i++) v->items->Add(Value());
            break;
        }
        case 11: v->raw = gcnew array<Byte>(10); Array::Copy(b, pos, v->raw, 0, 10); pos += 10; break;
        case 12: v->str = Str(U32()); break;
        default:
            // 7 = 引用。删掉被引用的对象会让后面的引用错位，所以不支持。
            throw gcnew Exception(String::Format(L"不支持的 AMF0 类型 {0}（位置 {1}）", v->type, pos - 1));
        }
        return v;
    }
};

ref class Writer {
    MemoryStream^ m;
public:
    Writer() { m = gcnew MemoryStream(); }
    array<Byte>^ ToArray() { return m->ToArray(); }
    void U8(int v) { m->WriteByte((Byte)v); }
    void U16(int v) { U8(v >> 8); U8(v); }
    void U32(int v) { U8(v >> 24); U8(v >> 16); U8(v >> 8); U8(v); }
    void Bytes(array<Byte>^ a) { m->Write(a, 0, a->Length); }
    void Str16(String^ s) { array<Byte>^ a = Encoding::UTF8->GetBytes(s); U16(a->Length); Bytes(a); }
    void Num(double d) {
        array<Byte>^ a = BitConverter::GetBytes(d);
        for (int i = 7; i >= 0; i--) U8(a[i]);
    }
    void Props(List<Prop^>^ list) {
        for each (Prop^ p in list) { Str16(p->name); Value(p->value); }
        U16(0); U8(9);
    }
    void Value(Amf^ v) {
        U8(v->type);
        switch (v->type) {
        case 0: Num(v->num); break;
        case 1: U8(v->flag ? 1 : 0); break;
        case 2: Str16(v->str); break;
        case 3: Props(v->props); break;
        case 8: U32(v->props->Count); Props(v->props); break;
        case 10: U32(v->items->Count); for each (Amf^ i in v->items) Value(i); break;
        case 11: Bytes(v->raw); break;
        case 12: { array<Byte>^ a = Encoding::UTF8->GetBytes(v->str); U32(a->Length); Bytes(a); break; }
        }
    }
};

// .sol 文件：头部 + 若干顶层属性（每个后面跟一个 0 字节）。
ref class SolFile {
public:
    array<Byte>^ header;   // "TCSO" 之后到属性区开始之前的原始字节（含名字和 AMF 版本）
    Amf^ root;             // 顶层属性，放在一个对象里方便处理

    static SolFile^ Load(String^ path) {
        array<Byte>^ b = File::ReadAllBytes(path);
        if (b->Length < 16 || b[0] != 0x00 || b[1] != 0xBF || Encoding::ASCII->GetString(b, 6, 4) != L"TCSO")
            throw gcnew Exception(L"不是有效的 .sol 文件");
        Reader r(b, 16);
        int nameLen = r.U16();
        r.pos += nameLen;
        if (r.U32() != 0) throw gcnew Exception(L"只支持 AMF0 格式的存档");
        SolFile^ f = gcnew SolFile();
        f->header = gcnew array<Byte>(r.pos - 10);
        Array::Copy(b, 10, f->header, 0, f->header->Length);
        f->root = gcnew Amf();
        f->root->type = 3;
        f->root->props = gcnew List<Prop^>();
        while (!r.AtEnd()) {
            String^ k = r.Str(r.U16());
            f->root->props->Add(gcnew Prop(k, r.Value()));
            if (r.U8() != 0) throw gcnew Exception(L"存档格式异常（属性结尾不是 0）");
        }
        return f;
    }

    array<Byte>^ ToBytes() {
        Writer body;
        body.Bytes(Encoding::ASCII->GetBytes(L"TCSO"));
        body.Bytes(header);
        for each (Prop^ p in root->props) { body.Str16(p->name); body.Value(p->value); body.U8(0); }
        array<Byte>^ rest = body.ToArray();
        Writer w;
        w.U16(0x00BF);
        w.U32(rest->Length);
        w.Bytes(rest);
        return w.ToArray();
    }
};

// ------------------------------------------------------------- 存档逻辑 ----

// 列表中的一行：某个用户名的某个 Act 存档槽。
ref class SlotRef {
public:
    String^ key;      // profiles 里的键（小写用户名）
    String^ slot;     // "a1".."a7"；旧格式为 nullptr
    int act;
};

static String^ Money(Amf^ data) {
    Amf^ c = data == nullptr ? nullptr : data->Get(L"cash");
    return (c != nullptr && c->type == 0) ? String::Format(L"${0:N0}", c->num) : L"";
}

// 删除一个存档槽。用户名下一个槽都不剩时，连用户一起删掉，
// 否则这个用户名会变成空壳：LOAD GAME 读不到，CREATE PROFILE 又提示已存在。
// 返回 true 表示整个用户被删除。
static bool DeleteSlot(Amf^ profiles, SlotRef^ s) {
    Amf^ p = profiles->Get(s->key);
    if (p == nullptr) return false;
    Amf^ slots = p->Get(L"slots");
    if (s->slot != nullptr && slots != nullptr && slots->IsObject()) {
        slots->Remove(s->slot);
        if (slots->props->Count > 0) return false;
    }
    profiles->Remove(s->key);
    return true;
}

// ---------------------------------------------------------------- 界面 ----

ref class MainForm : public Form {
    String^ path;
    SolFile^ sol;
    ListView^ list;
    Label^ info;

public:
    MainForm(String^ p) : path(p) {
        Text = L"Sift Heads World 存档管理";
        Font = gcnew System::Drawing::Font(L"Microsoft YaHei UI", 9);
        AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
        ClientSize = Drawing::Size(520, 380);
        StartPosition = FormStartPosition::CenterScreen;

        list = gcnew ListView();
        list->View = View::Details;
        list->CheckBoxes = true;
        list->FullRowSelect = true;
        list->Dock = DockStyle::Fill;
        list->Columns->Add(L"用户名", 160);
        list->Columns->Add(L"存档槽", 110);
        list->Columns->Add(L"金钱", 100);
        list->Columns->Add(L"备注", 120);

        info = gcnew Label();
        info->Dock = DockStyle::Top;
        info->AutoSize = false;
        info->Height = 40;
        info->Padding = System::Windows::Forms::Padding(6, 4, 6, 0);
        info->Text = L"勾选要删除的存档槽。删除后，在那一部点 LOAD GAME 会重新从前一部继承进度。";

        FlowLayoutPanel^ bar = gcnew FlowLayoutPanel();
        bar->Dock = DockStyle::Bottom;
        bar->Height = 40;
        bar->FlowDirection = FlowDirection::RightToLeft;
        bar->Padding = System::Windows::Forms::Padding(4);
        Button^ del = gcnew Button();
        del->Text = L"删除选中";
        del->AutoSize = true;
        del->Click += gcnew EventHandler(this, &MainForm::OnDelete);
        Button^ refresh = gcnew Button();
        refresh->Text = L"刷新";
        refresh->AutoSize = true;
        refresh->Click += gcnew EventHandler(this, &MainForm::OnRefresh);
        bar->Controls->Add(del);
        bar->Controls->Add(refresh);

        Controls->Add(list);
        Controls->Add(info);
        Controls->Add(bar);
        Reload();
    }

private:
    void Reload() {
        list->Items->Clear();
        sol = nullptr;
        if (!File::Exists(path)) { info->Text = L"还没有存档：" + path; return; }
        try { sol = SolFile::Load(path); }
        catch (Exception^ e) { info->Text = L"读取存档失败：" + e->Message; return; }

        Amf^ profiles = sol->root->Get(L"profiles");
        if (profiles == nullptr || !profiles->IsObject()) return;
        for each (Prop^ up in profiles->props) {
            Amf^ p = up->value;
            if (p == nullptr || !p->IsObject()) continue;
            Amf^ nm = p->Get(L"shw_name");
            String^ shown = (nm != nullptr && nm->type == 2) ? nm->str : up->name;
            Amf^ slots = p->Get(L"slots");
            List<SlotRef^>^ rows = gcnew List<SlotRef^>();
            if (slots != nullptr && slots->IsObject()) {
                for each (Prop^ sp in slots->props) {
                    int act = 0;
                    if (sp->name->StartsWith(L"a")) Int32::TryParse(sp->name->Substring(1), act);
                    SlotRef^ r = gcnew SlotRef();
                    r->key = up->name; r->slot = sp->name; r->act = act;
                    rows->Add(r);
                }
            } else {
                // 旧格式（还没被游戏迁移过）：整份进度属于 act_playing 那一部
                Amf^ a = p->Get(L"act_playing");
                SlotRef^ r = gcnew SlotRef();
                r->key = up->name; r->slot = nullptr;
                r->act = (a != nullptr && a->type == 0) ? (int)a->num : 1;
                rows->Add(r);
            }
            rows->Sort(gcnew Comparison<SlotRef^>(&MainForm::ByAct));
            for each (SlotRef^ r in rows) {
                Amf^ data = r->slot != nullptr ? slots->Get(r->slot) : p;
                ListViewItem^ it = gcnew ListViewItem(shown);
                it->SubItems->Add(r->act > 0 ? L"Act " + r->act : r->slot);
                it->SubItems->Add(Money(data));
                it->SubItems->Add(r->slot == nullptr ? L"旧格式" : L"");
                it->Tag = r;
                list->Items->Add(it);
            }
        }
        if (list->Items->Count == 0) info->Text = L"存档里没有任何用户。";
    }

    static int ByAct(SlotRef^ a, SlotRef^ b) { return a->act.CompareTo(b->act); }

    static bool GameRunning() {
        return Diagnostics::Process::GetProcessesByName(L"ruffle")->Length > 0;
    }

    void OnRefresh(Object^, EventArgs^) { Reload(); }

    void OnDelete(Object^, EventArgs^) {
        if (sol == nullptr || list->CheckedItems->Count == 0) {
            MessageBox::Show(this, L"请先勾选要删除的存档槽。", Text);
            return;
        }
        if (GameRunning()) {
            MessageBox::Show(this, L"游戏（ruffle.exe）正在运行。\n请先关闭游戏再删除，否则游戏退出时可能把旧存档写回去。",
                Text, MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        StringBuilder^ sb = gcnew StringBuilder(L"将删除以下存档槽：\n\n");
        for each (ListViewItem^ it in list->CheckedItems)
            sb->AppendFormat(L"  {0}  {1}\n", it->Text, it->SubItems[1]->Text);
        sb->Append(L"\n删除前会自动备份。继续吗？");
        if (MessageBox::Show(this, sb->ToString(), Text, MessageBoxButtons::YesNo, MessageBoxIcon::Question) != System::Windows::Forms::DialogResult::Yes)
            return;

        // 重新读一次文件，避免覆盖界面打开后游戏写入的新进度
        SolFile^ fresh;
        try { fresh = SolFile::Load(path); }
        catch (Exception^ e) { MessageBox::Show(this, L"读取存档失败：" + e->Message, Text); return; }
        Amf^ profiles = fresh->root->Get(L"profiles");

        List<String^>^ goneUsers = gcnew List<String^>();
        for each (ListViewItem^ it in list->CheckedItems) {
            SlotRef^ r = safe_cast<SlotRef^>(it->Tag);
            if (DeleteSlot(profiles, r) && !goneUsers->Contains(it->Text)) goneUsers->Add(it->Text);
        }

        String^ backup = path + L"." + DateTime::Now.ToString(L"yyyyMMdd-HHmmss") + L".bak";
        try {
            File::Copy(path, backup);
            String^ tmp = path + L".tmp";
            File::WriteAllBytes(tmp, fresh->ToBytes());
            File::Delete(path);
            File::Move(tmp, path);
        }
        catch (Exception^ e) { MessageBox::Show(this, L"写入失败：" + e->Message, Text); return; }

        String^ msg = L"已删除。备份：\n" + backup;
        if (goneUsers->Count > 0)
            msg += L"\n\n以下用户已没有任何存档槽，整个用户也已删除：\n  " + String::Join(L"、", goneUsers);
        MessageBox::Show(this, msg, Text);
        Reload();
    }
};

// ---------------------------------------------------------------- 入口 ----

#ifdef SELFTEST
// 测试入口：SaveManager_test.exe <in.sol> <out.sol> [用户名 act ...]
// 不删除任何东西时，输出应与输入逐字节相同。
int main(array<String^>^ args) {
    SolFile^ f = SolFile::Load(args[0]);
    Amf^ profiles = f->root->Get(L"profiles");
    for (int i = 2; i + 1 < args->Length; i += 2) {
        SlotRef^ r = gcnew SlotRef();
        r->key = args[i]->ToLower();
        Amf^ p = profiles->Get(r->key);
        Amf^ slots = p == nullptr ? nullptr : p->Get(L"slots");
        r->slot = slots != nullptr ? L"a" + args[i + 1] : nullptr;
        Console::WriteLine(L"{0} a{1} -> user removed: {2}", r->key, args[i + 1], DeleteSlot(profiles, r));
    }
    File::WriteAllBytes(args[1], f->ToBytes());
    return 0;
}
#else
[STAThread]
int main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    String^ path = args->Length > 0 ? args[0]
        : Path::Combine(AppDomain::CurrentDomain->BaseDirectory, L"player\\data\\saves\\localhost\\siftheads_world_offline.sol");
    Application::Run(gcnew MainForm(path));
    return 0;
}
#endif
