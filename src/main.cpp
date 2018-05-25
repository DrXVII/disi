//c++ std lib
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <random>
#include <fstream>
#include <memory>
using std::shared_ptr;
#include <map>
using std::map;
#include <sstream>
#include <limits>

//c std lib
#include <math.h>

#define PROGRAM_NAME "Dice Simulator"
#define PROGRAM_VERSION "v1.3.2"

#ifdef DEBUG
#    define PROGRAM_VERSION_DBG "(DBG)"
#else
#    define PROGRAM_VERSION_DBG ""
#endif

#define DBFILE_DEFAULT "./configs.dat"

typedef uint32_t u32;
typedef int32_t i32;
typedef int64_t i64;

struct Dice {
    u32 sides;
    i32 min_val;
    i32 increment;
    u32 count;
};

int throw_dice(Dice* _dice,
        std::mt19937* _rng = nullptr,
        std::uniform_int_distribution<i32>* _dist = nullptr,
        bool _silent = false, int* result_ = nullptr);
int custom_throw();
int add_config(vector<Dice*>* _configs);
Dice* dice_from_input();
int select_config(vector<Dice*>* _configs, Dice** cur_config_);
int remove_config(vector<Dice*>* _configs);
string dice_to_str(Dice* _dice);
void print_configs(vector<Dice*>* _configs);
int calc_stats(Dice* _dice);
void free_configs(vector<Dice*>* _configs);
int get_int(int _default, int _min, int _max, string _prompt = "> ");
unsigned get_uint(unsigned _default, unsigned _min, unsigned _max,
    string _prompt = "> ");
int write_configs(vector<Dice*>* _configs);
int read_configs(vector<Dice*>* _configs);

int main()
{
    string program_info(PROGRAM_NAME " " PROGRAM_VERSION_DBG PROGRAM_VERSION);

    vector<Dice*> throw_configs;
    read_configs(&throw_configs);

    //creating the default config at position 0 if no configs loaded
    if(throw_configs.size() < 1) {
        cout << "no configs found, setting default config\n";
        throw_configs.push_back(new Dice {
            .sides = 6, .min_val = 1, .increment = 1, .count = 3});
    }

    string def_cfg_str(std::to_string(throw_configs[0]->count) + "d"
            + std::to_string(throw_configs[0]->sides));

    Dice* cur_config = throw_configs[0]; //currently selected config

    string cmd;
    while (cmd[0] != 'q') {
        cout << "\n\n"
             << "*** " << program_info << " ***\n"
             << "t - throw " << dice_to_str(cur_config) << "\n"
             << "c - custom throw\n"
             << "s - select config\n"
             << "a - add config\n"
             << "r - remove config\n"
             << "w - write configs to file\n"
             << "l - load configs from file\n"
             << "p - see probability statistics\n"
             << "q - quit\n";

        cout << "> ";
        getline(cin, cmd);

        switch(cmd[0]) {
        case 't': throw_dice(cur_config); break;
        case 'c': custom_throw(); break;
        case 's': select_config(&throw_configs, &cur_config); break;
        case 'a': add_config(&throw_configs); break;
        case 'r': remove_config(&throw_configs); break;
            cout << "Sorry, this part is currently under construction.\n";
            break;
        case 'w': write_configs(&throw_configs); break;
        case 'l': read_configs(&throw_configs); break;
        case 'p': calc_stats(cur_config); break;
        case 'q': break;
        default:
            cout << "invalid selection (" << cmd << ")\n";
        }
    }

    free_configs(&throw_configs);

    return 0;
}

int throw_dice(Dice* _dice,
        std::mt19937* _rng, std::uniform_int_distribution<i32>* _dist,
        bool _silent, int* result_)
{
    if(_dice == nullptr) {
        cout << "dice structure is not initialised (nullptr)";
    }

    if(_dice->sides < 1) {
        cout << "a slight problem: dice can not have less than 1 side\n";
        return -1;
    }

    i32 min = _dice->min_val;
    i32 max = min + (_dice->increment * (_dice->sides - 1));

    std::mt19937* rng;
    //std::uniform_int_distribution<i32> dist(min, max);
    std::uniform_int_distribution<i32>* dist;
    if(_rng != nullptr && _dist != nullptr) {
        rng = _rng;
        dist = _dist;
    }
    else {
        rng = new std::mt19937;
        dist = new std::uniform_int_distribution<i32>(min, max);

        rng->seed(std::random_device()());
    }

    i64 total = 0;
    for(unsigned i = 0; i < _dice->count; ++i) {
        i32 rand_val = (*dist)(*rng);
        if(!_silent) { cout << "d#" << i+1 << ": " << rand_val << "\n"; }
        total += rand_val;
    }

    if(!_silent) {
        cout << "total" << dice_to_str(_dice) << ": "
             << total << "\n";
    }

    if(result_ != nullptr) { *result_ = total; }

    if(_rng == nullptr) { delete rng; }
    if(_dist == nullptr) { delete dist; }
    return 0;
}

