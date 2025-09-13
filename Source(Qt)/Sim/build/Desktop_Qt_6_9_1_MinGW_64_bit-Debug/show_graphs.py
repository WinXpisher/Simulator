import os
import re
import random
import tkinter as tk
from tkinter import ttk, filedialog, messagebox
from datetime import datetime
import pandas as pd
from collections import defaultdict
from openpyxl.styles import Font
import matplotlib.pyplot as plt

# -----------------------------
# Загальний паттерн для часового штампу
# -----------------------------
TIMESTAMP_PATTERN = r'\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}'

# -----------------------------
# 1) Допоміжні функції
# -----------------------------
def parse_metadata(log_path: str) -> int:
    res_re = re.compile(r'started running on resource\s+(\w+)', re.IGNORECASE)
    resources = set()
    with open(log_path, encoding='utf-8', errors='ignore') as f:
        for line in f:
            if m := res_re.search(line):
                resources.add(m.group(1))
    return max(len(resources), 1)

def parse_log_metrics(log_path: str) -> tuple[float, float, float]:
    run_re  = re.compile(
        rf'(?P<ts>{TIMESTAMP_PATTERN}).*\[RUN\].*started running on resource',
        re.IGNORECASE
    )
    perf_re = re.compile(
        rf'(?P<ts>{TIMESTAMP_PATTERN}).*\[PERFORMED\].*has been performed',
        re.IGNORECASE
    )
    start_times    = {}
    resource_times = defaultdict(float)
    first_ts = last_ts = last_run_ts = None

    with open(log_path, encoding='utf-8', errors='ignore') as f:
        for line in f:
            if m := run_re.search(line):
                ts  = datetime.strptime(m.group('ts'), '%Y-%m-%d %H:%M:%S')
                tid = re.search(r'Task\s*\[(.*?)\]', line).group(1)
                first_ts    = ts if first_ts      is None or ts < first_ts    else first_ts
                last_ts     = ts if last_ts       is None or ts > last_ts     else last_ts
                last_run_ts = ts if last_run_ts   is None or ts > last_run_ts else last_run_ts
                start_times[tid] = ts
                continue
            if m := perf_re.search(line):
                ts  = datetime.strptime(m.group('ts'), '%Y-%m-%d %H:%M:%S')
                tid = re.search(r'Task\s*\[(.*?)\]', line).group(1)
                first_ts = ts if first_ts is None or ts < first_ts else first_ts
                last_ts  = ts if last_ts  is None or ts > last_ts  else last_ts
                if tid in start_times:
                    resource_times[tid] += (ts - start_times.pop(tid)).total_seconds()
                continue

    Tk     = (last_ts - first_ts).total_seconds() if first_ts and last_ts else 0.0
    t_av   = (last_run_ts - first_ts).total_seconds() if first_ts and last_run_ts else Tk
    tk_sum = sum(resource_times.values())
    return Tk, t_av, tk_sum

# -----------------------------
# 2) process_logs 
# -----------------------------

