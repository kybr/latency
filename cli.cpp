// build like this (uses c++11 features)
//
// g++ -std=c++11 click_karl_edits.cpp
//
// run like this....
//
// ./a.out blockSize:1 sampleRate:2 impulseMethod:pin priority:high
//     or
// ./a.out bs:1 sr:2 im:pin pr:high rs:beatles pi:3 po:4 ky:9
//
// -- Karl Yerkes (2015-07-08)

#include <cstdio>
#include <utility>
#include <string>
#include <unordered_map>
#include <cassert>

using namespace std;

// utility function splits "key:value" into separate strings "key" and "value"
//
void split(const string& possibleOption, string& key, string& value) {
  size_t whereTheColonIs = possibleOption.find(":");
  assert(whereTheColonIs != string::npos);  // die if we find no : char
  key = possibleOption.substr(0, whereTheColonIs);
  value = possibleOption.substr(whereTheColonIs + 1);  // +1 to skip the :
  // no return value. input arguments are changed
}

// a struct is exactly like a class with everything public...
//
struct Options {
  // class members with defaults (set defaults here)
  //
  unsigned device = 0, blockSize = 512, sampleRate = 44100;
  enum { KEY, PIN, PORT } impulseMethod = KEY;
  enum { NORMAL, HIGH, REALTIME } priority = NORMAL;
  enum { CLICK, DING, BEATLES } responseSound = CLICK;
  unsigned pin = 20, port = 9999, key = 13;

  // print out the current state of the object, marking where an option differs
  // from the default value.
  //
  void print() {
    static Options defaults;

    printf("%c device:%u\n", device == defaults.device ? ' ' : '!',
           device);
    printf("%c blockSize:%u\n", blockSize == defaults.blockSize ? ' ' : '!',
           blockSize);
    printf("%c sampleRate:%u\n", sampleRate == defaults.sampleRate ? ' ' : '!',
           sampleRate);
    printf("%c pin:%u\n", pin == defaults.pin ? ' ' : '!', pin);
    printf("%c port:%u\n", port == defaults.port ? ' ' : '!', port);
    printf("%c key:%u\n", key == defaults.key ? ' ' : '!', key);

    // these use anonymous lookup tables to map enums to strings
    //
    printf("%c impulseMethod:%s\n",
           impulseMethod == defaults.impulseMethod ? ' ' : '!',
           (const char* []) { "key", "pin", "port", }[impulseMethod]);
    printf("%c priority:%s\n", priority == defaults.priority ? ' ' : '!',
           (const char* []) { "normal", "high", "realtime" }[priority]);
    printf("%c responseSound:%s\n",
           responseSound == defaults.responseSound ? ' ' : '!',
           (const char* []) { "click", "ding", "beatles" }[responseSound]);
  }

  // deal with a possible option string (key:value)
  //
  void dealWith(const string& possibleOption) {
    // extract the key and the value
    //
    string key, value;
    split(possibleOption, key, value);

    // parse all possible keys and use values to change settings
    //
    if (key == "impulseMethod" || key == "im")
      if (value == "key")
        impulseMethod = KEY;
      else if (value == "pin")
        impulseMethod = PIN;
      else if (value == "port")
        impulseMethod = PORT;
      else
        assert(false);
    else if (key == "priority" || key == "pr")
      if (value == "high")
        priority = HIGH;
      else if (value == "normal")
        priority = NORMAL;
      else if (value == "realtime")
        priority = REALTIME;
      else
        assert(false);
    else if (key == "responseSound" || key == "rs")
      if (value == "click")
        responseSound = CLICK;
      else if (value == "ding")
        responseSound = DING;
      else if (value == "beatles")
        responseSound = BEATLES;
      else
        assert(false);
    else if (key == "device" || key == "dv")
      device = atoi(value.c_str());
    else if (key == "blockSize" || key == "bs")
      blockSize = atoi(value.c_str());
    else if (key == "sampleRate" || key == "sr")
      sampleRate = atoi(value.c_str());
    else if (key == "pin" || key == "pi")
      pin = atoi(value.c_str());
    else if (key == "port" || key == "po")
      port = atoi(value.c_str());
    else if (key == "key" || key == "ky")
      this->key = atoi(value.c_str());
    else
      assert(false);
  }
};

int main(int argumentCount, char* argumentList[]) {
  // make default options and print
  //
  Options options;

  // deal with each command line argument, then show changed object
  //
  for (int i = 1; i < argumentCount; ++i) options.dealWith(argumentList[i]);
  options.print();

  // continue program with good options
  //
}
