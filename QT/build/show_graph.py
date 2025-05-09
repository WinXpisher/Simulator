import tkinter as tk
from tkinter import ttk, messagebox
import os
import matplotlib.pyplot as plt

def show_log():
    method = method_var.get().lower()
    log_number = log_entry.get()

    folder_path = os.path.join(os.getcwd(), method)
    file_name = f"log{log_number}.txt"
    file_path = os.path.join(folder_path, file_name)

    if not os.path.exists(folder_path):
        messagebox.showerror("Помилка", f"Папка '{method}' не існує!")
        return
    elif not os.path.exists(file_path):
        messagebox.showerror("Помилка", f"Файл '{file_name}' не знайдено у папці '{method}'!")
        return

    with open(file_path, "r") as file:
        lines = file.readlines()

    title = "Графіки"
    timestamps, tasks, waiting_times, stagnations = [], [], [], []
    
    for line in lines:
        if "Simulation started at:" in line:
            title += f" ({line.split(': ')[1].strip()})"
        elif "[!]" in line:
            parts = line.split("|")
            timestamps.append(int(parts[0].split(": ")[1].split(" ")[0]))
            waiting_times.append(float(parts[1].split(": ")[1].strip().replace(" minutes", "")))
            stagnations.append(float(parts[2].split(": ")[1].strip().replace("%", "")))
        elif "[RUN]" in line or "[SEND]" in line:
            task_id = line.split("[")[1].split("]")[0]
            tasks.append(task_id)
    
    plt.figure(figsize=(8, 5))
    plt.plot(timestamps, stagnations, label="Resource Stagnation")
    plt.plot(timestamps, waiting_times, label="Avg Waiting Time")

    plt.xlabel("Час (хвилини)")
    plt.ylabel("Відсотки / Хвилини")
    plt.title(title)
    plt.xticks(timestamps[::5])  # Відображати кожен другий значок часу
    plt.legend()
    plt.grid()

    plt.show()

root = tk.Tk()
root.title("Графіки")
root.geometry("200x200")  # Встановлюємо розмір вікна

# Отримуємо розміри екрана
screen_width = root.winfo_screenwidth()
screen_height = root.winfo_screenheight()

# Обчислюємо позицію для центрування
x_position = (screen_width // 2) - (100 // 2)
y_position = (screen_height // 2) - (150 // 2)

root.geometry(f"200x200+{x_position}+{y_position}")  # Центруємо вікно

method_label = tk.Label(root, text="Обрати метод")
method_label.pack(pady=5)

method_var = tk.StringVar(value="FCFS")
method_dropdown = ttk.Combobox(root, textvariable=method_var, values=("FCFS", "LIFO", "HPF", "BACKFILL", "SIMPLEX", "SMART", "MFQS", "Penguin"))
method_dropdown.pack(pady=5)

log_label = tk.Label(root, text="Написати номер логу")
log_label.pack(pady=5)

log_entry = tk.Entry(root)
log_entry.pack(pady=5)

show_button = tk.Button(root, text="Показати", command=show_log)
show_button.pack(pady=5)

root.mainloop()
