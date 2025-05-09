import tkinter as tk
from tkinter import ttk, messagebox
import os
import matplotlib.pyplot as plt

def show_log():
    method = method_var.get().lower().strip()
    log_number = log_entry.get().strip()

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

    # Оновлення заголовка
    log_title = f"Графік (Лог {log_number}, Метод {method})"
    timestamps, waiting_times, stagnations = [], [], []
    
    total_execution_time = 0  # Загальний час виконання

    for line in lines:
        if "[!]" in line:
            parts = line.split("|")
            clock_time = int(parts[0].split(": ")[1].split(" ")[0])
            timestamps.append(clock_time)
            waiting_times.append(float(parts[1].split(": ")[1].strip().replace(" minutes", "")))
            stagnations.append(float(parts[2].split(": ")[1].strip().replace("%", "")))
            total_execution_time = clock_time  # Оновлюємо загальний час

    log_title += f", Загальний час виконання: {total_execution_time} хв."

    plt.figure(figsize=(10, 5))
    plt.plot(timestamps, stagnations, label="Resource Stagnation")
    plt.plot(timestamps, waiting_times, label="Avg Waiting Time")

    plt.xlabel("Час (хвилини)")
    plt.ylabel("Відсотки / Хвилини")
    plt.title(log_title)
    plt.xticks(timestamps[::5])  
    plt.gca().xaxis.set_major_locator(plt.MaxNLocator(nbins=10))
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