def process_logs(paths: list[str]):
    pat = re.compile(r'^log\d+_([^\.]+)')
    by_method = {}

    for p in paths:
        name = os.path.basename(p)
        if m := pat.match(name):
            by_method.setdefault(m.group(1), []).append(p)
        else:
            messagebox.showerror("Невірне ім’я",
                f"'{name}' має формат log<число>_<метод>.txt")
            return

    counts = [len(v) for v in by_method.values()]
    if len(set(counts)) != 1:
        messagebox.showerror("Нерівні повторення",
            "Кожен метод має однакову кількість логів")
        return

    pool_count = counts[0]
    methods    = sorted(by_method.keys())

    # Базова таблиця
    rows = []
    for metric in ("T_k", "t_av", "P_d"):
        for pool_idx in range(1, pool_count+1):
            rows.append({"pool": pool_idx, "metric": metric})
    base_df = pd.DataFrame(rows)

    # Заповнюємо T_k, t_av, P_d
    for method in methods:
        tks, tavs, pds = [], [], []
        for log_path in sorted(by_method[method]):
            Tk, t_av, tk_sum = parse_log_metrics(log_path)
            ch = parse_metadata(log_path)
            Pd = abs((Tk*ch - tk_sum)/(Tk*ch)*100) if Tk and ch else 0.0
            tks.append(round(Tk,2))
            tavs.append(round(t_av,2))
            pds.append(round(Pd,2))
        base_df[method] = tks + tavs + pds

    # Додаємо "Найменше число" і "Коеф"
    base_df["Найменше число"] = base_df[methods].min(axis=1)
    coeffs = [0.0]*len(base_df)
    for pool_idx in range(1, pool_count+1):
        idxs = base_df.index[base_df["pool"]==pool_idx].tolist()
        while True:
            a, b = sorted((random.randint(0,9), random.randint(0,9)))
            i, j, k = a, b-a, 10-b
            if i and j and k:
                break
        for idx, coef in zip(idxs, (i/10, j/10, k/10)):
            coeffs[idx] = coef
    base_df["Коеф"] = coeffs

    base_df["Показник"] = base_df.apply(
        lambda r: f"ПУЛ №{r.pool} {r.metric}", axis=1
    )
    out_cols = ["Показник"] + methods + ["Коеф", "Найменше число"]
    out_df   = base_df[out_cols]

    # Комбіновані рядки: T_k·t_av, T_k·P_d, t_av·P_d, T_k·t_av·P_d
    comb_rows = []
    for pool_idx in range(1, pool_count+1):
        c = base_df.loc[base_df.pool==pool_idx, "Коеф"].tolist()
        mins = base_df.loc[base_df.pool==pool_idx, "Найменше число"].tolist()
        # індекси: 0→T_k,1→t_av,2→P_d в блоці по пулу
        for label, idxs in [
            ("T_k·t_av",    (0,1)),
            ("T_k·P_d",     (0,2)),
            ("t_av·P_d",    (1,2)),
            ("T_k·t_av·P_d",(0,1,2))
        ]:
            row = {"Показник": f"{label} (пул {pool_idx})"}
            for m in methods:
                vals = out_df.loc[out_df["Показник"]==f"ПУЛ №{pool_idx} T_k", m]
                vals = [out_df.loc[out_df["Показник"]==f"ПУЛ №{pool_idx} T_k", m].iloc[0],
                        out_df.loc[out_df["Показник"]==f"ПУЛ №{pool_idx} t_av", m].iloc[0],
                        out_df.loc[out_df["Показник"]==f"ПУЛ №{pool_idx} P_d", m].iloc[0]]
                s = sum( c[i] * (vals[i]/mins[i]) for i in idxs )
                row[m] = round(s,2)
            row["Найменше число"] = min(row[m] for m in methods)
            comb_rows.append(row)
    comb_df = pd.DataFrame(comb_rows, columns=["Показник"]+methods+["Найменше число"])

    # Об’єднуємо з порожнім рядком між
    blank = pd.DataFrame([[""]*len(out_df.columns)], columns=out_df.columns)
    final_df = pd.concat([out_df, blank, comb_df], ignore_index=True)

    # Пишемо в Excel
    os.makedirs("Multi_graphs", exist_ok=True)
    fname = "_".join(methods+[str(pool_count)])+".xlsx"
    out_p = os.path.join("Multi_graphs", fname)
    with pd.ExcelWriter(out_p, engine="openpyxl") as writer:
        final_df.to_excel(writer, index=False, sheet_name="Metrics")
        ws = writer.sheets["Metrics"]
        for cell in ws[1]:
            cell.font = Font(bold=True)
        for col in ws.columns:
            w = max(len(str(c.value)) for c in col)+2
            ws.column_dimensions[col[0].column_letter].width = w


