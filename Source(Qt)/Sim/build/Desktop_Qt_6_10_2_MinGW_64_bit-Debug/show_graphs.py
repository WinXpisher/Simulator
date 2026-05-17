import os
import re
import sys
import random
import tkinter as tk
from tkinter import ttk, filedialog, messagebox
from datetime import datetime
import pandas as pd
from collections import defaultdict
from openpyxl.styles import Font
import matplotlib.pyplot as plt

# -----------------------------
# Localization and configuration setup
# -----------------------------
# Determine the absolute path to lang.conf 
if getattr(sys, 'frozen', False):
    BASE_DIR = os.path.dirname(sys.executable)
else:
    BASE_DIR = os.path.dirname(os.path.abspath(__file__))

CONFIG_PATH = os.path.join(BASE_DIR, "lang.conf")
lang = "en"  # Default language 

if os.path.exists(CONFIG_PATH):
    with open(CONFIG_PATH, "r", encoding="utf-8") as conf:
        first_line = conf.readline().strip().lower()
        if first_line in ["en", "ukr"]:
            lang = first_line

# Dictionary storing all user-facing strings for both languages
translations = {
    "en": {
        "title": "Log processing",
        "label_hist_type": "Choose type of histogram:",
        "label_add_files": "Add log files:",
        "btn_next": "Next",
        "btn_add_files": "Add files",
        "btn_show": "Show",
        "err_name_title": "Name error",
        "err_name_msg": "'{name}' has the following format log<number>_<distribution_method_name>.txt",
        "err_count_title": "Error number of logs",
        "err_count_msg": "Each method must have an equal number of log files",
        "err_no_files_title": "Error",
        "err_no_files_msg": "Haven't chosen any file",
        "fd_title": "Choose log files",
        "fd_text_files": "Text files",
        "fd_all_files": "All files",
        "metric_tk": "General time of performing",
        "metric_tav": "Avarage time of waiting",
        "metric_pd": "Coefficient of stagnation",
        "ylabel_seconds": "Seconds",
        "ylabel_percents": "Percents (%)",
        "chart_title": "Histogram: {metric}",
        "chart_xlabel": "Pool of tasks' number",
        "chart_legend": "Distribution method",
        "pool_label": "Pool of tasks {i}",
        "col_smallest": "The smallest number",
        "col_coeff": "Coeff",
        "col_pointer": "Pointer",
        "pointer_pattern": "POOL OF TASKS №{pool} {metric}",
        "comb_pool_label": "{label} (pool {pool_idx})",
        "sheet_metrics": "Metrics"
    },
    "ukr": {
        "title": "Обробка логів",
        "label_hist_type": "Оберіть тип гістограми:",
        "label_add_files": "Додати лог-файли:",
        "btn_next": "Далі",
        "btn_add_files": "Додати файли",
        "btn_show": "Показати",
        "err_name_title": "Помилка імені",
        "err_name_msg": "'{name}' має мати такий формат: log<номер>_<назва_методу_розподілу>.txt",
        "err_count_title": "Помилка кількості логів",
        "err_count_msg": "Кожен метод повинен мати однакову кількість лог-файлів",
        "err_no_files_title": "Помилка",
        "err_no_files_msg": "Не обрано жодного файлу",
        "fd_title": "Оберіть лог-файли",
        "fd_text_files": "Текстові файли",
        "fd_all_files": "Всі файли",
        "metric_tk": "Загальний час виконання",
        "metric_tav": "Середній час очікування",
        "metric_pd": "Коефіцієнт простою",
        "ylabel_seconds": "Секунди",
        "ylabel_percents": "Відсотки (%)",
        "chart_title": "Гістограма: {metric}",
        "chart_xlabel": "Номер пулу завдань",
        "chart_legend": "Метод розподілу",
        "pool_label": "Пул завдань {i}",
        "col_smallest": "Найменше значення",
        "col_coeff": "Коеф.",
        "col_pointer": "Показник",
        "pointer_pattern": "ПУЛ ЗАВДАНЬ №{pool} {metric}",
        "comb_pool_label": "{label} (пул {pool_idx})",
        "sheet_metrics": "Метрики"
    }
}

# Translation helper function (similar to tr() in Qt)
def _(key):
    return translations[lang].get(key, key)


# -----------------------------
# General pattern for time stamp
# -----------------------------
TIMESTAMP_PATTERN = r'\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}'


