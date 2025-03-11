#pragma once
#include <fstream>
#include <string>
#include <filesystem>
#include <regex>

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

    int getNextFileNumber(const std::string& prefix, const std::string& extension) {
        int maxNumber = 0;
        // патерн prefix[number]extension
        std::regex filePattern(prefix + "(\\d+)" + extension);

        // проходимо по файлам поточної директорії
        for (const auto& entry : std::filesystem::directory_iterator(currentDir)) {
            const auto& filename = entry.path().filename().string();
            std::smatch match;

            // шукаємо файли, які відповідають патерну
            if (std::regex_match(filename, match, filePattern)) {
                int number = std::stoi(match[1].str());
                maxNumber = std::max(maxNumber, number);
            }
        }

        return maxNumber + 1;
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

        path currentDirCopy = currentDir;
        currentDirCopy.append(dirName);

        bool isDirExists = createDirIfNotExist(currentDirCopy.string());
        if (isDirExists)
            currentDir = currentDirCopy;
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

    void openFileStream(std::ifstream& stream, const std::string& fileName)
    {
        path fullPath = currentDir;
        fullPath.append(fileName);
        stream.open(fullPath);
    }

    void openFileStream(std::ofstream& stream, const std::string& fileName)
    {
        path fullPath = currentDir;
        fullPath.append(fileName);
        stream.open(fullPath);
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