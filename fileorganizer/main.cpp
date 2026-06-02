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
	return fs::path(utf8_to_wstring(str));
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
		extensionMap[".avi"] = "Видео";
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
		std::string ext = filepath.extension().string();
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
	// Генерация уникального имени при конфликте
	fs::path getUniqueFilename(const fs::path& targetPath) {
		if (!fs::exists(targetPath)) {
			return targetPath;
		}
		fs::path parent = targetPath.parent_path();
		std::string stem = targetPath.stem().string();
		std::string extension = targetPath.extension().string();

		int counter = 1;
		fs::path newPath;
		do {
			std::string newName = stem + " (" + std::to_string(counter) + ")" + extension;
			newPath = parent / utf8_to_path(newName);
			counter++;
		} while (fs::exists(newPath));

		return newPath;
	}
public:
	FileOrganizer() {
		initializeExtensionMap();
	}
	// Добавление пользовательского правила
	void addRule(const std::string& extension, const std::string& category) {
		std::string ext = extension;
		if (ext.empty()) return;
		if (ext[0] != '.') ext = "." + ext;
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		extensionMap[ext] = category;
		std::cout << "Добавлено правило:" << ext << " -> " << category << std::endl;
	}
	// удаление правила 
	void removeRule(const std::string& extension) {
		std::string ext = extension;
		if (ext.empty()) return;
		if (ext[0] != '.') ext = "." + ext;
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		auto it = extensionMap.find(ext);
		if (it != extensionMap.end()) {
			extensionMap.erase(it);
			std::cout << "Удалено правило для:" << ext << std::endl;
		}
		else {
			std::cout << "Правило не найдено:" << ext << std::endl;
		}
	}
	// Показать все правила
	void showRules() {
		std::cout << "\n ТЕКУЩИЕ ПРАВИЛА СОРТИРОВКИ:\n";
		std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

		std::set<std::string> categories;
		for (const auto& [ext, cat] : extensionMap) {
			categories.insert(cat);
		}

		for (const auto& cat : categories) {
			std::cout << "📂" << cat << ":\n    ";
			for (const auto& [ext, cat2] : extensionMap) {
				if (cat2 == cat) {
					std::cout << ext << " ";
				}
			}
			std::cout << "\n";
		}
		std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	}
	// основная функция сортировки 
	int organize(const std::string& directoryPath) {
		// Проверяем существование папки
		fs::path dirPath = utf8_to_path(directoryPath);

		if (!fs::exists(dirPath)) {
			std::cerr << "Ошибка: Папка не существует!\n";
			return 1;
		}

		if (!fs::is_directory(dirPath)) {
			std::cerr << "Ошибка: Указанный путь не является папкой!\n";
			return 1;
		}

		std::cout << "\nСОРТИРОВКА ПАПКИ:\n";
		std::cout << "   " << directoryPath << "\n";
		std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

		int filesProcessed = 0;
		int filesMoved = 0;

		for (const auto& entry : fs::directory_iterator(dirPath)) {
			// Пропускаем папки
			if (!fs::is_regular_file(entry.status())) {
				continue;
			}

			fs::path filePath = entry.path();
			std::string fileName = wstring_to_utf8(filePath.filename().wstring());
			std::string ext = getExtension(filePath);

			filesProcessed++;
			// определение категории 
			std::string category = "Разное";
			auto it = extensionMap.find(ext);
			if (it != extensionMap.end()) {
				category = it->second;
			}
			// папка для категории 
			if (!createFolderIfNeeded(dirPath, category)) {
				continue;
			}
			// путь назначения 
			fs::path targetPath = dirPath / utf8_to_path(category) / filePath.filename();
			fs::path uniquePath = getUniqueFilename(targetPath);
			// перемещение файла 
			try {
				fs::rename(filePath, uniquePath);
				if (targetPath != uniquePath) {
					std::cout << "⚠️" << fileName << " -> " << category << " (переименован)\n";
				}
				else {
					std::cout << "✅" << fileName << " -> " << category << "\n";
				}
				filesMoved++;
			}
			catch (const fs::filesystem_error& e) {
				std::cerr << "Ошибка при перемещении " << fileName << ": " << e.what() << "\n";
			}
		}

		std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
		std::cout << "СОРТИРОВКА ЗАВЕРШЕНА!\n";
		std::cout << "   Обработано файлов: " << filesProcessed << "\n";
		std::cout << "   Перемещено файлов: " << filesMoved << "\n";

		return 0;
	}
	// настройка консоли 
	void setupConsole() {

		// Устанавливаем кодировку UTF-8 для консоли Windows
		SetConsoleOutputCP(CP_UTF8);
		SetConsoleCP(CP_UTF8);

		// Включаем поддержку VT-последовательностей (для цветов) на Windows 10+
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD dwMode = 0;
		GetConsoleMode(hOut, &dwMode);
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hOut, dwMode);

		// Устанавливаем русскую локаль
		setlocale(LC_ALL, "ru_RU.UTF-8");
		std::locale::global(std::locale("ru_RU.UTF-8"));
	}
	void printBanner() {
		std::cout << "\n";
		std::cout << "╔══════════════════════════════════════════════════════════════════╗\n";
		std::cout << "║                   ФАЙЛОВЫЙ ОРГАНАЙЗЕР v1.0                        ║\n";
		std::cout << "║                   Автоматическая сортировка файлов                ║\n";
		std::cout << "╚══════════════════════════════════════════════════════════════════╝\n";
		std::cout << "\n";
	}
	void printHelp() {
		std::cout << "Команды:\n";
		std::cout << "  sort [путь]     - отсортировать указанную папку\n";
		std::cout << "  sort .          - отсортировать текущую папку\n";
		std::cout << "  rules           - показать все правила сортировки\n";
		std::cout << "  add .ext папка  - добавить новое правило\n";
		std::cout << "  remove .ext     - удалить правило\n";
		std::cout << "  help            - показать эту справку\n";
		std::cout << "  exit            - выход\n";
		std::cout << "\n";
	}