#include <string>
#include <fstream>
#include <iostream>
#include <sys/time.h>

#include <v8.h>

#include "data.h"

using namespace std;

bool read_file(std::string& output, const std::string& filename) {
    std::ifstream t(filename.c_str());
    if (!t.good()) {
        return false;
    }

    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    output.resize(size);
    t.seekg(0);
    t.read(&output[0], size);
    return true;
}

v8::Handle<v8::Value> DataSize(const v8::Arguments& args) {
    return v8::Integer::New(data_size());
}

v8::Handle<v8::Value> DataElem(const v8::Arguments& args) {
    if (args.Length() != 1 || !args[0]->IsUint32()) {
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }

    unsigned n = args[0]->Uint32Value();
    return v8::String::New(data_elem(n));
}

v8::Handle<v8::Value> Print(const v8::Arguments& args) {
  bool first = true;
  for (int i = 0; i < args.Length(); i++) {
    v8::HandleScope handle_scope;
    if (first) {
      first = false;
    } else {
      printf(" ");
    }
    v8::String::Utf8Value str(args[i]);
    printf("%s", *str);
  }
  printf("\n");
  fflush(stdout);
  return v8::Undefined();
}


string runV8(const string& script) {
    v8::HandleScope handle_scope;

    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
    global->Set(v8::String::New("data_size"), v8::FunctionTemplate::New(DataSize));
    global->Set(v8::String::New("data_elem"), v8::FunctionTemplate::New(DataElem));
    global->Set(v8::String::New("print"), v8::FunctionTemplate::New(Print));

    // Create a new execution environment containing the built-in functions
    v8::Persistent<v8::Context> context = v8::Context::New(NULL, global);

    // Enter the execution environment before evaluating any code.
    v8::Context::Scope context_scope(context);

    v8::Handle<v8::String> source = v8::String::New(script.c_str());

    v8::Handle<v8::Script> v8_script = v8::Script::New(source, v8::String::New("script_name"));

    v8::Handle<v8::Value> result = v8_script->Run();

    v8::String::Utf8Value str(result);

    return *str;
}


int main(int argc, char** argv) {
    struct timeval start, end, diff;

    load_data(argc > 1 ? argv[1] : "data_10k");

    string script;

    read_file(script, "v8.js");

    gettimeofday(&start, NULL);
    string ret = runV8(script);
    gettimeofday(&end, NULL);

    cout << ret << endl;

    timersub(&end, &start, &diff);
    cerr << "ran in " << 1e3 * (diff.tv_sec + diff.tv_usec/1e6) << " ms" << endl;

    return 0;
}
