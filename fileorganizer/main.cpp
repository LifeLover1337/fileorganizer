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

//создается короткое имя fs для команды std::filesystem

namespace fs = std::filesystem;

// преобразование UTF-8 строки в широкую строку (wstring) для windows api функия для работы с кодировками (русс.яз.)

std:: wstring utf8_to_wstring (const std::string& str) {
	if (str.empty()) return L"";
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
	return wstr;
}
