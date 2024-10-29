//
//  main.cpp
//  AlgLab
//
//  Created by vika on 26.10.2024.
//

#include <iostream>
#include <ctime>
#include <cmath>
#include <bitset>
#include <chrono>
#include <vector>
#include <numeric>

using namespace std;

#define POLYNOMIAL 0x1021
#define REVERSED_POLYNOMIAL 0x8408
#define INIT 0xFFFF

uint16_t simple_sequential_alg(uint8_t *byte_array, size_t size){
    uint16_t crc = INIT;
    
    for (size_t i = 0; i < size; i++){
        crc = crc ^ (uint16_t) (byte_array[i] << 8);
        for (uint8_t bit = 0; bit < 8; ++bit){
            if (crc & 0x8000){
                crc = (crc << 1) ^ POLYNOMIAL;
            }
            else{
                crc = crc << 1;
            }
        }
    }
    
    return crc;
}

uint16_t reverse_sequential_alg(uint8_t *byte_array, size_t size){
    uint16_t crc = INIT;
    for (size_t i = 0; i < size; i++){
        crc = crc ^ byte_array[i];
        for (uint8_t bit = 0; bit < 8; ++bit){
            if (crc & 0x0001){
                crc = (crc >> 1) ^ REVERSED_POLYNOMIAL;
            }
            else{
                crc = crc >> 1;
            }
        }
    }
    return crc;
}

uint16_t simple_table[256];

void generate_simple_table(){
    for (int i = 0; i < 256; i++){
        uint16_t value = (uint16_t) (i << 8);
        
        for (uint8_t bit = 0; bit < 8; ++bit){
            if (value & 0x8000){
                value = (value << 1) ^ POLYNOMIAL;
            }
            else{
                value = value << 1;
            }
        }
        simple_table[i] = value;
    }
}

uint16_t reverse_table[256];

void generate_reverse_table(){
    for (int i = 0; i < 256; i++){
        uint16_t value = (uint16_t) i;
        
        for (uint8_t bit = 0; bit < 8; ++bit){
            if (value & 0x0001){
                value = (value >> 1) ^ REVERSED_POLYNOMIAL;
            }
            else{
                value = value >> 1;
            }
        }
        reverse_table[i] = value;
    }
}

uint16_t simple_table_alg(uint8_t *byte_array, uint16_t* table, size_t size){
    uint16_t crc = INIT;
    
    for (size_t i = 0; i < size; i++){
        uint8_t index = (crc >> 8) ^ byte_array[i];
        crc = table[index] ^ (crc << 8);
    }
    
    return crc;
}

uint16_t reversed_table_alg(uint8_t *byte_array, uint16_t* table, size_t size){
    uint16_t crc = INIT;

    for (size_t i = 0; i < size; i++){
        uint8_t index = (crc ^ byte_array[i]) & 0xFF;
        crc = table[index] ^ (crc >> 8);
    }
    
    return crc;
}



