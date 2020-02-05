#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>

const std::string filter = "red";

void parseDOM(const rapidjson::Value& obj, int& total){
    bool found_red = false;
    if (obj.IsInt()){
        total += obj.GetInt();
        return;
    }
    else if (obj.IsArray()){ //if array
        for (rapidjson::SizeType i = 0; i < obj.Size(); i++) {
            if (obj[i].IsNumber()) {
                //if array value integer
                total += obj[i].GetInt();
            }
            else if (obj[i].IsArray() or obj[i].IsObject()){
                parseDOM(obj[i], total);
            }
        }
    }
    if (obj.IsObject()) {
        found_red = false;
        for (rapidjson::Value::ConstMemberIterator itr = obj.MemberBegin(); itr != obj.MemberEnd(); ++itr){
            if (itr->value.IsString() and itr->value.GetString() == filter) {
                found_red = true;
            }
        }
        if (found_red){
            return;
        }
        for (rapidjson::Value::ConstMemberIterator itr = obj.MemberBegin(); itr != obj.MemberEnd(); ++itr){
            const rapidjson::Value& objName = obj[itr->name.GetString()]; //make object value
            //printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
            if (itr->value.IsNumber()) {
                total += itr->value.GetInt();
            }
            else if (itr->value.IsArray()){ //if array
                for (rapidjson::SizeType i = 0; i < itr->value.Size(); i++) {
                    if (itr->value[i].IsNumber()) {
                        //if array value integer
                        total += itr->value[i].GetInt();
                    }
                    else if (itr->value[i].IsArray() or itr->value[i].IsObject()){ //if array value object
                        const rapidjson::Value& m = itr->value[i];
                        parseDOM(m, total);
                    }
                }
            }
            if (itr->value.IsObject()){
                parseDOM(objName, total);
            }
        }
    }
}

int main(int argc, char** argv){
    std::string file_name, input;
    int total = 0;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    rapidjson::Document doc {};
    input_file.open(file_name);
    rapidjson::IStreamWrapper isw { input_file };
    doc.ParseStream( isw );
    input_file.close();
    parseDOM(doc, total);
    std::cout << "Total is " << total << std::endl;
}
