#include <iostream>
#include <switch.h>
#include <filesystem>
#include "util/lang.hpp"
#include "util/config.hpp"

namespace Language {
    json lang;

    void Load() {
        std::ifstream ifs;
        std::string languagePath;
        int langInt = inst::config::languageSetting;
        if (langInt == 99) {
            SetLanguage ourLang;
            u64 lcode = 0;
            setInitialize();
            setGetSystemLanguage(&lcode);
            setMakeLanguage(lcode, &ourLang);
            setExit();
            langInt = (int)ourLang;
        } 
        switch (langInt) {
            case 0:
                languagePath = "romfs:/lang/en.json";
                break;
            case 1:
                languagePath = "romfs:/lang/jp.json";
                break;
            case 2:
                languagePath = "romfs:/lang/fr.json";
                break;
            case 3:
                languagePath = "romfs:/lang/de.json";
                break;
            case 4:
                languagePath = "romfs:/lang/it.json";
                break;
            case 5:
                languagePath = "romfs:/lang/ru.json";
                break;
            case 6:
                languagePath = "romfs:/lang/zh-rTW.json";
                break;
            case 99:
                languagePath = "romfs:/lang/en.json";
                break;
            default:
            		if (std::filesystem::exists(inst::config::appDir + "/lang/custom.json")) {
            			languagePath = (inst::config::appDir + "/lang/custom.json");
            		}
            		else{
            			languagePath = "romfs:/lang/en.json";
            		}
        }
        if (std::filesystem::exists(languagePath)) ifs = std::ifstream(languagePath);
        else ifs = std::ifstream("romfs:/lang/en.json");
        if (!ifs.good()) {
            std::cout << "[FAILED TO LOAD LANGUAGE FILE]" << std::endl;
            return;
        }
        lang = json::parse(ifs);
        ifs.close();
    }

    std::string LanguageEntry(std::string key) {
        json j = GetRelativeJson(lang, key);
        if (j == nullptr) {
            return "didn't find: " + key;
        }
        return j.get<std::string>();
    }

    std::string GetRandomMsg() {
        json j = Language::GetRelativeJson(lang, "inst.finished");
        srand(time(NULL));
        return(j[rand() % j.size()]);
    }
}