# -----------------------------
# 3) ПОКАЗ ГІСТОГРАМИ
# -----------------------------
def display_histogram(paths: list[str], metric_idx: int):
    """
    metric_idx: 0→Tk, 1→t_av, 2→Pd
    """
    labels = ["Загальний час виконання", "Середній час очікування", "Коефіцієнт простою"]
    ylabel = "Секунди" if metric_idx < 2 else "Відсотки (%)"

    pat = re.compile(r'^log\d+_([^\.]+)')
    by_method = defaultdict(list)
    for p in paths:
        m = pat.match(os.path.basename(p))
        by_method[m.group(1)].append(p)

    methods    = sorted(by_method.keys())
    pool_count = len(next(iter(by_method.values())))

    data = {m: [] for m in methods}
    for m in methods:
        for lp in sorted(by_method[m]):
            Tk, t_av, tk_sum = parse_log_metrics(lp)
            ch = parse_metadata(lp)
            Pd = abs((Tk*ch - tk_sum)/(Tk*ch)*100) if Tk and ch else 0.0
            values = [Tk, t_av, Pd]
            data[m].append(values[metric_idx])

    df = pd.DataFrame(
        data,
        index=[f"Пул {i}" for i in range(1, pool_count+1)]
    )
    ax = df.plot.bar(figsize=(8,5))
    ax.set_title(f"Гістограма: {labels[metric_idx]}")
    ax.set_xlabel("Номер пулу")
    ax.set_ylabel(ylabel)
    ax.legend(title="Метод")
    plt.tight_layout()
    plt.show()

# -----------------------------
# 4) GUI
# -----------------------------
root = tk.Tk()
root.title("Обробка логів")
root.update_idletasks()

# Центруємо головне вікно
W, H = 400, 200
X = (root.winfo_screenwidth() - W)//2
Y = (root.winfo_screenheight() - H)//2
root.geometry(f"{W}x{H}+{X}+{Y}")

ttk.Label(root, text="Виберіть гістограму:").pack(pady=(20,5))
hist_cb = ttk.Combobox(
    root,
    values=["Загальний час виконання", "Середній час очікування", "Коефіцієнт простою"],
    state="readonly"
)
hist_cb.pack(fill="x", padx=20)
hist_cb.current(0)  # за замовчуванням перша

def open_multi_window():
    root.withdraw()
    win = tk.Toplevel()
    win.title("Обробка логів")
    win.update_idletasks()

    # Центруємо друге вікно
    WW, HH = 600, 350
    XX = (win.winfo_screenwidth() - WW)//2
    YY = (win.winfo_screenheight() - HH)//2
    win.geometry(f"{WW}x{HH}+{XX}+{YY}")

    ttk.Label(win, text="Додайте лог-файли:").pack(anchor="w", padx=10, pady=(10,0))
    lb = tk.Listbox(win, width=80, height=10)
    lb.pack(padx=10, pady=(0,10))

    def add_files():
        files = filedialog.askopenfilenames(
            title="Виберіть лог-файли",
            filetypes=[("Text files","*.txt"),("All files","*.*")]
        )
        for f in files:
            if f not in lb.get(0, tk.END):
                lb.insert(tk.END, f)

    def on_process():
        logs = lb.get(0, tk.END)
        if not logs:
            messagebox.showwarning("Помилка", "Не обрано жодного файлу")
            return

        # запишемо таблицю
        process_logs(list(logs))
        # визначимо індекс за обраним текстом
        idx = hist_cb.current()  # 0,1 або 2
        display_histogram(list(logs), idx)

        win.destroy()
        root.deiconify()

    frm = ttk.Frame(win)
    frm.pack(pady=(0,10))
    ttk.Button(frm, text="Додати файли", command=add_files).pack(side="left", padx=5)
    ttk.Button(frm, text="Показати",     command=on_process).pack(side="left", padx=5)

ttk.Button(root, text="Далі", command=open_multi_window).pack(pady=20)
root.mainloop()