# -----------------------------
# 1) Helper functions
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
            # Localized name structural pattern error
            messagebox.showerror(_("err_name_title"), _("err_name_msg").format(name=name))
            return

    counts = [len(v) for v in by_method.values()]
    if len(set(counts)) != 1:
        # Localized unbalanced log entries count error
        messagebox.showerror(_("err_count_title"), _("err_count_msg"))
        return

    pool_count = counts[0]
    methods    = sorted(by_method.keys())

    # Base table definition
    rows = []
    for metric in ("T_k", "t_av", "P_d"):
        for pool_idx in range(1, pool_count+1):
            rows.append({"pool": pool_idx, "metric": metric})
    base_df = pd.DataFrame(rows)

    # Fill T_k, t_av, P_d matrices
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

    # Add translated evaluation headers
    base_df[_("col_smallest")] = base_df[methods].min(axis=1)
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
    base_df[_("col_coeff")] = coeffs

    # Set localized metrics label row
    base_df[_("col_pointer")] = base_df.apply(
        lambda r: _("pointer_pattern").format(pool=r.pool, metric=r.metric), axis=1
    )
    out_cols = [_("col_pointer")] + methods + [_("col_coeff"), _("col_smallest")]
    out_df   = base_df[out_cols]

    # Combine rows dynamically using the translated data frame layout keys
    comb_rows = []
    for pool_idx in range(1, pool_count+1):
        c = base_df.loc[base_df.pool==pool_idx, _("col_coeff")].tolist()
        mins = base_df.loc[base_df.pool==pool_idx, _("col_smallest")].tolist()
        
        for label, idxs in [
            ("T_k·t_av",    (0,1)),
            ("T_k·P_d",     (0,2)),
            ("t_av·P_d",    (1,2)),
            ("T_k·t_av·P_d",(0,1,2))
        ]:
            row = {_("col_pointer"): _("comb_pool_label").format(label=label, pool_idx=pool_idx)}
            for m in methods:
                vals = [out_df.loc[out_df[_("col_pointer")] == _("pointer_pattern").format(pool=pool_idx, metric="T_k"), m].iloc[0],
                        out_df.loc[out_df[_("col_pointer")] == _("pointer_pattern").format(pool=pool_idx, metric="t_av"), m].iloc[0],
                        out_df.loc[out_df[_("col_pointer")] == _("pointer_pattern").format(pool=pool_idx, metric="P_d"), m].iloc[0]]
                s = sum( c[i] * (vals[i]/mins[i]) for i in idxs )
                row[m] = round(s,2)
            row[_("col_smallest")] = min(row[m] for m in methods)
            comb_rows.append(row)
            
    comb_df = pd.DataFrame(comb_rows, columns=[_("col_pointer")]+methods+[_("col_smallest")])

    # Merge structures with an analytical blank spacing row
    blank = pd.DataFrame([[""]*len(out_df.columns)], columns=out_df.columns)
    final_df = pd.concat([out_df, blank, comb_df], ignore_index=True)

    # Exporting into target localized Excel worksheet structures
    os.makedirs("Multi_graphs", exist_ok=True)
    fname = "_".join(methods+[str(pool_count)])+".xlsx"
    out_p = os.path.join("Multi_graphs", fname)
    with pd.ExcelWriter(out_p, engine="openpyxl") as writer:
        final_df.to_excel(writer, index=False, sheet_name=_("sheet_metrics"))
        ws = writer.sheets[_("sheet_metrics")]
        for cell in ws[1]:
            cell.font = Font(bold=True)
        for col in ws.columns:
            w = max(len(str(c.value)) for c in col)+2
            ws.column_dimensions[col[0].column_letter].width = w


# -----------------------------
# 3) SHOW HISTOGRAM
# -----------------------------
def display_histogram(paths: list[str], metric_idx: int):
    """
    metric_idx: 0→Tk, 1→t_av, 2→Pd
    """
    labels = [_("metric_tk"), _("metric_tav"), _("metric_pd")]
    ylabel = _("ylabel_seconds") if metric_idx < 2 else _("ylabel_percents")

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

    # Localize labels, chart axis, and configurations
    df = pd.DataFrame(
        data,
        index=[_("pool_label").format(i=i) for i in range(1, pool_count+1)]
    )
    ax = df.plot.bar(figsize=(8,5))
    ax.set_title(_("chart_title").format(metric=labels[metric_idx]))
    ax.set_xlabel(_("chart_xlabel"))
    ax.set_ylabel(ylabel)
    ax.legend(title=_("chart_legend"))
    plt.tight_layout()
    plt.show()


# -----------------------------
# 4) GUI
# -----------------------------
root = tk.Tk()
root.title(_("title"))
root.update_idletasks()

# Centralize main window
W, H = 400, 200
X = (root.winfo_screenwidth() - W)//2
Y = (root.winfo_screenheight() - H)//2
root.geometry(f"{W}x{H}+{X}+{Y}")

ttk.Label(root, text=_("label_hist_type")).pack(pady=(20,5))
hist_cb = ttk.Combobox(
    root,
    values=[_("metric_tk"), _("metric_tav"), _("metric_pd")],
    state="readonly"
)
hist_cb.pack(fill="x", padx=20)
hist_cb.current(0)

def open_multi_window():
    root.withdraw()
    win = tk.Toplevel()
    win.title(_("title"))
    win.update_idletasks()

    # Centralize selection window
    WW, HH = 600, 350
    XX = (win.winfo_screenwidth() - WW)//2
    YY = (win.winfo_screenheight() - HH)//2
    win.geometry(f"{WW}x{HH}+{XX}+{YY}")

    ttk.Label(win, text=_("label_add_files")).pack(anchor="w", padx=10, pady=(10,0))
    lb = tk.Listbox(win, width=80, height=10)
    lb.pack(padx=10, pady=(0,10))

    def add_files():
        files = filedialog.askopenfilenames(
            title=_("fd_title"),
            filetypes=[(_("fd_text_files"), "*.txt"), (_("fd_all_files"), "*.*")]
        )
        for f in files:
            if f not in lb.get(0, tk.END):
                lb.insert(tk.END, f)

    def on_process():
        logs = lb.get(0, tk.END)
        if not logs:
            messagebox.showwarning(_("err_no_files_title"), _("err_no_files_msg"))
            return

        # Process and save analytical table structures
        process_logs(list(logs))
        # Evaluate index extracted from localized UI combobox selections
        idx = hist_cb.current()
        display_histogram(list(logs), idx)

        win.destroy()
        root.deiconify()

    frm = ttk.Frame(win)
    frm.pack(pady=(0,10))
    ttk.Button(frm, text=_("btn_add_files"), command=add_files).pack(side="left", padx=5)
    ttk.Button(frm, text=_("btn_show"), command=on_process).pack(side="left", padx=5)

ttk.Button(root, text=_("btn_next"), command=open_multi_window).pack(pady=20)
root.mainloop()