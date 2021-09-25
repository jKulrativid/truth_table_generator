#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#define _OPENTHEME "Hardware is suck, always has been.\n";
#define _HELP "Sry, I am too lazy to write the help.\n";

// global var
std::string read_path = "", save_path = "";
int term = 1, base = 0, output_size = 1;
bool for_espresso = false;

void start() {
	std::cout << _OPENTHEME;
}

std::map<std::string, std::vector<std::string>> read_data() {
	std::map<std::string, std::vector<std::string>> data_map;
	std::string variable_line, value_line;

	std::ifstream file(read_path);
	std::getline(file, variable_line);
	std::getline(file, value_line);

	std::vector<std::string> variables;
	std::stringstream var_ss(variable_line);
	std::string variable;
	// golang style loop (because while loop is bad)
	for (int i = 0; i < 1; i = 0) {
		if (!(var_ss >> variable)) {
			break;

		}
		variables.push_back(variable);
	}
	data_map.insert(std::make_pair("var", variables));

	std::vector<std::string> values;
	std::stringstream val_ss(value_line);
	std::string value;
	for (int i = 0; i < 1; i = 0) {
		if (!(val_ss >> value)) {
			break;

		}
		values.push_back(value);
	}
	data_map.insert(std::make_pair("value", values));
	return data_map;

}

int table_row_size(int n_var) {
	int total = 1;
	for (size_t i = 0; i < n_var; i++) {
		total *= 2;
	}
	return total;

}

std::vector<int> get_binary_value(int value, int n_digit) {
	std::vector<int> binary_value(n_digit);
	for (size_t i = 1; i <= n_digit; i++) {
		binary_value[n_digit-i] = value % 2;
		value /= 2;
	}
	return binary_value;

}

// generate truth table of row-1 variables
std::vector<std::vector<int>> get_table(int row, int column) {
	std::vector<std::vector<int>> empty_table(row);
	for (size_t r = 0; r < row; r++) {
		std::vector<int> empty_row(column);
		std::vector<int> bin_value = get_binary_value(r, column-1);
		for (size_t c = 0; c < column-1; c++) {
			empty_row[c] = bin_value[c];
		}
		empty_row[column-1] = base;
		empty_table[r] = empty_row;
	}
	return empty_table;

}

void write_txt(std::vector<std::vector<int>> &truth_table, std::vector<std::string> &vars) {
	size_t n_row = truth_table.size(), n_col = vars.size() + output_size;
	std::ofstream out(save_path, std::fstream::in | std::fstream::out | std::fstream::app);
	for (size_t r = 0; r < vars.size(); r++) {
		out << vars[r] << " ";
	}
	out << std::endl;
	for (size_t r = 0; r < n_row; r++) {
		for (size_t c = 0; c < n_col-output_size; c++) {
			int more_space_size = vars[c].length() - 1;
			out << truth_table[r][c] << " ";
			for (size_t i = 0; i < more_space_size; i++) {
				out << " ";
			}
		}
		out << truth_table[r][n_col-output_size];
		out << std::endl;
	}

}

void write_pla(std::vector<std::vector<int>> &truth_table, std::vector<std::string> &vars) {
	size_t n_row = truth_table.size(), n_col = vars.size() + output_size; 
	std::ofstream out(save_path, std::fstream::in | std::fstream::out | std::fstream::app);
	out << ".i" << " " <<  n_col - output_size << std::endl;
	out << ".o" << " " << output_size << std::endl;
	out << "ilb" << " ";
	for (auto &var: vars) { out << var << " "; }
	out << std::endl;
	out << ".ob Y" << std::endl;
	for (size_t r = 0; r < n_row; r++) {
		int value = truth_table[r][n_col-output_size];
		if (value == 1) {
			for (size_t c = 0; c < n_col - output_size; c++) {
				out << truth_table[r][c];
			}
			out << " " << truth_table[r][n_col-output_size] << std::endl;
		}
	}
	out << ".e";
}

void make_table(std::map<std::string, std::vector<std::string>> &data_map) {
	std::vector<std::string> &vars = data_map["var"], &vals = data_map["value"];
	
	int n_col = vars.size() + output_size;
	int n_row = table_row_size(n_col-output_size);
	std::vector<std::vector<int>> truth_table = get_table(n_row, n_col);

	// ---------- config truth table to match the given canonical form ------------- //
	for (auto &str_val: vals) {
		int val = std::stoi(str_val);
		truth_table[val][n_col-1] = term;
	}

	// ---------- write data  ---------- //
	if (for_espresso) {
		write_pla(truth_table, vars);
	} else {
		write_txt(truth_table, vars);
	}
}

void process() {
	std::map<std::string, std::vector<std::string>> data_map = read_data();
	make_table(data_map);
}

std::string validate_file(std::string path) {
	size_t path_length = path.length();
	if (path == "") {
		return "you didn't give any file path";

	}
	std::string file_type = path.substr(path_length - 3, 3);
	if (file_type != "txt") {
		return "must be txt file";

	}
	std::ifstream file;
	file.open(path);
	if (!file) {
		return "file not found **(note that you have to move you .txt file to the same directory as the execuatable file)";

	}
	return "OK";

}

int main(int argc, char** argv) {

	start();

	for (int i = 0; i < argc; i++) { // i = 1 for skipping first arg which ./program_name
		std::string cmd = std::string(argv[i]);
		if (cmd == "--help") {
			std::cout << _HELP;
			exit(0);
		}
		if (cmd == "-f") {
			try {
				i++;
				if (i >= argc) {
					throw std::string("NO_GIVEN_READ_PATH");
				}
				read_path = std::string(argv[i]);
			}
			catch (std::exception &e) {
				std::cout << e.what() << "\n";
				exit(1);
			}
			catch (std::string &error_string) {
				std::cout << "ERROR: " + error_string << "\n";
				exit(1);
			}
		}
		else if (cmd == "-s") {
			try {
				i++;
				if (i >= argc) {
					throw std::string("NO_GIVEN_SAVE_PATH");
				}
				read_path = std::string(argv[i]);
			}
			catch (std::exception &e) {
				std::cout << e.what() << "\n";
			}
			catch (std::string &error_string) {
				std::cout << "ERROR: " + error_string << "\n";
			}
		}
		else if (cmd == "-M") {
			term = 0, base = 1;  // max term
		}
		else if (cmd == "-m") {
			term = 1, base = 0;  // min term
		}
		else if (cmd == "--espresso") {
			for_espresso = true;
		}
	}

	if (save_path == "") {
		save_path = "out.txt";
		std::ofstream save_file;
		save_file.open(save_path);
		save_file.close();
	}

	std::string read_path_valid = validate_file(read_path);
	std::string save_path_valid = validate_file(save_path);

	if (read_path_valid != "OK") {
		std::cout << "ERROR on read path: " << read_path_valid;
		exit(1);
	}
	if (save_path_valid != "OK") {
		std::cout << "ERROR on save path: " << save_path_valid;
		exit(1);
	}

	process();

}