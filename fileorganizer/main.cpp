#include <iostream>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <algorithm>
#include <cctype>
#include <windows.h>
#include <codecvt>
#include <locale>

// создается короткое имя fs для длинного std::filesystem 
namespace fs = std::filesystem;

// преобразование UTF-8 строки в широкую строку (wstring) для windows api функия для работы с кодировками (русс.яз.)
std:: wstring utf8_to_wstring (const std::string& str) {
	if (str.empty()) return L"";
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
	return wstr;
}
// преобразование широкой строки utf-8
std::string wstring_to_utf8(const std::wstring& wstr) {
	if (wstr.empty()) return "";
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string str(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &str[0], size_needed, NULL, NULL);
	return str;
}
// преобразование std::string utf-8 в std::filesystem::path поддерживает русский 
fs::path utf8_to_path(const std::string& str) {
	return fs::path(const std::wstring(str));
}
// class- тип данных , который объединяет переменные и функции для работы с ними, private- поле extensionmap доступно только внутри самого класса
class FileOrganizer {
private:
	std::unordered_map<std::string, std::string> extensionMap;
	// правила сортировки
	void initializeExtensionMap() {

		// Изображения
		extensionMap[".jpg"] = "Изображения";
		extensionMap[".jpeg"] = "Изображения";
		extensionMap[".png"] = "Изображения";
		extensionMap[".gif"] = "Изображения";
		extensionMap[".bmp"] = "Изображения";
		extensionMap[".tiff"] = "Изображения";
		extensionMap[".webp"] = "Изображения";
		extensionMap[".svg"] = "Изображения";
		extensionMap[".ico"] = "Изображения";

		// видео 
		extensionMap[".mp4"] = "Видео";
		exteextensionMap[".avi"] = "Видео";
		extensionMap[".mkv"] = "Видео";
		extensionMap[".mov"] = "Видео";
		extensionMap[".wmv"] = "Видео";
		extensionMap[".flv"] = "Видео";
		extensionMap[".webm"] = "Видео";
		extensionMap[".mpeg"] = "Видео";
		extensionMap[".mpg"] = "Видео";

		// музыка
		extensionMap[".mp3"] = "Музыка";
		extensionMap[".wav"] = "Музыка";
		extensionMap[".flac"] = "Музыка";
		extensionMap[".ogg"] = "Музыка";
		extensionMap[".m4a"] = "Музыка";
		extensionMap[".aac"] = "Музыка";
		extensionMap[".wma"] = "Музыка";

		// документы 
		extensionMap[".pdf"] = "Документы";
		extensionMap[".doc"] = "Документы";
		extensionMap[".docx"] = "Документы";
		extensionMap[".txt"] = "Документы";
		extensionMap[".rtf"] = "Документы";
		extensionMap[".xls"] = "Документы";
		extensionMap[".xlsx"] = "Документы";
		extensionMap[".ppt"] = "Документы";
		extensionMap[".pptx"] = "Документы";
		extensionMap[".odt"] = "Документы";
		extensionMap[".ods"] = "Документы";
		extensionMap[".odp"] = "Документы";
		extensionMap[".csv"] = "Документы";

		// архивы 
		extensionMap[".zip"] = "Архивы";
		extensionMap[".rar"] = "Архивы";
		extensionMap[".7z"] = "Архивы";
		extensionMap[".tar"] = "Архивы";
		extensionMap[".gz"] = "Архивы";
		extensionMap[".bz2"] = "Архивы";
		extensionMap[".xz"] = "Архивы";
	}
	// этот блок приводит расширение файла к нижнему регистру
	std::string getExtension(const fs::path& filepath) {
		std::string ext = filePath.extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		return ext;
	}
	// создание папки
	bool createFolderIfNeeded(const fs::path& basePath, const std::string& folderName) {
		fs::path folderPath = basePath / utf8_to_path(folderName);
		if (!fs::exists(folderPath)) {
			if (fs::create_directory(folderPath)) {
				std::cout << "Создана папка:" << folderName << std::endl;
				return true;
			}
			else {
				std::cerr << "Не удалось создать папку:" << folderName << std::endl;
				return false;
			}
		}
		return true;
	}



