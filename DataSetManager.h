#pragma once

#include <vector>
#include <sstream>
#include <filesystem>
#include "SimulatorPrimitives.h"
#include "FileManager.h"

class DataSetManager
{
    /*
        Клас відповідає за запис наборів даних (завдань та ресурсів)
        до файлів та зчитування цих даних з файлів.
    */
private:
    FileManager fm;

    // Метод пропускає пусті символи та символи переходу на новий рядок.
    // Повертає true, якщо було досягнуто кінця файлу.
    bool skipEmptyChars(std::ifstream& ifs)
    {
        while (std::isspace(ifs.peek()) && ifs.peek() != EOF)
        {
            ifs.get();
        }
        return ifs.peek() == EOF;

    }
public:
    // --- TASKS ---
    int writeTasks(const std::vector<Task>& tasks)
    {
        fm.selectDir("");
        fm.selectDir("Simulation data");
        fm.selectDir("Task set");

        const std::string& prefix = "tasks_set";
        const std::string& extension = ".txt";
        
        int taskSetNumber = fm.getNextFileNumber(prefix, extension);
        const std::string fileName = prefix +
            std::to_string(taskSetNumber) +
            extension;

        std::ostringstream taskString;
        for (const Task& task : tasks)
        {
            taskString << task;
        }
        fm.writeString(fileName, taskString.str());
        
        return taskSetNumber;
    }

    void loadTasks(std::vector<Task>& tasks, int taskSetNum)
    {
        fm.selectDir("");
        fm.selectDir("Simulation data");
        fm.selectDir("Task set");

        const std::string prefix = "tasks_set";
        const std::string extension = ".txt";
        const std::string fileName = prefix + std::to_string(taskSetNum) + extension;

        std::ifstream ifs;
        fm.openFileStream(ifs, fileName);
        if (!ifs.is_open())
            return;

        while (true)
        {
            if (skipEmptyChars(ifs))
                break;

            Task task;
            if (!(ifs >> task)) break;
            tasks.push_back(task);
        }
    }

    // --- RESOURCES ---
    int writeResources(const std::vector<Resource>& resources)
    {
        fm.selectDir("");
        fm.selectDir("Simulation data");
        fm.selectDir("Resource set");

        const std::string prefix = "resources_set";
        const std::string extension = ".txt";

        int resSetNumber = fm.getNextFileNumber(prefix, extension);
        const std::string fileName = prefix +
            std::to_string(resSetNumber) +
            extension;

        std::ostringstream resString;
        for (const Resource& res : resources)
        {
            resString << res;
        }
        fm.writeString(fileName, resString.str());

        return resSetNumber;
    }

    void loadResources(std::vector<Resource>& resources, int resourceSetNum)
    {
        fm.selectDir("");
        fm.selectDir("Simulation data");
        fm.selectDir("Resource set");

        const std::string prefix = "resources_set";
        const std::string extension = ".txt";
        const std::string fileName = prefix + std::to_string(resourceSetNum) + extension;

        std::ifstream ifs;
        fm.openFileStream(ifs, fileName);
        if (!ifs.is_open())
            return;

        while (true)
        {
            if (skipEmptyChars(ifs))
                break;

            Resource res;
            if (!(ifs >> res)) break;
            resources.push_back(res);
        }
    }
};