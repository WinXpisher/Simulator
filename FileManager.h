#pragma once
#include <fstream>
#include <string>
#include <filesystem>
#include <fstream>

class FileManager
{
private:
    using path = std::filesystem::path;
    path currentDir;

    path getProgramPath()
    {
        return std::filesystem::current_path();
    }

    // Створити папку в поточній директорії з ім'ям dirName.
    // Повертає true, якщо папка або вже існувала, або була
    // успішно створена.
    bool createDirIfNotExist(std::string dirName)
    {
        path resultPath = getProgramPath();
        resultPath.append(dirName);
        if (!std::filesystem::exists(resultPath))
            return std::filesystem::create_directories(resultPath);
        return true;
    }
public:
    FileManager()
    {
        currentDir = getProgramPath();
    }
    
    // Вибрати поточну папку для запису. Якщо dirName="",
    // вибирається папка проекту.
    bool selectDir(std::string dirName="")
    {
        if (dirName == "")
        {
            currentDir = getProgramPath();
            return true;
        }

        bool isDirExists = createDirIfNotExist(dirName);
        if (isDirExists)
            currentDir.append(dirName);
        return isDirExists;
    }

    bool writeString(
        std::string fileName,
        std::string stringToWrite
    )
    {
        path resultPath = currentDir;
        resultPath.append(fileName);

        ofstream fileWriter(resultPath, ios::app);
        if (!fileWriter.is_open())
            return false;

        fileWriter << stringToWrite;
        return true;
    }

    void removeAllDirectoriesInCurrentDir()
    {
        for (const auto& entry : std::filesystem::directory_iterator(currentDir))
        {
            if (entry.is_directory())
                std::filesystem::remove_all(entry.path());
        }
    }
};