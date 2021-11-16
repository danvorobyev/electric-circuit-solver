#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <exception>
#include <algorithm>

const std::string vertex_delim = "--";
const std::string resistance_delim = ",";
const std::string voltage_delim = "V";
bool is_voltage_required = false;

bool is_empty_string(std::string& s) {
    if (s.empty())
        return true;
    return std::all_of(s.begin(),s.end(), [] (const char c) {
        return (c == ' ' || c == '\n');
    });
}

void check_input_stream_remainder(std::istringstream& input_stream) {
    std::string remainder;
    getline(input_stream, remainder, '\0');

    if (!is_empty_string(remainder))
        throw std::runtime_error("Invalid Input");
}
void add_new_edge(std::istringstream& input_stream, double first_number) {

    int output_vertex = static_cast<int>(first_number);
    output_vertex+=3;

    int input_vertex;
    double resistance;
    std::string new_vertex_delim, new_resistance_delim;

    input_stream >> std::setw(vertex_delim.length()) >> new_vertex_delim >> input_vertex
        >> std::setw(resistance_delim.length()) >> new_resistance_delim >> resistance;

    if(!input_stream
       || new_vertex_delim != vertex_delim
       || new_resistance_delim != resistance_delim)
        throw std::runtime_error("Invalid Input");

    check_input_stream_remainder(input_stream);

    is_voltage_required = true;
}

void add_edge_voltage(std::istringstream& input_stream, double first_number) {
    std::string new_voltage_delim;
    first_number+=1;
    input_stream >> std::setw(voltage_delim.length()) >> new_voltage_delim;

    if (!input_stream || new_voltage_delim != voltage_delim)
        throw std::runtime_error("Invalid Input");

    check_input_stream_remainder(input_stream);

    is_voltage_required = false;
}

void choose_next_input(std::istringstream& input_stream, double first_number) {
    const char delim = static_cast<char>(input_stream.peek());

    if (delim == vertex_delim[0])
        add_new_edge(input_stream, first_number);
    else if (delim == voltage_delim[0] && is_voltage_required)
        add_edge_voltage(input_stream, first_number);
    else
        throw std::runtime_error("Invalid Input");
}

void add_next_input(std::istringstream& input_stream) {
    double first_number;
    input_stream >>  first_number >> std::ws;

    if (input_stream) {
        choose_next_input(input_stream, first_number);
    } else {
        throw std::runtime_error("Invalid Input");
    }
}
int main()
{
    std::string input_line;

    while (std::getline(std::cin, input_line, ';')) {
        if (is_empty_string(input_line))
            continue;

        std::istringstream input_stream(input_line);

        add_next_input(input_stream);
    }
    return 0;
}