void analize(size_t trial_amount, size_t num_of_bytes){
    vector<double> simple_sequential_alg_time;
    vector<double> reverse_sequential_alg_time;
    vector<double> simple_table_alg_time;
    vector<double> reverse_table_alg_time;
    
    cout << "Analizing performance for " << trial_amount << " trial(s)" << endl << endl;
    for (size_t i = 0; i < trial_amount; i++){
        cout << "Trial " << i+1 << endl;
        uint8_t current_message_bytes[num_of_bytes];
        for (auto &i: current_message_bytes){
            i = static_cast<uint8_t>(rand() % 255 + 1);
        }
        
        auto start = chrono::steady_clock::now();
        const uint16_t crc_simple_seq = simple_sequential_alg(current_message_bytes, num_of_bytes);
        auto end = chrono::steady_clock::now();
        double time = static_cast<chrono::duration<double>>(end - start).count();
        cout << "CRC simple sequential result: 0x" << hex << crc_simple_seq << ", time: " << time << "s" << endl;
        simple_sequential_alg_time.push_back(time);
        
        start = chrono::steady_clock::now();
        const uint16_t crc_reverse_seq = reverse_sequential_alg(current_message_bytes, num_of_bytes);
        end = chrono::steady_clock::now();
        time = static_cast<chrono::duration<double>>(end - start).count();
        cout << "CRC reverse sequential result: 0x" << hex << crc_reverse_seq << ", time: " << time << "s" << endl;
        reverse_sequential_alg_time.push_back(time);
        
        start = chrono::steady_clock::now();
        const uint16_t crc_simple_table = simple_table_alg(current_message_bytes, simple_table, num_of_bytes);
        end = chrono::steady_clock::now();
        time = static_cast<chrono::duration<double>>(end - start).count();
        cout << "CRC simple table result: 0x" << hex << crc_simple_table << ", time: " << time << "s" << endl;
        simple_table_alg_time.push_back(time);
        
        start = chrono::steady_clock::now();
        const uint16_t crc_reverse_table = reversed_table_alg(current_message_bytes, reverse_table, num_of_bytes);
        end = chrono::steady_clock::now();
        time = static_cast<chrono::duration<double>>(end - start).count();
        cout << "CRC reverse table result: 0x" << hex << crc_reverse_table << ", time: " << time << "s" << endl;
        reverse_table_alg_time.push_back(time);
    }
    
    cout << endl;
    
    cout << "Total / mean value of " << trial_amount << " trials of simple sequential algorithm: " << accumulate(simple_sequential_alg_time.begin(), simple_sequential_alg_time.end(), 0.00) << "s / " <<  accumulate(simple_sequential_alg_time.begin(), simple_sequential_alg_time.end(), 0.00) / (double) (trial_amount) << "s" << "\n";
    cout << "Total / mean value of " << trial_amount << " trials of reverse sequential algorithm: " << accumulate(reverse_sequential_alg_time.begin(), reverse_sequential_alg_time.end(), 0.00) << "s / " <<  accumulate(reverse_sequential_alg_time.begin(), reverse_sequential_alg_time.end(), 0.00) / (double) (trial_amount) << "s" << "\n";
    cout << "Total / mean value of " << trial_amount << " trials of simple table algorithm: " << accumulate(simple_table_alg_time.begin(), simple_table_alg_time.end(), 0.00) << "s / " <<  accumulate(simple_table_alg_time.begin(), simple_table_alg_time.end(), 0.00) / (double) (trial_amount) << "s" << "\n";
    cout << "Total / mean value of " << trial_amount << " trials of reverse table algorithm: "<< accumulate(reverse_table_alg_time.begin(), reverse_table_alg_time.end(), 0.00) << "s / " <<  accumulate(reverse_table_alg_time.begin(), reverse_table_alg_time.end(), 0.00) / (double) (trial_amount) << "s" << "\n";
}

int main(int argc, const char * argv[]) {
    size_t iterations = 1;
    size_t length = 1000;
    size_t num_of_bytes = length / 8;
    
    srand(static_cast<unsigned int>(time(nullptr)));
  
    auto total_start = chrono::steady_clock::now();
    
    auto start = chrono::steady_clock::now();
    generate_simple_table();
    auto end = chrono::steady_clock::now();
    double time = static_cast<chrono::duration<double>>(end - start).count();
    cout << "Simple table (2^15) generation time: " << time << "s, size: " << sizeof(simple_table) << " bytes" << endl;
    
    start = chrono::steady_clock::now();
    generate_reverse_table();
    end = chrono::steady_clock::now();
    time = static_cast<chrono::duration<double>>(end - start).count();
    cout << "Reverse table (2^15) generation time: " << time << "s, size: " << sizeof(reverse_table)<< " bytes" << endl << endl;
    
    analize(iterations, num_of_bytes);
    
    auto total_end = chrono::steady_clock::now();
    double total_time = static_cast<chrono::duration<double>>(total_end - total_start).count();
    cout << endl << "Total performance time with " << iterations << " iteration(s): " << total_time << "s" << endl;
    return 0;
}
