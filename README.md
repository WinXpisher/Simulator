# **Імітаційне середовище розподілу ресурсів на методи/Simulation environment for resource allocation across methods** 

<!-- TOC -->
- [English](#english)
- [Українська](#українська)
<!-- /TOC -->

---

## Українська

**Опис:**
Ця програма була розроблена студентами ХНУРЕ: Василенко Д.О., Бирька Е.М., Лук'янов Я.І., для дослідження методів розподілу (LIFO, FIFO, HPF, SMART й т.д). Ця програма може розширюватись та допомогати з дослідженням нових методів для різних ОС.

---

## **Зміст**

- [Початок роботи на Windows](#початок-роботи-на-windows)
- [Початок роботи на Linux](#початок-роботи-на-linux)
- [Інструкція](#інструкція)
- [Ліцензія](#ліцензія)

---

## **Початок роботи на Windows**

1. Скачати готову програму:
   [Перейти на сайт релізів](https://github.com/WinXpisher/Simulator/releases)
3. Розархівувати архів та перейти в каталог:
   ```bash
   cd Розархівований_архів/App
4. Запустіть готову програму:
   ```bash
   Core.exe

---

## **Початок роботи на Linux**

1. Скачати готову програму:
   [Перейти на сайт релізів](https://github.com/WinXpisher/Simulator/releases)
3. Розархівувати архів та перейти в каталог:
   ```bash
   cd Розархівований_архів/App(Linux)
4. Запустіть готову програму:
   ```bash
   Core.AppImage

---

## **Інструкція**

1. Після відкриття зустрічає таке вікно:

![First Window](github_images/first_window_ukr.png) 

2. Після обирання "Генерувати", ми бачимо таке вікно з вибором методів розподілу та з ручним вводом завдань та ресурсів(зберігаються в папку для наборів):

![Generate](github_images/generate_ukr.png)

![Generate](github_images/generate_1_ukr.png)

3. Якщо ви обрали "Обрати", то побачите таке вікно з вибором набору симуляції (зберігаються такі набори в папці App/Simulation data):

![Choose](github_images/choose_ukr.png)

4. Незалежно від вибору у вас з'явиться вікно з налаштуваннями набору:

![Generate/Choose](github_images/choose_1_ukr.png)

5. Далі з'явиться наступне вікно зі спостереженням щодо виконання задач:

![Second window](github_images/second_window_ukr.png)

6. У цьому вікні, окрім спостереження, можна перетягувати рамку Resources, затиснувши ЛКМ на рамці, а також за бажанням вимкнути:

![Second window res](github_images/second_window_res_ukr.png)

![Second window res 1](github_images/second_window_res_1_ukr.png)

![Second window res 2](github_images/second_window_res_2_ukr.png)

7. Для наступного аналізу, після того як лічильник "Clock" перестане рахувати, ми можемо вивести гістограму на основі 1 логу або більше:

![Second window show](github_images/second_window_show_ukr.png)

8. Якщо ви обрали "Показати графік" (для обробки 1 логу), то побачите наступне вікно з вибором бажаного лог файлу:

![Show graph](github_images/show_graph_ukr.png)

![Show graph 1](github_images/show_graph_1_ukr.png)

9. Після обрання та натискання кнопки "Показати", ви попадете в папку з логами відповідного методу, який ви обрали раніше:

![Show graph 2](github_images/show_graph_2.png)

10. Після вибору файла, на екрані з'явиться гістограма, де по осі x бачимо "Години", а по осі y "Кількість задач", а також синє - задачі, які виконуються, а оранжеве - в черзі:

![Show graph hist](github_images/show_graph_hist_ukr.png)

11. Якщо ви обрали "Показати графіки" (для обробки декількох логів), то рекомендація створити папку в корені проєкту з назвою multi_logs (App/multi_logs) для зручного додавання, а також щоб набор відповідав набору (в папці TestSets буде надано файли та відповідні набори, які використовувались, для прикладу). У вікні можна обрати 3 показника: "Загальний час виконання", "Середній час очікування", "Коефіційєнт простою":

![Show graphs](github_images/show_graphs_ukr.png)

![Show graphs 1](github_images/show_graphs_1_ukr.png)
    
12. Після натискання кнопки "Далі", ми можемо обрати декілька логів й воно відобразиться в списку:

![Show graphs 2](github_images/show_graphs_2.png)

![Show graphs 3](github_images/show_graphs_3.png)

![Show graphs 4](github_images/show_graphs_4_ukr.png)

13. Після натискання кнопки "Показати", ми побачимо гістограму, де по осі x буде "Номер пулу", а по осі y "Cекунди(Години)" й різними кольорами відповідні методи, які обрали раніше:

![Show graphs hist](github_images/show_graphs_hist_ukr.png)

14. Для глибшого вивчення 1 логу, в корневій папці, створиться папка Graphs (App/Graphs), де містяться 2 excel таблиці, в перший показано дані для гістограми (Години, Виконуються, В черзі), а у другій - для детального вивчення методу (Кількість ресурсів, Кількість запущених завдань на ресурсі(K_i), Загальний час використання ресурсу(tk_i), Відсоток завантаження ресурсу(RT_i), Відсоток простою(DT_i)):

![Graphs folder](github_images/Graphs_folder.png)

![Graphs folder 1](github_images/Graphs_folder_1.png)

![Graphs excel](github_images/graphs_excel_ukr.png)

![Graphs excel 1](github_images/graphs_excel_1_ukr.png)

15. Для глибшого вивчення декількох логів, в корневій папці, створиться папка Multi_graphs (App/Multi_graphs), де буде зберігатись таблиця з назвою методів_та кількістю пулів, а всередині стовбець "Показник", де номера пулу та показники (Tk-час виконання пулу завдань, tav-середній час очікування завдань в черзі, Pd-показник простою обчислювальних ресурсів системи):

![Multi graphs folder](github_images/multi_graphs_folder.png)

![Multi graphs folder 1](github_images/multi_graphs_1.png)

![Multi graphs excel](github_images/multi_graphs_excel_ukr.png)

---

## **Ліцензія**

Цей проєкт поширюється під ліцензією **GPLv2**.
[Переглянути ліцензію](LICENSE)

---

## English

**Description:**
This program was developed by students of KhNURE: Vasylenko D.O., Birka E.M., Lukianov Y.I., to study distribution methods (LIFO, FIFO, HPF, SMART, etc.). This program can be expanded and help with the study of new methods for different OS.

---

## **Content**

- [Getting started on Windows](#getting-started-on-windows)
- [Getting started on Linux](#getting-started-on-linux)
- [Instruction](#instruction)
- [License](#license)

---

## **Getting started on Windows**

1. Download ready-to-use program:
   [Go to releases website](https://github.com/WinXpisher/Simulator/releases)
3. Unzip archive and go to the catalog:
   ```bash
   cd Unzipped_archive/App
4. Run the program:
   ```bash
   Core.exe

---

## **Getting started on Linux**

1. Download ready-to-use program:
   [Go to releases website](https://github.com/WinXpisher/Simulator/releases)
3. Unzip archive and go to the catalog:
   ```bash
   cd Unzipped_archive/App(Linux)
4. Run the program:
   ```bash
   Core.AppImage

---

## **Instruction**

1. After opening, you will see this window:

![First Window](github_images/first_window.png) 

2. After selecting "Generate", we see the following window with a choice of distribution methods and manual entry of tasks and resources (saved in the folder for sets):

![Generate](github_images/generate.png)

![Generate](github_images/generate_1.png)

3. If you selected "Choose", you will see the following window with a selection of a simulation set (such sets are stored in the App/Simulation data folder):

![Choose](github_images/choose.png)

4. Regardless of your choice, you will see a window with dialling settings:

![Generate/Choose](github_images/choose_1.png)

5. The following window will appear with task performance monitoring:

![Second window](github_images/second_window.png)

6. In this window, in addition to observing, you can drag the Resources frame by holding down the LMB on the frame, and also turn it off if desired:

![Second window res](github_images/second_window_res.png)

![Second window res 1](github_images/second_window_res_1.png)

![Second window res 2](github_images/second_window_res_2.png)

7. For further analysis, after the "Clock" counter stops counting, we can output a histogram based on 1 log or more:

![Second window show](github_images/second_window_show.png)

8. If you selected "Show graph" (to process 1 log), you will see the following window with the selection of the desired log file:

![Show graph](github_images/show_graph.png)

![Show graph 1](github_images/show_graph_1.png)

9. After selecting and clicking the Show button, you will be taken to the folder with the logs of the corresponding method you selected earlier:

![Show graph 2](github_images/show_graph_2.png)

10. After selecting a file, a histogram will appear on the screen, where on the x-axis we see Hours, and on the y-axis Number of tasks, as well as blue - tasks that are being executed, and orange - in the queue:

![Show graph hist](github_images/show_graph_hist.png)

11. If you selected "Show graphs" (for processing multiple logs), it is recommended to create a folder in the root of the project called multi_logs (App/multi_logs) for easy addition, and also so that the set corresponds to the set (for example, the TestSets folder will provide files and the corresponding sets that were used). In the window you can select 3 indicators: General time of performing, Avarage time of waiting, Coefficient of stagnation:

![Show graphs](github_images/show_graphs.png)

![Show graphs 1](github_images/show_graphs_1.png)
    
12. After clicking the Next button, we can select multiple logs and they will be displayed in the list:

![Show graphs 2](github_images/show_graphs_2.png)

![Show graphs 3](github_images/show_graphs_3.png)

![Show graphs 4](github_images/show_graphs_4.png)

13. After clicking the Show button, we will see a histogram where the x-axis will be Pool Number, and the y-axis will be Seconds(Hours) and the corresponding methods selected earlier will be shown in different colors:

![Show graphs hist](github_images/show_graphs_hist.png)

14. For a deeper study of 1 log, in the root folder, a Graphs folder (App/Graphs) will be created, which contains 2 excel tables, the first shows data for the histogram (Hours, Running, Waiting), and the second - for a detailed study of the method (Number of resources, Number of running tasks on the resource(K_i), Total resource usage time(tk_i), Resource load percentage(RT_i), Stagnation time(DT_i)):

![Graphs folder](github_images/Graphs_folder.png)

![Graphs folder 1](github_images/Graphs_folder_1.png)

![Graphs excel](github_images/graphs_excel.png)

![Graphs excel 1](github_images/graphs_excel_1.png)

15. For a deeper study of several logs, in the root folder, a Multi_graphs folder (App/Multi_graphs) is created, where a table with the name of the methods and the number of pools will be stored, and inside the tables a column Pointer with pool numbers and indicators (Tk-task pool execution time, tav-average waiting time for tasks in the queue, Pd-indicator of system computing resources downtime):

![Multi graphs folder](github_images/multi_graphs_folder.png)

![Multi graphs folder 1](github_images/multi_graphs_1.png)

![Multi graphs excel](github_images/multi_graphs_excel.png)

---

## **License**

This project is distributed under the license **GPLv2**.
[View license](LICENSE)

---
