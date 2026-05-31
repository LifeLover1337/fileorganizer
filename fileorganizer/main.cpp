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

