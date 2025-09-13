# **Імітаційне середовище розподілу ресурсів на методи/** 

<!-- TOC -->
- [English](#english)
- [Українська](#українська)
<!-- /TOC -->

---

## Українська

**Опис:**
Ця програма була розроблена студентами ХНУРЕ: Василенко Д.О., Бирька Е.М., Лук'янов Я.І., для дослідження методів розподілу (LIFO, FIFO, HPF, SMART й т.д) на ресурси. Ця програма може розширюватись та допомогати з дослідженням нових методів для різних ОС.

---

## **Зміст**

- [Початок роботи](#початок-роботи)
- [Інструкція](#інструкція)
- [Ліцензія](#ліцензія)

---

## **Початок роботи**

1. Клонувати репозиторій:
   ```bash
   git clone https://github.com/WinXpisher/Simulator.git
2. Перейти в каталог(з готовою програмою):
   ```bash
   cd Simulator/App
3. Запустіть готову програму:
   ```bash
   Core.exe

---

## **Інструкція**

1. Після відкриття зустрічає таке вікно:

![First Window](github_images/first_window.png) 

2. Після обирання Generate, ми бачимо таке вікно з вибором методів розподілу та з ручним вводом завдань та ресурсів(зберігаються в папку для наборів):

![Generate](github_images/generate.png)

![Generate](github_images/generate_1.png)

3. Якщо ви обрали choose, то побачите таке вікно з вибором набору симуляції (зберігаються такі набори в папці App/Simulation data)

![Choose](github_images/choose.png)

4. Незалежно від вибору у вас з'явиться вікно з налаштуваннями набору:

![Generate/Choose](github_images/choose_1.png)

5. Далі з'явиться наступне вікно зі спостереженням щодо виконання задач:

![Second window](github_images/second_window.png)

6. У цьому вікні окрім спостереження можна перетягувати рамку Resources, затиснувши ЛКМ на рамці, а також за бажанням вимкнути:

![Second window res](github_images/second_window_res.png)

![Second window res 1](github_images/second_window_res_1.png)

![Second window res 2](github_images/second_window_res_2.png)

7. Для наступного аналізу, після того як лічильник Clock перестане рахувати, ми можемо вивести гістограму на основі 1 логу або більше:

![Second window show](github_images/second_window_show.png)

8. Якщо ви обрали Show graph(для обробки 1 логу), то побачите наступне вікно з вибором бажаного лог файлу:

![Show graph](github_images/show_graph.png)

![Show graph 1](github_images/show_graph_1.png)

9. Після обрання та натискання кнопки "Показати", ви попадете в папку з логами відповідного методу, який ви обрали раніше:

![Show graph 2](github_images/show_graph_2.png)

10. Після вибору файла, на екрані з'явиться гістограма, де по осі x бачимо "Години", а по осі y "Кількість задач", а також синє - задачі, які виконуються, а оранжеве - в черзі:

![Show graph hist](github_images/show_graph_hist.png)

11. Якщо ви обрали Show graphs(для обробки декількох логів), то рекомендація створити папку в корені проєкту з назвою multi_logs (App/multi_logs) для зручного додавання, а також щоб набор відповідав нгабору(в папці TestSets буде надано файли та відповідні набори, які використовувались). У вікні можна обрати 3 показника: "Загальний час виконання", "Середній час очікування", "Коефіційєнт простою":

![Show graphs](github_images/show_graphs.png)

![Show graphs 1](github_images/show_graphs_1.png)
    
12. Після натискання кнопки "Далі", ми можемо обрати декілька логів й воно відобразиться в списку:

![Show graphs 2](github_images/show_graphs_2.png)

![Show graphs 3](github_images/show_graphs_3.png)

![Show graphs 4](github_images/Show_graphs_4.png)

13. Після натискання кнопки "Показати", ми побачимо гісограму, де по осі x буде "Номер пулу", а по осі y "Секунди" й різними кольорами відповідні методи, які обрали раніше:

![Show graphs hist](github_images/show_graphs_hist.png)

14. Для глибшого вивчення 1 логу, в корневій папці, створиться папка Graphs (App/Graphs), де містяться 2 excel таблиці, в перший показано дані для гістограми (Години, Виконуються, В черзі), а у другій - для детального вивчення методу (Кількість ресурсів, кількість запущених завдань на ресурсі, загальний час використання ресурсу, Відсоток завантаження ресурсу, відсоток простою):

![Graphs folder](github_images/Graphs_folder.png)

![Graphs folder 1](github_images/Graphs_folder_1.png)

![Graphs excel](github_images/graphs_excel.png)

![Graphs excel 1](github_images/graphs_excel_1.png)

15. Для глибшого вивчення декількох логів, в корневій папці, стовриться папка Multi_graphs (App/Multi_graphs), де буде зберігатись таблиця з назвою методів_та кількістю пулів, а всередині стовбець "Показник", де номера пулу та показники (Tk-час виконання пулу завдань, tav-середній час очікування завдань в черзі, Pd-показник простою обчислювальних ресурсів системи):

![Multi graphs folder](github_images/multi_graphs_folder.png)

![Multi graphs folder 1](github_images/multi_graphs_1.png)

![Multi graphs excel](github_images/multi_graphs_excel.png)

---

## **Ліцензія**

Цей проєкт поширюється під ліцензією **GPLv2**.
[Дивитись ліцензію](LICENSE)
---