int custom_throw()
{
    Dice* dice = dice_from_input();

    throw_dice(dice);

    delete dice;

    return 0;
}

Dice* dice_from_input()
{
    Dice* dice = new Dice;

    u32 def_sides = 6;
    u32 min_sides = 2;
    u32 max_sides = 1000000;
    i32 def_min_val = 1;
    i32 min_min_val = std::numeric_limits<i32>::min();
    i32 max_min_val = std::numeric_limits<i32>::max();
    i32 def_increment = 1;
    i32 min_increment = std::numeric_limits<i32>::min();
    i32 max_increment = std::numeric_limits<i32>::max();
    u32 def_dice_count = 6;
    u32 min_dice_count = 1;
    u32 max_dice_count = std::numeric_limits<u32>::max();

    dice->sides = get_uint(def_sides, min_sides, max_sides,
                           "number of sides: ");
    //cout << "dice sides selected: " << dice->sides << endl;
    dice->min_val = get_int(def_min_val, min_min_val, max_min_val,
                            "minimum value: ");
    //cout << "min val selected: " << dice->min_val << endl;
    dice->increment = get_int(def_increment, min_increment, max_increment,
                              "value increment (usually 1): ");
    //cout << "increment selected: " << dice->increment << endl;
    dice->count = get_uint(def_dice_count, min_dice_count, max_dice_count,
                           "number of dice to throw: ");
    //cout << "dice count selected: " << dice->count << endl;

    return dice;
}

int add_config(vector<Dice*>* _configs)
{
    _configs->push_back(dice_from_input());

    return 0;
}

int select_config(vector<Dice*>* _configs, Dice** cur_config_)
{
    //just printing for now to test adding of a config
    cout << "*** DICE CONFIGS ***\n";
    print_configs(_configs);

    unsigned def_sel = 0;
    unsigned sel = get_int(def_sel, 0, _configs->size() - 1);

    *cur_config_ = (*_configs)[sel];
    cout << "selected " << dice_to_str(*cur_config_) << endl;

    return 0;
}

int remove_config(vector<Dice*>* _configs)
{
    cout << "*** REMOVE CONFIGS ***\n";
    print_configs(_configs);

    if(_configs->size() <= 1) {
        cout << "too few configurations left, refusing to remove\n";
        return -1;
    }

    unsigned def_sel = 0;
    unsigned sel = get_int(def_sel, 0, _configs->size() - 1);

    cout << "deleting... ";
    delete (*_configs)[sel];
    (*_configs)[sel] = _configs->back();
    _configs->pop_back();
    cout << " deleted\n";

    return 0;
}

string dice_to_str(Dice* _dice)
{
    i32 die_max = _dice->min_val
        + (_dice->increment * (_dice->sides - 1));

    string notation_str(std::to_string(_dice->count) + "d"
        + std::to_string(_dice->sides));
    string die_min_str(std::to_string(_dice->min_val));
    string die_max_str(std::to_string(die_max));

    string result(notation_str
        + "(" + die_min_str + " - " + die_max_str
        + ")[" + std::to_string(i64(_dice->min_val) * _dice->count)
        + " - " + std::to_string(i64(die_max) * _dice->count) + "]"
    );

    return result;
}

void print_configs(vector<Dice*>* _configs) {
    for(unsigned i = 0; i < _configs->size(); ++i) {
        cout << i << ": " << dice_to_str((*_configs)[i]) << "\n";
    }
}

