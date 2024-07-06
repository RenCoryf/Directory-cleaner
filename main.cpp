#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
using namespace std;

bool preset() {
	std::filesystem::create_directory("C:/Dir cleaner");
	std::ofstream file;
	file.open("C:/Dir cleaner/Settings.txt");
	file.close();
	return std::filesystem::exists("C:/Dir cleaner/Settings.txt");
}

bool add_extension(const filesystem::path& settings_file_name,string extension,string dir) {
	fstream file;
	file.open(settings_file_name, ios_base::app);
	if (!file.is_open()) return false;
	file << extension << ' ' << dir<<'\n';
	file.close();
	return true;
}

filesystem::path find_dir(string extension, const filesystem::path& settings_file_name) {
	std::ifstream file;
	file.open(settings_file_name);
	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		string ext, dir;
 		if (!(iss >> ext >> dir)) { break; }
		if (extension == ext) return dir;
	}
	return  "None"; // in case there's no such extension	
}

bool move_file(const filesystem::path& file_name,const filesystem::path& destination_point) {
	filesystem::copy_file(file_name, destination_point);
	return  filesystem::remove(file_name);
}

vector<filesystem::path> get_directory_files_names(string directory_path) {
	vector<filesystem::path> names;
	for (const auto& dir_entry : filesystem::recursive_directory_iterator{ directory_path }) {
		if (!dir_entry.is_directory()) {
			names.push_back(dir_entry);
		} 
	}
	return names;
}

int main() {
	string file_name = "C:/Dir cleaner/Settings.txt";
	if (!filesystem::exists("C:/Dir cleaner/Settings.txt")) { preset(); }
	bool run = true;
	while (1) {
		cout << "Enter one out of four:\n\
	* Absolute path to the directory you want to clean\n\
	* --s to add extension to settings\n\
	* --h to get help with the application\n\
	* --c to close the application\n";
		string comand;
		cin >> comand;
		if (comand == "--s") {
			cout << "Enter extension which you want to add: ";
			string extension;
			cin >> extension;
			cout << "Enter directory where files with such extension should be moved: ";
			string dir;
			cin >> dir;
			add_extension(file_name, extension, dir) == true ? cout << "\nExtension " << extension << " was su\
ccessfuly added. Files with that extension will be moved to " << dir : cout << "Something went wrong. Open " << file_name << " \
and try to fix the problem. For more information read README.md";
			cout << "\n\n\n\n\n";
		}



		else if (comand == "--h") {
			cout << "This application is a directory cleaner tool for Windows os. It goes through all the files\
 in directory which you have chosen and checks the extensions are in settings_file, if so it copies the file to metioned\
 directory, if not it passes it.\n To add extension to settings do one of two:\n\
	* Choose --s option in menu\n\
	* Go to "+file_name+" and add extensions manualy.\n\n\n\n\n";
		}




		else if (comand == "--c") {
			break;
		}
		else {
			vector<filesystem::path> files;
			files = get_directory_files_names(comand);
			for (auto file : files) {
				string dest_point = find_dir(file.extension().generic_string(), file_name).generic_string();
				if (dest_point == "None") continue;
				dest_point += '/';
				dest_point+=file.stem().generic_string();
				dest_point += file.extension().generic_string();
				if (!move_file(file.generic_string(), dest_point)) {
					cout << file.generic_string() << " - something went wrong with this file";
				};
			}
			cout << comand << " is cleaned" << "\n\n\n\n\n";
		}
	}
	return 0;
}
// sub to https://github.com/RenCoryf