int calc_stats(Dice* _dice)
{
    string cmdbuf;
    unsigned def_sample_sz = 1000000; //the default sample size if no input
    unsigned sample_sz = def_sample_sz;
    i32 min = _dice->min_val;
    i32 max = min + (_dice->increment * (_dice->sides - 1));
    std::stringstream prompt;

    prompt << "sample size (" << def_sample_sz << "): ";

    sample_sz = get_uint(def_sample_sz,
            _dice->count * (abs(min) + abs(max)), u32(-1), prompt.str());

    //cout << "sample size will be " << sample_sz << endl;

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<i32> dist(min, max);

    //filling a map with statistic counters
    map<int, unsigned> val_stats;
    for(i64 i = i64(min) * _dice->count;
        i <= max * _dice->count;
        i += _dice->increment)
    {
        //cout << "mapping stat value for " << i << endl;
        val_stats.insert_or_assign(i, 0);
    }

    //generating and counting the values (brute force)
    for(unsigned i = 0; i < sample_sz; ++i) {
        i32 total = 0;
        for(unsigned j = 0; j < _dice->count; ++j) {
            throw_dice(_dice, &rng, &dist, true, &total);
        }

        auto found = val_stats.find(total);
        found->second += 1;
        val_stats.insert_or_assign(found->first, found->second);
    }

    //printing out the result
    size_t max_num_ch_count = std::to_string(i64(max) * _dice->count).size();
    size_t min_num_ch_count = std::to_string(i64(min) * _dice->count).size();
    size_t max_needed_padding = (max_num_ch_count > min_num_ch_count)
                            ? max_num_ch_count : min_num_ch_count;
    //cout << "max_needed_padding = " << max_needed_padding << endl;

    cout << "*** STATISTICS ***\n";
    for(i64 i = i64(min) * _dice->count;
        i <= max * _dice->count;
        i += _dice->increment)
    {
        auto stat = val_stats.find(i);

        //horribly inefficient, but we only need a few lines printed
        size_t need_pad_len =
            max_needed_padding - std::to_string(stat->first).size();
        string outp_padding(need_pad_len, ' ');

        cout << stat->first << ": " << outp_padding
             << 100.0d * stat->second / sample_sz << "% ("
             << stat->second << ")\n";
    }

    return 0;
}

void free_configs(vector<Dice*>* _configs) {
    for(unsigned i = 0; i < _configs->size(); ++i) {
        delete (*_configs)[i];
        (*_configs)[i] = nullptr;
    }
}

int get_int(int _default, int _min, int _max, string _prompt)
{
    string in_buf;
    int ret_val = _default;

    cout << _prompt;
    //cout << "(" << _min << "/" << _max << ")";
    std::getline(cin, in_buf);

    try {
        ret_val = std::stoi(in_buf);
    } catch(const std::invalid_argument& _e) {
        cout << "bad input format (" << in_buf <<
            "), selecting default value of " << _default << endl;
    } catch(const std::exception& _e) {
        cout << "conversion exception, what: " << _e.what() << endl;
    }

    if(ret_val < _min || ret_val > _max) {
        cout << "selection out of range, selecting default value of "
             << _default << endl;
        ret_val = _default;
    }

    return ret_val;
}

unsigned get_uint(unsigned _default, unsigned _min, unsigned _max,
    string _prompt)
{
    string in_buf;
    unsigned ret_val = _default;

    cout << _prompt;
    //cout << "(" << _min << "/" << _max << ")";
    std::getline(cin, in_buf);

    try {
        ret_val = std::stoul(in_buf);
    } catch(const std::invalid_argument& _e) {
        cout << "bad input format (" << in_buf <<
            "), selecting default value of " << _default << endl;
    } catch(const std::exception& _e) {
        cout << "conversion exception, what: " << _e.what() << endl;
    }

    if(ret_val < _min || ret_val > _max) {
        cout << "selection out of range, selecting default value of "
             << _default << endl;
        ret_val = _default;
    }

    return ret_val;
}

int write_configs(vector<Dice*>* _configs)
{
    std::ofstream ofs(DBFILE_DEFAULT, std::ios::binary);

    if(!ofs.is_open()) {
        cout << "could not open configs data file for writing\n";
        return -1;
    }

    cout << "*** WRITING CONFIGS TO FILE ***\n";

    unsigned i = 0;
    size_t tot_size = 0;
    for(; i < _configs->size(); ++i) {
        cout << "writing config #" << i << ": " << dice_to_str((*_configs)[i])
             << endl;
        size_t size = sizeof *(*_configs)[i];
        tot_size += size;
        ofs.write(reinterpret_cast<char*>((*_configs)[i]), size);
    }

    ofs.close();

    cout << "[WRITING DONE], total written: "
         << i << " entries, " << tot_size << " bytes\n";

    return 0;
}

int read_configs(vector<Dice*>* _configs)
{
    std::ifstream ifs(DBFILE_DEFAULT, std::ios::binary | std::ios::ate);

    if(!ifs.is_open()) {
        cout << "could not open configs data file for reading\n";
        return -1;
    }

    size_t element_size = sizeof *(*_configs)[0];
    auto file_size = ifs.tellg();
    size_t configs_in_file = file_size / element_size;
    ifs.seekg(0);

    free_configs(_configs);
    *_configs = vector<Dice*>(configs_in_file);

    cout << "*** LOADING CONFIGS FROM FILE ***\n";

    unsigned i = 0;
    for(; i < configs_in_file; ++i) {
        (*_configs)[i] = new Dice;
        ifs.read(reinterpret_cast<char*>((*_configs)[i]), element_size);
        cout << "read config #" << i << ": " << dice_to_str((*_configs)[i])
             << endl;
    }

    ifs.close();

    cout << "[READING DONE], total read: " << i << " entries\n";


    return 0;
}
